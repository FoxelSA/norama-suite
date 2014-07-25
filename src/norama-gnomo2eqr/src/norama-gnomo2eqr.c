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


/* 
    Source - Includes
 */

    # include "norama-gnomo2eqr.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Image variables */
        IplImage * nrIImage = NULL;
        IplImage * nrMImage = NULL;
        IplImage * nrOImage = NULL;

        /* Image path variables */
        char nrIPath[256] = { 0 };
        char nrMPath[256] = { 0 };
        char nrEPath[256] = { 0 };
        char nrOPath[256] = { 0 };

        /* Interpolation descriptor variables */
        char nrMethod[256] = { 0 };

        /* Gnomonic projection variables */
        float nrNadirHor = 0.0;
        float nrNadirVer = 0.0;
        float nrApperHor = 45.0;
        float nrApperVer = 45.0;

        /* Interpolation method variables */
        inter_Method_t nrInter = inter_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular"     , "-e" ), argv,   nrEPath   , __STDP_STRING );
        stdp( stda( argc, argv,  "--mask"                , "-m" ), argv,   nrMPath   , __STDP_STRING );
        stdp( stda( argc, argv,  "--rectilinear"         , "-r" ), argv,   nrIPath   , __STDP_STRING );
        stdp( stda( argc, argv,  "--output"              , "-o" ), argv,   nrOPath   , __STDP_STRING );
        stdp( stda( argc, argv,  "--nadir-horizontal"    , "-u" ), argv, & nrNadirHor, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--nadir-vertical"      , "-v" ), argv, & nrNadirVer, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-horizontal", "-a" ), argv, & nrApperHor, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-vertical"  , "-b" ), argv, & nrApperVer, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation"       , "-i" ), argv,   nrMethod  , __STDP_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( nrMethod, "bilinear" ) == 0 ) nrInter = inter_bilinearf;
        if ( strcmp( nrMethod, "bicubic"  ) == 0 ) nrInter = inter_bicubicf;
        if ( strcmp( nrMethod, "bipentic" ) == 0 ) nrInter = inter_bipenticf;

        /* Convert angles to radian */
        nrNadirHor *= ( GNOMONIC_PI / 180.0 );
        nrNadirVer *= ( GNOMONIC_PI / 180.0 );
        nrApperHor *= ( GNOMONIC_PI / 180.0 );
        nrApperVer *= ( GNOMONIC_PI / 180.0 );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) ) {

            /* Display usage */
            fprintf( stdout, NG_HELP );

        } else {

            /* Import input image */
            nrIImage = cvLoadImage( nrIPath, CV_LOAD_IMAGE_COLOR     );
            nrMImage = cvLoadImage( nrMPath, CV_LOAD_IMAGE_GRAYSCALE );
            nrOImage = cvLoadImage( nrEPath, CV_LOAD_IMAGE_COLOR     );

            /*  Verify input image reading */
            if ( ( nrIImage != NULL ) && ( nrMImage != NULL ) && ( nrOImage != NULL ) ) {

                /* Verify rectilinear image and mask consistency */
                if ( ( nrIImage->width == nrMImage->width ) && ( nrIImage->height == nrMImage->height ) ) {

                    /* Gnomonic reprojection */
                    gnomonic_gte_blend(

                        ( inter_C8_t * ) nrOImage->imageData,
                        nrOImage->width, 
                        nrOImage->height, 
                        nrOImage->nChannels, 
                        ( inter_C8_t * ) nrIImage->imageData,
                        ( inter_C8_t * ) nrMImage->imageData,
                        nrIImage->width, 
                        nrIImage->height,
                        nrIImage->nChannels,
                        nrNadirHor,
                        nrNadirVer,
                        nrApperHor,
                        nrApperVer,
                        nrInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( nrOPath, nrOImage, NULL ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write output image\n" );

                    }

                } else { fprintf( stdout, "Error : Rectilinear image and its mask have different size\n" ); }

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image(s)\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

