cmake_minimum_required (VERSION 3.10)
cmake_policy(VERSION 3.10)

project (ffmpeg_demo)	# cross-platform

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)
find_package(Iconv REQUIRED)
find_package(GLFW3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_CXX_LINK_FLAGS " \
 -framework Security \
 -framework CoreFoundation \
 -framework CoreVideo \
 -framework CoreMedia \
 -framework QuartzCore \
 -framework CoreFoundation \
 -framework VideoDecodeAcceleration \
 -framework Cocoa \
 -framework AudioToolbox \
 -framework VideoToolbox \
 -framework OpenCL")

###############################################Samples#######################################################
file(GLOB_RECURSE SampleFiles ${CMAKE_CURRENT_SOURCE_DIR}/demo/*.h ${CMAKE_CURRENT_SOURCE_DIR}/demo/*.cpp)
file(GLOB_RECURSE GladFiles ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include/glad/*.h ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include/KHR/*.h)

set(SamplerSrc ${SampleFiles} ${GladFiles})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image)
include_directories(${GLFW3_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SamplerSrc})

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LIBLZMA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${BZIP2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Iconv_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW3_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libavcodec.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libavdevice.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libavfilter.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libavformat.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libavutil.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libswresample.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/ffmpeg/lib/libswscale.a)

