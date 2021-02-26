package com.example.bensinpriser;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import static android.content.ContentValues.TAG;
import static java.lang.String.valueOf;


public class UserFragment extends Fragment {
    private OnFragmentInteractionListener mListener;
    private int contributions;
    private DatabaseReference dBase;
    private static final String TAG = "UserLog";

    public UserFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        dBase = FirebaseDatabase.getInstance().getReference();

        this.setRetainInstance(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View mView = inflater.inflate(R.layout.fragment_user, container, false);
        FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
        if (user == null) {
            mView.findViewById(R.id.btn_to_login).setVisibility(View.VISIBLE);
        } else {
            mView.findViewById(R.id.btn_logout).setVisibility(View.VISIBLE);
            ValueEventListener countListener = new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    contributions = Integer.valueOf(dataSnapshot.child("contributions").child(user.getUid()).child("count").getValue().toString());
                    Log.d(TAG, String.valueOf(contributions));
                    UpdateUI(user, mView);
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {
                    // Getting Post failed, log a message
                    Log.w(TAG, "loadPost:onCancelled", databaseError.toException());
                    // ...
                }
            };
            UpdateUI(user, mView);

        }
        return mView;
    }

    public void UpdateUI(FirebaseUser user, View view) {
        if (!user.isAnonymous()) {
            TextView current = (TextView) view.findViewById(R.id.user_name);
            current.setText(String.format(getString(R.string.user_name),user.getDisplayName()));

            current = (TextView) view.findViewById(R.id.member_since);
            current.setText(String.format(getString(R.string.member_since), valueOf(user.getMetadata().getCreationTimestamp())));


            if (contributions > 0) {
                current = (TextView) view.findViewById(R.id.contribution_count);
                current.setText(String.format(getString(R.string.contribution_count), valueOf(contributions)));
            }
        } else {
            TextView current = (TextView) view.findViewById(R.id.user_name);
            current.setText(getString(R.string.anonymous_user_name));

            current = (TextView) view.findViewById(R.id.member_since);
            current.setText(getString(R.string.anonymous_user_name));

            current = (TextView) view.findViewById(R.id.contribution_count);
            current.setText(getString(R.string.no_track_contributions));
        }

    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    public void toLogin(View view) {
        ((MainActivity) getActivity()).toLogin(view);

    }

    public void logout(View view) {
        ((MainActivity) getActivity()).logout(view);
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
