package com.android.lantern.empty_app;

import android.app.NativeActivity;
import android.os.Bundle;

public class LaternEmptyAppNativeActivity extends NativeActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		System.loadLibrary("gnustl_shared");
		super.onCreate(savedInstanceState);
	}
}