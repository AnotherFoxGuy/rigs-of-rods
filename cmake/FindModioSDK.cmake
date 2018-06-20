# The MIT License (MIT)
#
# Copyright (c) 2018 Edgar (Edgar@AnotherFoxGuy.com)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Find ModioSDK
# ----------------
#
# Find ModioSDK include directories and libraries.
# This module will set the following variables:
#
# * ModioSDK_FOUND       - True if ModioSDK is found
# * ModioSDK_INCLUDE_DIR - The include directory
# * ModioSDK_LIBRARY     - The libraries to link against
#
# In addition the following imported targets are defined:
#
# * ModioSDK::ModioSDK
# * ModioSDK::use_namespace
#

find_path(ModioSDK_INCLUDE_DIR modio.h)
find_library(ModioSDK_LIBRARY modio PATH_SUFFIXES Debug Release)

set(ModioSDK_INCLUDE_DIRS ${ModioSDK_INCLUDE_DIR})
set(ModioSDK_LIBRARIES ${ModioSDK_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ModioSDK FOUND_VAR ModioSDK_FOUND
        REQUIRED_VARS ModioSDK_INCLUDE_DIRS ModioSDK_LIBRARIES
        )

if (ModioSDK_FOUND)
    add_library(ModioSDK::ModioSDK INTERFACE IMPORTED)
    set_target_properties(ModioSDK::ModioSDK PROPERTIES
            INTERFACE_LINK_LIBRARIES "${ModioSDK_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${ModioSDK_INCLUDE_DIRS}"
            )
    add_library(ModioSDK::use_namespace INTERFACE IMPORTED)
    set_target_properties(ModioSDK::use_namespace PROPERTIES INTERFACE_COMPILE_DEFINITIONS AS_USE_NAMESPACE)
endif ()

mark_as_advanced(ModioSDK_INCLUDE_DIR ModioSDK_LIBRARY)
