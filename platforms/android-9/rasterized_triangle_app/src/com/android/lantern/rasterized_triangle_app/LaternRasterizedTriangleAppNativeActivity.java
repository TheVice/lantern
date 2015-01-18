package com.android.lantern.rasterized_triangle_app;

import android.app.NativeActivity;
import android.os.Bundle;

public class LaternRasterizedTriangleAppNativeActivity extends NativeActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		System.loadLibrary("gnustl_shared");
		super.onCreate(savedInstanceState);
	}
}