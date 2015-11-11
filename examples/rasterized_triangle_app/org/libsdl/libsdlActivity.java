package org.libsdl;

import org.libsdl.app.SDLActivity;

import android.os.Bundle;
import android.util.Log;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ApplicationInfo;
import android.view.KeyEvent;

import java.util.zip.ZipFile;
import java.io.IOException;
import java.lang.IllegalStateException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.File;

public class libsdlActivity extends SDLActivity {
	
	// Load the .so
	static {
        System.loadLibrary("rasterized_triangle_app");
	}
	
    // Setup
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.v("SDL", "libsdlActivity::onCreate():" + mSingleton);
        super.onCreate(savedInstanceState);
    }

/*
    //public static native void changeDir(String dirname);

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
*/
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
/*
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
*/

    /** Called when the activity is first created. */
/*  @Override
    public void onCreate(Bundle savedInstanceState) {

        String cacheDir = getCacheDir().getAbsolutePath();
        // changeDir(cacheDir);
        // makeDir(cacheDir + "/resources");

        File file = new File(cacheDir + "/resources");
        file.mkdir();

        String apkPath = getPathToApk();
        extractZipInto(apkPath, "assets/resources/triangle.obj", cacheDir + "/resources/triangle.obj");
        extractZipInto(apkPath, "assets/resources/chess.png", cacheDir + "/resources/chess.png");
        extractZipInto(apkPath, "assets/resources/Ubuntu-L.ttf", cacheDir + "/resources/Ubuntu-L.ttf");

		super.onCreate(savedInstanceState);

        System.setProperty("user.dir", cacheDir);
        System.loadLibrary("rasterized_triangle_app");
    }
*/
}
