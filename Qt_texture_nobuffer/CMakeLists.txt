cmake_minimum_required(VERSION 2.8)

project(texture_nobuffer)

file(GLOB qt_path /opt/Qt*)

if(EXISTS ${qt_path})
  set(CMAKE_PREFIX_PATH ${qt_path}/5.6/gcc_64/lib/cmake)
  message("-- CMAKE_PREFIX_PATH is " ${qt_path}/5.6/gcc_64/lib/cmake)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(OpenGL)
find_package(Qt5 QUIET COMPONENTS Core Xml OpenGL Gui Widgets)

set (QtRENDERING3D_CPP mainwindow.cpp openglwidget.cpp)
set (QtRENDERING3D_HEADER mainwindow.h openglwidget.h)
set (QtRENDERING3D_UI  mainwindow.ui)
set (QtRENDERING3D_MAIN main.cpp)

if(Qt5_FOUND)
  QT5_WRAP_UI(UI_HEADERS ${QtRENDERING3D_UI})
  QT5_WRAP_CPP(UI_SOURCES ${QtRENDERING3D_HEADER})
  QT5_ADD_RESOURCES(SHADER_RSC_ADDED resources.qrc)

  include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}
)
  set(QtRENDERING3D_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES}
)
endif()

add_library(qttextureshader_library 
  ${QtRENDERING3D_HEADER} ${QtRENDERING3D_CPP}
  ${UI_HEADERS} ${UI_SOURCES}
)

add_executable(qttextureshader ${QtRENDERING3D_MAIN}  ${SHADER_RSC_ADDED})

target_link_libraries(qttextureshader_library ${QGLVIEWER_LIBRARY} ${QtRENDERING3D_LIBRARIES} ${OPENGL_LIBRARY})
target_link_libraries(qttextureshader qttextureshader_library)
