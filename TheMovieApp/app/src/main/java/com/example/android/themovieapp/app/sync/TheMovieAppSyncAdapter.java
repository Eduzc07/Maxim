package com.example.android.themovieapp.app.sync;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.AbstractThreadedSyncAdapter;
import android.content.ContentProviderClient;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SyncRequest;
import android.content.SyncResult;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.text.format.Time;
import android.util.Log;

import com.example.android.themovieapp.app.BuildConfig;
import com.example.android.themovieapp.app.MainActivity;
import com.example.android.themovieapp.app.R;
import com.example.android.themovieapp.app.data.MovieContract;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Locale;
import java.util.Vector;

public class TheMovieAppSyncAdapter extends AbstractThreadedSyncAdapter {
    public final String LOG_TAG = TheMovieAppSyncAdapter.class.getSimpleName();
    // Interval at which to sync with the MovieDB, in seconds.
    // 60 seconds (1 minute) * 60 minutes (1 hour) * 24 hours = 1 day
    public static final int SYNC_INTERVAL = 60 * 60 * 24;
    public static final int SYNC_FLEXTIME = SYNC_INTERVAL / 3;
//    private static final long DAY_IN_MILLIS = 1000 * 60 * 60 * 24;
    private static final long DAY_IN_MILLIS = 1000 * 60 * 60 ;
    private static final int MOVIE_NOTIFICATION_ID = 3004;

    public static String mMovieQuery = "popular";
    public static String mLanguage = "es-PE";

    public static int mPage = 1;

    public static long mNowPlaying = 0;
    public static long mPopular = 0;
    public static long mTopRated = 0;
    public static long mUpcoming = 0;

    private static String mMostPopularMovieID;

    private static final String[] NOTIFY_MOVIE_PROJECTION = new String[]{
            MovieContract.MovieEntry.TABLE_NAME + "." + MovieContract.MovieEntry._ID,
            MovieContract.MovieEntry.COLUMN_MOVIE_ID,
            MovieContract.MovieEntry.COLUMN_MOVIE_NUMBER,
            MovieContract.MovieEntry.COLUMN_VOTE_AVERAGE,
            MovieContract.MovieEntry.COLUMN_TITLE,
            MovieContract.MovieEntry.COLUMN_POSTER_PATH,
            MovieContract.MovieEntry.COLUMN_ORIGINAL_LANGUAGE,
            MovieContract.MovieEntry.COLUMN_ORIGINAL_TITLE,
            MovieContract.MovieEntry.COLUMN_BACKDROP_PATH,
            MovieContract.MovieEntry.COLUMN_ADULT,
            MovieContract.MovieEntry.COLUMN_OVERVIEW,
            MovieContract.MovieEntry.COLUMN_RELEASE_DATE,
            MovieContract.MovieEntry.COLUMN_MOVIE_KEY
    };

    // these indices must match the projection
    static final int COL_MOVIE_ROW = 0;
    static final int COL_MOVIE_ID = 1;
    static final int COL_MOVIE_NUMBER = 2;
    static final int COL_VOTE_AVERAGE  = 3;
    static final int COL_TITLE = 4;
    static final int COL_POSTER_PATH = 5;
    static final int COL_ORIGINAL_LANGUAGE  = 6;
    static final int COL_ORIGINAL_TITLE = 7;
    static final int COL_BACKDROP_PATH = 8;
    static final int COL_ADULT = 9;
    static final int COL_OVERVIEW = 10;
    static final int COL_RELEASE_DATE = 11;
    static final int COL_MOVIE_KEY = 12;

    public TheMovieAppSyncAdapter(Context context, boolean autoInitialize) {
        super(context, autoInitialize);
    }

