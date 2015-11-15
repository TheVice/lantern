package org.lantern;

import android.os.Bundle;
import android.util.Log;

public class EmptyAppNativeActivity extends org.libsdl.app.SDLActivity {

    /** Called when the activity is first created. */
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // Load the .so
        System.loadLibrary("freetype");
        System.loadLibrary("empty_app");
        super.onCreate(savedInstanceState);
    }

}
