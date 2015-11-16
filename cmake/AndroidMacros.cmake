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

    if(release STREQUAL ${ant_build_type})
        if(EXISTS ${SDK_PATH}/build-tools/23.0.2/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/23.0.2/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/23.0.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/23.0.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/23.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/23.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/22.0.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/22.0.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/22.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/22.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.1.2/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.1.2/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.1.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.1.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.1.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.1.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.0.2/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.0.2/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.0.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.0.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/21.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/21.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/20.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/20.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/19.1.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/19.1.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/19.0.3/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/19.0.3/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/19.0.2/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/19.0.2/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/19.0.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/19.0.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/19.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/19.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/18.1.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/18.1.1/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/18.1.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/18.1.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/18.0.1/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/18.0.1/${ZIPALIGN_EXECUTABLE})
        # elseif(EXISTS ${SDK_PATH}/build-tools/18.0.0/${ZIPALIGN_EXECUTABLE})
        #     set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/18.0.0/${ZIPALIGN_EXECUTABLE})
        elseif(EXISTS ${SDK_PATH}/build-tools/17.0.0/${ZIPALIGN_EXECUTABLE})
            set(ZIPALIGN_EXECUTABLE ${SDK_PATH}/build-tools/17.0.0/${ZIPALIGN_EXECUTABLE})
        else()
            # message(FATAL_ERROR "Unable to found ${ZIPALIGN_EXECUTABLE}")
        endif()
        message(${ZIPALIGN_EXECUTABLE})
    endif()

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


    if(EXISTS ${source_resources})
        # execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_resources} ${project_directory}/assets/resources)
        add_custom_target(${target_name}_build-apk
            DEPENDS ${target_name}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_AndroidManifest} ${project_directory}/AndroidManifest.xml
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_strings} ${project_directory}/res/values/strings.xml
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_sdl_java} ${project_directory}/src/org
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_java} ${project_directory}/src/org
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_resources} ${project_directory}/assets/resources
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2.so
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2_image.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2_image.so
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libfreetype.so ${project_directory}/libs/${ANDROID_ABI}/libfreetype.so
            COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${project_directory}
        )
    else()
        add_custom_target(${target_name}_build-apk
            DEPENDS ${target_name}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_AndroidManifest} ${project_directory}/AndroidManifest.xml
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${source_strings} ${project_directory}/res/values/strings.xml
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_sdl_java} ${project_directory}/src/org
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${source_java} ${project_directory}/src/org
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2.so
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libSDL2_image.so ${project_directory}/libs/${ANDROID_ABI}/libSDL2_image.so
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_ABI}/libfreetype.so ${project_directory}/libs/${ANDROID_ABI}/libfreetype.so
            COMMAND ${ANT_EXECUTABLE} ${ant_build_type} -buildfile ${project_directory}
        )
    endif()

    if(release STREQUAL ${ant_build_type})
        if(EXISTS ${CMAKE_BINARY_DIR}/my-release-key.keystore)
            message("Found private key ${CMAKE_BINARY_DIR}/my-release-key.keystore")
        else()
            message("There is no private key ${CMAKE_BINARY_DIR}/my-release-key.keystore so let create it")
            execute_process(COMMAND keytool -genkey -v -keystore my-release-key.keystore -alias alias_name -keyalg RSA -keysize 2048 -validity 730 -dname "cn=, ou=, o=, c=" -keypass 123456 -storepass 123456)
        endif()
        add_custom_target(${target_name}_install-apk
            DEPENDS ${target_name}_build-apk
            COMMAND jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore my-release-key.keystore ${project_directory}/bin/${target_name}-${ant_build_type}-unsigned.apk alias_name -storepass 123456
            COMMAND jarsigner -verify -verbose -certs ${project_directory}/bin/${target_name}-${ant_build_type}-unsigned.apk
            COMMAND ${ZIPALIGN_EXECUTABLE} -f -v 4 ${project_directory}/bin/${target_name}-${ant_build_type}-unsigned.apk ${project_directory}/bin/${target_name}-${ant_build_type}.apk
            COMMAND ${ZIPALIGN_EXECUTABLE} -c -v 4 ${project_directory}/bin/${target_name}-${ant_build_type}.apk
            COMMAND adb install -r ${project_directory}/bin/${target_name}-${ant_build_type}.apk
        )
    else()
        add_custom_target(${target_name}_install-apk
            DEPENDS ${target_name}_build-apk
            COMMAND adb install -r ${project_directory}/bin/${target_name}-${ant_build_type}.apk
        )
    endif()

    add_custom_target(${target_name}_run-apk
        DEPENDS ${target_name}_install-apk
        COMMAND adb shell am start -a android.intent.action.MAIN -n ${package_name}/.${activity_name}
    )

    add_custom_target(${target_name}_uninstall-apk
        COMMAND adb uninstall ${package_name}
    )
endmacro(add_Android_Project target_name activity_name package_name project_directory target_source_files target_include_dirs target_link_libs)