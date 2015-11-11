package org.libsdl;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;
import android.util.Log;

public class libsdlActivity extends SDLActivity {

    /** Called when the activity is first created. */
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // Load the .so
        System.loadLibrary("empty_app");
        super.onCreate(savedInstanceState);
    }

}