    @Override
    public void onPerformSync(Account account, Bundle extras, String authority, ContentProviderClient provider, SyncResult syncResult) {
        Log.d(LOG_TAG, "------> Starting sync <------");

        // These two need to be declared outside the try/catch
        // so that they can be closed in the finally block.
        HttpURLConnection urlConnection = null;
        BufferedReader reader = null;

        // Will contain the raw JSON response as a string.
        String ratedMovieJsonStr = null;

        try {
            //Movie Rated
            final String MOVIE_BASE = "https://api.themoviedb.org/3/";
            final String APPID_PARAM = "api_key";
            final String MOVIE = "movie";
            final String QUERY = mMovieQuery; //top_rated //upcoming //popular
            final String LANGUAGE = "language";
            final String PAGE = "page";
            final String MOVIE_LANGUAGE = mLanguage;

            Uri builtUri = Uri.parse(MOVIE_BASE).buildUpon()
                    .appendPath(MOVIE)
                    .appendPath(QUERY)
                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
                    .appendQueryParameter(LANGUAGE, MOVIE_LANGUAGE)
                    .appendQueryParameter(PAGE, String.valueOf(mPage))
                    .build();

            URL url = new URL(builtUri.toString());

            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + builtUri.toString());

            // Create the request to OpenWeatherMap, and open the connection
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.setRequestMethod("GET");
            urlConnection.connect();

            // Read the input stream into a String
            InputStream inputStream = urlConnection.getInputStream();
            StringBuffer buffer = new StringBuffer();
            if (inputStream == null) {
                // Nothing to do.
                return;
            }
            reader = new BufferedReader(new InputStreamReader(inputStream));

            String line;
            while ((line = reader.readLine()) != null) {
                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
                // But it does make debugging a *lot* easier if you print out the completed
                // buffer for debugging.
                buffer.append(line + "\n");
            }

            if (buffer.length() == 0) {
                // Stream was empty.  No point in parsing.
                return;
            }

            ratedMovieJsonStr = buffer.toString();
            getRatedMovies(ratedMovieJsonStr);

        } catch (IOException e) {
            Log.e(LOG_TAG, "Error ", e);
            // If the code didn't successfully get the weather data, there's no point in attempting
            // to parse it.
        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
            e.printStackTrace();
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
            if (reader != null) {
                try {
                    reader.close();
                } catch (final IOException e) {
                    Log.e(LOG_TAG, "Error closing stream", e);
                }
            }
        }
        return;
    }

    private void getRatedMovies(String dataJsonStr)
            throws JSONException {

        final String PAGES = "page";
        final String TOTAL_RESULTS = "total_results";
        final String TOTAL_PAGES = "total_pages";

        final String RESULTS = "results";

        final String MOVIE_ID = "id";
        final String VOTE_AVERAGE = "vote_average";
        final String TITLE = "title";
        final String POSTER_PATH = "poster_path";
        final String ORIGINAL_LANGUAGE = "original_language";
        final String ORIGINAL_TITLE = "original_title";
        final String GENRE_IDS = "genre_ids";
        final String BACKDROP_PATH = "backdrop_path";
        final String ADULT = "adult";
        final String OVERVIEW = "overview";
        final String RELEASE_DATE = "release_date";

        //https://www.youtube.com/watch?v=3VbHg5fqBYw

        try {
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
            JSONObject moviesJson = new JSONObject(dataJsonStr);

            int page = moviesJson.getInt(PAGES);
            int results = moviesJson.getInt(TOTAL_RESULTS);
            int pages = moviesJson.getInt(TOTAL_PAGES);

            JSONArray resultMoviesArray = moviesJson.getJSONArray(RESULTS);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  -- " + page);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  -- " + results);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  -- " + pages);

            // Insert the new weather information into the database
            Vector<ContentValues> cVVector = new Vector<ContentValues>(resultMoviesArray.length());

            for (int i = 0; i < resultMoviesArray.length(); i++) {
                // These are the values that will be collected.
                String movieID;
                String voteAverage;
                String title;
                String posterPath;
                String originalLanguage;
                String originalTitle;
                JSONArray genreIdsArray;
                String backdropPath;
                String adult;
                String overview;
                String releaseDate;
                String keyMovie;

                JSONObject currentMovie = resultMoviesArray.getJSONObject(i);

                movieID = currentMovie.getString(MOVIE_ID);
                voteAverage = currentMovie.getString(VOTE_AVERAGE);
                title = currentMovie.getString(TITLE);
                posterPath = currentMovie.getString(POSTER_PATH);
                originalLanguage = currentMovie.getString(ORIGINAL_LANGUAGE);
                originalTitle = currentMovie.getString(ORIGINAL_TITLE);
                backdropPath = currentMovie.getString(BACKDROP_PATH);
                adult = currentMovie.getString(ADULT);
                overview = currentMovie.getString(OVERVIEW);
                releaseDate = currentMovie.getString(RELEASE_DATE);

                keyMovie = getVideo(movieID);

                long base = 0; // "now_playing"

                if (mMovieQuery == "now_playing"){
                    base = 0;
                    mNowPlaying = i + (mPage-1)*20;
                    base += mNowPlaying;
                }

                if (mMovieQuery == "popular"){
                    base = 1000;
                    mPopular = i + (mPage-1)*20;
                    base += mPopular;
                    if (mPopular == 0) {
                        mMostPopularMovieID = movieID;
                    }
                }
                if (mMovieQuery == "top_rated"){
                    base = 2000;
                    mTopRated = i + (mPage-1)*20;
                    base += mTopRated;
                }
                if (mMovieQuery == "upcoming"){
                    base = 3000;
                    mUpcoming = i + (mPage-1)*20;
                    base += mUpcoming;
                }


                //Check if the Movie is already in the DB, update de number of movie
                if (updateMovieNumber(movieID, base))
                    continue;



                String genreIdsArrayString;
                genreIdsArray = currentMovie.getJSONArray(GENRE_IDS);
                genreIdsArrayString = String.valueOf(genreIdsArray.length());

                Log.d(LOG_TAG, "###### " + i + " movieID = " + movieID);
                Log.d(LOG_TAG, "###### " + i + " number of Movie = " + String.valueOf(base));
                Log.d(LOG_TAG, "###### " + i + " voteAverage = " + voteAverage);
                Log.d(LOG_TAG, "###### " + i + " title = " + title);
                Log.d(LOG_TAG, "###### " + i + " posterPath = " + posterPath);
                Log.d(LOG_TAG, "###### " + i + " originalLanguage = " + originalLanguage);
                Log.d(LOG_TAG, "###### " + i + "  originalTitle= " + originalTitle);
                for (int j = 0; j < genreIdsArray.length(); j++) {
                    int gentIds = genreIdsArray.getInt(j);
                    genreIdsArrayString += "-" + String.valueOf(gentIds);
                }
                Log.d(LOG_TAG, "###### " + i + " genreIdsArrayString = " + genreIdsArrayString);
                Log.d(LOG_TAG, "###### " + i + " backdropPath = " + backdropPath);
                Log.d(LOG_TAG, "###### " + i + " adult = " + adult);
                Log.d(LOG_TAG, "###### " + i + " overview = " + overview);
                Log.d(LOG_TAG, "###### " + i + " releaseDate = " + releaseDate);
                Log.d(LOG_TAG, "###### " + i + " keyMovie = " + keyMovie);
                Log.d(LOG_TAG, " ------------------------");

                //Adding to ContentValues
                ContentValues movieValues = new ContentValues();
                movieValues.put(MovieContract.MovieEntry.COLUMN_MOVIE_ID, movieID);
                movieValues.put(MovieContract.MovieEntry.COLUMN_MOVIE_NUMBER, base);
                movieValues.put(MovieContract.MovieEntry.COLUMN_VOTE_AVERAGE, voteAverage);
                movieValues.put(MovieContract.MovieEntry.COLUMN_TITLE, title);
                movieValues.put(MovieContract.MovieEntry.COLUMN_POSTER_PATH, posterPath);
                movieValues.put(MovieContract.MovieEntry.COLUMN_ORIGINAL_LANGUAGE, originalLanguage);
                movieValues.put(MovieContract.MovieEntry.COLUMN_ORIGINAL_TITLE, originalTitle);
                movieValues.put(MovieContract.MovieEntry.COLUMN_BACKDROP_PATH, backdropPath);
                movieValues.put(MovieContract.MovieEntry.COLUMN_ADULT, adult);
                movieValues.put(MovieContract.MovieEntry.COLUMN_OVERVIEW, overview);
                movieValues.put(MovieContract.MovieEntry.COLUMN_RELEASE_DATE, releaseDate);
                movieValues.put(MovieContract.MovieEntry.COLUMN_MOVIE_KEY, keyMovie);
                movieValues.put(MovieContract.MovieEntry.COLUMN_GENRES, genreIdsArrayString);

                cVVector.add(movieValues);
            }

            //Delete database before load new
//            getContext().getContentResolver().delete(
//                    MovieContract.MovieEntry.CONTENT_URI,
//                    null,
//                    null
//            );

            int inserted = 0;
            // add to database
            if (cVVector.size() > 0) {
                // Student: call bulkInsert to add the weatherEntries to the database here
                ContentValues[] cvArray = new ContentValues[cVVector.size()];
                cVVector.toArray(cvArray);
                inserted = getContext().getContentResolver().bulkInsert(MovieContract.MovieEntry.CONTENT_URI, cvArray);

                notifyMovie();
            }

            Log.d(LOG_TAG, "Sync Complete. " + cVVector.size() + " Inserted");
            Log.d(LOG_TAG, "Sync Complete. " + (mPage-1)*inserted + " Inserted");

        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
            e.printStackTrace();
        }
    }

    private final String getVideo(String movieID) {
        HttpURLConnection urlConnection = null;
        BufferedReader reader = null;

        // Will contain the raw JSON response as a string.
        String videosJsonStr = null;

        try {
            //Movie Rated
            final String MOVIE_BASE = "https://api.themoviedb.org/3/";
            final String APPID_PARAM = "api_key";
            final String MOVIE = "movie";
            final String MOVIE_ID = movieID;
            final String VIDEOS = "videos";
            final String LANGUAGE = "language";
            final String MOVIE_LANGUAGE = mLanguage;

            Uri builtUri = Uri.parse(MOVIE_BASE).buildUpon()
                    .appendPath(MOVIE)
                    .appendPath(MOVIE_ID)
                    .appendPath(VIDEOS)
                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
                    .appendQueryParameter(LANGUAGE, MOVIE_LANGUAGE)
                    .build();

            URL url = new URL(builtUri.toString());

            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + builtUri.toString());

            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.setRequestMethod("GET");
            urlConnection.connect();

            // Read the input stream into a String
            InputStream inputStream = urlConnection.getInputStream();
            StringBuffer buffer = new StringBuffer();
            if (inputStream == null) {
                // Nothing to do.
                return "none";
            }

            reader = new BufferedReader(new InputStreamReader(inputStream));

            String line;
            while ((line = reader.readLine()) != null) {
                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
                // But it does make debugging a *lot* easier if you print out the completed
                // buffer for debugging.
                buffer.append(line + "\n");
            }

            if (buffer.length() == 0) {
                // Stream was empty.  No point in parsing.
                return "none";
            }

            videosJsonStr = buffer.toString();
            return getKeyVideoMovie(videosJsonStr);

        } catch (IOException e) {
            Log.e(LOG_TAG, "Error ", e);
            // If the code didn't successfully get the weather data, there's no point in attempting
            // to parse it.
        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
            e.printStackTrace();
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
            if (reader != null) {
                try {
                    reader.close();
                } catch (final IOException e) {
                    Log.e(LOG_TAG, "Error closing stream", e);
                }
            }
        }
        return "none";
    }

    private String getKeyVideoMovie(String dataJsonStr)
            throws JSONException {

        final String ID = "id";
        final String RESULTS = "results";

        final String KEY = "key";
        final String NAME = "name";
        final String SITE = "site";
        final String SIZE = "size";

        String keyMovie = "none";

        //https://www.youtube.com/watch?v=3VbHg5fqBYw

        try {
            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
            JSONObject videosJson = new JSONObject(dataJsonStr);

//            String id = videosJson.getString(ID);
            JSONArray resultVideosArray = videosJson.getJSONArray(RESULTS);
//            for (int i = 0; i < resultVideosArray.length(); i++) {
//                // These are the values that will be collected.
//                JSONObject currentVideo = resultVideosArray.getJSONObject(i);
//                keyMovie = currentVideo.getString(KEY);
//                Log.d(LOG_TAG, "###### " + i + " keyMovie = " + keyMovie);
//            }

            if (resultVideosArray.length() == 0)
                return "none";

            //Get the first movie
            JSONObject currentVideo = resultVideosArray.getJSONObject(0);
            keyMovie = currentVideo.getString(KEY);
            return keyMovie;
        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
            e.printStackTrace();
        }
        return "none";
    }

    private void notifyMovie() {
        Context context = getContext();
        //checking the last update and notify if it' the first of the day
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

        String displayNotificationsKey = context.getString(R.string.pref_enable_notifications_key);
        boolean displayNotifications = prefs.getBoolean(displayNotificationsKey,
                Boolean.parseBoolean(context.getString(R.string.pref_enable_notifications_default)));

        if ( displayNotifications ) {
            String lastNotificationKey = context.getString(R.string.pref_last_notification);
            long lastSync = prefs.getLong(lastNotificationKey, 0);

            if (System.currentTimeMillis() - lastSync >= DAY_IN_MILLIS) {
                // Last sync was more than 1 day ago, let's send a notification with the weather.
                Uri movieUri = MovieContract.MovieEntry.buildMoviewithID(mMostPopularMovieID);

                // we'll query our contentProvider, as always
                Cursor cursor = context.getContentResolver().query(
                        movieUri,
                        NOTIFY_MOVIE_PROJECTION,
                        null,
                        null,
                        null);

                if (cursor.moveToFirst()) {
                    Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< notifyMovie >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                    //Read the poster of the movie
                    String poster = cursor.getString(COL_BACKDROP_PATH);
                    String imageUri = "https://image.tmdb.org/t/p/w500/" + poster;

                    InputStream in = null;

                    try {
                        URL url = new URL(imageUri);
                        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                        connection.setDoInput(true);
                        connection.connect();
                        in = connection.getInputStream();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    Bitmap myBitmap = BitmapFactory.decodeStream(in);

                    //Read the title of the movie
                    String movieTitle = cursor.getString(COL_TITLE);

                    String voteAverage = cursor.getString(COL_VOTE_AVERAGE);
                    double val = Double.parseDouble(voteAverage);

                    String rate;
                    if (val % 1 == 0){
                        rate = "\u2605" + voteAverage + ".0";
                    }else{
                        rate = "\u2605" + voteAverage;
                    }

                    int iconId = R.drawable.ic_movie;

                    Resources resources = context.getResources();
                    Bitmap largeIcon = BitmapFactory.decodeResource(resources,
                            R.drawable.ic_movie);
                    String titleApp = context.getString(R.string.app_name);
//
                    // Define the text of the forecast.
                    String contentText = "Watch the most popular Movie!!";

//                    String contentText =
                    // NotificationCompatBuilder is a very convenient way to build backward-compatible
                    // notifications.  Just throw in some data.
                    NotificationCompat.Builder mBuilder =
                            new NotificationCompat.Builder(getContext(), String.valueOf(MOVIE_NOTIFICATION_ID))
                                    .setContentTitle(contentText)
                                    .setContentText("\"" + movieTitle + "\"")
                                    .setSubText("Rating:" + rate)
                                    .setColor(resources.getColor(R.color.movie_light_green))
                                    .setSmallIcon(iconId)
                                    .setLargeIcon(largeIcon)
                                    .setStyle(new NotificationCompat.BigPictureStyle()
                                            .bigPicture(myBitmap)
                                            .bigLargeIcon(null))
//                                    .setStyle(new NotificationCompat.BigPictureStyle().bigPicture(myBitmap))
//                                    .setStyle(new NotificationCompat.BigTextStyle().bigText(contentText))
                                    .setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION))
                                    .setVibrate(new long[] {1, 1, 1})
                                    .setDefaults(NotificationCompat.DEFAULT_SOUND)
                                    .setDefaults(NotificationCompat.DEFAULT_ALL);


                    // Make something interesting happen when the user clicks on the notification.
                    // In this case, opening the app is sufficient.
                    Intent resultIntent = new Intent(context, MainActivity.class);

                    // The stack builder object will contain an artificial back stack for the
                    // started Activity.
                    // This ensures that navigating backward from the Activity leads out of
                    // your application to the Home screen.
                    TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
                    stackBuilder.addNextIntent(resultIntent);
                    PendingIntent resultPendingIntent =
                            stackBuilder.getPendingIntent(
                                    0,
                                    PendingIntent.FLAG_UPDATE_CURRENT
                            );
                    mBuilder.setContentIntent(resultPendingIntent);

                    NotificationManager mNotificationManager =
                            (NotificationManager) getContext().getSystemService(Context.NOTIFICATION_SERVICE);
                    // MOVIE_NOTIFICATION_ID allows you to update the notification later on.
                    mNotificationManager.notify(MOVIE_NOTIFICATION_ID, mBuilder.build());

                    //refreshing last sync
                    SharedPreferences.Editor editor = prefs.edit();
                    editor.putLong(lastNotificationKey, System.currentTimeMillis());
                    editor.commit();
                }
                cursor.close();
            }
        }

    }

    private Boolean updateMovieNumber (String movie_ID, long movieNumber) {
        long movieId;
        Boolean valueToReturn = false;

        Cursor movieCursor = getContext().getContentResolver().query(
                MovieContract.MovieEntry.CONTENT_URI,
                null,
                MovieContract.MovieEntry.COLUMN_MOVIE_ID + " = ?",
                new String[]{movie_ID},
                null);

        if (movieCursor.moveToFirst()) {
            int movieIdIndex = movieCursor.getColumnIndex(MovieContract.MovieEntry._ID);
            movieId = movieCursor.getLong(movieIdIndex);

            Log.d(LOG_TAG, "####################updateMovieNumber################ movieId "+ movieId);

            ContentValues movieUpdateValues = new ContentValues();
            for( int i = 1; i <= NOTIFY_MOVIE_PROJECTION.length - 1; i++) {
                String columnName = NOTIFY_MOVIE_PROJECTION[i];
                int idx = movieCursor.getColumnIndex(columnName);
                if (idx != -1){
                    Log.d(LOG_TAG, "----- Column: " + idx + "    Name: " + columnName + "   Value: " + movieCursor.getString(idx));
                    movieUpdateValues.put( NOTIFY_MOVIE_PROJECTION[i], movieCursor.getString(idx));
                }
            }
            //Update Only the ItemNumber
            movieUpdateValues.put(NOTIFY_MOVIE_PROJECTION[2], String.valueOf(movieNumber));

            //Update Values
            getContext().getContentResolver().update(
                    MovieContract.MovieEntry.CONTENT_URI,
                    movieUpdateValues,
                    MovieContract.MovieEntry.COLUMN_MOVIE_ID + "= ?",
                    new String[] {movie_ID});

            valueToReturn = true;
        }

        Log.d(LOG_TAG, "####################newMovieId################");
        movieCursor.close();

        // Wait, that worked?  Yes!
        return valueToReturn;
    }

    /**
    * Helper method to schedule the sync adapter periodic execution
    */
    public static void configurePeriodicSync(Context context, int syncInterval, int flexTime) {
        Account account = getSyncAccount(context);
        String authority = context.getString(R.string.content_authority);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            // we can enable inexact timers in our periodic sync
            SyncRequest request = new SyncRequest.Builder().
            syncPeriodic(syncInterval, flexTime).
            setSyncAdapter(account, authority).
            setExtras(new Bundle()).build();
                ContentResolver.requestSync(request);
        } else {
            ContentResolver.addPeriodicSync(account,
            authority, new Bundle(), syncInterval);
        }
    }

    /**
     * Helper method to have the sync adapter sync immediately
     * @param context The context used to access the account service
     */
    public static void syncImmediately(Context context) {
        Bundle bundle = new Bundle();
        bundle.putBoolean(ContentResolver.SYNC_EXTRAS_EXPEDITED, true);
        bundle.putBoolean(ContentResolver.SYNC_EXTRAS_MANUAL, true);
        ContentResolver.requestSync(getSyncAccount(context),
                context.getString(R.string.content_authority), bundle);
    }

    /**
     * Helper method to get the fake account to be used with SyncAdapter, or make a new one
     * if the fake account doesn't exist yet.  If we make a new account, we call the
     * onAccountCreated method so we can initialize things.
     *
     * @param context The context used to access the account service
     * @return a fake account.
     */
    public static Account getSyncAccount(Context context) {
        // Get an instance of the Android account manager
        AccountManager accountManager =
                (AccountManager) context.getSystemService(Context.ACCOUNT_SERVICE);

        // Create the account type and default account
        Account newAccount = new Account(
                context.getString(R.string.app_name), context.getString(R.string.sync_account_type));

        // If the password doesn't exist, the account doesn't exist
        if ( null == accountManager.getPassword(newAccount) ) {

        /*
         * Add the account and account type, no password or user data
         * If successful, return the Account object, otherwise report an error.
         */
            if (!accountManager.addAccountExplicitly(newAccount, "", null)) {
                return null;
            }
            /*
             * If you don't set android:syncable="true" in
             * in your <provider> element in the manifest,
             * then call ContentResolver.setIsSyncable(account, AUTHORITY, 1)
             * here.
             */
            onAccountCreated(newAccount, context);
        }
        return newAccount;
    }

    private static void onAccountCreated(Account newAccount, Context context) {
        /*
        * Since we've created an account
        */
        TheMovieAppSyncAdapter.configurePeriodicSync(context, SYNC_INTERVAL, SYNC_FLEXTIME);

        /*
         * Without calling setSyncAutomatically, our periodic sync will not be enabled.
         */
        ContentResolver.setSyncAutomatically(newAccount, context.getString(R.string.content_authority), true);

        /*
         * Finally, let's do a sync to get things started
         */
//        syncImmediately(context);
    }

    public static void initializeSyncAdapter(Context context) {
        getSyncAccount(context);
    }
}