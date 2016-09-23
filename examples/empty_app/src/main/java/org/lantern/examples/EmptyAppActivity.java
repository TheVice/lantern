package org.lantern.examples;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import org.lantern.Lantern;

import java.io.IOException;
import java.io.InputStream;

public class EmptyAppActivity extends Activity {

    private AssetManager manager;
    private SurfaceView surfaceView;
    private Holder holder;
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        manager = getAssets();
        Lantern.set_asset_manager(manager);

        /*try {

            InputStream stream = manager.open("webkit/youtube.html");
            byte[] buffer = new byte[4096];
            int readCount;

            while (-1 != (readCount = stream.read(buffer, 0, buffer.length))) {

                Log.d("Java", "" + readCount);
            }

            stream.close();

        } catch (IOException exc) {

            exc.printStackTrace();
        }*/

        surfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        holder = new Holder();
        surfaceHolder.addCallback(holder);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
    }

}
