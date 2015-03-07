package com.android.lantern.rasterized_triangle_app;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ApplicationInfo;
import java.util.zip.ZipFile;
import java.io.IOException;
import java.lang.IllegalStateException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;

public class LaternRasterizedTriangleAppActivity extends Activity {
    private GLSurfaceView mGLView;

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("lanternrasterizedtriangle");
    }

    public static native void info(String message);
    public static native void changeDir(String dirname);
    public static native void makeDir(String dirname);

    public static void writeFile(InputStream source, String destination) {

        FileOutputStream fileOutputStream = null;
        try {
            fileOutputStream = new FileOutputStream(destination);

        } catch (FileNotFoundException exc) {

            exc.printStackTrace();
        }

        try {

            byte buffer[] = new byte[4096];
            int readed = 0;

            while (-1 != (readed = source.read(buffer))) {

                fileOutputStream.write(buffer, 0, readed);
            }

        } catch (IOException exc) {

            exc.printStackTrace();
        }

        try {

            fileOutputStream.close();
        } catch (IOException exc) {

            exc.printStackTrace();
        }
    }

    public String getPathToApk() {

        PackageManager packMgmr = getPackageManager();
        ApplicationInfo appInfo = null;

        try {

            appInfo = packMgmr.getApplicationInfo(getPackageName(), 0);
        } catch (NameNotFoundException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to locate APK");
        }

        return appInfo.sourceDir;
    }

    public void extractZipInto(String fileName, String entryName, String destination) {

        ZipFile zipFile = null;
        try {

            zipFile = new ZipFile(fileName);
        } catch (IOException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to open zip file " + fileName);
        }

        ZipEntry arEntry = null;
        try {

            arEntry = zipFile.getEntry(entryName);
        } catch (IllegalStateException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to find entry " + entryName + " in zip file " + fileName);
        }

        /*
         * try {
         * 
         * Enumeration<ZipEntry> listOfFiles =
         * (Enumeration<ZipEntry>)zipFile.entries();
         * 
         * while(listOfFiles.hasMoreElements()) {
         * 
         * info(listOfFiles.nextElement().getName()); } }
         * catch(IllegalStateException exc) {
         * 
         * exc.printStackTrace(); }
         */

        InputStream input = null;
        try {

            input = zipFile.getInputStream(arEntry);
        } catch (IOException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to get input stream for entry " + entryName);
        }

        writeFile(input, destination);

        try {

            input.close();
        } catch (IOException exc) {

            exc.printStackTrace();
        }

        try {

            zipFile.close();
        } catch (IOException exc) {

            exc.printStackTrace();
        }
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        String cacheDir = getCacheDir().getAbsolutePath();
        changeDir(cacheDir);
        makeDir(cacheDir + "/resources");

        String apkPath = getPathToApk();
        extractZipInto(apkPath, "assets/resources/triangle.obj", cacheDir + "/resources/triangle.obj");

        info("cacheDir = " + cacheDir + " apkPath = " + apkPath);

        mGLView = new GLSurfaceView(this);
        mGLView.setRenderer(new Graphics());
        setContentView(mGLView);
    }

    /** Called when the activity is becoming visible to the user. */
    @Override
    public void onStart() {

        super.onStart();
    }

    /**
     * Called after your activity has been stopped, prior to it being started
     * again.
     */
    @Override
    public void onRestart() {

        super.onRestart();
    }

    /** Called when the activity will start interacting with the user. */
    @Override
    public void onResume() {

        super.onResume();
        mGLView.onResume();
    }

    /** Called when the system is about to start resuming a previous activity. */
    @Override
    public void onPause() {

        super.onPause();
        mGLView.onPause();
    }

    /**
     * Called when the activity is no longer visible to the user, because
     * another activity has been resumed and is covering this one.
     */
    @Override
    public void onStop() {

        super.onStop();
    }

    /** The final call you receive before your activity is destroyed. */
    @Override
    public void onDestroy() {

        super.onDestroy();
    }
}
