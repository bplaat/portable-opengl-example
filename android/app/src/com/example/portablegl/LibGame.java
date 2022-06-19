package com.example.portablegl;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import java.io.InputStream;

public class LibGame {
    static {
        System.loadLibrary("game");
    }

    public static Context context; // Hacky

    public static Texture Texture_Load(String path) {
        try {
            InputStream inputStream = context.getAssets().open(path);
            Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
            inputStream.close();
            int[] data = new int[bitmap.getHeight() * bitmap.getWidth()];
            bitmap.getPixels(data, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
            return new Texture(bitmap.getWidth(), bitmap.getHeight(), data);
        } catch (Exception exception) {
            Log.e("PortableGL", "Can't load the texture: " + path, exception);
            return null;
        }
    }

    public static native void init();

    public static native void onResize(int width, int height, float scale);

    public static native void update(float delta);

    public static native void render();
}
