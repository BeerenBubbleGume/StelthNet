cmake_minimum_required(VERSION 3.24)
project(goodbyedpi_plus)

set(CMAKE_CXX_STANDARD 23)

add_executable(goodbyedpi_plus main.cpp src/utils/Driver.cpp src/utils/Driver.h src/EventHandler.cpp src/EventHandler.h src/DPI.cpp src/DPI.h)


set(LOCAL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib/)
find_package(PkgConfig REQUIRED)
pkg_check_modules(EVENT REQUIRED IMPORTED_TARGET libevent)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LOCAL_LIB_DIR}/libpcap.dylib
                                            PkgConfig::EVENT)