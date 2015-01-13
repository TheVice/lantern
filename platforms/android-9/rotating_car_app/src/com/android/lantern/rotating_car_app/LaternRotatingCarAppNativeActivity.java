package com.android.lantern.rotating_car_app;

import android.app.NativeActivity;
import android.os.Bundle;

public class LaternRotatingCarAppNativeActivity extends NativeActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		System.loadLibrary("gnustl_shared");
		super.onCreate(savedInstanceState);
	}
}