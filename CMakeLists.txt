cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR}/cmake/find-modules)

################################################################################
# Project definition
################################################################################
set(PROJECT_NAME_SHORT "RoR")
set(PROJECT_NAME_UNDERSCORE "Rigs_of_Rods")
set(PROJECT_NAME_LONG "Rigs of Rods")

set(REQUIRED_DEPS_VERSION 30)

project(${PROJECT_NAME_UNDERSCORE})

################################################################################
# Available build options
################################################################################
set(BUILD_DOC_DOXYGEN ON)

add_subdirectory(doc)