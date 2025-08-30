cmake_minimum_required(VERSION 4.0)
project(the_colonizer)

set(CMAKE_CXX_STANDARD 20)

add_executable(the_colonizer main.cpp
        World.cpp
        lib.h
        Tile.cpp
        Tile.h
)

# Linux/macOS: use pkg-config (system packages)
if(UNIX AND NOT WIN32)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2MIXER REQUIRED SDL2_mixer)

    target_include_directories(the_colonizer PRIVATE
            ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} "embeded_img" "embeded_sound"
    )
    target_link_directories(the_colonizer PRIVATE
            ${SDL2_LIBRARY_DIRS} ${SDL2IMAGE_LIBRARY_DIRS} ${SDL2MIXER_LIBRARY_DIRS}
    )
    target_link_libraries(the_colonizer PRIVATE
            ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES}
    )

# Windows: use vcpkg (cleanest)
elseif(WIN32)
    # Configure CMake with the vcpkg toolchain file:
    # cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    target_link_libraries(the_colonizer PRIVATE SDL2::SDL2  SDL2::SDL2main SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)
    target_include_directories(the_colonizer PRIVATE "embeded_sound" "embeded_img")
endif()