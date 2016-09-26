package org.lantern.examples;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.SurfaceHolder;

public class EmptyAppHolder implements SurfaceHolder.Callback {

    private int area[];

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        final Canvas canvas =  holder.lockCanvas();
        final int width = canvas.getWidth();
        final int height = canvas.getHeight();
        EmptyApp.initialize(width, height);
        area = new int[4 * width * height];
        holder.unlockCanvasAndPost(canvas);
    }

    @Override
    public void surfaceChanged(
            SurfaceHolder holder, int format, int width, int height) {

        EmptyApp.frame(0.0f, width, height, area);
        Bitmap bitmap = Bitmap.createBitmap(area, 0, width, width, height, Bitmap.Config.ARGB_8888);
        final Canvas canvas =  holder.lockCanvas();
        canvas.drawBitmap(bitmap, 0, 0, new Paint());
        holder.unlockCanvasAndPost(canvas);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

}
