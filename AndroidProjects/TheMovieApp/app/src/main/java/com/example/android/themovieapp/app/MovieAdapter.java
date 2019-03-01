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

import android.content.Context;
import android.database.Cursor;
import android.support.v4.widget.CursorAdapter;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.example.android.themovieapp.app.sync.TheMovieAppSyncAdapter;
import com.squareup.picasso.Picasso;

/**
 * Provide views to RecyclerView with data from mDataSet.
 */
public class MovieAdapter extends RecyclerView.Adapter<MovieAdapter.ViewHolder> {
    private static final String TAG = "MovieAdapter";

    public Boolean mPrepareSync = false;
    CursorAdapter mCursorAdapter;
    Context mContext;

    public interface OnItemClickListener {
        void onItemClick(Cursor c, int position);
    }

    OnItemClickListener mListener;

    public void setOnItemClickListener(OnItemClickListener listener){
        this.mListener = listener;
    }

    public MovieAdapter (Context context, Cursor c) {
        mContext = context;

        mCursorAdapter = new CursorAdapter(mContext, c, CursorAdapter.FLAG_REGISTER_CONTENT_OBSERVER) {
            @Override
            public View newView(Context context, Cursor cursor, ViewGroup parent) {
                // Inflate the view here
                int layoutId = R.layout.list_item_movie;
                View view = LayoutInflater.from(context).inflate(layoutId, parent, false);

                ViewHolder viewHolder = new ViewHolder(view);
                view.setTag(viewHolder);
                return view;
            }

            @Override
            public void bindView(View view, Context context, Cursor cursor) {
                // Binding operations
                ViewHolder viewHolder = (ViewHolder) view.getTag();

                //Read the title of the movie
                String poster = cursor.getString(MovieFragment.COL_POSTER_PATH);
//                String imageUri = "https://image.tmdb.org/t/p/w500/" + poster;
                String imageUri = "https://image.tmdb.org/t/p/w780/" + poster;
                Picasso.with(context).load(imageUri).into(viewHolder.getImageView());
            }
        };
    }

    // BEGIN_INCLUDE(recyclerViewSampleViewHolder)
    /**
     * Provide a reference to the type of views that you are using (custom ViewHolder)
     */
    public static class ViewHolder extends RecyclerView.ViewHolder {
        public final View mView;
        private final ImageView movieView;

        public ViewHolder(View view) {
            super(view);
            mView = view;
            // Define click listener for the ViewHolder's View.
//            view.setOnClickListener(new View.OnClickListener() {
//                @Override
//                public void onClick(View v) {
//                    Log.d(TAG, "Element " + getAdapterPosition() + " clicked.");
//                    String forecast = String.valueOf(getAdapterPosition());
//                    Toast.makeText(v.getContext(), forecast, Toast.LENGTH_SHORT).show();
//                }
//            });

            movieView = (ImageView) view.findViewById(R.id.list_item_image);
        }

        public void bind(final CursorAdapter cursorAdapter, final OnItemClickListener listener) {
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Cursor cursor = cursorAdapter.getCursor();
                    if (!cursor.isClosed()){
                        cursor.moveToPosition( getAdapterPosition() );
                        listener.onItemClick( cursor, getAdapterPosition() );
                    }
                }
            });
        }

        public ImageView getImageView() {
            return movieView;
        }
    }
    // END_INCLUDE(recyclerViewSampleViewHolder)

    public Cursor swapCursor(Cursor cursor) {
        if (mCursorAdapter.getCursor() == cursor) {
            return null;
        }
        Cursor oldCursor = mCursorAdapter.getCursor();
        mCursorAdapter.swapCursor(cursor);
        if (cursor != null) {
            this.notifyDataSetChanged();
        }
        return oldCursor;
    }

    // BEGIN_INCLUDE(recyclerViewOnCreateViewHolder)
    // Create new views (invoked by the layout manager)
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        // Passing the inflater job to the cursor-adapter
        View v = mCursorAdapter.newView(mContext, mCursorAdapter.getCursor(), viewGroup);

        final ViewHolder vHolder = new ViewHolder(v);
        //Here is faster than in onBindViewHolder
        vHolder.bind(mCursorAdapter, mListener);
        return vHolder;
    }
    // END_INCLUDE(recyclerViewOnCreateViewHolder)

    // BEGIN_INCLUDE(recyclerViewOnBindViewHolder)
    // Replace the contents of a view (invoked by the layout manager)
    @Override
    public void onBindViewHolder(ViewHolder viewHolder, final int position) {
        // Passing the binding operation to cursor loader
        mCursorAdapter.getCursor().moveToPosition(position); //EDITED: added this line as suggested in the comments below, thanks :)
        mCursorAdapter.bindView(viewHolder.itemView, mContext, mCursorAdapter.getCursor());

        int numPage = TheMovieAppSyncAdapter.mPage;

        //Do not load more than one page at creation of View
        if (!mPrepareSync){
            if (position == 20-1){
                mPrepareSync = true;
            }
            return;
        }

        Log.d(TAG, "Element " + position + " set." + String.valueOf(mCursorAdapter.getCount()));
        Log.d(TAG, "mPage " + TheMovieAppSyncAdapter.mPage);
        if (position == 20*numPage-3) {
            TheMovieAppSyncAdapter.mPage++;
            if (position == mCursorAdapter.getCount()-3){
                TheMovieAppSyncAdapter.syncImmediately(mContext);
            }
        }
    }
    // END_INCLUDE(recyclerViewOnBindViewHolder)

    // Return the size of your dataset (invoked by the layout manager)
    @Override
    public int getItemCount() {
        return mCursorAdapter.getCount();
//        return mElementInView;
    }
}