/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example.android.themovieapp.app;

import android.arch.lifecycle.Lifecycle;
import android.content.Intent;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.StrictMode;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.CursorLoader;
import android.support.v4.content.Loader;
import android.support.v4.view.MenuItemCompat;
import android.support.v7.widget.ShareActionProvider;
import android.text.Html;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.android.themovieapp.app.data.MovieContract;
import com.example.android.themovieapp.app.data.MovieContract.MovieEntry;
import com.example.android.themovieapp.app.sync.TheMovieAppSyncAdapter;
import com.google.android.youtube.player.YouTubeInitializationResult;
import com.google.android.youtube.player.YouTubePlayer;
import com.google.android.youtube.player.YouTubePlayerSupportFragment;
import com.squareup.picasso.MemoryPolicy;
import com.squareup.picasso.NetworkPolicy;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import java.io.File;
import java.io.FileOutputStream;
import java.text.DateFormat;
import java.util.Date;
import java.util.Locale;

/**
* A placeholder fragment containing a simple view.
*/
public class DetailFragment extends Fragment implements LoaderManager.LoaderCallbacks<Cursor> {

    private static final String LOG_TAG = DetailFragment.class.getSimpleName();
    static final String DETAIL_URI = "URI";

    private static final String MOVIE_SHARE_HASHTAG = "#TheMovieApp";

    private ShareActionProvider mShareActionProvider;
    private static boolean mIsShareActionProviderCreated = false;
    private static boolean mIsLandscape = false;
    private String mMovie;
    private Bitmap mBitmap;
    private Uri mUri;
    private String mMovieKey;


    private static final int DETAIL_LOADER = 0;

    private static final String[] DETAIL_COLUMNS = {
            MovieEntry.TABLE_NAME + "." + MovieContract.MovieEntry._ID,
            MovieEntry.COLUMN_MOVIE_ID,
            MovieEntry.COLUMN_MOVIE_NUMBER,
            MovieEntry.COLUMN_VOTE_AVERAGE,
            MovieEntry.COLUMN_TITLE,
            MovieEntry.COLUMN_POSTER_PATH,
            MovieEntry.COLUMN_ORIGINAL_LANGUAGE,
            MovieEntry.COLUMN_ORIGINAL_TITLE,
            MovieEntry.COLUMN_BACKDROP_PATH,
            MovieEntry.COLUMN_ADULT,
            MovieEntry.COLUMN_OVERVIEW,
            MovieEntry.COLUMN_RELEASE_DATE,
            MovieEntry.COLUMN_MOVIE_KEY,
            MovieEntry.COLUMN_GENRES
    };

    // These indices are tied to DETAIL_COLUMNS.  If DETAIL_COLUMNS changes, these
    // must change.
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
    static final int COL_GENRES = 13;

    private ImageView mPosterView;
    private TextView mTitleView;
    private TextView mYear;
    private TextView mVoteAverage;
    private TextView mOriginalLanguage;
    private TextView mReleaseDate;
    private TextView mTitleOverview;
    private TextView mOverviewView;
    private TextView mGenresView;
    private TextView mTitleTrailer;
    private YouTubePlayerSupportFragment mYouTubePlayerFragment;
    private ProgressBar mProgressBar;
    private ImageView mBackgroundImage;

