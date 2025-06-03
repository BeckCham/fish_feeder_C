cmake_minimum_required(VERSION 3.25)
project(2024_2025_fish_C C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories (
		# jni.h header file location
		# jni_md.h header file location
		
        # macos
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/include
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/include/darwin
)

add_executable(2024_2025_fish_C main.c fish.c fish.h
        splashScreenImagePt1.h
        splashScreenImagePt1.c
        splashScreenImagePt1.c
        displayScreens.h
        displayScreens.c
        operatingMode.h
        operatingMode.c
        menus.c
        menus.h
        menusFunctions.c
        menusFunctions.h
        programStartup.c
        programStartup.h
        programShutdown.c
        programShutdown.h
        splashScreenImagePt2.c
        splashScreenImagePt2.h
        splashScreenImagePt3.c
        splashScreenImagePt3.h
        splashScreenImagePt4.c
        splashScreenImagePt4.h
)

target_link_libraries(2024_2025_fish_C)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC

        # macos
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/lib/libjli.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/lib/server/libjvm.dylib

)
set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/lib;${CMAKE_CURRENT_SOURCE_DIR}/FishFeederGUI/customjre/lib/server"
        BUILD_WITH_INSTALL_RPATH TRUE
)
target_compile_options(2024_2025_fish_C PRIVATE -Wall -Wextra ) #Compiles with error warnings
