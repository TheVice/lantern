package org.lantern.examples;

public class RasterizedTriangleApp {

    static {
        System.loadLibrary("lantern");
    }

    public static native void set_asset_manager(Object manager);
    public static native void initialize(int width, int height);
    public static native void frame(float dt, int width, int height, int[] area);
    public static native void on_key_down(short key);
}
