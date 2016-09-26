package org.lantern.examples;

import android.app.Activity;
import android.os.Bundle;

public class EmptyAppActivity extends Activity {

    private EmptyAppRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        renderer = new EmptyAppRenderer(this);
        renderer.setRenderer(renderer);
        setContentView(renderer);
    }

}
