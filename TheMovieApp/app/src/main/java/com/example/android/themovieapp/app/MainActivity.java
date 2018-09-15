package com.example.android.themovieapp.app;
import android.content.Intent;

import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import com.example.android.themovieapp.app.sync.TheMovieAppSyncAdapter;

import java.util.Locale;

public class MainActivity extends AppCompatActivity implements MovieFragment.Callback {

    private final String LOG_TAG = MainActivity.class.getSimpleName();
    private final String MOVIEFRAGMENT_TAG = "FFTAG";
    private static final String DETAILFRAGMENT_TAG = "DFTAG";

    private boolean mTwoPane;
    private String mLanguage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mLanguage = Locale.getDefault().toString().replace("_", "-");

        setContentView(R.layout.activity_main);

        if (findViewById(R.id.movie_detail_container) != null) {
            // The detail container view will be present only in the large-screen layouts
            // (res/layout-sw600dp). If this view is present, then the activity should be
            // in two-pane mode.
            mTwoPane = true;
            // In two-pane mode, show the detail view in this activity by
            // adding or replacing the detail fragment using a
            // fragment transaction.
            if (savedInstanceState == null) {
                getSupportFragmentManager().beginTransaction()
                .replace(R.id.movie_detail_container, new DetailFragment(), DETAILFRAGMENT_TAG)
                .commit();
            }
        } else {
            mTwoPane = false;
            getSupportActionBar().setElevation(0f);
        }

        //Display Icon in ActioBar
//        getSupportActionBar().setDisplayShowHomeEnabled(true);
//        getSupportActionBar().setIcon(R.mipmap.ic_launcher);

//        MovieFragment movieFragment =  ((MovieFragment)getSupportFragmentManager()
//                .findFragmentById(R.id.fragment_list_movies));

        //Open the current movie displayed in Notification
        Intent intent = getIntent();
        String message = intent.getStringExtra(this.getString(R.string.intent_movie));
        if (message != null) {
            (this).onItemSelected(Uri.parse(message));
        }

        loadLastMovieType();

        TheMovieAppSyncAdapter.initializeSyncAdapter(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            Intent intent = new Intent(this, SettingsActivity.class);
            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onResume() {
        super.onResume();
        String language = Utility.getPreferredLanguage( this );
        // update the location in our second pane using the fragment manager
        if (language != null && !language.equals(mLanguage) && !language.equals("-1")) {
            MovieFragment ff = (MovieFragment)getSupportFragmentManager().findFragmentById(R.id.fragment_list_movies);
            if ( null != ff ) {
                ff.onLanguageChanged(language);
            }
            mLanguage = language;
        }

        Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>----- onResume" );
    }

    @Override
    public void onItemSelected(Uri contentUri) {
        if (mTwoPane) {
            // In two-pane mode, show the detail view in this activity by
            // adding or replacing the detail fragment using a
            // fragment transaction.
            Bundle args = new Bundle();
                args.putParcelable(DetailFragment.DETAIL_URI, contentUri);

            DetailFragment fragment = new DetailFragment();
                fragment.setArguments(args);

            getSupportFragmentManager().beginTransaction()
                .replace(R.id.movie_detail_container, fragment, DETAILFRAGMENT_TAG)
                .commit();
        } else {
            Intent intent = new Intent(this, DetailActivity.class)
                    .setData(contentUri);
            startActivity(intent);
        }
    }

    private void loadLastMovieType() {
        //Read the Last Movie Option
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        int id = prefs.getInt(getString(R.string.pref_Movie_Type), R.id.action_popular );

        String title = getString(R.string.app_name);
        String secondName = "";

        switch (id) {
            case R.id.action_now_playing:
                TheMovieAppSyncAdapter.mMovieQuery = "now_playing";
                secondName = " - " + getString(R.string.action_now_playing);
                break;

            case R.id.action_popular:
                TheMovieAppSyncAdapter.mMovieQuery = "popular";
                secondName = " - " + getString(R.string.action_popular);
                break;

            case R.id.action_top_rated:
                TheMovieAppSyncAdapter.mMovieQuery = "top_rated";
                secondName = " - " + getString(R.string.action_top_rated);
                break;

            case R.id.action_upcoming:
                TheMovieAppSyncAdapter.mMovieQuery = "upcoming";
                secondName = " - " + getString(R.string.action_upcoming);
                break;
        }

        TheMovieAppSyncAdapter.mPage = 1;
        TheMovieAppSyncAdapter.mLanguage = mLanguage;

        MovieFragment movieFragment =  ((MovieFragment)getSupportFragmentManager()
                .findFragmentById(R.id.fragment_list_movies));

        movieFragment.mCurrentSelection = id;
        setTitle(title + secondName);
    }
}