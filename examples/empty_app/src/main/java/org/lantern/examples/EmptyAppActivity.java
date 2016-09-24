package org.lantern.examples;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class EmptyAppActivity extends Activity {

    private AssetManager manager;
    private SurfaceView surfaceView;
    private EmptyAppHolder holder;
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        manager = getAssets();
        EmptyApp.set_asset_manager(manager);

        surfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        holder = new EmptyAppHolder();
        surfaceHolder.addCallback(holder);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
    }

}
