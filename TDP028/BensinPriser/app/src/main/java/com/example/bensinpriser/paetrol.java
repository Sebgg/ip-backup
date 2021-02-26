package com.example.bensinpriser;

import android.app.Application;
import android.content.Context;

import com.google.android.libraries.places.api.Places;
import com.google.android.libraries.places.api.net.PlacesClient;

public class paetrol extends Application{
    private static Application sApplication;

    private static PlacesClient placesClient;

    public static Application getApplication() {
        return sApplication;
    }

    public static Context getContext() {
        return getApplication().getApplicationContext();
    }

    public static PlacesClient getPlacesClient() {
        return placesClient;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Places.initialize(getApplicationContext(), BuildConfig.GOOGLE_PLACES_KEY);

        sApplication = this;

        placesClient = Places.createClient(this);
    }
}
