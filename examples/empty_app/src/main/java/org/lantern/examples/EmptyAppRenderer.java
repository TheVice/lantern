package org.lantern.examples;

import android.content.Context;
import android.graphics.Bitmap;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class EmptyAppRenderer extends GLSurfaceView
        implements GLSurfaceView.Renderer {

    private int area[];
    private int textures[];
    private Bitmap bitmap;

    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;

    private final float vertices[] = {

            -1.0f, -1.0f,
            +1.0f, -1.0f,
            -1.0f, +1.0f,
            +1.0f, +1.0f
    };

    private static final float texCoordinate[] = {

            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
    };

    public EmptyAppRenderer(Context context) {

        super(context);
    }

    /**
     * Called to draw the current frame.
     *
     * @param  gl javax.microedition.khronos.opengles.GL10
     * @see android.opengl.GLSurfaceView
     */
    @Override
    public void onDrawFrame(GL10 gl) {

        EmptyApp.frame(0.0f, 4 * bitmap.getWidth(), bitmap.getHeight(), area);
        bitmap = bitmap.copy(bitmap.getConfig(), true);
        bitmap.setPixels(area, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());

        gl.glVertexPointer(2, GL10.GL_FLOAT, 0, vertexBuffer);
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
        //gl.glColorPointer(4, GL10.GL_UNSIGNED_BYTE, 0, mColorBuffer);
        //gl.glEnableClientState(GL10.GL_COLOR_ARRAY);

        gl.glEnable(GL10.GL_TEXTURE_2D);
        gl.glEnable(GL10.GL_BLEND);

        gl.glBlendFunc(GL10.GL_ONE, GL10.GL_SRC_COLOR);

        gl.glBindTexture(GL10.GL_TEXTURE_2D, textures[0]);
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
        gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, textureBuffer);
        gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);

        gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
        //gl.glDisableClientState(GL10.GL_COLOR_ARRAY);
        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);

        gl.glBindTexture(GL10.GL_TEXTURE_2D, 0);

        /*gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        gl.glClearColor(0.392f, 0.584f, 0.929f, 1.0f);*/
    }

    /**
     * Called when the surface changed size.
     *
     * @param  gl javax.microedition.khronos.opengles.GL10
     * @param  width of surface that changed
     * @param  height of surface that changed
     * @see android.opengl.GLSurfaceView
     */
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

        area = new int[4 * width * height];
        EmptyApp.initialize(width, height);
        bitmap = Bitmap.createBitmap(area, 0, width, width, height, Bitmap.Config.ARGB_8888);

        gl.glViewport(0, 0, width, height);
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textures[0]);
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
        gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
        gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
        gl.glBindTexture(GL10.GL_TEXTURE_2D, 0);
    }

    /**
     * Called when the surface is created or recreated.
     *
     * @param  gl javax.microedition.khronos.opengles.GL10
     * @param  config javax.microedition.khronos.egl.EGLConfig
     * @see android.opengl.GLSurfaceView
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        textures = new int[1];
        gl.glGenTextures(1, textures, 0);

        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
        vbb.order(ByteOrder.nativeOrder());
        vertexBuffer = vbb.asFloatBuffer();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);

        ByteBuffer tbb = ByteBuffer.allocateDirect(texCoordinate.length * 4);
        tbb.order(ByteOrder.nativeOrder());
        textureBuffer = tbb.asFloatBuffer();
        textureBuffer.put(texCoordinate);
        textureBuffer.position(0);
    }

}
