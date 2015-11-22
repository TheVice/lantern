package org.lantern;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Build;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

public class EmptyAppActivity extends Activity
    implements SurfaceHolder.Callback {

    public SurfaceView surfaceView;
    public SurfaceHolder surfaceHolder;

    static {
        System.loadLibrary("empty_app");
    }

    public native void getImage4Activity(int[] inputColors, int width, int height);

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        surfaceView = (SurfaceView)findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_NORMAL);
    }

    @Override
    protected void onStart(){

        super.onStart();
    }

    @Override
    protected void onRestart(){

        super.onRestart();
    }

    @Override
    protected void onResume(){

        super.onResume();
    }

    @Override
    protected void onPause(){

        super.onPause();
    }

    @Override
    protected void onStop(){

        super.onStop();
    }

    @Override
    protected void onDestroy(){

        super.onDestroy();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(
            SurfaceHolder holder, int format, int width, int height) {

        try {

            final Canvas canvas =  holder.lockCanvas();
            canvas.drawBitmap(createBitmap(width, height), 0, 0, new Paint());
            holder.unlockCanvasAndPost(canvas);
        }
        catch (Exception exc) {

//            if (Build.VERSION.SDK_INT < 19) {
                ShowToast(exc.toString());
//            }
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public Bitmap createBitmap(int width, int height) {

        int colors[] = new int[width * height];

        getImage4Activity(colors, width, height);

        Bitmap bitmap = Bitmap.createBitmap(colors, 0, width, width, height, Bitmap.Config.ARGB_8888);
        return bitmap;
    }

    private void ShowToast(String message) {

        Context context = getApplicationContext();
        int duration = Toast.LENGTH_LONG;
        Toast toast = Toast.makeText(context, message, duration);
        toast.show();
    }

}
