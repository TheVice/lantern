package org.lantern;

public class Lantern {

    static {
        System.loadLibrary("lantern");
    }

    public static native void start(int[] area, int width, int height);
}
