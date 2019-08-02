#
# Created by $melodic on 19-6-13.
#

cmake_minimum_required( VERSION 3.0 )
project( quaternionupdate )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-O3" )


include_directories( "/usr/include/eigen3" )

find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )


add_executable( update quaternionupdate.cpp )

target_link_libraries( update ${Sophus_LIBRARIES} )