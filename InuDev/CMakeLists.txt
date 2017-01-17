cmake_minimum_required( VERSION 2.8 )
project( InuDev )

find_package(OpenCV REQUIRED)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

link_directories(${PROJECT_SOURCE_DIR}/lib/ )


add_executable( main main.cpp inuitive.cpp)

target_link_libraries(main 
	${OpenCV_LIBRARIES}
	libInuStreams.so 
	libCommonUtilities.so
)

