package org.lantern.examples;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class RasterizedTriangleAppActivity extends Activity {

    private AssetManager manager;
    private SurfaceView surfaceView;
    private RasterizedTriangleAppHolder holder;
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        manager = getAssets();

        surfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        holder = new RasterizedTriangleAppHolder(manager);
        surfaceHolder.addCallback(holder);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
    }

}
