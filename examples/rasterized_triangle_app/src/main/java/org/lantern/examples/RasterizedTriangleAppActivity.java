package org.lantern.examples;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class RasterizedTriangleAppActivity extends Activity {

    private AssetManager manager;
    private RasterizedTriangleAppRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        manager = getAssets();
        renderer = new RasterizedTriangleAppRenderer(this, manager);
        renderer.setRenderer(renderer);
        setContentView(renderer);
    }

}
