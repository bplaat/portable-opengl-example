package com.example.portablegl;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.view.WindowInsetsController;
import android.view.WindowInsets;
import android.view.View;

public class MainActivity extends Activity {
    private GLSurfaceView glSurface;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LibGame.context = this; // Hacky

        glSurface = new GLSurfaceView(this);
        glSurface.setEGLContextClientVersion(3);
        glSurface.setPreserveEGLContextOnPause(true);
        glSurface.setRenderer(new GLRenderer(this));
        setContentView(glSurface);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (glSurface != null) {
            glSurface.onResume();
        }
    }

    @Override
    protected void onPause() {
        if (glSurface != null) {
            glSurface.onPause();
        }
        super.onPause();
    }

    @SuppressWarnings("deprecation")
    private void hideSystemUI() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            getWindow().setDecorFitsSystemWindows(false);
            WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            );
        }
    }
}
