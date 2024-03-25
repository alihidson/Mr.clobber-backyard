
cmake_minimum_required(VERSION 3.26)
project(final_project C)

set(CMAKE_C_STANDARD 23)

# Set the path to Allegro installation
set(ALLEGRO_PATH "/opt/homebrew/cellar/allegro/5.2.9.0")

# Add Allegro include directories
include_directories(${ALLEGRO_PATH}/include)

# Create the executable
add_executable(final_project main.c)

# Link against Allegro libraries
target_link_libraries(final_project PRIVATE ${ALLEGRO_PATH}/lib/liballegro.dylib ${ALLEGRO_PATH}/lib/liballegro_main.dylib ${ALLEGRO_PATH}/lib/liballegro_primitives.dylib
        ${ALLEGRO_PATH}/lib/liballegro_font.dylib ${ALLEGRO_PATH}/lib/liballegro_image.dylib ${ALLEGRO_PATH}/lib/liballegro_ttf.dylib)

# Set the RPATH for the executable
set_target_properties(final_project PROPERTIES INSTALL_RPATH "@executable_path/../lib")

# Ensure the RPATH is used during installation
set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
