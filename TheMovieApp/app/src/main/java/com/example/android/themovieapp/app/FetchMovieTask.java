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

import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.DatabaseUtils;
import android.graphics.Movie;
import android.net.Uri;
import android.os.AsyncTask;

import android.preference.PreferenceManager;
import android.text.format.Time;
import android.util.Log;

import com.example.android.themovieapp.app.data.WeatherContract;
import com.example.android.themovieapp.app.data.WeatherContract.WeatherEntry;

import com.example.android.themovieapp.app.data.MovieContract;
import com.example.android.themovieapp.app.data.MovieContract.MovieEntry;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Vector;

//public class FetchWeatherTask extends AsyncTask<String, Void, String[]> {
//    public class FetchMovieTask extends AsyncTask<String, Void, Void> {
//
//    private final String LOG_TAG = FetchMovieTask.class.getSimpleName();
//
//    //-----------------------------------------------------------
//    String m_token;
//    String m_session;
//    //-----------------------------------------------------------
//
////    private ArrayAdapter<String> mForecastAdapter;
//    private final Context mContext;
//
////    public FetchWeatherTask(Context context, ArrayAdapter<String> forecastAdapter) {
//    public FetchMovieTask(Context context) {
//        mContext = context;
////        mForecastAdapter = forecastAdapter;
//    }
//
//    private boolean DEBUG = true;
//
//    /* The date/time conversion code is going to be moved outside the asynctask later,
//     * so for convenience we're breaking it out into its own method now.
//     */
//    private String getReadableDateString(long time){
//        // Because the API returns a unix timestamp (measured in seconds),
//        // it must be converted to milliseconds in order to be converted to valid date.
//        Date date = new Date(time);
//        SimpleDateFormat format = new SimpleDateFormat("E, MMM d");
//        return format.format(date).toString();
//    }
//
//    /**
//     * Prepare the weather high/lows for presentation.
//     */
//    private String formatHighLows(double high, double low) {
//        // Data is fetched in Celsius by default.
//        // If user prefers to see in Fahrenheit, convert the values here.
//        // We do this rather than fetching in Fahrenheit so that the user can
//        // change this option without us having to re-fetch the data once
//        // we start storing the values in a database.
//        SharedPreferences sharedPrefs =
//                PreferenceManager.getDefaultSharedPreferences(mContext);
//        String unitType = sharedPrefs.getString(
//                mContext.getString(R.string.pref_units_key),
//                mContext.getString(R.string.pref_units_metric));
//
//        if (unitType.equals(mContext.getString(R.string.pref_units_imperial))) {
//            high = (high * 1.8) + 32;
//            low = (low * 1.8) + 32;
//        } else if (!unitType.equals(mContext.getString(R.string.pref_units_metric))) {
//            Log.d(LOG_TAG, "Unit type not found: " + unitType);
//        }
//
//        // For presentation, assume the user doesn't care about tenths of a degree.
//        long roundedHigh = Math.round(high);
//        long roundedLow = Math.round(low);
//
//        String highLowStr = roundedHigh + "/" + roundedLow;
//        return highLowStr;
//    }
//
//    /**
//     * Helper method to handle insertion of a new location in the weather database.
//     *
//     * @param locationSetting The location string used to request updates from the server.
//     * @param cityName A human-readable city name, e.g "Mountain View"
//     * @param lat the latitude of the city
//     * @param lon the longitude of the city
//     * @return the row ID of the added location.
//     */
//    long addLocation(String locationSetting, String cityName, double lat, double lon) {
//        long locationId;
//
//        Log.d(LOG_TAG, "----------------------------- 0" );
//        Log.d(LOG_TAG, "----------------------------- 0  " + WeatherContract.LocationEntry.CONTENT_URI);
//        Log.d(LOG_TAG, "----------------------------- 0  " + WeatherContract.LocationEntry._ID);
//        Log.d(LOG_TAG, "----------------------------- 0  " + WeatherContract.LocationEntry.COLUMN_LOCATION_SETTING + " = ?");
//        Log.d(LOG_TAG, "----------------------------- 0  " + locationSetting);
//        // First, check if the location with this city name exists in the db
//        Cursor locationCursor = mContext.getContentResolver().query(
//                WeatherContract.LocationEntry.CONTENT_URI,
//                new String[]{WeatherContract.LocationEntry._ID},
//                WeatherContract.LocationEntry.COLUMN_LOCATION_SETTING + " = ?",
//                new String[]{locationSetting},
//                null);
//        Log.d(LOG_TAG, "----------------------------- 1" );
//        if (locationCursor.moveToFirst()) {
//            int locationIdIndex = locationCursor.getColumnIndex(WeatherContract.LocationEntry._ID);
//            locationId = locationCursor.getLong(locationIdIndex);
//        } else {
//            // Now that the content provider is set up, inserting rows of data is pretty simple.
//            // First create a ContentValues object to hold the data you want to insert.
//            ContentValues locationValues = new ContentValues();
//
//            // Then add the data, along with the corresponding name of the data type,
//            // so the content provider knows what kind of value is being inserted.
//            locationValues.put(WeatherContract.LocationEntry.COLUMN_CITY_NAME, cityName);
//            locationValues.put(WeatherContract.LocationEntry.COLUMN_LOCATION_SETTING, locationSetting);
//            locationValues.put(WeatherContract.LocationEntry.COLUMN_COORD_LAT, lat);
//            locationValues.put(WeatherContract.LocationEntry.COLUMN_COORD_LONG, lon);
//
//            // Finally, insert location data into the database.
//            Uri insertedUri = mContext.getContentResolver().insert(
//                    WeatherContract.LocationEntry.CONTENT_URI,
//                    locationValues
//            );
//
//            // The resulting URI contains the ID for the row.  Extract the locationId from the Uri.
//            locationId = ContentUris.parseId(insertedUri);
//        }
//
//        locationCursor.close();
//
//        // Wait, that worked?  Yes!
//        return locationId;
//    }
//
//    /**
//     * Helper method to handle insertion of a new location in the weather database.
//     *
//     * @param movie_ID The location string used to request updates from the server.
//     * @param title A human-readable city name, e.g "Mountain View"
//     * @param poster the latitude of the city
//     * @param overview the longitude of the city
//     * @return the row ID of the added location.
//     */
//    long addMovie(String movie_ID, String title, String poster, String overview) {
//        long movieId;
//
//        Log.d(LOG_TAG, "----------------------------- 0" );
//        Log.d(LOG_TAG, "----------------------------- 0  " + MovieContract.MovieEntry.CONTENT_URI);
//        Log.d(LOG_TAG, "----------------------------- 0  " + MovieContract.MovieEntry._ID);
//        Log.d(LOG_TAG, "----------------------------- 0  " + MovieEntry.COLUMN_MOVIE_ID + " = ?");
//        Log.d(LOG_TAG, "----------------------------- 0  " + movie_ID);
//
//        Cursor movieCursor = mContext.getContentResolver().query(
//                MovieContract.MovieEntry.CONTENT_URI,
//                new String[]{MovieContract.MovieEntry._ID},
//                MovieEntry.COLUMN_MOVIE_ID + " = ?",
//                new String[]{movie_ID},
//                null);
//
//        if (movieCursor.moveToFirst()) {
//            int movieIdIndex = movieCursor.getColumnIndex(MovieContract.MovieEntry._ID);
//            movieId = movieCursor.getLong(movieIdIndex);
//        } else {
//            // Now that the content provider is set up, inserting rows of data is pretty simple.
//            // First create a ContentValues object to hold the data you want to insert.
//            ContentValues movieValues = new ContentValues();
//
//            // Then add the data, along with the corresponding name of the data type,
//            // so the content provider knows what kind of value is being inserted.
//            movieValues.put(MovieEntry.COLUMN_MOVIE_ID, movie_ID);
//            movieValues.put(MovieEntry.COLUMN_TITLE, title);
//            movieValues.put(MovieEntry.COLUMN_POSTER, poster);
//            movieValues.put(MovieEntry.COLUMN_OVERVIEW, overview);
//
//            // Finally, insert location data into the database.
//            Uri insertedUri = mContext.getContentResolver().insert(
//                    MovieContract.MovieEntry.CONTENT_URI,
//                    movieValues
//            );
//
//            // The resulting URI contains the ID for the row.  Extract the locationId from the Uri.
//            movieId = ContentUris.parseId(insertedUri);
//        }
//
//        movieCursor.close();
//
//        // Wait, that worked?  Yes!
//        return movieId;
//    }
//
//    /*
//        Students: This code will allow the FetchWeatherTask to continue to return the strings that
//        the UX expects so that we can continue to test the application even once we begin using
//        the database.
//     */
//    String[] convertContentValuesToUXFormat(Vector<ContentValues> cvv) {
//        // return strings to keep UI functional for now
//        String[] resultStrs = new String[cvv.size()];
//        for ( int i = 0; i < cvv.size(); i++ ) {
//            ContentValues weatherValues = cvv.elementAt(i);
//            String highAndLow = formatHighLows(
//                    weatherValues.getAsDouble(WeatherEntry.COLUMN_MAX_TEMP),
//                    weatherValues.getAsDouble(WeatherEntry.COLUMN_MIN_TEMP));
//            resultStrs[i] = getReadableDateString(
//                    weatherValues.getAsLong(WeatherEntry.COLUMN_DATE)) +
//                    " - " + weatherValues.getAsString(WeatherEntry.COLUMN_SHORT_DESC) +
//                    " - " + highAndLow;
//        }
//        return resultStrs;
//    }
//
//    /**
//     * Take the String representing the complete forecast in JSON Format and
//     * pull out the data we need to construct the Strings needed for the wireframes.
//     *
//     * Fortunately parsing is easy:  constructor takes the JSON string and converts it
//     * into an Object hierarchy for us.
//     */
////    private String[] getWeatherDataFromJson(String forecastJsonStr,
//    private void getWeatherDataFromJson(String forecastJsonStr,
//                                            String locationSetting)
//            throws JSONException {
//
//        // Now we have a String representing the complete forecast in JSON Format.
//        // Fortunately parsing is easy:  constructor takes the JSON string and converts it
//        // into an Object hierarchy for us.
//
//        // These are the names of the JSON objects that need to be extracted.
//
//        // Location information
//        final String OWM_CITY = "city";
//        final String OWM_CITY_NAME = "name";
//        final String OWM_COORD = "coord";
//
//        // Location coordinate
//        final String OWM_LATITUDE = "lat";
//        final String OWM_LONGITUDE = "lon";
//
//        // Weather information.  Each day's forecast info is an element of the "list" array.
//        final String OWM_LIST = "list";
//        final String OWM_MAIN = "main";
//
//        // All temperatures are children of the "temp" object.
//        final String OWM_TEMPERATURE = "temp";
//        final String OWM_MIN = "temp_min";
//        final String OWM_MAX = "temp_max";
//        final String OWM_PRESSURE = "pressure";
//        final String OWM_HUMIDITY = "humidity";
//
//        final String OWM_WIND = "wind";
//        final String OWM_WINDSPEED = "speed";
//        final String OWM_WIND_DIRECTION = "deg";
//
//        final String OWM_WEATHER = "weather";
//        final String OWM_DESCRIPTION = "main";
//        final String OWM_WEATHER_ID = "id";
//
//        try {
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
//            JSONObject forecastJson = new JSONObject(forecastJsonStr);
//            JSONArray weatherArray = forecastJson.getJSONArray(OWM_LIST);
//
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 1");
//            JSONObject cityJson = forecastJson.getJSONObject(OWM_CITY);
//            String cityName = cityJson.getString(OWM_CITY_NAME);
//
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 2");
//            JSONObject cityCoord = cityJson.getJSONObject(OWM_COORD);
//            double cityLatitude = cityCoord.getDouble(OWM_LATITUDE);
//            double cityLongitude = cityCoord.getDouble(OWM_LONGITUDE);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ locationSetting.replace(",de",""));
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ cityName );
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ cityLatitude );
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ cityLongitude );
//            long locationId = addLocation(locationSetting.replace(",de",""), cityName, cityLatitude, cityLongitude);
//            // Insert the new weather information into the database
//            Vector<ContentValues> cVVector = new Vector<ContentValues>(weatherArray.length());
//
//            // OWM returns daily forecasts based upon the local time of the city that is being
//            // asked for, which means that we need to know the GMT offset to translate this data
//            // properly.
//
//            // Since this data is also sent in-order and the first day is always the
//            // current day, we're going to take advantage of that to get a nice
//            // normalized UTC date for all of our weather.
//
//            Time dayTime = new Time();
//            dayTime.setToNow();
//
//            // we start at the day returned by local time. Otherwise this is a mess.
//            int julianStartDay = Time.getJulianDay(System.currentTimeMillis(), dayTime.gmtoff);
//
//            // now we work exclusively in UTC
//            dayTime = new Time();
//
//            for(int i = 0; i < weatherArray.length(); i++) {
//                // These are the values that will be collected.
//                long dateTime;
//                double pressure;
//                int humidity;
//                double windSpeed;
//                double windDirection;
//
//                double high;
//                double low;
//
//                String description;
//                int weatherId;
//
//                // Get the JSON object representing the day
//                JSONObject dayForecast = weatherArray.getJSONObject(i);
//
//                // Temperatures are in a child object called "temp".  Try not to name variables
//                // "temp" when working with temperature.  It confuses everybody.
//                JSONObject temperatureObject = dayForecast.getJSONObject(OWM_MAIN);
//                high = temperatureObject.getDouble(OWM_MAX);
//                low = temperatureObject.getDouble(OWM_MIN);
//                pressure = temperatureObject.getDouble(OWM_PRESSURE);
//                Log.d(LOG_TAG, "###### 1: " + pressure);
//                humidity = temperatureObject.getInt(OWM_HUMIDITY);
//
//                // Cheating to convert this to UTC time, which is what we want anyhow
//                dateTime = dayTime.setJulianDay(julianStartDay+i);
//                Log.d(LOG_TAG, "###### 0 --:" + dateTime);
//
//                JSONObject windObject = dayForecast.getJSONObject(OWM_WIND);
//                windSpeed = windObject.getDouble(OWM_WINDSPEED);
//                windDirection = windObject.getDouble(OWM_WIND_DIRECTION);
//
//                // Description is in a child array called "weather", which is 1 element long.
//                // That element also contains a weather code.
//                JSONObject weatherObject =
//                        dayForecast.getJSONArray(OWM_WEATHER).getJSONObject(0);
//                description = weatherObject.getString(OWM_DESCRIPTION);
//                weatherId = weatherObject.getInt(OWM_WEATHER_ID);
//
//                ContentValues weatherValues = new ContentValues();
//
//                weatherValues.put(WeatherEntry.COLUMN_LOC_KEY, locationId);
//                weatherValues.put(WeatherEntry.COLUMN_DATE, dateTime);
//                weatherValues.put(WeatherEntry.COLUMN_HUMIDITY, humidity);
//                weatherValues.put(WeatherEntry.COLUMN_PRESSURE, pressure);
//                weatherValues.put(WeatherEntry.COLUMN_WIND_SPEED, windSpeed);
//                weatherValues.put(WeatherEntry.COLUMN_DEGREES, windDirection);
//                weatherValues.put(WeatherEntry.COLUMN_MAX_TEMP, high);
//                weatherValues.put(WeatherEntry.COLUMN_MIN_TEMP, low);
//                weatherValues.put(WeatherEntry.COLUMN_SHORT_DESC, description);
//                weatherValues.put(WeatherEntry.COLUMN_WEATHER_ID, weatherId);
//
//                cVVector.add(weatherValues);
//            }
//            int inserted = 0;
//
//            // add to database
//            if ( cVVector.size() > 0 ) {
//                // Student: call bulkInsert to add the weatherEntries to the database here
//                ContentValues[] cvArray = new ContentValues[cVVector.size()];
//                cVVector.toArray(cvArray);
//                inserted = mContext.getContentResolver().bulkInsert(WeatherEntry.CONTENT_URI, cvArray);
//            }
//
//            // Sort order:  Ascending, by date.
//            String sortOrder = WeatherEntry.COLUMN_DATE + " ASC";
//            Uri weatherForLocationUri = WeatherEntry.buildWeatherLocationWithStartDate(
//                    locationSetting, System.currentTimeMillis());
//
//            // Students: Uncomment the next lines to display what what you stored in the bulkInsert
//            Cursor cur = mContext.getContentResolver().query(weatherForLocationUri,
//                    null, null, null, sortOrder);
//
//            cVVector = new Vector<ContentValues>(cur.getCount());
//            if ( cur.moveToFirst() ) {
//                do {
//                    ContentValues cv = new ContentValues();
//                    DatabaseUtils.cursorRowToContentValues(cur, cv);
//                    cVVector.add(cv);
//                } while (cur.moveToNext());
//            }
//
//            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + cVVector.size() + " Inserted");
//
//            String[] resultStrs = convertContentValuesToUXFormat(cVVector);
////            return resultStrs;
//            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + inserted + " Inserted");
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        }
////        return null;
//    }
//
//
//    private void getToken(String dataJsonStr)
//            throws JSONException {
//
//        final String TOKEN_SUCCES = "success";
//        final String TOKEN_EXPIRES = "expires_at";
//        final String TOKEN = "request_token";
//
//        try {
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
//            JSONObject movieJson = new JSONObject(dataJsonStr);
//
//            Boolean isSucces = movieJson.getBoolean(TOKEN_SUCCES);
//            String dateExpires = movieJson.getString(TOKEN_EXPIRES);
//            m_token = movieJson.getString(TOKEN);
//
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ isSucces);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ dateExpires );
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 3 -- "+ m_token );
//
////            // Insert the new weather information into the database
////            Vector<ContentValues> cVVector = new Vector<ContentValues>(weatherArray.length());
////
////            // OWM returns daily forecasts based upon the local time of the city that is being
////            // asked for, which means that we need to know the GMT offset to translate this data
////            // properly.
////
////            // Since this data is also sent in-order and the first day is always the
////            // current day, we're going to take advantage of that to get a nice
////            // normalized UTC date for all of our weather.
////
////            Time dayTime = new Time();
////            dayTime.setToNow();
////
////            // we start at the day returned by local time. Otherwise this is a mess.
////            int julianStartDay = Time.getJulianDay(System.currentTimeMillis(), dayTime.gmtoff);
////
////            // now we work exclusively in UTC
////            dayTime = new Time();
////
////            for(int i = 0; i < weatherArray.length(); i++) {
////                // These are the values that will be collected.
////                long dateTime;
////                double pressure;
////                int humidity;
////                double windSpeed;
////                double windDirection;
////
////                double high;
////                double low;
////
////                String description;
////                int weatherId;
////
////                // Get the JSON object representing the day
////                JSONObject dayForecast = weatherArray.getJSONObject(i);
////
////                // Temperatures are in a child object called "temp".  Try not to name variables
////                // "temp" when working with temperature.  It confuses everybody.
////                JSONObject temperatureObject = dayForecast.getJSONObject(OWM_MAIN);
////                high = temperatureObject.getDouble(OWM_MAX);
////                low = temperatureObject.getDouble(OWM_MIN);
////                pressure = temperatureObject.getDouble(OWM_PRESSURE);
////                Log.d(LOG_TAG, "###### 1: " + pressure);
////                humidity = temperatureObject.getInt(OWM_HUMIDITY);
////
////                // Cheating to convert this to UTC time, which is what we want anyhow
////                dateTime = dayTime.setJulianDay(julianStartDay+i);
////                Log.d(LOG_TAG, "###### 0 --:" + dateTime);
////
////                JSONObject windObject = dayForecast.getJSONObject(OWM_WIND);
////                windSpeed = windObject.getDouble(OWM_WINDSPEED);
////                windDirection = windObject.getDouble(OWM_WIND_DIRECTION);
////
////                // Description is in a child array called "weather", which is 1 element long.
////                // That element also contains a weather code.
////                JSONObject weatherObject =
////                        dayForecast.getJSONArray(OWM_WEATHER).getJSONObject(0);
////                description = weatherObject.getString(OWM_DESCRIPTION);
////                weatherId = weatherObject.getInt(OWM_WEATHER_ID);
////
////                ContentValues weatherValues = new ContentValues();
////
////                weatherValues.put(WeatherEntry.COLUMN_LOC_KEY, locationId);
////                weatherValues.put(WeatherEntry.COLUMN_DATE, dateTime);
////                weatherValues.put(WeatherEntry.COLUMN_HUMIDITY, humidity);
////                weatherValues.put(WeatherEntry.COLUMN_PRESSURE, pressure);
////                weatherValues.put(WeatherEntry.COLUMN_WIND_SPEED, windSpeed);
////                weatherValues.put(WeatherEntry.COLUMN_DEGREES, windDirection);
////                weatherValues.put(WeatherEntry.COLUMN_MAX_TEMP, high);
////                weatherValues.put(WeatherEntry.COLUMN_MIN_TEMP, low);
////                weatherValues.put(WeatherEntry.COLUMN_SHORT_DESC, description);
////                weatherValues.put(WeatherEntry.COLUMN_WEATHER_ID, weatherId);
////
////                cVVector.add(weatherValues);
////            }
////            int inserted = 0;
////            // add to database
////            if ( cVVector.size() > 0 ) {
////                // Student: call bulkInsert to add the weatherEntries to the database here
////                ContentValues[] cvArray = new ContentValues[cVVector.size()];
////                cVVector.toArray(cvArray);
////                inserted = mContext.getContentResolver().bulkInsert(WeatherEntry.CONTENT_URI, cvArray);
////            }
////
////            // Sort order:  Ascending, by date.
////            String sortOrder = WeatherEntry.COLUMN_DATE + " ASC";
////            Uri weatherForLocationUri = WeatherEntry.buildWeatherLocationWithStartDate(
////                    locationSetting, System.currentTimeMillis());
////
////            // Students: Uncomment the next lines to display what what you stored in the bulkInsert
////            Cursor cur = mContext.getContentResolver().query(weatherForLocationUri,
////                    null, null, null, sortOrder);
////
////            cVVector = new Vector<ContentValues>(cur.getCount());
////            if ( cur.moveToFirst() ) {
////                do {
////                    ContentValues cv = new ContentValues();
////                    DatabaseUtils.cursorRowToContentValues(cur, cv);
////                    cVVector.add(cv);
////                } while (cur.moveToNext());
////            }
////
////            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + cVVector.size() + " Inserted");
////
////            String[] resultStrs = convertContentValuesToUXFormat(cVVector);
//////            return resultStrs;
////            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + inserted + " Inserted");
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        }
////        return null;
//    }
//
//    private void getSession(String dataJsonStr)
//            throws JSONException {
//
//        final String SESSION_SUCCES = "success";
//        final String SESSION = "guest_session_id";
//        final String SESSION_EXPIRES = "expires_at";
//
//        try {
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
//            JSONObject movieJson = new JSONObject(dataJsonStr);
//
//            Boolean isSucces = movieJson.getBoolean(SESSION_SUCCES);
//            m_session = movieJson.getString(SESSION);
//            String dateExpires = movieJson.getString(SESSION_EXPIRES);
//
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 4 -- "+ isSucces);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 4 -- "+ m_session );
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 4 -- "+ dateExpires );
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        }
//    }
//
//    private void getRatedMovies(String dataJsonStr)
//            throws JSONException {
//
//        final String PAGES = "page";
//        final String TOTAL_RESULTS = "total_results";
//        final String TOTAL_PAGES = "total_pages";
//
//        final String RESULTS = "results";
//
//        try {
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 0");
//            JSONObject moviesJson = new JSONObject(dataJsonStr);
//
//            int page = moviesJson.getInt(PAGES);
//            int results = moviesJson.getInt(TOTAL_RESULTS);
//            int pages = moviesJson.getInt(TOTAL_PAGES);
//
//            JSONArray resultMoviesArray = moviesJson.getJSONArray(RESULTS);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + page);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + results );
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 5 -- " + pages );
//
//            // Insert the new weather information into the database
//            Vector<ContentValues> cVVector = new Vector<ContentValues>(resultMoviesArray.length());
//
//            for(int i = 0; i < resultMoviesArray.length(); i++) {
//                // These are the values that will be collected.
//                String movieID;
//                String title;
//                String poster;
//                String overview;
//
//
//                JSONObject currentMovie = resultMoviesArray.getJSONObject(i);
//                movieID = currentMovie.getString("id");
//                title = currentMovie.getString("title");
//                poster = currentMovie.getString("poster_path");
//                overview = currentMovie.getString("overview");
//
//
//                Log.d(LOG_TAG, "###### " + i + " = " + movieID);
//                Log.d(LOG_TAG, "###### " + i + " = " + title);
//                Log.d(LOG_TAG, "###### " + i + " = " + poster);
//                Log.d(LOG_TAG, " ------------------------");
//
//
//                ContentValues movieValues = new ContentValues();
//
//                movieValues.put(MovieEntry.COLUMN_MOVIE_ID, movieID);
//                movieValues.put(MovieEntry.COLUMN_TITLE, title);
//                movieValues.put(MovieEntry.COLUMN_POSTER, poster);
//                movieValues.put(MovieEntry.COLUMN_OVERVIEW, overview);
//
//                cVVector.add(movieValues);
//
////                addMovie(movieID, title, poster, overview);
//            }
//
//            int inserted = 0;
//            // add to database
//            if ( cVVector.size() > 0 ) {
//                // Student: call bulkInsert to add the weatherEntries to the database here
//                ContentValues[] cvArray = new ContentValues[cVVector.size()];
//                cVVector.toArray(cvArray);
//                inserted = mContext.getContentResolver().bulkInsert(MovieEntry.CONTENT_URI, cvArray);
//            }
//
//            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + cVVector.size() + " Inserted");
//            Log.d(LOG_TAG, "FetchWeatherTask Complete. " + inserted + " Inserted");
//
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        }
//    }
//
//
//    private void getDetailedMovie(String dataJsonStr)
//            throws JSONException {
//
//        final String MOVIE_TITTLE = "title";
//        final String MOVIE_POSTER = "poster_path";
//
//        try {
//            JSONObject movieJson = new JSONObject(dataJsonStr);
//
//
//            String tittle = movieJson.getString(MOVIE_TITTLE);
//            String postherPath = movieJson.getString(MOVIE_POSTER);
//
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 7 -- "+ tittle);
//            Log.d(LOG_TAG, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 7 -- "+ postherPath );
//
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        }
//    }
//
//
//    @Override
//    protected Void doInBackground(String... params) {
////    protected String[] doInBackground(String... params) {
//
//        // If there's no zip code, there's nothing to look up.  Verify size of params.
////        if (params.length == 0) {
////            return null;
////        }
//        String movieID = params[0];
//        Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>" + movieID);
//        // These two need to be declared outside the try/catch
//        // so that they can be closed in the finally block.
//        HttpURLConnection urlConnection = null;
//        BufferedReader reader = null;
//
//        // Will contain the raw JSON response as a string.
//        String forecastJsonStr = null;
//        String guestSessionJsonStr = null;
//        String ratedMovieJsonStr = null;
//        String detailedMovieJsonStr = null;
//
//        try {
//            // Construct the URL for the OpenWeatherMap query
//            // Possible parameters are avaiable at OWM's forecast API page, at
//            // http://openweathermap.org/API#forecast
//            final String MOVIE_BASE_URL =
//                    "https://api.themoviedb.org/3/authentication/token/new?";
//            final String APPID_PARAM = "api_key";
//
//            Uri builtUri = Uri.parse(MOVIE_BASE_URL).buildUpon()
//                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
//                    .build();
//
//            URL url = new URL(builtUri.toString());
//
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>" + builtUri.toString());
//
//            // Create the request to OpenWeatherMap, and open the connection
//            urlConnection = (HttpURLConnection) url.openConnection();
//            urlConnection.setRequestMethod("GET");
//            urlConnection.connect();
//
//            // Read the input stream into a String
//            InputStream inputStream = urlConnection.getInputStream();
//            StringBuffer buffer = new StringBuffer();
//            if (inputStream == null) {
//                // Nothing to do.
//                return null;
//            }
//            reader = new BufferedReader(new InputStreamReader(inputStream));
//
//            String line;
//            while ((line = reader.readLine()) != null) {
//                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
//                // But it does make debugging a *lot* easier if you print out the completed
//                // buffer for debugging.
//                buffer.append(line + "\n");
//            }
//
//            if (buffer.length() == 0) {
//                // Stream was empty.  No point in parsing.
//                return null;
//            }
//
//            forecastJsonStr = buffer.toString();
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>> end 3" + forecastJsonStr);
//            getToken(forecastJsonStr);
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>> end 4" );
//
//
//
//            final String MOVIE_BASE_SESSION =
//                    "https://www.themoviedb.org/authenticate/";
//            builtUri = Uri.parse(MOVIE_BASE_SESSION).buildUpon()
//                    .appendPath(m_token)
//                    .build();
//
////            url = new URL(builtUri.toString());
//
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>" + builtUri.toString());
//
//
//            final String MOVIE_BASE_GUESS_SESSION =
//                    "https://api.themoviedb.org/3/authentication/guest_session/new?";
//            builtUri = Uri.parse(MOVIE_BASE_GUESS_SESSION).buildUpon()
//                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
//                    .build();
//
//            url = new URL(builtUri.toString());
//
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>" + builtUri.toString());
//
//            // Create the request to OpenWeatherMap, and open the connection
//            urlConnection = (HttpURLConnection) url.openConnection();
//            urlConnection.setRequestMethod("GET");
//            urlConnection.connect();
//
//            // Read the input stream into a String
//            inputStream = urlConnection.getInputStream();
//            buffer = new StringBuffer();
//            if (inputStream == null) {
//                // Nothing to do.
//                return null;
//            }
//            reader = new BufferedReader(new InputStreamReader(inputStream));
//
//            while ((line = reader.readLine()) != null) {
//                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
//                // But it does make debugging a *lot* easier if you print out the completed
//                // buffer for debugging.
//                buffer.append(line + "\n");
//            }
//
//            if (buffer.length() == 0) {
//                // Stream was empty.  No point in parsing.
//                return null;
//            }
//
//            guestSessionJsonStr = buffer.toString();
//            getSession(guestSessionJsonStr);
//
//            //Movie Rated
//            final String MOVIE_BASE =
//                    "https://api.themoviedb.org/3/";
//
//            final String MOVIE = "movie";
//            final String TOP = "top_rated";
//            final String LANGUAGE = "Language";
//            final String PAGE = "page";
//
//            builtUri = Uri.parse(MOVIE_BASE).buildUpon()
//                    .appendPath(MOVIE)
//                    .appendPath(TOP)
//                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
//                    .appendQueryParameter(LANGUAGE, "en-US")
//                    .appendQueryParameter(PAGE, "1")
//                    .build();
//
//            url = new URL(builtUri.toString());
//
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + builtUri.toString());
//
//            // Create the request to OpenWeatherMap, and open the connection
//            urlConnection = (HttpURLConnection) url.openConnection();
//            urlConnection.setRequestMethod("GET");
//            urlConnection.connect();
//
//            // Read the input stream into a String
//            inputStream = urlConnection.getInputStream();
//            buffer = new StringBuffer();
//            if (inputStream == null) {
//                // Nothing to do.
//                return null;
//            }
//            reader = new BufferedReader(new InputStreamReader(inputStream));
//
//            while ((line = reader.readLine()) != null) {
//                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
//                // But it does make debugging a *lot* easier if you print out the completed
//                // buffer for debugging.
//                buffer.append(line + "\n");
//            }
//
//            if (buffer.length() == 0) {
//                // Stream was empty.  No point in parsing.
//                return null;
//            }
//
//            ratedMovieJsonStr = buffer.toString();
//            getRatedMovies(ratedMovieJsonStr);
//
//
//            //Movie Detailed
//            final String MOVIE_ID = "19404";
//            final String MOVIE_LANGUAGE = "en-US";
//
//            builtUri = Uri.parse(MOVIE_BASE).buildUpon()
//                    .appendPath(MOVIE)
//                    .appendPath(MOVIE_ID)
//                    .appendQueryParameter(APPID_PARAM, BuildConfig.THE_MOVIE_DB_API_KEY)
//                    .appendQueryParameter(LANGUAGE, MOVIE_LANGUAGE)
//                    .build();
//
//            url = new URL(builtUri.toString());
//
//            Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>>-----" + builtUri.toString());
//
//            // Create the request to OpenWeatherMap, and open the connection
//            urlConnection = (HttpURLConnection) url.openConnection();
//            urlConnection.setRequestMethod("GET");
//            urlConnection.connect();
//
//            // Read the input stream into a String
//            inputStream = urlConnection.getInputStream();
//            buffer = new StringBuffer();
//            if (inputStream == null) {
//                // Nothing to do.
//                return null;
//            }
//            reader = new BufferedReader(new InputStreamReader(inputStream));
//
//            while ((line = reader.readLine()) != null) {
//                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
//                // But it does make debugging a *lot* easier if you print out the completed
//                // buffer for debugging.
//                buffer.append(line + "\n");
//            }
//
//            if (buffer.length() == 0) {
//                // Stream was empty.  No point in parsing.
//                return null;
//            }
//
//            detailedMovieJsonStr = buffer.toString();
//            getDetailedMovie(detailedMovieJsonStr);
//
//        } catch (IOException e) {
//            Log.e(LOG_TAG, "Error ", e);
//            // If the code didn't successfully get the weather data, there's no point in attempting
//            // to parse it.
//            return null;
//        } catch (JSONException e) {
//            Log.e(LOG_TAG, e.getMessage(), e);
//            e.printStackTrace();
//        } finally {
//            if (urlConnection != null) {
//                urlConnection.disconnect();
//            }
//            if (reader != null) {
//                try {
//                    reader.close();
//                } catch (final IOException e) {
//                    Log.e(LOG_TAG, "Error closing stream", e);
//                }
//            }
//        }
//        Log.d(LOG_TAG, ">>>>>>>>>>>>>>>>>>>>>>> end" );
////        try {
////            return getWeatherDataFromJson(forecastJsonStr, locationQuery);
////        } catch (JSONException e) {
////            Log.e(LOG_TAG, e.getMessage(), e);
////            e.printStackTrace();
////        }
////        // This will only happen if there was an error getting or parsing the forecast.
//        return null;
//    }
//
////    @Override
////    protected void onPostExecute(String[] result) {
////        if (result != null && mForecastAdapter != null) {
////            mForecastAdapter.clear();
////            for(String dayForecastStr : result) {
////                mForecastAdapter.add(dayForecastStr);
////            }
////            // New data is back from the server.  Hooray!
////        }
////    }
//}