project(bob_io_py${PYVER})

# This defines the dependencies of this package
set(bob_deps "bob_io;bob_python")
set(incdir ${py_incdir};${HDF5_CXX_INCLUDE_DIR})
set(shared "${bob_deps}")

# This defines the list of source files inside this package.
set(src
   "version.cc"
   "file.cc"
   "hdf5_extras.cc"
   "hdf5.cc"
   "datetime.cc"
   "main.cc"
   )
 
# If we have matio installed, enable the compilation of relevant modules
if(WITH_MATIO)
  list(APPEND incdir "${matio_INCLUDE_DIRS}")
endif()

if(NETPBM_FOUND)
  list(APPEND incdir "${NETPBM_INCLUDE_DIRS}")
endif()

if(JPEG_FOUND)
  list(APPEND incdir "${JPEG_INCLUDE_DIRS}")
endif()

if(PNG_FOUND)
  list(APPEND incdir "${PNG_INCLUDE_DIRS}")
endif()

if(TIFF_FOUND)
  list(APPEND incdir "${TIFF_INCLUDE_DIR}")
endif()

if(GIF_FOUND)
  list(APPEND incdir "${GIF_INCLUDE_DIR}")
endif()

if(WITH_FFMPEG)
  list(APPEND src "video.cc")
  list(APPEND incdir "${FFMPEG_INCLUDE_DIRS}")
  add_definitions("-D__STDC_CONSTANT_MACROS")
endif()

# Define the library, compilation and linkage options
bob_sort_headers(incdir)
foreach(inc ${incdir})
  include_directories(SYSTEM ${inc})
endforeach()
bob_add_library(${PROJECT_NAME} "${src}")
target_link_libraries(${PROJECT_NAME} ${shared})

# Pkg-Config generator
bob_pkgconfig(${PROJECT_NAME} "${bob_deps}")
