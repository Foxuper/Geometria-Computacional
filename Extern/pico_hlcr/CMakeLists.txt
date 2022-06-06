# Minimum CMake Version
cmake_minimum_required (VERSION 3.20.0)

# Project Name
project (pico_hlcr)

# Include Macros
include (CMake/Macros.cmake)

# Initialize Project
initialize_project ()

# The project requires GNU Extensions
set (PROJECT_C_EXTENSIONS ON)

# Main Library Files
add_unit (${PROJECT})

# Classes
add_unit (Class/Tuple)
add_unit (Class/Ptr)
add_unit (Class/Ref)
add_unit (Class/Int)
add_unit (Class/Float)
add_unit (Class/String)
add_unit (Class/Function)
add_unit (Class/Array)
add_unit (Class/List)
add_unit (Class/Table)
add_unit (Class/Range)
add_unit (Class/Filter)
add_unit (Class/Map)
add_unit (Class/Zip)
add_unit (Class/Slice)
add_unit (Class/File)
add_unit (Class/Thread)
add_unit (Class/Mutex)
add_unit (Class/Exception)

# Interfaces
add_source_code (Interface/Size)
add_source_code (Interface/Alloc)
add_source_code (Interface/New)
add_source_code (Interface/Assign)
add_source_code (Interface/C_Types)
add_source_code (Interface/Compare)
add_source_code (Interface/Hash)
add_source_code (Interface/Len)
add_source_code (Interface/Resize)
add_source_code (Interface/Call)
add_source_code (Interface/Current)
add_source_code (Interface/Run)
add_source_code (Interface/Lock)
add_source_code (Interface/Context)
add_source_code (Interface/Iter)
add_source_code (Interface/Cast)
add_source_code (Interface/Concat)
add_source_code (Interface/Sublist)
add_source_code (Interface/Access)
add_source_code (Interface/Content)
add_source_code (Interface/Insert)
add_source_code (Interface/Pointer)
add_source_code (Interface/IO)
add_source_code (Interface/Format)
add_source_code (Interface/Stream)

# Create Library
create_library ()

# External Libraries
find_package (Threads REQUIRED)
add_header_library (pico_types)

# Linking External Libraries
link_library (Threads::Threads)
link_library (pico_types)

# Project Tests
add_test (Test)
add_test (Test2)