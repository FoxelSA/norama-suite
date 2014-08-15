/*
 * norama suite - Panorama tools suite
 *
 * Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch
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

    /*! \file   norama-tile2gnomo.h
     *  \author Nils Hamel (n.hamel@foxel.ch)
     *   
     *  Software main header (single function software)
     */

    /*! \mainpage norama-tile2gnomo
     *
     *  \section norama-suite
     *  \section _ Panorama tools suite
     *
     *  This software suite offers tools for numerical panoramas manipulation.
     *  Mainly based on equirectangular mappings, the norama-suite tools are each
     *  designed to solve a single task on panoramas, in order to maintain a good
     *  modularity of the suite. A typical example is given by rectilinear
     *  mappings extraction from a entire equirectangular mapped image.
     *
     *  \section Documentation
     *
     *  More documentation can be found on the wiki provided on the project github
     *  repository (https://github.com/FoxelSA/norama-suite/wiki).
     *
     *  \section Copyright
     * 
     *  Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch \n
     *  This program is part of the FOXEL project <http://foxel.ch>.
     *  
     *  Please read the COPYRIGHT.md file for more information.
     *
     *  \section License
     *
     *  This program is licensed under the terms of the GNU Affero General Public License
     *  v3 (GNU AGPL), with two additional terms. The content is licensed under the terms 
     *  of the Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA) license.
     *
     *  You must read <http://foxel.ch/license> for more information about our Licensing
     *  terms and our Usage and Attribution guidelines.
     */

/* 
    Header - Include guard
 */

    # ifndef __NR_EQR2GNOMO__
    # define __NR_EQR2GNOMO__

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
    # include <gnomonic-all.h>
    # include <opencv/cv.h>
    # include <opencv/highgui.h>
    # include "common-stdap.h"

/* 
    Header - Preprocessor definitions
 */

    /* Standard help */
    # define NR_HELP "norama-tile2gnomo - norama-suite :\n\n"                           \
                     "    norama-tile2gnomo [arguments] [parameters] ...\n\n"           \
                     "Arguments and parameters are :\n\n"                               \
                     "    -e equirectangular tile image\n"                              \
                     "    -r rectilinear image\n"                                       \
                     "    -w rectilinear image width\n"                                 \
                     "    -t rectilinear image height\n"                                \
                     "    -f full equirectangular image width\n"                        \
                     "    -g full equirectangular image height\n"                       \
                     "    -x position of tile in full equirectangular image\n"          \
                     "    -y position of tile in full equirectangular image\n"          \
                     "    -c gnomonic center x-position in the tile image\n"            \
                     "    -d gnomonic center y-position in the tile image\n"            \
                     "    -i interpolation method\n\n"                                  \
                     "Copyright :\n\n"                                                  \
                     "    Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch\n\n"

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

    /*! \brief Software main function (single function software)
     *  
     *  The main function frame follows : parameters are initialized and read. The
     *  equirectangular tile is imported and the gnomonic image allocation is
     *  created. The gnomonic projection is performed according to parameters and
     *  the result is exported.
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


