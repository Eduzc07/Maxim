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

import android.content.Intent;
import android.net.Uri;
import android.database.Cursor;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.CursorLoader;
import android.support.v4.content.Loader;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.Toast;

import com.example.android.themovieapp.app.data.MovieContract;
import com.example.android.themovieapp.app.sync.TheMovieAppSyncAdapter;

/**
 * Encapsulates fetching the forecast and displaying it as a {@link ListView} layout.
 */
//public class MovieFragment extends Fragment
public class MovieFragment extends Fragment implements LoaderManager.LoaderCallbacks<Cursor> {

    public static final String LOG_TAG = MovieFragment.class.getSimpleName();
    private int mPosition = ListView.INVALID_POSITION;
    private boolean mUseTodayLayout;

    private static final String SELECTED_KEY = "selected_position";

    private static final int MOVIE_LOADER = 0;

    private static final String KEY_LAYOUT_MANAGER = "layoutManager";
    private static final int SPAN_COUNT = 3; //Number of columns in GridLayoutManager

    private enum LayoutManagerType {
        GRID_LAYOUT_MANAGER,
        LINEAR_LAYOUT_MANAGER
    }
    protected LayoutManagerType mCurrentLayoutManagerType;

    protected RecyclerView mRecyclerView;
    private MovieAdapter mMovieAdapter;
    protected RecyclerView.LayoutManager mLayoutManager;


    // Specify the columns we need.
    private static final String[] MOVIE_COLUMNS = new String[] {
            // In this case the id needs to be fully qualified with a table name, since
            // the content provider joins the location & weather tables in the background
            // (both have an _id column)
            // On the one hand, that's annoying.  On the other, you can search the weather table
            // using the location set by the user, which is only in the Location table.
            // So the convenience is worth it.
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

    /**
    * A callback interface that all activities containing this fragment must
    * implement. This mechanism allows activities to be notified of item
    * selections.
    */
    public interface Callback {
        /**
        * DetailFragmentCallback for when an item has been selected.
        */
        public void onItemSelected(Uri dateUri);
    }

    public MovieFragment() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Add this line in order for this fragment to handle menu events.
        setHasOptionsMenu(true);

        //Add the name of the current Menu
        String title = getResources().getString(R.string.app_name);
        getActivity().setTitle(title + " - " +
                TheMovieAppSyncAdapter.mMovieQuery.substring(0, 1).toUpperCase() +
                TheMovieAppSyncAdapter.mMovieQuery.substring(1).toLowerCase());
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.moviefragment, menu);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        getLoaderManager().initLoader(MOVIE_LOADER, null, this);
        super.onActivityCreated(savedInstanceState);
    }

    private void updateMovies(){
        TheMovieAppSyncAdapter.syncImmediately(getActivity());
        getLoaderManager().restartLoader(MOVIE_LOADER, null, this);
    }

    // since we read the location when we create the loader, all we need to do is restart things
    void onLanguageChanged( String newLanguage ) {
        TheMovieAppSyncAdapter.mPage = 1;
        TheMovieAppSyncAdapter.mLanguage = newLanguage;
        //Delete database before load new
        getContext().getContentResolver().delete(
            MovieContract.MovieEntry.CONTENT_URI,
            null,
            null
        );
        updateMovies();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        String title = getString(R.string.app_name);
        String secondName = "";

        if (id == R.id.action_now_playing) {
            TheMovieAppSyncAdapter.mPage = 1;
            TheMovieAppSyncAdapter.mMovieQuery = "now_playing";
            secondName = getString(R.string.action_now_playing);
        }

        if (id == R.id.action_popular) {
            TheMovieAppSyncAdapter.mPage = 1;
            TheMovieAppSyncAdapter.mMovieQuery = "popular";
            secondName = getString(R.string.action_popular);
        }

        if (id == R.id.action_top_rated) {
            TheMovieAppSyncAdapter.mPage = 1;
            TheMovieAppSyncAdapter.mMovieQuery = "top_rated";
            secondName = getString(R.string.action_top_rated);
        }

        if (id == R.id.action_upcoming) {
            TheMovieAppSyncAdapter.mPage = 1;
            TheMovieAppSyncAdapter.mMovieQuery = "upcoming";
            secondName = getString(R.string.action_upcoming);
        }
        getActivity().setTitle(title + " - " + secondName);
        updateMovies();

        return super.onOptionsItemSelected(item);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_main, container, false);

        // BEGIN_INCLUDE(initializeRecyclerView)
        mRecyclerView = (RecyclerView) rootView.findViewById(R.id.my_recycler_view);
        mRecyclerView.setHasFixedSize(true);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getContext()));

        // LinearLayoutManager is used here, this will layout the elements in a similar fashion
        // to the way ListView would layout elements. The RecyclerView.LayoutManager defines how
        // elements are laid out.
        mLayoutManager = new LinearLayoutManager(getContext());
        mCurrentLayoutManagerType = LayoutManagerType.GRID_LAYOUT_MANAGER;
        if (savedInstanceState != null) {
            // Restore saved layout manager type.
            mCurrentLayoutManagerType = (LayoutManagerType) savedInstanceState
                    .getSerializable(KEY_LAYOUT_MANAGER);
        }
        setRecyclerViewLayoutManager(mCurrentLayoutManagerType);

        mMovieAdapter = new MovieAdapter(getActivity(), null);
        // Set CustomAdapter as the adapter for RecyclerView.
        mRecyclerView.setAdapter(mMovieAdapter);
        // END_INCLUDE(initializeRecyclerView)

        mMovieAdapter.setOnItemClickListener(new MovieAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(Cursor cursor, int position) {
//                String keyVideo = TheMovieAppSyncAdapter.getVideo(cursor.getString(COL_MOVIE_ID));
//                Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + cursor.getString(COL_MOVIE_ID));
//                Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + keyVideo);

//                AsyncMovieTask getVideoTask = new AsyncMovieTask(getContext());
//                getVideoTask.execute(cursor.getString(COL_MOVIE_ID), TheMovieAppSyncAdapter.mLanguage);

                if (cursor != null) {
                    ((Callback) getActivity())
                            .onItemSelected(MovieContract.MovieEntry.buildMoviewithID(cursor.getString(COL_MOVIE_ID)));
                }
                mPosition = position;
            }
        });

        // If there's instance state, mine it for useful information.
        // The end-goal here is that the user never knows that turning their device sideways
        // does crazy lifecycle related things.  It should feel like some stuff stretched out,
        // or magically appeared to take advantage of room, but data or place in the app was never
        // actually *lost*.
        if (savedInstanceState != null && savedInstanceState.containsKey(SELECTED_KEY)) {
        // The listview probably hasn't even been populated yet.  Actually perform the
        // swapout in onLoadFinished.
            mPosition = savedInstanceState.getInt(SELECTED_KEY);
        }

