cmake_minimum_required(VERSION 3.9)
# You can adjust the description for your project, Other changes to the project() command should NOT be made.
project(BebProject VERSION 3.0 DESCRIPTION "Starter project for C3 and R3s")

#[[

This is the CMake Master File for this project and should not be changed.

The list of source files and folders can be found in ProjectSourceFilesConfig.cmake which should be found alongside this file.

ProjectSourceFilesConfig.cmake is fully editable and should be updated to include a list of the source files that should be included in the project.

The only changes that should be made to this file are:
- to enable or disable the use of a libary by commenting or uncommenting the relevant set statment (e.g. for USE_CT2KLIB_LIBRARY)
- to change the description in the project command at the top

]]

# This is Really important and must NOT be changed. It should be the target processor for this project.
set(TARGETPROCESSOR "IMX28")

# include the project setup and list of source files.
set(ProjectSourceFiles_DIR ${CMAKE_SOURCE_DIR})
message("Including the list of Project Information and Source Files from ProjectSourceFilesConfig.Cmake Found here " ${ProjectSourceFiles_DIR})
find_package(ProjectSourceFiles REQUIRED)

# Setup CT2KLib if the Licence is Present. (If you do this without the licence you will have problems during your build processes)
 set(USE_CT2KLIB_LIBRARY "Y")
set(USE_UIGAUGE_LIBRARY "Y")

# Setup the simulator related includes, also add the Simulator related source files to the source file list.
set(SimulatorInclude_DIR ${CMAKE_SOURCE_DIR}/../../CMake)
message("Including the Simulator CMake File from " ${SimulatorInclude_DIR})
find_package(SimulatorInclude REQUIRED)

# Setup the target specific includes.
set(TargetInclude_DIR ${CMAKE_SOURCE_DIR}/../../CMake)
message("Including the Target CMake File from " ${TargetInclude_DIR})
find_package(TargetInclude REQUIRED)

# Setup the STM32
set(Stm32Include_DIR ${CMAKE_SOURCE_DIR}/../../CMake)
message("Including the STM32 CMake file from " ${Stm32Include_DIR})
find_package(Stm32Include REQUIRED)

# Setup the Common Includes
set(CommonInclude_DIR ${CMAKE_SOURCE_DIR}/../../CMake)
message("Including the Common CMake File from " ${CommonInclude_DIR})
find_package(CommonInclude REQUIRED)