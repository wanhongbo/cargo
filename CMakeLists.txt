#Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
#
# @file   CMakeLists.txt
# @author Jan Olszak (j.olszak@samsung.com)
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.6.2)
PROJECT(cargo)

IF(NOT DEFINED VERSION)
    SET(VERSION "0.1.2")
ENDIF(NOT DEFINED VERSION)

## pkgconfig ###################################################################
INCLUDE(FindPkgConfig)

## default CMAKE_INSTALL_* variables ###########################################
INCLUDE(GNUInstallDirs)

## Color output if it's possible:
IF (( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
      OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ))

    IF (CARGO_BUILD_FORCE_COMPILER_COLORS)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    ENDIF()
ENDIF()

## Compiler flags, depending on the build type #################################
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "DEBUG")
ENDIF(NOT CMAKE_BUILD_TYPE)

## Print build information #####################################################
MESSAGE(STATUS "-------------------------------------------------")
MESSAGE(STATUS "Compiler:          " ${CMAKE_CXX_COMPILER_ID})
MESSAGE(STATUS "Compiler version:  " ${CMAKE_CXX_COMPILER_VERSION})
MESSAGE(STATUS "Build type:        " ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "-------------------------------------------------")

# special case for a GCC < 4.7, assume rest is fine
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
    SET(CXX_11_STD "c++0x")
else()
    SET(CXX_11_STD "c++11")
endif()

