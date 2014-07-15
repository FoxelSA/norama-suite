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

    # include "norama-eqr2gnomo.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Image variables */
        IplImage * ngIimage = NULL;
        IplImage * ngOimage = NULL;

        /* Image path variables */
        char ngIpath[256] = "input.png";
        char ngOpath[256] = "output.png";

        /* Interpolation descriptor variables */
        char ngMethod[256] = "bicubic";

        /* Image parameters variables */
        int ngOwidth  = 512;
        int ngOheight = 512;

        /* Gnomonic projection variables */
        float ngNadir_hor = 0.0;
        float ngNadir_ver = 0.0;
        float ngApper_hor = 45.0;
        float ngApper_ver = 45.0;

        /* Interpolation method variables */
        interp ngInter = gnomonic_interp_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular"     , "-e" ), argv,   ngIpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--rectilinear"         , "-r" ), argv,   ngOpath    , __STDP_STRING );
        stdp( stda( argc, argv,  "--width"               , "-w" ), argv, & ngOwidth   , __STDP_INT    );
        stdp( stda( argc, argv,  "--height"              , "-t" ), argv, & ngOheight  , __STDP_INT    );
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
            ngIimage = cvLoadImage( ngIpath, CV_LOAD_IMAGE_COLOR );

            /*  Verify input image reading */
            if ( ngIimage != NULL ) {

                /* Initialize output image structure */
                ngOimage = cvCreateImage( cvSize( ngOwidth, ngOheight ), IPL_DEPTH_8U , ngIimage->nChannels );

                /* Verify output image creation */
                if ( ngOimage != NULL ) {

                    /* Gnomonic reprojection */
                    gnomonic_etg(

                        ( unsigned char * ) ngIimage->imageData,
                        ngIimage->width, 
                        ngIimage->height, 
                        ngIimage->nChannels, 
                        ( unsigned char * ) ngOimage->imageData, 
                        ngOimage->width, 
                        ngOimage->height,
                        ngOimage->nChannels,
                        ngNadir_hor,
                        ngNadir_ver,
                        ngApper_hor,
                        ngApper_ver,
                        ngInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( ngOpath, ngOimage, NULL ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write gnomonic image\n" );

                    }

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create gnomonic image\n" ); }

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read equirectangular image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

