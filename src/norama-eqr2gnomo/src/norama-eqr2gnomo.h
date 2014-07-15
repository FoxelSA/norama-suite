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

    //! @file   norama-eqr2gnomo.h
    //! @author Nils Hamel (nils.hamel@foxel.ch)
    //! 
    //! Software main header.

/* 
    Header - Include guard
 */

    # ifndef __NORAMA_EQR2GNOMO__
    # define __NORAMA_EQR2GNOMO__

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
    # define NG_HELP "norama-eqr2gnomo - norama-suite\n" \
                     "Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch\n" \
                     "See https://github.com/FoxelSA/norama-suite for more information\n"

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

    //! The main function starts with parameters initialization and parameters 
    //! reading. It then import the input image and create, through OpenCV,
    //! the rectilinear image. The libgnomonic is then called to perform the
    //! gnomonic reprojection. The rectilinear image is then saved.
    //! 
    //! @param argc Standard main parameters
    //! @param argv Standard main parameters

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


