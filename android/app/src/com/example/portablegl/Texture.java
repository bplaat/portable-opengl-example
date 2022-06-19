package com.example.portablegl;

public class Texture {
    public int width;
    public int height;
    public int[] data;

    public Texture(int width, int height, int[] data) {
        this.width = width;
        this.height = height;
        this.data = data;
    }
}
