package com.example.bensinpriser;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.snackbar.Snackbar;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;


public class MainActivity extends AppCompatActivity implements UserFragment.OnFragmentInteractionListener,
        BottomNavigationView.OnNavigationItemSelectedListener {

    private Fragment uFragment;
    private Fragment listFragment;
    private Fragment mapFragment;

    private static final int RC_FINE_LOCATION = 1312;
    private static final int RC_INTERNET = 1313;


    private static final String TAG = "MainActivityLog";
    private static final String EXTRA_MESSAGE = "com.example.paetrol.TAG";

    private boolean backPressedOnce;

    @SuppressLint("StringFormatInvalid")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        BottomNavigationView bar = (BottomNavigationView) findViewById(R.id.bottom_navigation);

        bar.setOnNavigationItemSelectedListener(this);
        checkLocationPermission();
        checkInternetPermission();
        if (savedInstanceState == null) {
            // fresh start
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                    new StationsFragmentNew()).commit();
            FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
            if (user != null && user.getDisplayName() != null) {
                Snackbar.make(findViewById(R.id.main_coordinator_layout),
                        String.format(getString(R.string.welcome_user), user.getDisplayName()),
                        Snackbar.LENGTH_SHORT).show();
            } else if (user != null) {
                Snackbar.make(findViewById(R.id.main_coordinator_layout), getString(R.string.welcome_string_anon),
                        Snackbar.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private void checkLocationPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION)
            == PackageManager.PERMISSION_DENIED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, RC_FINE_LOCATION);
        }
    }

    private void checkInternetPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.INTERNET)
                == PackageManager.PERMISSION_DENIED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.INTERNET}, RC_INTERNET);
        }
    }

    @Override
    public void onBackPressed() {
        if (!backPressedOnce) {
            backPressedOnce = true;
            Toast.makeText(this, getString(R.string.double_back_to_exit), Toast.LENGTH_SHORT).show();
        } else {
            finish();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == RC_FINE_LOCATION) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Snackbar.make(findViewById(R.id.main_coordinator_layout), getString(R.string.permission_granted_location),
                        Snackbar.LENGTH_SHORT).show();
            }
        } else if (requestCode == RC_INTERNET){
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Snackbar.make(findViewById(R.id.main_coordinator_layout), getString(R.string.permission_internet_granted),
                        Snackbar.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        switch(item.getItemId()) {
            case R.id.action_to_user:
                if (uFragment == null) {
                    uFragment = new UserFragment();
                    listFragment = null;
                    mapFragment = null;
                }
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        uFragment).commit();
                break;
            case R.id.action_to_list:
                if (listFragment == null) {
                    listFragment = new StationsFragmentNew();
                    uFragment = null;
                    mapFragment = null;
                }
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        listFragment).commit();
                break;
            case R.id.action_to_map:
                if (mapFragment == null) {
                    mapFragment = new MapsFragment();
                    listFragment = null;
                    uFragment = null;
                }
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container,
                        mapFragment).commit();
                break;
            default:
                break;
        }
        return true;
    }

    public void toLogin(View view) {
        Intent intent = new Intent(MainActivity.this, AuthActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        startActivity(intent);
    }

    public void logout(View view) {
        FirebaseAuth.getInstance().signOut();
        FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
        if (user == null) {
            Snackbar.make(findViewById(R.id.main_coordinator_layout), getString(R.string.log_out_success),
                    Snackbar.LENGTH_SHORT).show();
            Intent intent = new Intent(getApplicationContext(), AuthActivity.class);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivity(intent);
        }
    }

    @Override
    public void onFragmentInteraction(Uri uri) {
        // Supposed to be empty
    }
}
