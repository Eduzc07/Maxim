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

import android.database.Cursor;
import android.test.AndroidTestCase;

import com.example.android.themovieapp.app.data.MovieContract;

public class TestFetchMovieTask extends AndroidTestCase{
    static final String ADD_MOVIE_ID = "14516";
    static final String ADD_COLUMN_TITLE = "Dilwale Dulhania Le Jayenge";
    static final String ADD_COLUMN_POSTER = "/uC6TTUhPpQCmgldGyYveKRAu8JN.jpg";
    static final String ADD_COLUMN_OVERVIEW = "Raj is a rich, carefree, happy-go-lucky second generation NRI. Simran is the daughter of Chaudhary Baldev Singh, who in spite of being an NRI is very strict about adherence to Indian values. Simran has left for India to be married to her childhood fiancé. Raj leaves for India with a mission at his hands, to claim his lady love under the noses of her whole family. Thus begins a saga.";

    /*
        Students: uncomment testAddLocation after you have written the AddLocation function.
        This test will only run on API level 11 and higher because of a requirement in the
        content provider.
     */
//    @TargetApi(11)
//    public void testAddMovie() {
////         start from a clean state
//        getContext().getContentResolver().delete(MovieContract.MovieEntry.CONTENT_URI,
//                MovieContract.MovieEntry.COLUMN_MOVIE_ID + " = ?",
//                new String[]{ADD_MOVIE_ID});
//
//        FetchMovieTask fwt = new FetchMovieTask(getContext());
//        long movieId = fwt.addMovie(ADD_MOVIE_ID,
//                ADD_COLUMN_TITLE,
//                ADD_COLUMN_POSTER,
//                ADD_COLUMN_OVERVIEW);
//        fwt.doInBackground("");
//        // does addLocation return a valid record ID?
//        assertFalse("Error: addMovie returned an invalid ID on insert",
//                movieId == -1);
//
//        // test all this twice
//        for ( int i = 0; i < 2; i++ ) {
//
//            // does the ID point to our movie?
//            Cursor movieCursor = getContext().getContentResolver().query(
//                    MovieContract.MovieEntry.CONTENT_URI,
//                    new String[]{
//                            MovieContract.MovieEntry._ID,
//                            MovieContract.MovieEntry.COLUMN_MOVIE_ID,
//                            MovieContract.MovieEntry.COLUMN_OVERVIEW,
//                            MovieContract.MovieEntry.COLUMN_POSTER,
//                            MovieContract.MovieEntry.COLUMN_TITLE
//                    },
//                    MovieContract.MovieEntry.COLUMN_MOVIE_ID + " = ?",
//                    new String[]{ADD_MOVIE_ID},
//                    null);
//
//            // these match the indices of the projection
//            if (movieCursor.moveToFirst()) {
//                assertEquals("Error: the queried value of movieID does not match the returned value" +
//                        "from addMovie", movieCursor.getLong(0), movieId);
//                assertEquals("Error: the queried value of Movie ID is incorrect",
//                        movieCursor.getString(1), ADD_MOVIE_ID);
//                assertEquals("Error: the queried value of the Overview is incorrect",
//                        movieCursor.getString(2), ADD_COLUMN_OVERVIEW);
//                assertEquals("Error: the queried value of poster is incorrect",
//                        movieCursor.getString(3), ADD_COLUMN_POSTER);
//                assertEquals("Error: the queried value of tittle is incorrect",
//                        movieCursor.getString(4), ADD_COLUMN_TITLE);
//            } else {
//                fail("Error: the id you used to query returned an empty cursor");
//            }
//
//            // there should be no more records
//            assertFalse("Error: there should be only one record returned from a location query",
//                    movieCursor.moveToNext());
//
//            // add the movie again
//            long newMovieId = fwt.addMovie(ADD_MOVIE_ID, ADD_COLUMN_TITLE,
//                    ADD_COLUMN_POSTER, ADD_COLUMN_OVERVIEW);
//
//            assertEquals("Error: inserting a movie again should return the same ID",
//                    movieId, newMovieId);
//        }
//        // reset our state back to normal
//        getContext().getContentResolver().delete(
//                MovieContract.MovieEntry.CONTENT_URI,
//                MovieContract.MovieEntry.COLUMN_MOVIE_ID + " = ?",
//                new String[]{ADD_MOVIE_ID});
//
//        // clean up the test so that other tests can use the content provider
//        getContext().getContentResolver().
//                acquireContentProviderClient(MovieContract.MovieEntry.CONTENT_URI).
//                getLocalContentProvider().shutdown();
//
//    }
}
