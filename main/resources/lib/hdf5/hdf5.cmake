##############################################################
# Written by Andrei Bondarenko, Striders                     #
##############################################################
# Based on following tutorial:                               #
# https://www.selectiveintellect.net/blog/2016/7/29/         #
# using-cmake-to-add-third-party-libraries-to-your-project-1 #
##############################################################

# Includes
include(ProcessorCount)
include(ExternalProject)

# Count processor cores
ProcessorCount(NCPU)

set(HDF5_PREFIX hdf5)

# Future proofing for possible windows support
if (WIN32)
	# Future proofing
	message(FATAL_ERROR "Windows not supported. Alter CMake files to play around yourself.")
else (WIN32)
	set(HDF5_MAKE make)
endif (WIN32)

# Define some vars to make commands below more readable
set(HDF5_CONFIG_CMD CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} CFLAGS=-w ${CMAKE_CURRENT_LIST_DIR}/configure)
#set(HDF5_CONFIG_CMD ${PROJECT_BINARY_DIR}/main/resources/lib/hdf5/${HDF5_PREFIX}/src/${HDF5_PREFIX}/configure)
set(HDF5_CONFIG_OPTIONS --prefix=${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-install --enable-cxx --disable-static)
#set(HDF5_CONFIG_OPTIONS --prefix=${PROJECT_BINARY_DIR}/main/resources/lib/hdf5/install --enable-cxx --disable-static)


# Build the HDF5 source
ExternalProject_Add(${HDF5_PREFIX}
		SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
		BINARY_DIR ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-build
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		TEST_COMMAND ""
#		CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} CFLAGS=-w ${HDF5_CONFIG_CMD} ${HDF5_CONFIG_OPTIONS}
#		BUILD_COMMAND  ${HDF5_MAKE} -j${NCPU}
#		INSTALL_COMMAND ${HDF5_MAKE} install
#		LOG_DOWNLOAD 1
#		LOG_BUILD 1
#		LOG_INSTALL 1
)

message(STATUS "")
message(STATUS "------> STARTED LOCAL HDF5 CONFIGURATION")
execute_process(
		COMMAND ${HDF5_CONFIG_CMD} ${HDF5_CONFIG_OPTIONS}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-build
		OUTPUT_VARIABLE hdf5_out ERROR_VARIABLE hdf5_out # Suppress configuration output
)
message(STATUS "<------ COMPLETED LOCAL HDF5 CONFIGURATION")
message(STATUS "------> STARTED LOCAL HDF5 BUILD")
execute_process(
		COMMAND make
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-build
		OUTPUT_VARIABLE hdf5_out ERROR_VARIABLE hdf5_out # Suppress build output
)
message(STATUS "<------ COMPLETED LOCAL HDF5 BUILD")
message(STATUS "------> STARTED LOCAL HDF5 INSTALL")
execute_process(
		COMMAND make install
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-build
		OUTPUT_VARIABLE hdf5_out ERROR_VARIABLE hdf5_out # Suppress install output
)
message(STATUS "<------ COMPLETED LOCAL HDF5 INSTALL")

add_library(lib${HDF5_PREFIX} SHARED IMPORTED GLOBAL)
add_library(lib${HDF5_PREFIX}-cpp SHARED IMPORTED GLOBAL)
add_library(lib${HDF5_PREFIX}-hl SHARED IMPORTED GLOBAL)
add_library(lib${HDF5_PREFIX}-hl-cpp SHARED IMPORTED GLOBAL)
set_target_properties(
		lib${HDF5_PREFIX} PROPERTIES
		IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-install/lib/libhdf5.dylib
)
set_target_properties(
		lib${HDF5_PREFIX}-cpp PROPERTIES
		IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-install/lib/libhdf5_cpp.dylib
)
set_target_properties(
		lib${HDF5_PREFIX}-hl PROPERTIES
		IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-install/lib/libhdf5_hl.dylib
)
set_target_properties(
		lib${HDF5_PREFIX}-hl-cpp PROPERTIES
		IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/${HDF5_PREFIX}-install/lib/libhdf5_hl_cpp.dylib
)
add_dependencies(libstride lib${HDF5_PREFIX} lib${HDF5_PREFIX}-cpp lib${HDF5_PREFIX}-hl lib${HDF5_PREFIX}-hl-cpp)


## Configure target for linking purposes
#set(prefix ${CMAKE_SHARED_LIBRARY_PREFIX})
#set(suffix ${CMAKE_SHARED_LIBRARY_SUFFIX})
#set(HDF5_LIB_DIR ${PROJECT_BINARY_DIR}/main/resources/lib/hdf5/install/lib)
#set(HDF5_CXX_LIBRARIES
#		"${HDF5_LIB_DIR}/${prefix}hdf5${suffix}"
#		"${HDF5_LIB_DIR}/${prefix}hdf5_cpp${suffix}"
#		)
#set(HDF5_CXX_HL_LIBRARIES
#		"${HDF5_LIB_DIR}/${prefix}hdf5_hl${suffix}"
#		"${HDF5_LIB_DIR}/${prefix}hdf5_hl_cpp${suffix}"
#		)
##link_directories(${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES})
##set_target_properties(libhdf5 PROPERTIES LINK_LIBRARIES ${HDF5_CXX_LIBRARIES})
#add_library(HDF5lib SHARED IMPORTED)
#message(STATUS "XXXXX ${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES} XXXXX")
#set_target_properties(HDF5lib PROPERTIES IMPORTED_LOCATION "${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES}")
#message(STATUS "XXXXX ${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES} XXXXX")