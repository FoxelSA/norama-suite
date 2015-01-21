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

    /*! \file   norama-earth.h
     *  \author Nils Hamel <n.hamel@foxel.ch>
     *   
     *  Software main header
     */

    /*! \mainpage norama-rotate
     *
     *  \section norama-suite
     *  \section _ Panorama tools suite
     *
     *  This software suite offers tools for numerical panoramas manipulation.
     *  Mostly designed for equirectangular mappings, its role is to provide an
     *  interface to libgnomonic and its algorithms. It then offers an interface
     *  to projection and transformation features implemented in the gnomonic
     *  library. Il also comes with a panorama viewer to complete the suite.
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

    # ifndef __NR_EARTH__
    # define __NR_EARTH__

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
    # include <string.h>
    # include <opencv/cv.h>
    # include <opencv/highgui.h>
    # include <gnomonic-all.h>
    # include <csps-all.h>
    # include <common-all.h>

/* 
    Header - Preprocessor definitions
 */

    /* Standard help */
    # define NR_HELP "Usage summary :\n\n"                 \
    "\tnorama-earth [Arguments] [Parameters] ...\n\n"      \
    "Short arguments and parameters summary :\n\n"         \
    "\t-a\tInput equirectangular mapping image\n"          \
    "\t-b\tOutput equirectangular mapping image\n"         \
    "\t-p\tPath to CSPS directory structure\n"             \
    "\t-i\tIMU device tag\n"                               \
    "\t-s\tIMU device module\n"                            \
    "\t-u\tTimestamp second part\n"                        \
    "\t-v\tTimestamp micro-second part\n"                  \
    "\t-n\tInterpolation method\n"                         \
    "\t-t\tNumber of threads\n\n"                          \
    "norama-earth - norama-suite\n"                        \
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
     *  The main function starts by importing the input equirectangular mapping
     *  and allocating the output one. The query on CSPS is made by timestamp to
     *  retrive the mapping earth-attached orientation in matrix form. The 
     *  rotation is applied on the input mapping and stored in output mapping
     *  that is finally saved.
     *  
     *  \param argc Standard main parameter
     *  \param argv Standard main parameter
     */

    int main ( int argc, char ** argv );

    /*! \brief CSPS interface methods
     *
     *  This function performs a query through CSPS query interface to obtain
     *  rotation matrix of a camera capture pointed by the timestamp given as
     *  parameter according to earth local frame. The matrix provided as 
     *  parameter is filled with the CSPS query coefficients.
     *
     *  \param nrPath   Path to CSPS directory structure
     *  \param nrTag    Orientation device tag
     *  \param nrModule Orientation device module
     *  \param nrSecond Orientation query timestamp second part
     *  \param nrMicro  Orientation query timestamp micro-second part
     *  \param nrMatrix Returned rotation matrix
     */

    void nr_earth_matrix( 

        lp_Char_t const * const nrPath, 
        lp_Char_t const * const nrTag, 
        lp_Char_t const * const nrModule, 
        lp_Time_t const         nrSecond,
        lp_Time_t const         nrMicro,
        lp_Real_t               nrMatrix[3][3]

    );

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


