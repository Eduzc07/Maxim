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
package com.example.android.sunshine.app.data;

import android.content.ContentValues;
import android.net.Uri;
import android.test.AndroidTestCase;
import android.util.Log;

/*
    Students: This is NOT a complete test for the WeatherContract --- just for the functions
    that we expect you to write.
 */
public class TestWeatherContract extends AndroidTestCase {

    // intentionally includes a slash to make sure Uri is getting quoted correctly
    private static final String TEST_WEATHER_LOCATION = "/North Pole";
    private static final long TEST_WEATHER_DATE = 1419033600L;  // December 20th, 2014

    /*
        Students: Uncomment this out to test your weather location function.
     */
    public void testBuildWeatherLocation() {
        Uri locationUri = WeatherContract.WeatherEntry.buildWeatherLocation(TEST_WEATHER_LOCATION);
//        Uri locationUri = WeatherContract.WeatherEntry.buildWeatherUri(2896514);
        assertNotNull("Error: Null Uri returned.  You must fill-in buildWeatherLocation in " +
                        "WeatherContract.", locationUri);
        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                TEST_WEATHER_LOCATION, locationUri.getLastPathSegment());
        assertEquals("Error: Weather location Uri doesn't match our expected result",
                locationUri.toString(),
                "content://com.example.android.sunshine.app/weather/%2FNorth%20Pole");
    }

    public void testLocalEntry() {
        String TEST_LOCATION = "99705";
//        String TEST_LOCATION = "location_setting";
        ContentValues testValues = new ContentValues();
        testValues.put(WeatherContract.LocationEntry.COLUMN_LOCATION_SETTING, TEST_LOCATION);
        testValues.put(WeatherContract.LocationEntry.COLUMN_CITY_NAME, "North Pole");
        testValues.put(WeatherContract.LocationEntry.COLUMN_COORD_LAT, 64.7488);
        testValues.put(WeatherContract.LocationEntry.COLUMN_COORD_LONG, -147.353);

        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                "location", WeatherContract.LocationEntry.TABLE_NAME);
        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                TEST_LOCATION, testValues.get(WeatherContract.LocationEntry.COLUMN_LOCATION_SETTING));
        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                "North Pole", testValues.get(WeatherContract.LocationEntry.COLUMN_CITY_NAME));
        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                64.7488, testValues.get(WeatherContract.LocationEntry.COLUMN_COORD_LAT));
        assertEquals("Error: Weather location not properly appended to the end of the Uri",
                -147.353, testValues.get(WeatherContract.LocationEntry.COLUMN_COORD_LONG));
    }
}
