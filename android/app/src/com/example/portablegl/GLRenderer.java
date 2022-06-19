package com.example.portablegl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLES30;
import android.util.DisplayMetrics;

public class GLRenderer implements Renderer {
    private Context context;
    private boolean isInited = false;

    GLRenderer(Context context) {
        this.context = context;
    }

    @Override
    public void onSurfaceCreated(GL10 notUsed, EGLConfig config) {}

    @Override
    public void onSurfaceChanged(GL10 notUsed, int width, int height) {
        DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        LibGame.onResize((int)(width / metrics.density), (int)(height / metrics.density), metrics.density);
        if (!isInited) {
            isInited = true;
            LibGame.init();
        }
    }

    @Override
    public void onDrawFrame(GL10 notUsed) {
        LibGame.update(1.f / 60.f);
        LibGame.render();
    }
}
