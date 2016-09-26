package org.lantern.examples;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.KeyEvent;

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

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {

            renderer.onKeyDown((short)'W');
            return true;
        } else if (keyCode == KeyEvent.KEYCODE_VOLUME_UP) {

            renderer.onKeyDown((short)'S');
            return true;
        } else {

            return super.onKeyDown(keyCode, event);
        }
    }

}
