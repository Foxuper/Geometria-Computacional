# Minimum CMake Version
cmake_minimum_required (VERSION 3.20.0)

# Project Name
project (Main)

# Include Macros
include (CMake/Macros.cmake)

# Initialize Project
initialize_project ()

# The project requires GNU Extensions
set (PROJECT_C_EXTENSIONS ON)

# External Libraries
add_external_library (pico_hlcr)
add_external_library (raylib)
add_header_library   (raygui)
add_header_library   (dark_theme)
add_header_library   (light_theme)
add_header_library   (pico_glm)

# Library Files
add_unit (Principal)
add_unit (General)
add_unit (Triangulacion)
add_unit (Convexo2D)
add_unit (Convexo3D)
add_unit (Delaunay)

# Create Project
create_project ()

# Link Libraries
link_library (pico_hlcr)
link_library (raylib)
link_library (raygui)
link_library (dark_theme)
link_library (light_theme)
link_library (pico_glm)