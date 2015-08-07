macro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)
    add_library(${target_name} SHARED ${target_source_files})
    target_include_directories(${target_name} SYSTEM PRIVATE ${target_include_dirs})
    target_link_libraries(${target_name} ${target_link_libs})

    execute_process(COMMAND ${ANDROID_EXECUTABLE} create project -n ${target_name} -a ${activity_name} -k ${package_name} -t android-${ANDROID_NATIVE_API_LEVEL} -p ${project_directory})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${project_directory}/res/layout)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${project_directory}/src/com)

    FILE(READ ${project_directory}/AndroidManifest.xml file_content)
    string(REPLACE "android:versionName=\"1.0\">" "android:versionName=\"1.0\">\n    <uses-sdk\n        android:minSdkVersion=\"${ANDROID_NATIVE_API_LEVEL}\" />" file_content ${file_content})
    string(REPLACE "<activity android:name=\"${activity_name}\"" "<activity android:name=\"android.app.NativeActivity\"" file_content ${file_content})
    string(REPLACE "android:label=\"@string/app_name\">" "android:label=\"@string/app_name\">\n            <meta-data android:name=\"android.app.lib_name\"\n                       android:value=\"${target_name}\" />" file_content ${file_content})
    string(REPLACE "</manifest> " "</manifest>" file_content ${file_content})
    FILE(WRITE ${project_directory}/AndroidManifest.xml ${file_content})

    FILE(READ ${project_directory}/res/values/strings.xml file_content)
    string(REPLACE "<string name=\"app_name\">${activity_name}</string>" "<string name=\"app_name\">${target_name}</string>" file_content ${file_content})
    FILE(WRITE ${project_directory}/res/values/strings.xml ${file_content})

    add_custom_target(${target_name}[build-apk]
        DEPENDS ${target_name}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBRARY_OUTPUT_PATH_ROOT}/libs ${project_directory}/libs
        COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${project_directory}
    )

    add_custom_target(${target_name}[install-apk]
        DEPENDS ${target_name}[build-apk]
        COMMAND adb install -r ${project_directory}/bin/${target_name}-${ant_build_type}.apk
    )

    add_custom_target(${target_name}[run-apk]
        DEPENDS ${target_name}[install-apk]
        COMMAND adb shell am start -a android.intent.action.MAIN -n ${package_name}/android.app.NativeActivity
    )

    add_custom_target(${target_name}[uninstall-apk]
        COMMAND adb uninstall ${package_name}
    )
endmacro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)