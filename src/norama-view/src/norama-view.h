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

    /*! \file   norama-view.h
     *  \author Nils Hamel <n.hamel@foxel.ch>
     *   
     *  Software main header (single function software)
     */

    /*! \mainpage norama-rotate
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
     *  A detailed documentation can be generated through doxygen. A more general
     *  documentation can be consulted at https://github.com/FoxelSA/norama-suite/wiki
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

    # ifndef __NR_VIEW__
    # define __NR_VIEW__

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

//    # include <X11/Xlib.h>
    # include <GL/glut.h>

    # include <gnomonic-all.h>

/* 
    Header - Preprocessor definitions
 */

    /* Standard help */
    # define NR_HELP "Usage summary :\n\n"                 \
    "\tnorama-view [Arguments] [Parameters] ...\n\n"       \
    "Short arguments and parameters summary :\n\n"         \
    "\t-i\tInput equirectangular mapping image\n"          \
    "\t-t\tForce thread number\n"                          \
    "\t-s\tScale applied on display buffer\n\n"            \
    "norama-view - norama-suite\n"                         \
    "Copyright (c) 2013-2014 FOXEL SA - http://foxel.ch\n"

    /* Define standard types */
    # define NR_NULL          0
    # define NR_STRING        1
    # define NR_CHAR          2
    # define NR_SHORT         3
    # define NR_INT           4
    # define NR_LONG          5
    # define NR_LLONG         6
    # define NR_UCHAR         7
    # define NR_USHORT        8
    # define NR_UINT          9
    # define NR_ULONG        10
    # define NR_ULLONG       11
    # define NR_FLOAT        12
    # define NR_DOUBLE       13

    /* Define keyevent codes */
    # define NR_KEY_ESCAPE   27
    # define NR_KEY_F       102
    # define NR_KEY_R       114

    /* Define default scale */
    # define NR_DFT_SCALE   1.0

    /* Define default thread */
    # define NR_DFT_THREAD    8

    /* Define mouse motion mode */
    # define NR_MS_NONE       0
    # define NR_MS_MOVE       1
    # define NR_MS_CFOV       2  

    /* Define FOV constants */
    # define NR_MIN_APPER   (  20.0 * ( LG_PI / 180.0 ) )
    # define NR_MAX_APPER   ( 120.0 * ( LG_PI / 180.0 ) )
    # define NR_DFT_APPER   ( 100.0 * ( LG_PI / 180.0 ) )

/* 
    Header - Preprocessor macros
 */

/* 
    Header - Typedefs
 */

/* 
    Header - Structures
 */

    /*! \struct nr_Mouse_struct
     *  \brief Mouse control structure
     *  
     *  This structure is used to ensure communication between display function
     *  and the mouse event callback function.
     *  
     *  \var nr_Mouse_struct::msAzim
     *  Display view azimuth angle, in radian
     *  \var nr_Mouse_struct::msElev
     *  Display view elevation angle, in radian
     *  \var nr_Mouse_struct::msAppe
     *  Display view gnomonic apperture, in radian
     */

    typedef struct nr_Mouse_struct {

        double msAzim;
        double msElev;
        double msAppe;
        double msWidth;

    } nr_Mouse;

/* 
    Header - Function prototypes
 */

    /*! \brief Software main function (single function software)
     *  
     *  The main function is responsible for panoramic image loading and the
     *  management of the display. A pseudo-infinite loop is handled by the main
     *  function in which mouse and keyboard events are managed. The computation
     *  of the display buffer is also performed in this same loop.
     *  
     *  \param argc Standard main parameter
     *  \param argv Standard main parameter
     */

    int main ( int argc, char ** argv );

    /*! \brief Mouse callback
     *
     *  This function is the mouse event callback that is used throught OpenCV
     *  library. It is responsible for display view motion based on mouse move.
     *
     *  \param event    Mouse Event code
     *  \param x        Mouse position x at event
     *  \param y        Mouse position y at event
     *  \param userdata Userdata handle, used for motion management
     */

    void nr_view_mouse ( int event, int x, int y, int flag, void * userdata );

    /*! \brief Screen resolution
     *
     *  This function allows to obtain the current screen resolution. The width
     *  and height, in pixels, are returned using the variable pointers given as
     *  parameter.
     *
     *  The scale parameter is applied on the two screen dimension. This is done
     *  in order to be able to reduce the display buffer size when the local
     *  computer is not enough powerful for the desired rendering.
     *
     *  If the function fails to get the screen resolution, it returns a default
     *  screen resolution that corresponds to the statistically most used one at
     *  the time this line is written.
     *
     *  \param nrWidth  Buffer that recieves the screen width
     *  \param nrHeight Buffer that recieves the screen height
     *  \param nrScale  Screen dimension scale factor
     */

    void nr_view_display( int * nrWidth, int * nrHeight, float nrScale );

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

