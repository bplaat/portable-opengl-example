package com.example.portablegl;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.Log;
import java.io.InputStream;

public class LibGame {
    static {
        System.loadLibrary("game");
    }

    public static native void init();

    public static native void onResize(int width, int height, float scale);

    public static native void update(float delta);

    public static native void render();

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

    public static Typeface Font_Load(String path) {
        return Typeface.createFromAsset(context.getAssets(), path);
    }

    public static int Font_MeasureText(Typeface typeface, String text, int size) {
        Paint paint = new Paint();
        paint.setTypeface(typeface);
        paint.setTextSize(size);
        return (int)paint.measureText(text);
    }

    public static Texture Font_RenderText(Typeface typeface, String text, int size, int color) {
        Paint paint = new Paint();
        paint.setTypeface(typeface);
        paint.setTextSize(size);
        Bitmap bitmap = Bitmap.createBitmap((int)paint.measureText(text), size, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        paint.setColor(Color.rgb(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff));
        canvas.drawText(text, 0, size, paint);
        int[] data = new int[bitmap.getHeight() * bitmap.getWidth()];
        bitmap.getPixels(data, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
        return new Texture(bitmap.getWidth(), bitmap.getHeight(), data);
    }
}
