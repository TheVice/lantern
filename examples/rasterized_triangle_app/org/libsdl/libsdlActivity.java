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

import java.lang.String;
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
	
	/** Called when the activity is first created. */
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        final String cacheDir = getCacheDir().getAbsolutePath();
        final File file = new File(cacheDir + "/resources");

        if (!file.exists()) {
        	file.mkdir();
        }
        
        final String pathToApk = getPathToApk();
        final ZipFile zipFile = openZipFile(pathToApk);
        final Vector<String> files = listZip(zipFile);

        Log.v("SDL", "Start file list");
		for(String fileName : files) {
			if (fileName.contains("resources/")) {
                Log.v("SDL", fileName);

                final int index = fileName.indexOf("resources/") + "resources".length();
                final String destination = file.getAbsolutePath() + fileName.substring(index);
                Log.v("SDL", destination);

                /*try {

                    extractEntryInto(zipFile, fileName, destination);
                } catch (RuntimeException exc) {

                    exc.printStackTrace();
                }*/
            }
		}
		Log.v("SDL", "End file list");

        closeZipFile(zipFile);

        super.onCreate(savedInstanceState);
    }

    public static void writeFile(InputStream source, String destination, int bufferLength) throws IOException {

        FileOutputStream fileOutputStream = null;
        try {

            fileOutputStream = new FileOutputStream(destination);
        } catch (FileNotFoundException exc) {

            exc.printStackTrace();
		    throw new RuntimeException("Unable to extract into " + destination);
        }

        byte buffer[] = new byte[bufferLength];
        int readed = 0;

        while (-1 != (readed = source.read(buffer))) {

            fileOutputStream.write(buffer, 0, readed);
        }

        fileOutputStream.close();
    }

    public static void writeFile(InputStream source, String destination) throws IOException {

        writeFile(source, destination, 4096);
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
    
	public static Vector<String> listZip(ZipFile zipFile) {

		Vector<String> files = new Vector<String>(1024);
		Enumeration<ZipEntry> listOfFiles = (Enumeration<ZipEntry>) zipFile.entries();
		while(listOfFiles.hasMoreElements()) {
			files.add(listOfFiles.nextElement().getName());
		}

		return files;
	}
	
	public static void extractEntryInto(ZipFile zipFile, String entryName, String destination) {
		
	    ZipEntry arEntry = null;
	    try {

	        arEntry = zipFile.getEntry(entryName);
	    } catch (IllegalStateException exc) {

	        exc.printStackTrace();
	        throw new RuntimeException("Unable to find entry " + entryName + " in zip file " + zipFile.getName());
	    }

        InputStream input = null;
        try {

            input = zipFile.getInputStream(arEntry);
        } catch (IOException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to get input stream for entry " + entryName);
        }

        try {

        	writeFile(input, destination);
        } catch (IOException exc) {

            exc.printStackTrace();
            throw new RuntimeException("Unable to extract entry " + entryName + " from " + zipFile.getName() + " into " + destination);
        }
        
        try {

            input.close();
        } catch (IOException exc) {

            exc.printStackTrace();
        }
	}

    public static void closeZipFile(ZipFile zipFile) {
	    try {
	
	        zipFile.close();
	    } catch (IOException exc) {
	
	        exc.printStackTrace();
	    }
	}
    
/*  @Override
    public void onCreate(Bundle savedInstanceState) {

        System.loadLibrary("rasterized_triangle_app");
    }
*/
}
