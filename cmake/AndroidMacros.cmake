macro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)
    add_library(${target_name} SHARED ${target_source_files})
    target_include_directories(${target_name} SYSTEM PRIVATE ${target_include_dirs})
    target_link_libraries(${target_name} ${target_link_libs})

    # string(REPLACE "." "/" java_path ${package_name})

    execute_process(COMMAND ${ANDROID_EXECUTABLE} create project -n ${target_name} -a ${activity_name} -k ${package_name} -t android-${ANDROID_NATIVE_API_LEVEL} -p ${project_directory})
    # execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${project_directory}/src/${java_path}/${activity_name}.java)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${project_directory}/res/layout)
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${project_directory}/src/com)

    add_custom_command(
        TARGET ${target_name}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/examples/${target_name}/org
        ${project_directory}/src/org)
    # execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/examples/${target_name}/org ${project_directory}/src/org)

    # FILE(READ ${project_directory}/src/${java_path}/libsdlActivity.java file_content)
    # string(REPLACE "package org.libsdl;" "package ${package_name};" file_content "${file_content}")
    # string(REPLACE "import org.libsdl.app.SDLActivity;" "import ${package_name}.app.SDLActivity;" file_content "${file_content}")
    # FILE(WRITE ${project_directory}/src/${java_path}/libsdlActivity.java "${file_content}")

    # FILE(READ ${project_directory}/src/${java_path}/app/SDLActivity.java file_content)
    # string(REPLACE "package org.libsdl.app;" "package ${package_name}.app;" file_content "${file_content}")
    # FILE(WRITE ${project_directory}/src/${java_path}/app/SDLActivity.java "${file_content}")

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/examples/AndroidManifest.xml ${project_directory}/AndroidManifest.xml)

    # FILE(READ ${project_directory}/AndroidManifest.xml file_content)
    # string(REPLACE "package=\"org.libsdl\"" "package=\"${package_name}\"" file_content "${file_content}")
    # FILE(WRITE ${project_directory}/AndroidManifest.xml "${file_content}")

    FILE(READ ${project_directory}/res/values/strings.xml file_content)
    string(REPLACE "<string name=\"app_name\">${activity_name}</string>" "<string name=\"app_name\">${target_name}</string>" file_content "${file_content}")
    FILE(WRITE ${project_directory}/res/values/strings.xml "${file_content}")

    add_custom_target(${target_name}_build-apk
        DEPENDS ${target_name}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBRARY_OUTPUT_PATH_ROOT}/libs ${project_directory}/libs
        COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${project_directory}
    )

    add_custom_target(${target_name}_install-apk
        DEPENDS ${target_name}_build-apk
        COMMAND adb install -r ${project_directory}/bin/${target_name}-${ant_build_type}.apk
    )

    add_custom_target(${target_name}_run-apk
        DEPENDS ${target_name}_install-apk
        COMMAND adb shell am start -a android.intent.action.MAIN -n org.libsdl/.libsdlActivity
    )

    add_custom_target(${target_name}_uninstall-apk
        COMMAND adb uninstall org.libsdl
    )
endmacro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)