    public DetailFragment() {
        setHasOptionsMenu(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {

            int orientation = getResources().getConfiguration().orientation;
            if (orientation == Configuration.ORIENTATION_LANDSCAPE) {
                // In landscape
                Log.v(LOG_TAG, "In onCreateView -- landscape");
                mIsLandscape = true;
            } else {
                // In portrait
                Log.v(LOG_TAG, "In onCreateView -- portrait");
                mIsLandscape = false;
            }


            Bundle arguments = getArguments();
            if (arguments != null) {
                mUri = arguments.getParcelable(DetailFragment.DETAIL_URI);
            }

            View rootView = inflater.inflate(R.layout.fragment_detail, container, false);
            mPosterView = (ImageView) rootView.findViewById(R.id.detail_poster_image);
            mBackgroundImage = (ImageView) rootView.findViewById(R.id.detail_backgroundImage);
            mTitleView = (TextView) rootView.findViewById(R.id.detail_title_textview);
            mYear = (TextView) rootView.findViewById(R.id.detail_year_textview);
            mVoteAverage = (TextView) rootView.findViewById(R.id.detail_vote_average_textview);
            mOriginalLanguage = (TextView) rootView.findViewById(R.id.detail_original_language_textview);
            mReleaseDate = (TextView) rootView.findViewById(R.id.detail_release_date_textview);
            mTitleOverview = (TextView) rootView.findViewById(R.id.detail_title_overview_textview);
            mOverviewView = (TextView) rootView.findViewById(R.id.detail_overview_textview);
            mGenresView = (TextView) rootView.findViewById(R.id.detail_genres);
            mProgressBar = (ProgressBar) rootView.findViewById(R.id.loadingPanel);
            mTitleTrailer = (TextView) rootView.findViewById(R.id.detail_title_trailer_textview);

            mPosterView.setVisibility(View.GONE);

            mYouTubePlayerFragment = YouTubePlayerSupportFragment.newInstance();
            FragmentTransaction transaction = getFragmentManager().beginTransaction();
            transaction.replace(R.id.youtube_fragment, mYouTubePlayerFragment);
            transaction.commit();

        return rootView;
        }

    @Override
    public Lifecycle getLifecycle() {
        return super.getLifecycle();
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        // Inflate the menu; this adds items to the action bar if it is present.
        inflater.inflate(R.menu.detailfragment, menu);

        MenuItem menuItem = menu.findItem(R.id.action_share);

        // Get the provider and hold onto it to set/change the share intent.
        mShareActionProvider = (ShareActionProvider) MenuItemCompat.getActionProvider(menuItem);


        // mShareActionProvider will be created when the las value is ready
        // If onLoadFinished happens before this, we can go ahead and set the share intent now.
//        if (mBitmap != null) {
//            mShareActionProvider.setShareIntent(createShareMovieIntent());
//        }
    }

    private Intent createShareMovieIntent(){
        try {
            File file = new File(getContext().getExternalCacheDir(), "posterMovie.png");
            FileOutputStream fOut = new FileOutputStream(file);
            mBitmap.compress(Bitmap.CompressFormat.PNG, 100,fOut);
            fOut.flush();
            fOut.close();
            file.setReadable(true, false);

            StrictMode.VmPolicy.Builder builder = new StrictMode.VmPolicy.Builder();
            StrictMode.setVmPolicy(builder.build());

            final Intent shareIntent = new Intent(Intent.ACTION_SEND);
            shareIntent.setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            shareIntent.putExtra(Intent.EXTRA_TEXT, mMovie);
            shareIntent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(file));
            shareIntent.setType("image/png");
            return shareIntent;
        } catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        getLoaderManager().initLoader(DETAIL_LOADER, null, this);
        super.onActivityCreated(savedInstanceState);
    }

    @Override
    public Loader<Cursor> onCreateLoader(int id, Bundle args) {

        if ( null != mUri ) {
            // Now create and return a CursorLoader that will take care of
            // creating a Cursor for the data being displayed.
            return new CursorLoader(
                getActivity(),
                mUri,
                DETAIL_COLUMNS,
                null,
                null,
                null
            );
        }
        return null;
    }

    @Override
    public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
        Log.v(LOG_TAG, "In onLoadFinished --");
        if (data != null && data.moveToFirst()) {
            //Read the poster of the movie
            String poster = data.getString(COL_POSTER_PATH);
//            "w92", "w154", "w185", "w342", "w500", "w780", "original"
            String imageUri = "https://image.tmdb.org/t/p/w500" + poster;
            Picasso pImage = Picasso.with(getContext());
            pImage.load(imageUri)
                    .into(mTarget);

            //Read the poster of the movie
            String backPoster;
            if (mIsLandscape) {
                backPoster = data.getString(COL_BACKDROP_PATH);
            } else {
                backPoster = data.getString(COL_POSTER_PATH);
            }
//                String imageUri = "https://image.tmdb.org/t/p/w500/" + poster;
//                "w300", "w780", "w1280", "original"
            String imageBackUri = "https://image.tmdb.org/t/p/original" + backPoster;
            Picasso pBackgroundImage = Picasso.with(getContext());
            pBackgroundImage.load(imageBackUri)
                    .into(mBackgroundTarget);

            //Read the title of the movie
            String title = data.getString(COL_TITLE);
            mTitleView.setText(title);

            String voteAverage = data.getString(COL_VOTE_AVERAGE);
            double val = Double.parseDouble(voteAverage);

            String rate;
            if (val % 1 == 0){
                rate = "\u2605" + voteAverage + ".0";
            }else{
                rate = "\u2605" + voteAverage;
            }
            mVoteAverage.setText(rate);

            String genres = data.getString(COL_GENRES);
            String genresNames = Utility.getGenres(getContext(), genres, TheMovieAppSyncAdapter.mLanguage);
            mGenresView.setText(Html.fromHtml(
                    genresNames));

            //Build the Language
            String originalLanguage = data.getString(COL_ORIGINAL_LANGUAGE);
            Locale newLocale = new Locale(originalLanguage);
            String str = newLocale.getDisplayLanguage();
            String currentLanguage = str.substring(0, 1).toUpperCase() + str.substring(1);
            String languageText = getContext().getString(R.string.pref_language_label);
            mOriginalLanguage.setText(Html.fromHtml(
                    "<b>" + languageText +": </b>" + currentLanguage));

            String releaseDate = data.getString(COL_RELEASE_DATE);
            String[] parts = releaseDate.split("-");
            String year = parts[0];
            String month = parts[1];
            String day = parts[2];

            Date date = new Date(Integer.parseInt(year) - 1900,
                    Integer.parseInt(month),
                    Integer.parseInt(day));
            DateFormat dateFormat = android.text.format.DateFormat.getMediumDateFormat(getContext());

            String releasedText = getContext().getString(R.string.pref_released);
            mReleaseDate.setText(Html.fromHtml(
                    "<b>" + releasedText + ": </b>" + dateFormat.format(date)));
            mYear.setText(year);

            String overview = data.getString(COL_OVERVIEW);
//            mOverviewView.setJustificationMode(Layout.JUSTIFICATION_MODE_INTER_WORD);
            mOverviewView.setText(overview );

            if (overview == null || overview.isEmpty()) {
                mTitleOverview.setVisibility(View.GONE);
            }

            //https://www.youtube.com/watch?v=3VbHg5fqBYw
            mMovieKey = data.getString(COL_MOVIE_KEY);
            String videoUri = "https://www.youtube.com/watch?v=" + mMovieKey;

            String youText = getContext().getString(R.string.pref_you);
            String ratingText = getContext().getString(R.string.pref_rating);
            String watchText = getContext().getString(R.string.pref_watch);

            // We still need this for the share intent
            mMovie = MOVIE_SHARE_HASHTAG + "\n" +
                    youText + "\n *\"" + title + "\"*" + "\n" +
                    "*" + releasedText + "*: " + dateFormat.format(date) + "\n" +
                    "*" + ratingText + "*: " + rate + "\n" +
                    watchText + "\n" + videoUri;

            //Do not display You Tube if there is not video
            if (mMovieKey.equals("none")) {
                mTitleTrailer.setVisibility(View.GONE);
                mYouTubePlayerFragment.getView().setVisibility(View.GONE);
                return;
            }
            mYouTubePlayerFragment.setRetainInstance(true);
            mYouTubePlayerFragment.initialize(BuildConfig.YOUTUBE_API_KEY, new YouTubePlayer.OnInitializedListener() {
                @Override
                public void onInitializationSuccess(YouTubePlayer.Provider arg0, YouTubePlayer youTubePlayer, boolean b) {
                    if (!b) {
                        youTubePlayer.setFullscreen(false);
                        youTubePlayer.loadVideo(mMovieKey);
                        youTubePlayer.play();
                    }
                }

                @Override
                public void onInitializationFailure(YouTubePlayer.Provider arg0, YouTubeInitializationResult arg1) {
                    // TODO Auto-generated method stub
                }
            });
        }
    }

    private Target mTarget = new Target() {
        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            mBitmap = bitmap;
            if (mBitmap != null && !mIsShareActionProviderCreated) {
                mShareActionProvider.setShareIntent(createShareMovieIntent());
                mIsShareActionProviderCreated = true;
            }
            //Set it in the ImageView
            mPosterView.setImageBitmap(bitmap);
            mProgressBar.setVisibility(View.GONE);
            mPosterView.setVisibility(View.VISIBLE);
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    };

    private Target mBackgroundTarget = new Target() {
        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            Drawable drawable = new BitmapDrawable(getResources(), bitmap);
            drawable.setAlpha(60);
            mBackgroundImage.setImageDrawable(drawable);
            Log.v(LOG_TAG, "Loaded");
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    };

    @Override
    public void onLoaderReset(Loader<Cursor> loader) { }
}