package org.lantern.examples;

import android.app.Activity;
import android.os.Bundle;

import org.lantern.Lantern;

public class EmptyAppActivity extends Activity {

    private int area[];

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        int width = 800;
        int height = 900;
        area = new int[width * height];

        Lantern.start(area, width, height);
    }

}
