macro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)
    add_library(${target_name} SHARED ${target_source_files})
    target_include_directories(${target_name} SYSTEM PRIVATE ${target_include_dirs})
    target_link_libraries(${target_name} ${target_link_libs})

    set_target_properties(
        ${target_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${project_directory}/libs/${ANDROID_ABI})

    set(source_AndroidManifest ${CMAKE_SOURCE_DIR}/examples/${target_name}/AndroidManifest.xml)
    set(source_strings ${CMAKE_SOURCE_DIR}/examples/${target_name}/res/values/strings.xml)
    set(source_resources ${CMAKE_SOURCE_DIR}/examples/${target_name}/resources)
    set(source_sdl_java ${CMAKE_SOURCE_DIR}/SDL2/android-project/src/org)
    set(source_java ${CMAKE_SOURCE_DIR}/examples/${target_name}/org)

    if(EXISTS ${source_AndroidManifest})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_AndroidManifest} ${project_directory}/AndroidManifest.xml)
        execute_process(COMMAND ${ANDROID_EXECUTABLE} update project -n ${target_name} -t android-${ANDROID_NATIVE_API_LEVEL} -p ${project_directory})
        
        file(READ ${project_directory}/AndroidManifest.xml file_content)

        string(FIND ${file_content} "package=\"" index)
        if(-1 LESS ${index})
            math(EXPR index "9 + ${index}")
            string(SUBSTRING ${file_content} ${index} -1 package)
            string(FIND ${package} "\"" index)
            string(SUBSTRING ${package} 0 ${index} package)

            if((${package_name} STRLESS ${package}) OR (${package_name} STRGREATER ${package}))
                message(WARNING "Package name defined at ${project_directory}/AndroidManifest.xml - ${package} different than at come to this macros - ${package_name}")
            endif()
        else()
            message(WARNING "Package name not found at ${project_directory}/AndroidManifest.xml")
        endif()

        string(FIND ${file_content} "android:name=\"" index)
        if(-1 LESS ${index})
            math(EXPR index "14 + ${index}")
            string(SUBSTRING ${file_content} ${index} -1 activity)
            string(FIND ${activity} "\"" index)
            string(SUBSTRING ${activity} 0 ${index} activity)
            
            if((${activity_name} STRLESS ${activity}) OR (${activity_name} STRGREATER ${activity}))
                message(WARNING "Activity name defined at ${project_directory}/AndroidManifest.xml - ${activity} different than at come to this macros - ${activity_name}")
            endif()
        else()
            message(WARNING "Activity name not found at ${project_directory}/AndroidManifest.xml")
        endif()
    else()
        execute_process(COMMAND ${ANDROID_EXECUTABLE} create project -n ${target_name} -a ${activity_name} -k ${package_name} -t android-${ANDROID_NATIVE_API_LEVEL} -p ${project_directory})
    endif()

    if(EXISTS ${source_strings})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_strings} ${project_directory}/res/values/strings.xml)
    else()
        if(EXISTS ${project_directory}/res/values/strings.xml)
            file(READ ${project_directory}/res/values/strings.xml file_content)
            string(REPLACE "<string name=\"app_name\">${activity_name}</string>" "<string name=\"app_name\">${target_name}</string>" file_content "${file_content}")
            file(WRITE ${project_directory}/res/values/strings.xml "${file_content}")
        endif()
    endif()

    if(EXISTS ${source_sdl_java})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_sdl_java} ${project_directory}/src/org)
    endif()

    if(EXISTS ${source_java})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_java} ${project_directory}/src/org)
    endif()

    add_custom_target(${target_name}_copy-AndroidManifest
        DEPENDS ${target_name}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_AndroidManifest} ${project_directory}/AndroidManifest.xml
    )

    add_custom_target(${target_name}_copy-strings
        DEPENDS ${target_name}_copy-AndroidManifest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_strings} ${project_directory}/res/values/strings.xml
    )

    if(EXISTS ${source_resources})
        add_custom_target(${target_name}_copy-resources
            DEPENDS ${target_name}_copy-strings
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_resources} ${project_directory}/assets/resources
        )
        add_custom_target(${target_name}_copy-SDL-java
            DEPENDS ${target_name}_copy-resources
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_sdl_java} ${project_directory}/src/org
        )
    else()
        add_custom_target(${target_name}_copy-SDL-java
            DEPENDS ${target_name}_copy-strings
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_sdl_java} ${project_directory}/src/org
        )
    endif()

    add_custom_target(${target_name}_copy-java
        DEPENDS ${target_name}_copy-SDL-java
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_java} ${project_directory}/src/org
    )

    add_custom_target(${target_name}_build-apk
        DEPENDS ${target_name}_copy-java
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2.so
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2_image.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2_image.so
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libfreetype.so ${project_directory}/libs/${ANDROID_ABI}/libfreetype.so
        COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${project_directory}
    )

    add_custom_target(${target_name}_install-apk
        DEPENDS ${target_name}_build-apk
        COMMAND adb install -r ${project_directory}/bin/${target_name}-${ant_build_type}.apk
    )

    add_custom_target(${target_name}_run-apk
        DEPENDS ${target_name}_install-apk
        COMMAND adb shell am start -a android.intent.action.MAIN -n ${package_name}/.${activity_name}
    )

    add_custom_target(${target_name}_uninstall-apk
        COMMAND adb uninstall ${package_name}
    )
endmacro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)