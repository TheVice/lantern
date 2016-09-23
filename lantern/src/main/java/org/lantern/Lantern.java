package org.lantern;

public class Lantern {

    static {
        System.loadLibrary("lantern");
    }

    public static native int set_asset_manager(Object manager);
    public static native void initialize(int width, int height);
    public static native void frame(float dt, int width, int height, int[] area);
}
