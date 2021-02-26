package com.example.bensinpriser;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;

import java.util.Objects;

public class LoginFragment extends Fragment implements View.OnClickListener{
    private EditText email;
    private EditText password;

    private OnFragmentInteractionListener mListener;

    public LoginFragment() {
        // Required empty public constructor
    }

    private static LoginFragment newInstance(String param1, String param2) {
        return new LoginFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View mView = inflater.inflate(R.layout.fragment_login, container, false);
        email = (EditText) mView.findViewById(R.id.et_email);
        password = (EditText) mView.findViewById(R.id.et_password);
        Button login = (Button) mView.findViewById(R.id.btn_login);
        login.setOnClickListener(this);
        mView.findViewById(R.id.btn_login_google).setOnClickListener(this);
        mView.findViewById(R.id.btn_login_anon).setOnClickListener(this);
        return mView;
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

    private void sendBack() {
        String emailRet = email.getText().toString();
        String passwordRet = password.getText().toString();
        Log.d("LoginFragment", "email: " + emailRet + " password: " + passwordRet);
        if (emailRet.equals("") || passwordRet.equals("")) {
            Snackbar.make(Objects.requireNonNull(getView()), getString(R.string.invalid_email_or_pword_login),
                    Snackbar.LENGTH_SHORT).show();
        } else {
            ((AuthActivity) Objects.requireNonNull(getActivity())).login(emailRet, passwordRet);
        }
    }

    @Override
    public void onClick(View view) {
        Log.d("loginfragment","Button clicked");
        if (view.getId() == R.id.btn_login) {
            sendBack();
        }
        else if (view.getId() == R.id.btn_login_google) {
            ((AuthActivity) Objects.requireNonNull(getActivity())).googleSignIn();
        }
        else if (view.getId() == R.id.btn_login_anon) {
            Log.d("loginfragment","Started anonymous login");
            ((AuthActivity) Objects.requireNonNull(getActivity())).AnonLogin();
        }
    }

    public interface OnFragmentInteractionListener {
        void onFragmentInteraction(Uri uri);
    }
}
