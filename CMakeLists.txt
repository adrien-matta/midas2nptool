cmake_minimum_required (VERSION 2.8)
include(CheckCXXCompilerFlag)

project (midas2nptool)
set (M2N_VERSION_MAJOR 1)
set (M2N_VERSION_MINOR 0)
set (M2N_VERSION_DETS 0)

set(CMAKE_BUILD_TYPE Release)

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# This suppress the Up-to-Date message of file installed for cmake 3.1 and above
set(CMAKE_INSTALL_MESSAGE LAZY) 

# Set the Search path for library
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Finding NPLib
find_package(NPLib)
include(${NPLib_USE_FILE})
link_directories(${NPLIB}/lib)
configure_file(Core/M2NVersion.h.in Core/M2NVersion.h @ONLY) 

# look for Root
include("ressources/CMake/Root.cmake")

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
  if( UNIX AND NOT APPLE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  endif()
endif()

# Adjust the compiler language flag
set(NOCPPFLAGS true)
if(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*11")
  set(NOCPPFLAGS false)
elseif(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*0x")
  set(NOCPPFLAGS false) 
endif()

if(NOCPPFLAGS)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  if(COMPILER_SUPPORTS_CXX11)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  endif()

  if(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()


# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics -Wno-deprecated-register -Wno-shadow")
endif()

set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/include )

#include
include_directories("Core")
# compilation
add_subdirectory("Core") 
add_subdirectory("Detectors") 
add_subdirectory("Utility") 

add_executable(midas2nptool midas2nptool.cxx)
target_Link_libraries(midas2nptool ${ROOT_LIBRARIES} M2NCore)

install(PROGRAMS midas2nptool DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
configure_file(scripts/m2n_folder.sh "${CMAKE_BINARY_OUTPUT_DIRECTORY}/m2n_folder" @ONLY)
install(SCRIPT ./scripts/post_install.cmake)
