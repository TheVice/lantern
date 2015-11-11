package org.libsdl;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;
import android.util.Log;

public class libsdlActivity extends SDLActivity {
	
	// Load the .so
	static {
        System.loadLibrary("empty_app");
	}
	
    // Setup
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.v("SDL", "libsdlActivity::onCreate():" + mSingleton);
        super.onCreate(savedInstanceState);
    }

}
