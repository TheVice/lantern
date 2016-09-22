package org.lantern.examples;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class EmptyAppActivity extends Activity {

    private SurfaceView surfaceView;
    private Holder holder;
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        surfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        holder = new Holder();
        surfaceHolder.addCallback(holder);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
    }

}