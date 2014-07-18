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
        IplImage * ngIimage = NULL;
        IplImage * ngMimage = NULL;
        IplImage * ngOimage = NULL;

        /* Image path variables */
        char ngIpath[256] = "input.png";
        char ngMpath[256] = "mask.png";
        char ngEpath[256] = "eqr.png";
        char ngOpath[256] = "output.png";

        /* Interpolation descriptor variables */
        char ngMethod[256] = "bicubic";

        /* Gnomonic projection variables */
        float ngNadir_hor = 0.0;
        float ngNadir_ver = 0.0;
        float ngApper_hor = 45.0;
        float ngApper_ver = 45.0;

        /* Interpolation method variables */
        interp ngInter = gnomonic_interp_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular"     , "-e" ), argv,   ngEpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--mask"                , "-m" ), argv,   ngMpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--rectilinear"         , "-r" ), argv,   ngIpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--output"              , "-o" ), argv,   ngOpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--nadir-horizontal"    , "-u" ), argv, & ngNadir_hor, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--nadir-vertical"      , "-v" ), argv, & ngNadir_ver, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-horizontal", "-a" ), argv, & ngApper_hor, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-vertical"  , "-b" ), argv, & ngApper_ver, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation"       , "-i" ), argv,   ngMethod   , __STDP_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( ngMethod, "bilinear" ) == 0 ) ngInter = gnomonic_interp_bilinearf;
        if ( strcmp( ngMethod, "bicubic"  ) == 0 ) ngInter = gnomonic_interp_bicubicf;
        if ( strcmp( ngMethod, "bipentic" ) == 0 ) ngInter = gnomonic_interp_bipenticf;

        /* Convert angles to radian */
        ngNadir_hor *= ( GNOMONIC_PI / 180.0 );
        ngNadir_ver *= ( GNOMONIC_PI / 180.0 );
        ngApper_hor *= ( GNOMONIC_PI / 180.0 );
        ngApper_ver *= ( GNOMONIC_PI / 180.0 );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) ) {

            /* Display usage */
            fprintf( stdout, NG_HELP );

        } else {

            /* Import input image */
            ngIimage = cvLoadImage( ngIpath, CV_LOAD_IMAGE_COLOR     );
            ngMimage = cvLoadImage( ngMpath, CV_LOAD_IMAGE_GRAYSCALE );
            ngOimage = cvLoadImage( ngEpath, CV_LOAD_IMAGE_COLOR     );

            /*  Verify input image reading */
            if ( ( ngIimage != NULL ) && ( ngMimage != NULL ) && ( ngOimage != NULL ) ) {

                /* Verify rectilinear image and mask consistency */
                if ( ( ngIimage->width == ngMimage->width ) && ( ngIimage->height == ngMimage->height ) ) {

                    /* Gnomonic reprojection */
                    gnomonic_gte_blend(

                        ( unsigned char * ) ngOimage->imageData,
                        ngOimage->width, 
                        ngOimage->height, 
                        ngOimage->nChannels, 
                        ( unsigned char * ) ngIimage->imageData,
                        ( unsigned char * ) ngMimage->imageData,
                        ngIimage->width, 
                        ngIimage->height,
                        ngIimage->nChannels,
                        ngNadir_hor,
                        ngNadir_ver,
                        ngApper_hor,
                        ngApper_ver,
                        ngInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( ngOpath, ngOimage, NULL ) == 0 ) {

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

