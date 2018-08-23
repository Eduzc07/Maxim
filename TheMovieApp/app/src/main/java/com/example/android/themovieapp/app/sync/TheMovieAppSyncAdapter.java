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
import com.example.android.themovieapp.app.Utility;
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
import java.util.Vector;

public class TheMovieAppSyncAdapter extends AbstractThreadedSyncAdapter {
    public final String LOG_TAG = TheMovieAppSyncAdapter.class.getSimpleName();
    // Interval at which to sync with the MovieDB, in seconds.
    // 60 seconds (1 minute) * 60 minutes (1 hour) * 24 hours = 1 day
    public static final int SYNC_INTERVAL = 60 * 60 * 24;
    public static final int SYNC_FLEXTIME = SYNC_INTERVAL/3;

    public static String mMovieQuery = "popular";


    private static final String[] NOTIFY_MOVIE_PROJECTION = new String[] {
            MovieContract.MovieEntry.COLUMN_TITLE,
            MovieContract.MovieEntry.COLUMN_POSTER
    };


    // these indices must match the projection
    private static final int INDEX_MOVIE_ID = 0;
    private static final int INDEX_POSTER = 1;
    private static final int INDEX_TITTLE = 2;

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
            final String MOVIE_BASE =
                    "https://api.themoviedb.org/3/";

            final String APPID_PARAM = "api_key";
            final String MOVIE = "movie";
            final String QUERY = mMovieQuery; //top_rated //upcoming //popular
            final String LANGUAGE = "Language";
            final String PAGE = "page";
            final String MOVIE_LANGUAGE = "en-US";

            Uri builtUri = Uri.parse(MOVIE_BASE).buildUpon()
                    .appendPath(MOVIE)
                    .appendPath(QUERY)
                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
                    .appendQueryParameter(LANGUAGE, MOVIE_LANGUAGE)
                    .appendQueryParameter(PAGE, "1")
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

        try {
            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
            JSONObject moviesJson = new JSONObject(dataJsonStr);

            int page = moviesJson.getInt(PAGES);
            int results = moviesJson.getInt(TOTAL_RESULTS);
            int pages = moviesJson.getInt(TOTAL_PAGES);

            JSONArray resultMoviesArray = moviesJson.getJSONArray(RESULTS);
            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + page);
            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + results );
            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + pages );

            // Insert the new weather information into the database
            Vector<ContentValues> cVVector = new Vector<ContentValues>(resultMoviesArray.length());

            for(int i = 0; i < resultMoviesArray.length(); i++) {
                // These are the values that will be collected.
                String movieID;
                String title;
                String poster;
                String overview;

                JSONObject currentMovie = resultMoviesArray.getJSONObject(i);
                movieID = currentMovie.getString("id");
                title = currentMovie.getString("title");
                poster = currentMovie.getString("poster_path");
                overview = currentMovie.getString("overview");

                Log.d(LOG_TAG, "###### " + i + " = " + movieID);
                Log.d(LOG_TAG, "###### " + i + " = " + title);
                Log.d(LOG_TAG, "###### " + i + " = " + poster);
                Log.d(LOG_TAG, " ------------------------");

                ContentValues movieValues = new ContentValues();

                movieValues.put(MovieContract.MovieEntry.COLUMN_MOVIE_ID, movieID);
                movieValues.put(MovieContract.MovieEntry.COLUMN_TITLE, title);
                movieValues.put(MovieContract.MovieEntry.COLUMN_POSTER, poster);
                movieValues.put(MovieContract.MovieEntry.COLUMN_OVERVIEW, overview);

                cVVector.add(movieValues);
            }

            //Delete database before load new
            getContext().getContentResolver().delete(
                    MovieContract.MovieEntry.CONTENT_URI,
                    null,
                    null
            );

            int inserted = 0;
            // add to database
            if ( cVVector.size() > 0 ) {
                // Student: call bulkInsert to add the weatherEntries to the database here
                ContentValues[] cvArray = new ContentValues[cVVector.size()];
                cVVector.toArray(cvArray);
                inserted = getContext().getContentResolver().bulkInsert(MovieContract.MovieEntry.CONTENT_URI, cvArray);
            }

            Log.d(LOG_TAG, "Sync Complete. " + cVVector.size() + " Inserted");
            Log.d(LOG_TAG, "Sync Complete. " + inserted + " Inserted");

        } catch (JSONException e) {
            Log.e(LOG_TAG, e.getMessage(), e);
            e.printStackTrace();
        }
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
        syncImmediately(context);
    }

    public static void initializeSyncAdapter(Context context) {
        getSyncAccount(context);
    }
}