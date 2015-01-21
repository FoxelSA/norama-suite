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

    # include "norama-earth.h"

/*
    Source - Software main function
 */

    int main ( int argc, char ** argv ) {

        /* Parallel processing variables */
        int nrThread = 1;

        /* Interpolation descriptor variables */
        char nrMethod[256] = { 0 };

        /* Image path variables */
        char nriPath[256] = { 0 };
        char nroPath[256] = { 0 };
        char nrcPath[256] = { 0 };

        /* CSPS switch variables */
        char nroTag[256] = { 0 };
        char nroMod[256] = { 0 };

        /* Timestamp variables */
        lp_Time_t nrtSec = lp_Time_s( 0 );
        lp_Time_t nrtUse = lp_Time_s( 0 );

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* Rotation matrix variables */
        lp_Real_t nrMatrix[3][3] = { { 0.0 } };

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--input"        , "-a" ), argv,   nriPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"       , "-b" ), argv,   nroPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--path"         , "-p" ), argv,   nrcPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--imu-tag"      , "-i" ), argv,   nroTag  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--imu-mod"      , "-s" ), argv,   nroMod  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--second"       , "-u" ), argv, & nrtSec  , LC_ULLONG );
        lc_stdp( lc_stda( argc, argv, "--micro-second" , "-v" ), argv, & nrtUse  , LC_ULLONG );
        lc_stdp( lc_stda( argc, argv, "--interpolation", "-n" ), argv,   nrMethod, LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread, LC_INT    );

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

                    /* Query rotation matrix */
                    nr_earth_matrix( 

                        nrcPath, 
                        nroTag, 
                        nroMod, 
                        nrtSec, 
                        nrtUse, 
                        nrMatrix 

                    );

                    /* Apply equirectangular transformation */
                    lg_transform_matrixp( 

                        ( inter_C8_t * ) nriImage->imageData,
                        ( inter_C8_t * ) nroImage->imageData,
                        nriImage->width,
                        nriImage->height,
                        nriImage->nChannels,
                        nrMatrix,
                        lc_method( nrMethod ),
                        nrThread

                    );

                    /* Export output image */
                    if ( cvSaveImage( nroPath, nroImage, NULL ) == 0 ) {

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

/*
    Source - CSPS interface methods
 */

    void nr_earth_matrix( 

        lp_Char_t const * const nrPath, 
        lp_Char_t const * const nrTag, 
        lp_Char_t const * const nrModule, 
        lp_Time_t const         nrSecond,
        lp_Time_t const         nrMicro,
        lp_Real_t               nrMatrix[3][3]

    ) {

        /* Query structure variables */
        lp_Orient_t nrOrient;

        /* Create query structure */
        nrOrient = lp_query_orientation_create( nrPath, nrTag, nrModule );

        /* Query orientation by timestamp */
        lp_query_orientation( & nrOrient, lp_timestamp_compose( nrSecond, nrMicro ) );

        /* Rotation matrix method */
        lp_query_orientation_matrix( & nrOrient, nrMatrix );

        /* Release query structure */
        lp_query_orientation_delete( & nrOrient );        

    }

