# Initialize Project
macro (initialize_project)
	# Project
	set (PROJECT ${PROJECT_NAME})

	# Main Directories
	set (CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
	set (INCLUDE_DIR ${CURRENT_DIR}/Include)
	set (SOURCE_DIR ${CURRENT_DIR}/Source)
	set (EXTERN_DIR ${CURRENT_DIR}/Extern)
	set (TEST_DIR ${CURRENT_DIR}/Test)

	# List Initialization
	set (HEADER_FILES "")
	set (LIBRARY_FILES "")

	# Compiler Flags
	set (CMAKE_C_FLAGS_DEBUG "")
	set (CMAKE_C_FLAGS_RELEASE "")

	# Project Settings
	set (PROJECT_C_STANDARD 11)
	set (PROJECT_C_EXTENSIONS OFF)
	set (PROJECT_COMPILER_SCOPE PRIVATE)
endmacro ()

# Link Library
macro (link_library LIBRARY)
	target_link_libraries (${PROJECT} PUBLIC ${LIBRARY})
endmacro ()

# Add Test
macro (add_test SOURCE)
	add_executable (${SOURCE} ${TEST_DIR}/${SOURCE}.c)
	target_link_libraries (${SOURCE} PRIVATE ${PROJECT})
	configure_target (${SOURCE})
endmacro ()

# Add External Library
macro (add_external_library LIBRARY)
	add_subdirectory (${EXTERN_DIR}/${LIBRARY})
	include (${CURRENT_DIR}/CMake/Macros.cmake)
endmacro ()

# Add Header Library
macro (add_header_library LIBRARY)
	add_library (${LIBRARY} INTERFACE)
	target_sources (${LIBRARY} INTERFACE ${EXTERN_DIR}/${LIBRARY}.h)
	target_include_directories (${LIBRARY} SYSTEM INTERFACE ${EXTERN_DIR})
endmacro ()

# Add Header
macro (add_header HEADER)
	list (APPEND HEADER_FILES ${INCLUDE_DIR}/${PROJECT}/${HEADER}.h)
endmacro ()

# Add Source
macro (add_source_code SOURCE)
	list (APPEND LIBRARY_FILES ${SOURCE_DIR}/${SOURCE}.c)
endmacro ()

# Add Unit
macro (add_unit UNIT)
	add_header (${UNIT})
	add_source_code (${UNIT})
endmacro ()

# Create Library
macro (create_library)
	add_library (${PROJECT} STATIC ${HEADER_FILES} ${LIBRARY_FILES})
	target_include_directories (${PROJECT} PUBLIC ${INCLUDE_DIR})
	configure_target (${PROJECT})
endmacro ()

# Set C Standard
macro (set_c_standard TARGET STANDARD)
	set_property (TARGET ${TARGET} PROPERTY C_STANDARD ${STANDARD})
endmacro ()

# Set C Extensions
macro (set_c_extensions TARGET STATUS)
	if (${STATUS} STREQUAL ON)
		# Clang compiler option
		target_compile_options (${TARGET} PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
	endif ()

	set_property (TARGET ${TARGET} PROPERTY C_EXTENSIONS ${STATUS})
endmacro ()

# Configure compile options
macro (configure_compile_options TARGET SCOPE)
	# Clang compiler option
	target_compile_options (${TARGET} ${SCOPE} -Wno-newline-eof)

	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set_property (TARGET ${TARGET} PROPERTY WIN32_EXECUTABLE FALSE)
		target_compile_options (${TARGET} ${SCOPE} -O0 -g3 -Wall -Wextra -pedantic)
		target_compile_options (${TARGET} ${SCOPE} -Wnull-dereference -Wpointer-arith)
		target_compile_options (${TARGET} ${SCOPE} -Wdouble-promotion -Wcast-qual -Wshadow)
	endif ()

	if (CMAKE_BUILD_TYPE STREQUAL Release)
		set_property (TARGET ${TARGET} PROPERTY WIN32_EXECUTABLE TRUE)
		target_compile_options (${TARGET} ${SCOPE} -O3 -DNDEBUG)
	endif ()
endmacro ()

# Configure Target
macro (configure_target TARGET)
	set_c_standard (${TARGET} ${PROJECT_C_STANDARD})
	set_c_extensions (${TARGET} ${PROJECT_C_EXTENSIONS})
	configure_compile_options (${TARGET} ${PROJECT_COMPILER_SCOPE})
endmacro ()