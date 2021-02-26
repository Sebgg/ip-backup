package com.example.bensinpriser;


import android.location.Location;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.os.Parcelable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.libraries.places.api.Places;
import com.google.android.libraries.places.api.model.Place;
import com.google.android.libraries.places.api.net.PlacesClient;
import com.google.android.material.snackbar.Snackbar;
import com.google.gson.Gson;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.Executor;


/**
 * A simple {@link Fragment} subclass.
 */
public class StationsFragmentNew extends Fragment {
    private FusedLocationProviderClient fusedLocationProviderClient;
    private static final List<Place.Field> placeFields = Arrays.asList(Place.Field.ID,
            Place.Field.TYPES,
            Place.Field.NAME,
            Place.Field.OPENING_HOURS);

    private PlacesClient placesClient;

    private static final String TAG = "StationLog";

    private ListView listView;
    private ArrayList<PetrolStation> dataModels;
    ArrayList<PetrolStation> tempData;
    private CustomAdapter adapter;

    public StationsFragmentNew() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        this.setRetainInstance(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_stations_fragment_new, container, false);
        listView=(ListView) view.findViewById(R.id.stations_list);
        fusedLocationProviderClient = LocationServices.getFusedLocationProviderClient(Objects.requireNonNull(this.getContext()));

        if (!Places.isInitialized()) {
            Places.initialize(this.getContext(), BuildConfig.GOOGLE_PLACES_KEY);
        }

        placesClient = Places.createClient(this.getContext());
//        if (savedInstanceState != null) {
//            dataModels = savedInstanceState.getParcelable("dataModels");
//        } else {
//
//        }
        dataModels = new ArrayList<>();
        tempData = new ArrayList<>();
        getData();

        adapter = new CustomAdapter(dataModels, this.getContext());

        listView.setAdapter(adapter);

        listView.setOnItemClickListener((parent, view1, position, id) -> {

            PetrolStation dataModel= dataModels.get(position);


        });
        return view;
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);

//        if (dataModels != null) {
//            outState.putParcelable("dataModels", (Parcelable) dataModels);
//        }
    }

    private void getData() {
        fusedLocationProviderClient.getLastLocation().
                addOnSuccessListener(Objects.requireNonNull(this.getActivity()), location -> {
                    if (location != null) {
                        LatLng latLng = new LatLng(location.getLatitude(), location.getLongitude());
                        String placesUrl = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?"
                                + "location=" + latLng.latitude + "," + latLng.longitude
                                + "&rankby=distance"
                                + "&type=" + "gas_station"
                                + "&key=" + getString(R.string.google_server_key);
                        (new PopulateListAsync()).execute(placesUrl);
                    }
                });
    }

    private String readData(String httpUrl) {
        String httpData = "";
        InputStream iStream = null;
        HttpURLConnection urlConn = null;
        try {
            URL url = new URL(httpUrl);
            urlConn = (HttpURLConnection) url.openConnection();
            urlConn.connect();
            iStream = urlConn.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(iStream));
            StringBuffer buf = new StringBuffer();
            String line = "";
            while((line = reader.readLine()) != null){
                buf.append(line);
            }
            httpData = buf.toString();
            reader.close();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                iStream.close();
                urlConn.disconnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return httpData;
    }

    public void populateList(String result) {
        try {
            JSONObject temp = new JSONObject(result);
            JSONArray resultArray = temp.getJSONArray("results");
            for (int i = 0; i < resultArray.length(); i++){
                JSONObject wO = resultArray.getJSONObject(i);
                JSONObject wOLoc = wO.getJSONObject("geometry").getJSONObject("location");
                LatLng latLng = new LatLng(wOLoc.getDouble("lat"), wOLoc.getDouble("lng"));
                PetrolStation newStation = new PetrolStation(wO.getString("name"),
                        "No data.",
                        0.0,
                        latLng);
                tempData.add(newStation);
            }
        } catch (JSONException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    private class PopulateListAsync extends AsyncTask<String, Void, List<PetrolStation>> {
        private View view;
        @Override
        protected void onPreExecute() {
            view = getView();
            Objects.requireNonNull(view).findViewById(R.id.stations_fetching_spinner).setVisibility(View.VISIBLE);
        }

        @Override
        protected List<PetrolStation> doInBackground(String... strings) {
            populateList(readData(strings[0]));
            return tempData;
        }

        @Override
        protected void onPostExecute(List<PetrolStation> petrolStations) {
            super.onPostExecute(petrolStations);
            adapter.addAll(petrolStations);
            adapter.notifyDataSetChanged();
            Objects.requireNonNull(view).findViewById(R.id.stations_fetching_spinner).setVisibility(View.INVISIBLE);
            //Toast.makeText(getContext(), "Data downloaded.", Toast.LENGTH_SHORT).show();
        }
    }
}
