package org.lantern.examples;

public class RasterizedTriangleApp {

    static {
        System.loadLibrary("lantern");
    }

    public static native void initialize(Object manager, int width, int height);
    public static native void frame(float dt, int width, int height, int[] area);
}
