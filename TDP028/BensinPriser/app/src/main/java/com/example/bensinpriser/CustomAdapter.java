package com.example.bensinpriser;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextClock;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.google.android.material.snackbar.Snackbar;

import java.util.ArrayList;
import java.util.Objects;

public class CustomAdapter extends ArrayAdapter<PetrolStation> implements View.OnClickListener {
    private ArrayList<PetrolStation> dataSet;
    private Context mContext;

    private static class ViewHolder {
        TextView txtname;
        TextView txtopenhours;
        TextView txtprice;
    }

    public CustomAdapter(ArrayList<PetrolStation> data, @NonNull Context context) {
        super(context, R.layout.fragment_stations, data);
        this.dataSet = data;
        this.mContext = context;
    }

    @Override
    public void onClick(View v) {

        int position=(Integer) v.getTag();
        PetrolStation dataModel = getItem(position);

        if (v.getId() == R.id.station_price) {
            Snackbar.make(v, "Release date " + Objects.requireNonNull(dataModel).getPrice(), Snackbar.LENGTH_SHORT)
                    .setAction("No action", null)
                    .show();
        }
    }

    private int lastPosition = -1;

    @NonNull
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // Get the data item for this position
        PetrolStation dataModel = getItem(position);
        // Check if an existing view is being reused, otherwise inflate the view
        ViewHolder viewHolder; // view lookup cache stored in tag

        final View result;

        if (convertView == null) {

            viewHolder = new ViewHolder();
            LayoutInflater inflater = LayoutInflater.from(getContext());
            convertView = inflater.inflate(R.layout.fragment_stations, parent, false);
            viewHolder.txtname = convertView.findViewById(R.id.station_name);
            viewHolder.txtopenhours = convertView.findViewById(R.id.station_open_hours);
            viewHolder.txtprice = convertView.findViewById(R.id.station_price);

            result=convertView;

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
            result=convertView;
        }

        lastPosition = position;

        viewHolder.txtname.setText(Objects.requireNonNull(dataModel).getName());
        viewHolder.txtopenhours.setText(dataModel.getOpeningHours());
        viewHolder.txtprice.setText(String.valueOf(dataModel.getPrice()));
        viewHolder.txtprice.setOnClickListener(this);
        // Return the completed view to render on screen
        return convertView;
    }
}
