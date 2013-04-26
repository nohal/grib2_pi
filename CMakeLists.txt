##---------------------------------------------------------------------------
## Author:      Dave Register
##***************************************************************************
## *   Copyright (C) 2010 by David S. Register                               *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   Free Software Foundation, Inc.,                                       *
## *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
## ***************************************************************************

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 OLD)
  CMAKE_POLICY(SET CMP0005 OLD)
  CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

PROJECT(grib2_pi)

SET(PACKAGE_NAME grib2_pi)
SET(PLUGIN_SOURCE_DIR .)
MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# require proper c++
#ADD_DEFINITIONS( "-Wall -ansi -pedantic -Wno-variadic-macros" )
#TODO: Should we use  -fno-stack-protector
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT WIN32)
 ADD_DEFINITIONS( "-Wall -O0 -fexceptions -fvisibility=hidden" )

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)

ENDIF(NOT WIN32)

# Add some definitions to satisfy MS
IF(WIN32)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(WIN32)

IF(NOT DEFINED wxWidgets_USE_FILE)
  SET(wxWidgets_USE_LIBS base core net xml html adv)
  SET(BUILD_SHARED_LIBS TRUE)
  FIND_PACKAGE(wxWidgets REQUIRED)
ENDIF(NOT DEFINED wxWidgets_USE_FILE)

INCLUDE(${wxWidgets_USE_FILE})

FIND_PACKAGE(Gettext REQUIRED)

IF(NOT WIN32)
  FIND_PACKAGE(Jasper REQUIRED)
ELSE(NOT WIN32)
	SET(SRC_JASPER
            src/libjasper/dummy.c
            src/libjasper/base/jas_cm.c
            src/libjasper/base/jas_debug.c
            src/libjasper/base/jas_getopt.c
            src/libjasper/base/jas_icc.c
            src/libjasper/base/jas_iccdata.c
            src/libjasper/base/jas_image.c
            src/libjasper/base/jas_init.c
            src/libjasper/base/jas_malloc.c
            src/libjasper/base/jas_seq.c
            src/libjasper/base/jas_stream.c
            src/libjasper/base/jas_string.c
            src/libjasper/base/jas_tmr.c
            src/libjasper/base/jas_tvp.c
            src/libjasper/base/jas_version.c

            src/libjasper/bmp/bmp_cod.c
            src/libjasper/bmp/bmp_cod.h
            src/libjasper/bmp/bmp_dec.c
            src/libjasper/bmp/bmp_enc.c
            src/libjasper/bmp/bmp_enc.h

            src/libjasper/jp2/jp2_cod.c
            src/libjasper/jp2/jp2_cod.h
            src/libjasper/jp2/jp2_dec.c
            src/libjasper/jp2/jp2_dec.h
            src/libjasper/jp2/jp2_enc.c

            src/libjasper/jpc/jpc_bs.c
            src/libjasper/jpc/jpc_bs.h
            src/libjasper/jpc/jpc_cod.h
            src/libjasper/jpc/jpc_cs.c
            src/libjasper/jpc/jpc_cs.h
            src/libjasper/jpc/jpc_dec.c
            src/libjasper/jpc/jpc_dec.h
            src/libjasper/jpc/jpc_enc.c
            src/libjasper/jpc/jpc_enc.h
            src/libjasper/jpc/jpc_fix.h
            src/libjasper/jpc/jpc_flt.h
            src/libjasper/jpc/jpc_math.c
            src/libjasper/jpc/jpc_math.h
            src/libjasper/jpc/jpc_mct.c
            src/libjasper/jpc/jpc_mct.h
            src/libjasper/jpc/jpc_mqcod.c
            src/libjasper/jpc/jpc_mqcod.h
            src/libjasper/jpc/jpc_mqdec.c
            src/libjasper/jpc/jpc_mqdec.h
            src/libjasper/jpc/jpc_mqenc.c
            src/libjasper/jpc/jpc_mqenc.h
            src/libjasper/jpc/jpc_qmfb.c
            src/libjasper/jpc/jpc_qmfb.h
            src/libjasper/jpc/jpc_t1cod.c
            src/libjasper/jpc/jpc_t1cod.h
            src/libjasper/jpc/jpc_t1dec.c
            src/libjasper/jpc/jpc_t1dec.h
            src/libjasper/jpc/jpc_t1enc.c
            src/libjasper/jpc/jpc_t1enc.h
            src/libjasper/jpc/jpc_t2cod.c
            src/libjasper/jpc/jpc_t2cod.h
            src/libjasper/jpc/jpc_t2dec.c
            src/libjasper/jpc/jpc_t2dec.h
            src/libjasper/jpc/jpc_t2enc.c
            src/libjasper/jpc/jpc_t2enc.h
            src/libjasper/jpc/jpc_tagtree.c
            src/libjasper/jpc/jpc_tagtree.h
            src/libjasper/jpc/jpc_tsfb.c
            src/libjasper/jpc/jpc_tsfb.h
            src/libjasper/jpc/jpc_util.c
            src/libjasper/jpc/jpc_util.h

#            src/libjasper/jpg/jpg_cod.h
#            src/libjasper/jpg/jpg_dec.c
#            src/libjasper/jpg/jpg_dummy.c
#            src/libjasper/jpg/jpg_enc.c
#            src/libjasper/jpg/jpg_enc.h
#            src/libjasper/jpg/jpg_jpeglib.h
#            src/libjasper/jpg/jpg_val.c

            src/libjasper/mif/mif_cod.c
            src/libjasper/mif/mif_cod.h

            src/libjasper/pgx/pgx_cod.c
            src/libjasper/pgx/pgx_cod.h
            src/libjasper/pgx/pgx_dec.c
            src/libjasper/pgx/pgx_enc.c
            src/libjasper/pgx/pgx_enc.h

            src/libjasper/pnm/pnm_cod.c
            src/libjasper/pnm/pnm_cod.h
            src/libjasper/pnm/pnm_dec.c
            src/libjasper/pnm/pnm_enc.c
            src/libjasper/pnm/pnm_enc.h

            src/libjasper/ras/ras_cod.c
            src/libjasper/ras/ras_cod.h
            src/libjasper/ras/ras_dec.c
            src/libjasper/ras/ras_enc.c
            src/libjasper/ras/ras_enc.h
    )
    ADD_LIBRARY(LIB_JASPER ${SRC_JASPER})

    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/libjasper/include)