//        mForecastAdapter.setUseTodayLayout(mUseTodayLayout);

        return rootView;
    }

    /**
     * Set RecyclerView's LayoutManager to the one given.
     *
     * @param layoutManagerType Type of layout manager to switch to.
     */
    public void setRecyclerViewLayoutManager(LayoutManagerType layoutManagerType) {
        int scrollPosition = 0;

        // If a layout manager has already been set, get current scroll position.
        if (mRecyclerView.getLayoutManager() != null) {
            scrollPosition = ((LinearLayoutManager) mRecyclerView.getLayoutManager())
                    .findFirstCompletelyVisibleItemPosition();
        }

        switch (layoutManagerType) {
            case GRID_LAYOUT_MANAGER:
                mLayoutManager = new GridLayoutManager(getActivity(), SPAN_COUNT);
                mCurrentLayoutManagerType = LayoutManagerType.GRID_LAYOUT_MANAGER;
                break;
            case LINEAR_LAYOUT_MANAGER:
                mLayoutManager = new LinearLayoutManager(getActivity());
                mCurrentLayoutManagerType = LayoutManagerType.LINEAR_LAYOUT_MANAGER;
                break;
            default:
                mLayoutManager = new LinearLayoutManager(getActivity());
                mCurrentLayoutManagerType = LayoutManagerType.LINEAR_LAYOUT_MANAGER;
        }

        mRecyclerView.setLayoutManager(mLayoutManager);
        mRecyclerView.scrollToPosition(scrollPosition);
    }

        @Override
    public void onSaveInstanceState(Bundle outState) {
        // When tablets rotate, the currently selected list item needs to be saved.
        // When no item is selected, mPosition will be set to Listview.INVALID_POSITION,
        // so check for that before storing.

        // Save currently selected layout manager.
        outState.putSerializable(KEY_LAYOUT_MANAGER, mCurrentLayoutManagerType);
        super.onSaveInstanceState(outState);
    }

    @Override
    public Loader<Cursor> onCreateLoader(int i, Bundle bundle) {
//          String locationSetting = Utility.getPreferredLocation(getActivity());
//          This is called when a new Loader needs to be created.  This
//          fragment only uses one loader, so we don't care about checking the id.
//          To only show current and future dates, filter the query to return weather only for
//          dates after or including today.


        Uri movieUri = MovieContract.MovieEntry.CONTENT_URI;

        long minVal = 0;
        long maxVal = 0;
        if (TheMovieAppSyncAdapter.mMovieQuery == "now_playing"){
            minVal = 0;
            maxVal = 1000;
        }

        if (TheMovieAppSyncAdapter.mMovieQuery == "popular"){
            minVal = 1000;
            maxVal = 2000;
        }
        if (TheMovieAppSyncAdapter.mMovieQuery == "top_rated"){
            minVal = 2000;
            maxVal = 3000;
        }
        if (TheMovieAppSyncAdapter.mMovieQuery == "upcoming"){
            minVal = 3000;
            maxVal = 4000;
        }

        String selection;
        selection = MovieContract.MovieEntry.COLUMN_MOVIE_NUMBER + " >= " + String.valueOf(minVal) +
        " AND " + MovieContract.MovieEntry.COLUMN_MOVIE_NUMBER + " < " + String.valueOf(maxVal);

        String sortOrder = MovieContract.MovieEntry.COLUMN_MOVIE_NUMBER + " ASC";

        return new CursorLoader(getContext(),
                movieUri,
                MOVIE_COLUMNS,
                selection,
                null,
                sortOrder);
    }

    @Override
    public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
        mMovieAdapter.swapCursor(data);
//        if (mPosition != ListView.INVALID_POSITION) {
//            // If we don't need to restart the loader, and there's a desired position to restore
//            // to, do so now.
//            mListView.smoothScrollToPosition(mPosition);
//        }
    }

    @Override
    public void onLoaderReset(Loader<Cursor> loader) {
        mMovieAdapter.swapCursor(null);
    }

    public void setUseTodayLayout(boolean useTodayLayout) {
        mUseTodayLayout = useTodayLayout;
//        if (mForecastAdapter != null) {
//            mForecastAdapter.setUseTodayLayout(mUseTodayLayout);
//        }
    }
}


