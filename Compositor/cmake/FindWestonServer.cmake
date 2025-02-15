# - Try to find Weston Compositor library.
# Once done this will define
#  WESTON_SERVER_FOUND - System has weston compositor
#  WESTON_SERVER_INCLUDE_DIRS - The weston compositor include directories
#  WESTON_SERVER_LIBRARIES    - The libraries needed to use weston compositor
#
#  WestonServer::WestonServer, the weston compositor
#
# Copyright (C) 2021 Metrological.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if(WestonServer_FIND_QUIETLY)
    set(_WESTON_SERVER_MODE QUIET)
elseif(WestonServer_FIND_REQUIRED)
    set(_WESTON_SERVER_MODE REQUIRED)
endif()

find_package(PkgConfig)
pkg_check_modules(PC_WESTON_SERVER ${_WESTON_SERVER_MODE} libweston-8)
pkg_check_modules(PC_WESTON_SERVER_LIBEXEC ${_WESTON_SERVER_MODE} weston)

find_library(WESTON_SERVER_LIB NAMES libweston-8.so
        HINTS ${PC_WESTON_SERVER_LIBDIR} ${PC_WESTON_SERVER_LIBRARY_DIRS})
find_library(WESTON_SERVER_DESKTOP_LIB libweston-desktop-8.so
        HINTS ${PC_WESTON_SERVER_LIBDIR} ${PC_WESTON_SERVER_LIBRARY_DIRS})
find_library(WESTON_SERVER_EXECLIB NAMES libexec_weston.so
        HINTS ${PC_WESTON_SERVER_LIBEXEC_LIBDIR}/weston ${PC_WESTON_SERVER_LIBEXEC_LIBRARY_DIRS}/weston)

set(WESTON_SERVER_LIBRARIES "${WESTON_SERVER_LIB}" "${WESTON_SERVER_EXECLIB}" "${WESTON_SERVER_DESKTOP_LIB}")

mark_as_advanced(WESTON_SERVER_INCLUDE_DIRS WESTON_SERVER_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WestonServer DEFAULT_MSG PC_WESTON_SERVER_LIBEXEC_FOUND WESTON_SERVER_LIBRARIES)

if(WestonServer_FOUND AND NOT TARGET WestonServer::WestonServer)
    add_library(WestonServer::WestonServer UNKNOWN IMPORTED)
    set_target_properties(WestonServer::WestonServer PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${WESTON_SERVER_EXECLIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${PC_WESTON_SERVER_INCLUDE_DIRS}"
            INTERFACE_COMPILE_OPTIONS "${WESTON_SERVER_CFLAGS_OTHER}"
            INTERFACE_LINK_LIBRARIES "${WESTON_SERVER_LIBRARIES}"
            )
else()
    if(WestonServer_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find Weston Server")
    elseif(NOT WestonServer_FIND_QUIETLY)
        message(STATUS "Could NOT find Weston Server")
    endif()
endif()
