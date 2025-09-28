cmake_minimum_required(VERSION 3.10)
project(treeseg_gui)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(VTK COMPONENTS CommonCore RenderingOpenGL2 GUISupportQt REQUIRED)
find_package(PCL 1.12 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

set(VTK_RENDERING_BACKEND "OpenGL2" CACHE STRING "Rendering backend" FORCE)
add_executable(treeseg_gui treeseg_gui.cpp)
target_link_libraries(treeseg_gui PRIVATE Qt5::Widgets ${VTK_LIBRARIES} ${PCL_LIBRARIES})
