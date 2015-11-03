cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

if (NOT ParaView_BINARY_DIR)
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
endif()

include(ParaViewPlugins)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# create a paraview plugin containing server manager xml and the server
# manager classes to build
# this plugin can be loaded on the server side

ADD_PARAVIEW_PLUGIN(ComputeVariables "1.0"
  SERVER_MANAGER_XML ComputeVariables.xml
  SERVER_MANAGER_SOURCES vtkComputeVariables.cpp vtkCylindricalCoordinates.cpp vtkCylindricalCoordinatesVector.cpp vtkRotatingFrame.cpp vtkCylindricalProjection.cpp vtkDuplication.cpp vtkMakeVector.cpp vtkCellLocatorInterpolator.cpp vtkConeSlice.cpp vtkComputeVariables2.cpp vtkRotatingFrame2.cpp vtkCylindricalProjection2.cpp vtkCylindricalCoordinates2.cpp)

