cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17) # C++17...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED text highgui)
if(NOT OPENCV_TEXT_FOUND)
  message(FATAL_ERROR "Failed to find OpenCV text module")
elseif(NOT OPENCV_HIGHGUI_FOUND)
  message(FATAL_ERROR "Failed to find OpenCV highgui module")
elseif(${OpenCV_VERSION_MAJOR} LESS 4)
  message(WARNING "Expect OpenCV4")
endif()

## Set our project name
project(tokidolocreng)

set(tokidolocreng_src 
    OCRServer.cpp
    OCR/OCREngine.cpp
    OCR/OCREngine.hpp
    OCR/OCRMain.cpp
    OCR/OCRMain.hpp
    OCR/OCRReadInfo.hpp
    Trimming/HttpException.hpp
    Trimming/ImageInfo.cpp
    Trimming/ImageInfo.hpp
    Trimming/TrimmingMain.cpp
    Trimming/TrimmingMain.hpp
    Trimming/Rect.hpp
)
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()
## Define the executable
add_executable(ocreng ${tokidolocreng_src})
add_executable(stoptool TerminateManager.cpp)
target_include_directories(ocreng PUBLIC ${OpenCV_INCLUDE_DIRS} json/include httplib)
target_include_directories(stoptool PUBLIC httplib)
target_link_libraries(ocreng Threads::Threads ${OpenCV_LIBS})
if(WIN32)
  target_link_libraries(ocreng Ws2_32)
  target_link_libraries(stoptool Ws2_32)
endif()