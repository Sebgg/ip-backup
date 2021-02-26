package com.example.bensinpriser;

import com.google.android.gms.maps.model.LatLng;

public class PetrolStation {
    private final String name;
    private String openingHours;
    private double price;
    private String Address;
    private final LatLng latLng;

    public PetrolStation(String name, String openingHours, double price, LatLng latLng) {
        this.name = name;
        this.openingHours = openingHours;
        this.price = price;
        this.latLng = latLng;
    }

    public String getName() {
        return name;
    }

    public String getOpeningHours() {
        return openingHours;
    }

    public void setOpeningHours(String openingHours) {
        this.openingHours = openingHours;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public String getAddress() {
        return Address;
    }

    public void setAddress(String address) {
        Address = address;
    }

    public LatLng getLatLng() {
        return latLng;
    }
}
