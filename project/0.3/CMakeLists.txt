cmake_minimum_required( VERSION 3.10 )
project ( myslam )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "-std=c++11  -O0" )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build )


############### dependencies ######################
# Eigen
include_directories( "/usr/include/eigen3" )
# OpenCV
find_package( OpenCV 3.1 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Sophus 
find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )
# G2O


#set(G2O_ROOT  /home/aiways/workspace/slambook/3rdparty/g2o/install)

find_package( G2O REQUIRED)

include_directories( ${G2O_INCLUDE_DIR} )



set( THIRD_PARTY_LIBS 
    ${OpenCV_LIBS}
    ${Sophus_LIBRARIES}
#    ${G2O_CORE_LIBRARY_DEBUG}
#    ${G2O_STUFF_LIBRARY_DEBUG}
#    ${G2O_TYPES_SBA_DEBUG}
    
    ${G2O_CORE_LIBRARY}
    ${G2O_STUFF_LIBRARY}
    ${G2O_TYPES_SBA}
)
############### dependencies ######################
include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( src )
add_subdirectory( test )

#message("INCLUDE_DIRECTORIES=" ${INCLUDE_DIRECTORIES})
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()
#
#message("LINK_DIRECTORIES=" ${LINK_DIRECTORIES})
#link_directories(/usr)
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()
#
#message("Sophus_INCLUDE_DIRS=" ${Sophus_INCLUDE_DIRS})
#message("Sophus_LIBRARIES=" ${Sophus_LIBRARIES})
