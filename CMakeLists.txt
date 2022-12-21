cmake_minimum_required(VERSION 3.10)

project(carrie_cotg)

#set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-g)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.66 REQUIRED)

IF(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
ELSE()
  message(FATAL "${CMAKE_SOURCE_DIR}/../include")
ENDIF(Boost_FOUND)

include_directories(SYSTEM BEFORE include)
include_directories(BEFORE .)

set(link_libs
  PRIVATE Threads::Threads
  PRIVATE OpenSSL::Crypto
  PRIVATE OpenSSL::SSL
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extrlibs/libtelegram)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extrlibs/libtelegram/include)
include_directories(/opt/homebrew/Cellar/boost/1.79.0_1/include)
include_directories(/opt/homebrew/opt/openssl/include)

add_executable(carrie src/carrie2.cpp src/logger.cpp)
add_executable(fromTG src/fromTG.cpp src/logger.cpp) 
add_executable(toTG src/toTG.cpp src/logger.cpp)

add_executable(fifo_reader src/fifo_reader.c src/logger.cpp) 
add_executable(fifo_writer src/fifo_writer.c src/logger.cpp)

target_link_libraries(fromTG ${link_libs})
target_link_libraries(toTG ${link_libs})