SET(CMAKE_C_FLAGS_PROFILING    "-O0 -pg")
SET(CMAKE_CXX_FLAGS_PROFILING  "-std=${CXX_11_STD} -O0 -pg")
SET(CMAKE_C_FLAGS_DEBUG        "-O0 -ggdb")
SET(CMAKE_CXX_FLAGS_DEBUG      "-std=${CXX_11_STD} -O0 -ggdb")
SET(CMAKE_C_FLAGS_RELEASE      "-O2 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE    "-std=${CXX_11_STD} -O2 -DNDEBUG")
SET(CMAKE_C_FLAGS_CCOV         "-O0 --coverage")
SET(CMAKE_CXX_FLAGS_CCOV       "-std=${CXX_11_STD} -O0 --coverage")

IF(DEFINED SANITIZE)
    # Enable sanitize build.
    # It works with clang and gcc>=4.8
    # Possible arguments: address, thread and others (see doc.)
    # Note on thread sanitizer bugs left in out code:
    #  - we use non thread save boost test library
    #  - there are some mysterious problems with glib
    SET(SANITIZE_FLAGS "-fsanitize=${SANITIZE}")
    MESSAGE(STATUS "Sanitize flags: ${SANITIZE_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZE_FLAGS} -fPIE -fno-omit-frame-pointer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZE_FLAGS} -fPIE -fno-omit-frame-pointer")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie ${SANITIZE_FLAGS}")
ENDIF(DEFINED SANITIZE)

ADD_DEFINITIONS("-fPIC")   # Position Independent Code
ADD_DEFINITIONS("-Werror") # Make all warnings into errors
ADD_DEFINITIONS("-Wall")   # Generate all warnings
ADD_DEFINITIONS("-Wextra") # Generate even more extra warnings
ADD_DEFINITIONS("-pedantic") # Be pedantic
ADD_DEFINITIONS("-pedantic-errors") # Make pedantic warnings into errors
ADD_DEFINITIONS(-DPROGRAM_VERSION="${VERSION}")
ADD_DEFINITIONS(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # Warn about documentation problems
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdocumentation")

    IF(ALL_WARNINGS)
        # turn on every -W flags except a few explicitly mentioned
        ADD_DEFINITIONS("-Wno-error")
        ADD_DEFINITIONS("-Weverything")
        ADD_DEFINITIONS("-Wno-c++98-compat")
        ADD_DEFINITIONS("-Wno-c++98-compat-pedantic")
        ADD_DEFINITIONS("-Wno-padded")
        ADD_DEFINITIONS("-Wno-global-constructors")
        ADD_DEFINITIONS("-Wno-exit-time-destructors")
        ADD_DEFINITIONS("-Wno-weak-vtables")
    ENDIF(ALL_WARNINGS)
ENDIF()

IF(NOT WITHOUT_SYSTEMD)
    ADD_DEFINITIONS(-DHAVE_SYSTEMD)
ENDIF(NOT WITHOUT_SYSTEMD)

## Python packages directory ###################################################
IF(NOT DEFINED PYTHON_SITELIB)
    EXECUTE_PROCESS(COMMAND python -c
    "from distutils.sysconfig import get_python_lib; import sys; sys.stdout.write(get_python_lib())"
    OUTPUT_VARIABLE PYTHON_SITELIB)
ENDIF(NOT DEFINED PYTHON_SITELIB)

## Subdirectories ##############################################################
SET(COMMON_FOLDER ${PROJECT_SOURCE_DIR}/common)
SET(LIBS_FOLDER ${PROJECT_SOURCE_DIR}/libs)
SET(LOGGER_FOLDER ${PROJECT_SOURCE_DIR}/libs/logger)
SET(CARGO_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo)
SET(CARGO_JSON_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-json)
SET(CARGO_SQLITE_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-sqlite)
SET(CARGO_SQLITE_JSON_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-sqlite-json)
SET(CARGO_FD_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-fd)
SET(CARGO_GVARIANT_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-gvariant)
SET(CARGO_IPC_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-ipc)
SET(CARGO_VALIDATOR_FOLDER ${PROJECT_SOURCE_DIR}/libs/cargo-validator)
SET(TESTS_FOLDER ${PROJECT_SOURCE_DIR}/tests)


IF(NOT DEFINED SYSCONF_INSTALL_DIR)
    SET(SYSCONF_INSTALL_DIR "/etc")
ENDIF(NOT DEFINED SYSCONF_INSTALL_DIR)

IF(NOT DEFINED LIB_INSTALL_DIR)
    SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
ENDIF(NOT DEFINED LIB_INSTALL_DIR)

IF(NOT DEFINED INCLUDE_INSTALL_DIR)
    SET(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
ENDIF(NOT DEFINED INCLUDE_INSTALL_DIR)


IF(NOT DEFINED SCRIPT_INSTALL_DIR)
    SET(SCRIPT_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SBINDIR}")
ENDIF(NOT DEFINED SCRIPT_INSTALL_DIR)

IF(NOT DEFINED SYSTEMD_UNIT_DIR)
    SET(SYSTEMD_UNIT_DIR "${CMAKE_INSTALL_PREFIX}/lib/systemd/system")
ENDIF(NOT DEFINED SYSTEMD_UNIT_DIR)

IF(NOT DEFINED DATA_DIR)
    SET(DATA_DIR "${CMAKE_INSTALL_PREFIX}/share")
ENDIF(NOT DEFINED DATA_DIR)

IF(NOT DEFINED LIBEXEC_DIR)
    SET(LIBEXEC_DIR "${CMAKE_INSTALL_PREFIX}/libexec")
ENDIF(NOT DEFINED LIBEXEC_DIR)

IF(NOT DEFINED RUN_DIR)
    SET(RUN_DIR "/var/run")
ENDIF(NOT DEFINED RUN_DIR)

SET(CARGO_CONFIG_INSTALL_DIR ${SYSCONF_INSTALL_DIR}/cargo)

ADD_SUBDIRECTORY(${COMMON_FOLDER})
ADD_SUBDIRECTORY(${LOGGER_FOLDER})
ADD_SUBDIRECTORY(${CARGO_FOLDER})
ADD_SUBDIRECTORY(${CARGO_JSON_FOLDER})
ADD_SUBDIRECTORY(${CARGO_SQLITE_FOLDER})
ADD_SUBDIRECTORY(${CARGO_SQLITE_JSON_FOLDER})
ADD_SUBDIRECTORY(${CARGO_FD_FOLDER})
ADD_SUBDIRECTORY(${CARGO_GVARIANT_FOLDER})
ADD_SUBDIRECTORY(${CARGO_IPC_FOLDER})
ADD_SUBDIRECTORY(${CARGO_VALIDATOR_FOLDER})
ADD_SUBDIRECTORY(${TESTS_FOLDER})
