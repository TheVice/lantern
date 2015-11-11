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
import java.util.Vector;
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
        
        final String pathToApk = getPathToApk();
        Log.v("SDL", "pathToApk " + pathToApk);
        final ZipFile zipFile = openZipFile(pathToApk);
        Log.v("SDL", "Zip file opened " + pathToApk + " | " + zipFile.getName());
        
        Vector<String> files = listZip(zipFile);

        Log.v("SDL", "Start file list");
		for(String f : files) {
			Log.v("SDL", f);
		}
		Log.v("SDL", "End file list");

        closeZipFile(zipFile);
        Log.v("SDL", "Zip file closed " + pathToApk + " | " + zipFile.getName());
        
        super.onCreate(savedInstanceState);
    }

/*
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
*/
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

    public static ZipFile openZipFile(String fileName) {

        ZipFile zipFile = null;
        try {

            zipFile = new ZipFile(fileName);
        } catch (IOException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to open zip file " + fileName);
        }
        return zipFile;
    }
    
/*
        ZipEntry arEntry = null;
        try {

            arEntry = zipFile.getEntry(entryName);
        } catch (IllegalStateException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to find entry " + entryName + " in zip file " + fileName);
        }
*/
	public static Vector<String> listZip(ZipFile zipFile) {

		Vector<String> files = new Vector<String>(1024);
        /*
         * try {
         * 
         */	
		Enumeration<ZipEntry> listOfFiles = (Enumeration<ZipEntry>) zipFile.entries();
		while(listOfFiles.hasMoreElements()) {
			files.add(listOfFiles.nextElement().getName());
		}

		/*
		 * } catch(IllegalStateException exc) {
         * 
         * exc.printStackTrace(); }
         */
		return files;
	}
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
*/
    public static void closeZipFile(ZipFile zipFile) {
	    try {
	
	        zipFile.close();
	    } catch (IOException exc) {
	
	        exc.printStackTrace();
	    }
	}

    /** Called when the activity is first created. */
/*  @Override
    public void onCreate(Bundle savedInstanceState) {

        String cacheDir = getCacheDir().getAbsolutePath();

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
