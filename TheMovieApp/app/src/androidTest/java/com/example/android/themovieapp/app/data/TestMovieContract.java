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
package com.example.android.themovieapp.app.data;

import android.net.Uri;
import android.test.AndroidTestCase;
import android.util.Log;


public class TestMovieContract extends AndroidTestCase {

    // intentionally includes a slash to make sure Uri is getting quoted correctly

    private final String LOG_TAG = "test";

    private static final String TEST_LANGUAGE = "/en";
    private static final String TEST_MOVIE_ID = "12368";

    public void testBuildMovie() {
        Uri MovieUri = MovieContract.MovieEntry.CONTENT_URI;

        Log.d(LOG_TAG, "-------  " + MovieUri.toString());

        assertNotNull("Error: Null Uri returned.  You must fill-in buildWeatherLocation in " +
                "MovieContract.", MovieUri);
//        assertEquals("Error: Weather location not properly appended to the end of the Uri",
//                TEST_MOVIE_DATE, locationUri.getLastPathSegment());
        assertEquals("Error: Movie Uri doesn't match our expected result",
                MovieUri.toString(),
                "content://com.example.android.themovieapp.app/movie");
    }

    public void testBuildMovieWithID() {
        Uri movieWIthIDUri = MovieContract.MovieEntry.buildMoviewithID(TEST_MOVIE_ID);

        Log.d(LOG_TAG, "-------  " + movieWIthIDUri.toString());

        assertNotNull("Error: Null Uri returned.  You must fill-in BuildMovieWithID in " +
                "MovieContract.", movieWIthIDUri);
        assertEquals("Error: Movie with ID not properly appended to the end of the Uri",
                TEST_MOVIE_ID, movieWIthIDUri.getLastPathSegment());
        assertEquals("Error: Movie with ID Uri doesn't match our expected result",
                movieWIthIDUri.toString(),
                "content://com.example.android.themovieapp.app/movie/12368");
    }
}
