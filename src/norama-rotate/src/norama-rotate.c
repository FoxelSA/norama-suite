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


/* 
    Source - Includes
 */

    # include "norama-rotate.h"

/*
    Source - Software main function
 */

    int main ( int argc, char ** argv ) {

        /* Rotation angles variables */
        double nrAzim = 0.0;
        double nrElev = 0.0;
        double nrRoll = 0.0;

        /* Exportation options variables */
        int nrOption = -1;

        /* Parallel processing variables */
        int nrThread = 1;

        /* Interpolation descriptor variables */
        char nrMethod[256] = { 0 };

        /* Image path variables */
        char nriPath[256] = { 0 };
        char nroPath[256] = { 0 };

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--input"        , "-i" ), argv,   nriPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"       , "-o" ), argv,   nroPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--interpolation", "-n" ), argv,   nrMethod, LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--azimuth"      , "-a" ), argv, & nrAzim  , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--elevation"    , "-e" ), argv, & nrElev  , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--roll"         , "-r" ), argv, & nrRoll  , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread, LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--option"       , "-p" ), argv, & nrOption, LC_INT    );

        /* Software swicth */
        if ( lc_stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Import input image */
            nriImage = cvLoadImage( nriPath, CV_LOAD_IMAGE_UNCHANGED );

            /*  Verify input image reading */
            if ( nriImage != NULL ) {

                /* Create image allocation */
                nroImage = cvCreateImage( cvSize( nriImage->width, nriImage->height ), IPL_DEPTH_8U , nriImage->nChannels );

                /* Verify allocation creation */
                if ( nroImage != NULL ) {

                    /* Apply equirectangular transform */
                    lg_transform_rotatep( 

                        ( inter_C8_t * ) nriImage->imageData,
                        ( inter_C8_t * ) nroImage->imageData,
                        nriImage->width,
                        nriImage->height,
                        nriImage->nChannels,
                        nrAzim * ( LG_PI / 180.0 ),
                        nrElev * ( LG_PI / 180.0 ),
                        nrRoll * ( LG_PI / 180.0 ),
                        lc_method( nrMethod ),
                        nrThread

                    );

                    /* Export output image */
                    if ( lc_imwrite( nroPath, nroImage, nrOption ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write output image\n" );

                    }

                    /* Release image memory */
                    cvReleaseImage( & nroImage );

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create output image\n" ); }

                /* Release image memory */
                cvReleaseImage( & nriImage );

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

