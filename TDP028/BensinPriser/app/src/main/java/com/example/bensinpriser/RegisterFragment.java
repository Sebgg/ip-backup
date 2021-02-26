package com.example.bensinpriser;


import android.net.Uri;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.material.snackbar.Snackbar;

import java.util.Objects;

public class RegisterFragment extends Fragment implements View.OnClickListener{

    private EditText username;
    private EditText email;
    private EditText password;
    private EditText rePassword;


    public RegisterFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View myView = inflater.inflate(R.layout.fragment_signup, container, false);
        username = (EditText) myView.findViewById(R.id.et_user_name);
        email = (EditText) myView.findViewById(R.id.et_email);
        password = (EditText) myView.findViewById(R.id.et_password);
        rePassword = (EditText) myView.findViewById(R.id.et_repassword);
        Button signup = (Button) myView.findViewById(R.id.btn_register);
        signup.setOnClickListener(this);
        // Inflate the layout for this fragment
        return myView;
    }

    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }

    private void sendBack() {
        String emailRet = email.getText().toString();
        String passwordRet = password.getText().toString();
        String comparePass = rePassword.getText().toString();
        String usernameRet = username.getText().toString();

        if (emailRet.equals("") || passwordRet.equals("") || comparePass.equals("") || username.equals("")){
            Snackbar.make(getView(), getString(R.string.fill_all_fields_warning),
                    Snackbar.LENGTH_SHORT).show();
        } else {
            if (passwordRet.equals(comparePass)) {
                ((AuthActivity) Objects.requireNonNull(getActivity())).register(emailRet, passwordRet, usernameRet);
            } else {
                Snackbar.make(getView(), getString(R.string.password_match_warning),
                        Snackbar.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_register) {
            sendBack();
        }
    }
}
