package com.example.android.themovieapp.app;
import android.content.Intent;

import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;

import com.example.android.themovieapp.app.sync.TheMovieAppSyncAdapter;

public class MainActivity extends AppCompatActivity implements MovieFragment.Callback {

    private final String LOG_TAG = MainActivity.class.getSimpleName();
    private final String FORECASTFRAGMENT_TAG = "FFTAG";
    private static final String DETAILFRAGMENT_TAG = "DFTAG";

    private boolean mTwoPane;
    private String mLocation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mLocation = Utility.getPreferredLocation(this);

        setContentView(R.layout.activity_main);

        if (findViewById(R.id.weather_detail_container) != null) {
            // The detail container view will be present only in the large-screen layouts
            // (res/layout-sw600dp). If this view is present, then the activity should be
            // in two-pane mode.
            mTwoPane = true;
            // In two-pane mode, show the detail view in this activity by
            // adding or replacing the detail fragment using a
            // fragment transaction.
            if (savedInstanceState == null) {
                getSupportFragmentManager().beginTransaction()
                .replace(R.id.weather_detail_container, new DetailFragment(), DETAILFRAGMENT_TAG)
                .commit();
            }
        } else {
            mTwoPane = false;
            getSupportActionBar().setElevation(0f);
        }

        //Display Icon in ActioBar
//        getSupportActionBar().setDisplayShowHomeEnabled(true);
//        getSupportActionBar().setIcon(R.mipmap.ic_launcher);

        MovieFragment movieFragment =  ((MovieFragment)getSupportFragmentManager()
                .findFragmentById(R.id.fragment_forecast));
        movieFragment.setUseTodayLayout(!mTwoPane);

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
//
//        if (id == R.id.action_map) {
//            openPreferredLocationInMap();
//            return true;
//        }

        return super.onOptionsItemSelected(item);
    }

//    private void openPreferredLocationInMap(){
////        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
////        String location = prefs.getString(getString(R.string.pref_location_key),
////                getString(R.string.pref_location_default));
//
//        String location = Utility.getPreferredLocation(this);
//
//        // Using the URI scheme for showing a location found on a map.  This super-handy
//        // intent can is detailed in the "Common Intents" page of Android's developer site:
//        Uri geoLocation = Uri.parse("geo:0,0?").buildUpon()
//                .appendQueryParameter("q",location)
//                .build();
//
//        Intent intent = new Intent(Intent.ACTION_VIEW);
//        intent.setData(geoLocation);
//        if(intent.resolveActivity(getPackageManager()) != null){
//            startActivity(intent);
//        }else{
//            Log.d(LOG_TAG,"Couldn't call" + location + ", no Location exist.");
//        }
//    }

    @Override
    protected void onResume() {
        super.onResume();
//        String location = Utility.getPreferredLocation( this );
//        // update the location in our second pane using the fragment manager
////      if (location != null && !location.equals(mLocation)) {
////            MovieFragment ff = (MovieFragment)getSupportFragmentManager().findFragmentByTag(FORECASTFRAGMENT_TAG);
//        if (location != null && !location.equals(mLocation)) {
//            MovieFragment ff = (MovieFragment)getSupportFragmentManager().findFragmentById(R.id.fragment_forecast);
//            if ( null != ff ) {
//                ff.onLocationChanged();
//            }
//            DetailFragment df = (DetailFragment)getSupportFragmentManager().findFragmentByTag(DETAILFRAGMENT_TAG);
//            if ( null != df ) {
//                df.onLocationChanged(location);
//            }
//            mLocation = location;
//        }
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
                .replace(R.id.weather_detail_container, fragment, DETAILFRAGMENT_TAG)
                .commit();
        } else {
            Intent intent = new Intent(this, DetailActivity.class)
                    .setData(contentUri);
            startActivity(intent);
        }
    }
}