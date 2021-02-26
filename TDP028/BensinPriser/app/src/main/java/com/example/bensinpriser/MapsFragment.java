package com.example.bensinpriser;


import android.location.Location;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapView;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.CameraPosition;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PointOfInterest;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.android.libraries.places.api.Places;
import com.google.android.libraries.places.api.model.Place;
import com.google.android.libraries.places.api.net.FetchPlaceRequest;
import com.google.android.libraries.places.api.net.PlacesClient;
import com.google.android.material.snackbar.Snackbar;

import java.util.Arrays;
import java.util.List;

public class MapsFragment extends Fragment implements OnMapReadyCallback,
        GoogleMap.OnPoiClickListener, GoogleMap.OnMyLocationButtonClickListener {
    private static final String TAG = MapsFragment.class.getSimpleName();
    private static final String STATE_KEY_MAP_CAMERA = "110";

    private Location currentLocation;
    private FusedLocationProviderClient fusedLocationProviderClient;
    private static final int LOCATION_REQUEST_CODE = 201;
    private CameraPosition position;


    private GoogleMap mMap;
    private MapView mView;
    private PlacesClient placesClient;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        fusedLocationProviderClient = LocationServices.getFusedLocationProviderClient(this.getContext());
        Places.initialize(this.getContext(), BuildConfig.GOOGLE_PLACES_KEY);
        placesClient = Places.createClient(this.getContext());

        this.setRetainInstance(true);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        if (savedInstanceState != null) {
            position = savedInstanceState.getParcelable(STATE_KEY_MAP_CAMERA);
        }
        View view = inflater.inflate(R.layout.fragment_maps, container, false);

        mView = view.findViewById(R.id.map_view);

        mView.onCreate(savedInstanceState);

        fetchLastLocation();
        return view;
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        mMap.setOnPoiClickListener(this);
        mMap.setMyLocationEnabled(true);
        mMap.setOnMyLocationButtonClickListener(this);

        if (position == null) {
            LatLng latLng = new LatLng(currentLocation.getLatitude(),currentLocation.getLongitude());

            mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(latLng, 15));
        } else {
            mMap.moveCamera(CameraUpdateFactory.newCameraPosition(position));
        }

        mView.onResume();
    }

    @Override
    public boolean onMyLocationButtonClick() {
        Snackbar.make(getView(), getString(R.string.return_to_current_location),
                Snackbar.LENGTH_SHORT).show();
        // Return false so that we don't consume the event and the default behavior still occurs
        // (the camera animates to the user's current position).
        return false;
    }

    private void fetchLastLocation(){
        Task<Location> task = fusedLocationProviderClient.getLastLocation();
        task.addOnSuccessListener(location -> {
            if (location != null) {
                currentLocation = location;
                mView.getMapAsync(MapsFragment.this);
            }else{
                Snackbar.make(getView(), getString(R.string.no_location_recorded),
                        Snackbar.LENGTH_SHORT)
                        .show();
            }
        });
    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);

        if (mMap != null) {
            outState.putParcelable(STATE_KEY_MAP_CAMERA, mMap.getCameraPosition());
        }
    }

    public void onPoiClick(PointOfInterest poi) {
        List<Place.Field> placeFields = Arrays.asList(Place.Field.ID, Place.Field.NAME);

// Construct a request object, passing the place ID and fields array.
        FetchPlaceRequest request = FetchPlaceRequest.newInstance(poi.placeId, placeFields);

        placesClient.fetchPlace(request).addOnSuccessListener((response) -> {
            Place place = response.getPlace();
            Snackbar.make(getView(), "Clicked: " +
                            place.getName() + "\nOpen: " + place.getOpeningHours(),
                    Snackbar.LENGTH_SHORT)
                    .show();
        }).addOnFailureListener((exception) -> {
            if (exception instanceof ApiException) {
                ApiException apiException = (ApiException) exception;
                int statusCode = apiException.getStatusCode();
                // Handle error with given status code.
                Log.e(TAG, "Place not found: " + exception.getMessage());
            }
        });
    }

    @Override
    public void onResume() {
        super.onResume();
        mView.onResume();
    }
}
