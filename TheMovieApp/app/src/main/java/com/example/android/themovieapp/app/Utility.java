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

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.preference.PreferenceManager;
import android.text.format.Time;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Utility {
    public static String getPreferredLanguage(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        return prefs.getString(context.getString(R.string.pref_language_key),"-1");
    }

    public static String getGenres(Context context, String input, String language) {
        String genresNames ="<b>" + context.getString(R.string.pref_genres) +": </b>";
        String[] parts = input.split("-");

        for (int i = 1; i <= Integer.parseInt(parts[0]) ; i++) {
            String item = parts[i];
            genresNames += getNameGenres(context, Integer.parseInt(item), language);
            if (i < Integer.parseInt(parts[0])) {
                genresNames += ", ";
            }
        }

        return genresNames;
    }

    public static Bitmap getImageMovie(String imageUri){
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
        return myBitmap;
    }

    private static String getNameGenres(Context context, int val, String language){
        String name = "unknown";
        //English
        if (language.equals(context.getString(R.string.pref_language_English))){
            switch (val) {
                case 28:
                    name = "Action";
                    break;
                case 12:
                    name = "Adventure";
                    break;
                case 16:
                    name = "Animation";
                    break;
                case 35:
                    name = "Comedy";
                    break;
                case 80:
                    name = "Crime";
                    break;
                case 99:
                    name = "Documentary";
                    break;
                case 18:
                    name = "Drama";
                    break;
                case 10751:
                    name = "Family";
                    break;
                case 14:
                    name = "Fantasy";
                    break;
                case 36:
                    name = "History";
                    break;
                case 27:
                    name = "Horror";
                    break;
                case 10402:
                    name = "Music";
                    break;
                case 9648:
                    name = "Mystery";
                    break;
                case 10749:
                    name = "Romance";
                    break;
                case 878:
                    name = "Science Fiction";
                    break;
                case 10770:
                    name = "TV Movie";
                    break;
                case 53:
                    name = "Thriller";
                    break;
                case 10752:
                    name = "War";
                    break;
                case 37:
                    name = "Western";
                    break;
                default:
                    name = " ";
            }
        }

        //Spanish
        if (language.equals(context.getString(R.string.pref_language_Spanish))){
            switch (val) {
                case 28:
                    name = "Acción";
                    break;
                case 12:
                    name = "Aventura";
                    break;
                case 16:
                    name = "Animación";
                    break;
                case 35:
                    name = "Comedia";
                    break;
                case 80:
                    name = "Crimen";
                    break;
                case 99:
                    name = "Documental";
                    break;
                case 18:
                    name = "Drama";
                    break;
                case 10751:
                    name = "Familia";
                    break;
                case 14:
                    name = "Fantasía";
                    break;
                case 36:
                    name = "Historia";
                    break;
                case 27:
                    name = "Terror";
                    break;
                case 10402:
                    name = "Música";
                    break;
                case 9648:
                    name = "Misterio";
                    break;
                case 10749:
                    name = "Romance";
                    break;
                case 878:
                    name = "Ciencia ficción";
                    break;
                case 10770:
                    name = "Película de la Televisión";
                    break;
                case 53:
                    name = "Suspenso";
                    break;
                case 10752:
                    name = "Guerra";
                    break;
                case 37:
                    name = "Western";
                    break;
                default:
                    name = " ";
            }
        }

        //German
        if (language.equals(context.getString(R.string.pref_language_German))){
            switch (val) {
                case 28:
                    name = "Action";
                    break;
                case 12:
                    name = "Abenteuer";
                    break;
                case 16:
                    name = "Animation";
                    break;
                case 35:
                    name = "Komödie";
                    break;
                case 80:
                    name = "Krimi";
                    break;
                case 99:
                    name = "Dokumentarfilm";
                    break;
                case 18:
                    name = "Drama";
                    break;
                case 10751:
                    name = "Familie";
                    break;
                case 14:
                    name = "Fantasy";
                    break;
                case 36:
                    name = "Historie";
                    break;
                case 27:
                    name = "Horror";
                    break;
                case 10402:
                    name = "Musik";
                    break;
                case 9648:
                    name = "Mystery";
                    break;
                case 10749:
                    name = "Liebesfilm";
                    break;
                case 878:
                    name = "Science Fiction";
                    break;
                case 10770:
                    name = "TV-Film";
                    break;
                case 53:
                    name = "Thriller";
                    break;
                case 10752:
                    name = "Kriegsfilm";
                    break;
                case 37:
                    name = "Western";
                    break;
                default:
                    name = " ";
            }
        }


        return name;
    }
}