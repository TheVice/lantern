package com.android.lantern.rasterized_triangle_app;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView;

class Graphics implements GLSurfaceView.Renderer {

    public static native void on_surface_created();
    public static native void on_surface_changed(int width, int height);
    public static native void on_draw_frame();

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        on_surface_created();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {

        on_surface_changed(width, height);
    }

    public void onDrawFrame(GL10 gl) {

        on_draw_frame();
    }
}
