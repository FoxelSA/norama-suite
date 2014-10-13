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

    /*! \file   norama-gnomo2eqr.h
     *  \author Nils Hamel <n.hamel@foxel.ch>
     *   
     *  Software main header (single function software)
     */

    /*! \mainpage norama-gnomo2eqr
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

    # ifndef __NR_GNOMO2EQR__
    # define __NR_GNOMO2EQR__

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

/* 
    Header - Preprocessor definitions
 */

    /* Standard help */
    # define NR_HELP "Usage summary :\n\n"                            \
    "\tnorama-gnomo2eqr [Arguments] [Parameters] ...\n\n"             \
    "Short arguments and parameters summary :\n\n"                    \
    "\t-e\tEquirectangular image\n"                                   \
    "\t-m\tRectilinear mask image\n"                                  \
    "\t-r\tRectilinear image\n"                                       \
    "\t-o\tOutput equirectangular image\n"                            \
    "\t-u\tGnomonic projection center horizontal angle (degrees)\n"   \
    "\t-v\tGnomonic projection center vertical angle (degrees)\n"     \
    "\t-a\tGnomonic projection horizontal half apperture (degrees)\n" \
    "\t-b\tGnomonic projection vertical half apperture (degrees)\n"   \
    "\t-i\tInterpolation method\n\n"                                  \
    "norama-gnomo2eqr - norama-suite\n"                               \
    "Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch\n"

    /* Define standard types */
    # define NR_NULL        0
    # define NR_STRING      1
    # define NR_CHAR        2
    # define NR_SHORT       3
    # define NR_INT         4
    # define NR_LONG        5
    # define NR_LLONG       6
    # define NR_UCHAR       7
    # define NR_USHORT      8
    # define NR_UINT        9
    # define NR_ULONG      10
    # define NR_ULLONG     11
    # define NR_FLOAT      12
    # define NR_DOUBLE     13

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
     *  The main function frame follows : parameters are initialized and read.
     *  The input images are read and the inverted gnomonic projection is
     *  performed. The result is written in the input equirectangular image
     *  allocation. The result image is the exported.
     *  
     *  \param argc Standard main parameter
     *  \param argv Standard main parameter
     */

    int main ( int argc, char ** argv );

    /*! \brief Arguments common handler
     *  
     *  This function searches in the argv string array the position of the
     *  argument defined through ltag/stag and returns the detected index.
     *  
     *  \param  argc    Standard main parameter
     *  \param  argv    Standard main parameter
     *  \param  ltag    Long-form argument string
     *  \param  stag    Short-form argument string
     *
     *  \return         Returns index of parameter in argv
     */

    int stda ( int argc, char ** argv, char const * const ltag, char const * const stag );

    /*! \brief Parameters common handler
     *  
     *  This function interprets the parameter in the desired type and returns
     *  it through the param variable. The argi variable is typically set using
     *  stda function. If argi is set to CS_NULL, the function does nothing.
     *  
     *  \param argi     Index of the parameter in argv
     *  \param argv     Standard main parameter
     *  \param param    Pointer to the variable that recieve the interpreted
     *                  parameter
     *  \param type     Type to use for parameter interpretation
     */

    void stdp ( int argi, char ** argv, void * const param, int const type );

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


