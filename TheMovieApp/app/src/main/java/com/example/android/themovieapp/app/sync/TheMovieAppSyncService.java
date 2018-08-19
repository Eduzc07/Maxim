package com.example.android.themovieapp.app.sync;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class TheMovieAppSyncService extends Service {
    private static final Object sSyncAdapterLock = new Object();
    private static TheMovieAppSyncAdapter sTheMovieAppSyncAdapter = null;

    @Override
    public void onCreate() {
        Log.d("TheMovieAppSyncService", "onCreate - TheMovieAppSyncService");
        synchronized (sSyncAdapterLock) {
            if (sTheMovieAppSyncAdapter == null) {
                sTheMovieAppSyncAdapter = new TheMovieAppSyncAdapter(getApplicationContext(), true);
            }
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return sTheMovieAppSyncAdapter.getSyncAdapterBinder();
    }
}