ENDIF(NOT WIN32)

INCLUDE_DIRECTORIES(${JASPER_INCLUDE_DIR})

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out


SET(SRC_GRIB
    src/grib2.cpp
    src/grib2data.cpp
    src/grib2data.h
    src/grib2.h
    src/grib2_pi.cpp
    src/grib2_pi.h
    src/griboverlayfactory.cpp
    src/griboverlayfactory.h
    src/grib_reader.cpp
    src/grib_reader.h
    src/gui.cpp
    src/gui.h
    src/helper.cpp
    src/helper.h
    src/icons2.cpp
    src/icons2.h
    src/IsoLine2.cpp
    src/IsoLine2.h
    src/Sections/section0.cpp
    src/Sections/section0.h
    src/Sections/section1.cpp
    src/Sections/section1.h
    src/Sections/section2.cpp
    src/Sections/section2.h
    src/Sections/section3.cpp
    src/Sections/section3.h
    src/Sections/section4.cpp
    src/Sections/section4.h
    src/Sections/section5.cpp
    src/Sections/section5.h
    src/Sections/section6.cpp
    src/Sections/section6.h
    src/Sections/section7.cpp
    src/Sections/section7.h
)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_GRIB})

IF(WIN32)
  SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
  TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_JASPER ${OPENCPN_IMPORT_LIB} )
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${PARENT})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} ${JASPER_LIBRARIES})

IF(APPLE)
      INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/OpenCPN.app/Contents/PlugIns)
ENDIF(APPLE)

IF(UNIX AND NOT APPLE)
      IF(NOT DEFINED PREFIX_PLUGINS)
            SET(PREFIX_PLUGINS /usr/local/lib/opencpn)
      ENDIF(NOT DEFINED PREFIX_PLUGINS)

      INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME LIBRARY DESTINATION ${PREFIX_PLUGINS})
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")
ENDIF(WIN32)
 	  	 
INCLUDE("../PluginLocalization.cmake")
PLUGIN_LOCALIZATION()
