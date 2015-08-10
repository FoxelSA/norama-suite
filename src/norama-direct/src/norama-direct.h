/*
 * norama suite - Panorama tools suite
 *
 * Copyright (c) 2013-2015 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Nils Hamel <n.hamel@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

    /*! \file   norama-direct.h
     *  \author Nils Hamel <n.hamel@foxel.ch>
     *   
     *  Software main header
     */

    /*! \mainpage norama-direct
     *
     *  \section norama-suite
     *  \section _ Panorama tools suite
     *
     *  This software suite offers tools for numerical panoramas manipulation. 
     *  Mostly designed for equirectangular mappings, its role is to provide an
     *  interface to libgnomonic and its algorithms. It then offers an interface
     *  to projection and transformation features implemented in the gnomonic
     *  library. The norama-suite is also interfaced with libcsps in order to
     *  take advantage of camera motion traking to operate panoramas. Il also
     *  comes with a panorama viewer to complete the suite.
     *
     *  \section Documentation
     *
     *  A detailed documentation can be generated through doxygen. A more general
     *  documentation can be consulted at https://github.com/FoxelSA/norama-suite/wiki
     *
     *  \section Copyright
     * 
     *  Copyright (c) 2013-2015 FOXEL SA - http://foxel.ch \n
     *  This program is part of the FOXEL project <http://foxel.ch>.
     *  
     *  Please read the COPYRIGHT.md file for more information.
     *
     *  \section License
     *
     *  This program is licensed under the terms of the GNU Affero General Public
     *  License v3 (GNU AGPL), with two additional terms. The content is licensed
     *  under the terms of the Creative Commons Attribution-ShareAlike 4.0
     *  International (CC BY-SA) license.
     *
     *  You must read <http://foxel.ch/license> for more information about our
     *  Licensing terms and our Usage and Attribution guidelines.
     */

/* 
    Header - Include guard
 */

    # ifndef __NR_DIRECT__
    # define __NR_DIRECT__

/* 
    Header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/* 
    Header - Includes
 */

    # include <stdio.h>
    # include <stdlib.h>
    # include <opencv/cv.h>
    # include <opencv/highgui.h>
    # include <gnomonic-all.h>
    # include <common-all.h>

/* 
    Header - Preprocessor definitions
 */

    /* Standard help */
    # define NR_HELP "Usage summary :\n\n"                     \
    "\tnorama-direct [Arguments] [Parameters] ...\n\n"         \
    "Short arguments and parameters summary :\n\n"             \
    "\t-F\tForce input image in RGB format\n"                  \
    "\t-C\tClear output image initial content\n"               \
    "\t-N\tGeneric projection\n"                               \
    "\t-E\tElphel-specific projection\n"                       \
    "\t-T\tCentered-specific projection\n"                     \
    "\t-P\tApperture-specific projection\n"                    \
    "\t-i\tInput equirectangular image\n"                      \
    "\t-o\tOutput rectilinear image\n"                         \
    "\t-s\tOutput rectilinear image seed\n"                    \
    "\t-u\tApperture angle [°]\n"                              \
    "\t-x\tProjection x-sight in floating pixels\n"            \
    "\t-y\tProjection y-sight in floating pixels\n"            \
    "\t-a\tAzimuth angle [°] - rotation along z axis\n"        \
    "\t-d\tHeading angle [°] - azimuth correction\n"           \
    "\t-e\tElevation angle [°] - rotation along y axis\n"      \
    "\t-r\tRoll angle [°] - rotation along x axis\n"           \
    "\t-f\tRectilinear focal length [mm]\n"                    \
    "\t-p\tRectilinear pixel length [mm]\n"                    \
    "\t-R\tRed component of clear color\n"                     \
    "\t-G\tGreen component of clear color\n"                   \
    "\t-B\tBlue component of clear color\n"                    \
    "\t-k\tRectilinear output image width, in pixels\n"        \
    "\t-l\tRectilinear output image height, in pixels\n"       \
    "\t-W\tEntire equirectangular mapping width, in pixels\n"  \
    "\t-H\tEntire equirectangular mapping height, in pixels\n" \
    "\t-X\tEquirectangular tile x-position\n"                  \
    "\t-Y\tEquirectangular tile y-position\n"                  \
    "\t-t\tNumber of threads\n"                                \
    "\t-n\tInterpolation method\n"                             \
    "\t-q\tOutput image exportation options\n\n"               \
    "norama-direct - norama-suite\n"                           \
    "Copyright (c) 2013-2015 FOXEL SA - http://foxel.ch\n"

/* 
    Header - Preprocessor macros
 */

/* 
    Header - Typedefs
 */

/* 
    Header - Structures
 */

/* 
    Header - Function prototypes
 */

    /*! \brief Software main function
     *  
     *  The main function is responisble of input image loading and output image
     *  initialization. Then, according to the provided parameters, the main
     *  function calls the desired projection function. The output image is then
     *  exported.
     *  
     *  \param argc Standard main parameter
     *  \param argv Standard main parameter
     */

    int main ( int argc, char ** argv );

/* 
    Header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    Header - Include guard
 */

    # endif


