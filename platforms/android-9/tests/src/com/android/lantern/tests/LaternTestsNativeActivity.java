package com.android.lantern.tests;

import android.app.NativeActivity;
import android.os.Bundle;

public class LaternTestsNativeActivity extends NativeActivity {

    /*static {
        System.loadLibrary("gnustl_shared");
    }*/

	protected void onCreate(Bundle savedInstanceState) {
		System.loadLibrary("gnustl_shared");
		super.onCreate(savedInstanceState);
	}
}