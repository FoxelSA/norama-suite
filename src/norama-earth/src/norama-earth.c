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

        /* Timestamp string variables */
        char nrsSec[256] = { 0 };
        char nrsUse[256] = { 0 };

        /* Timestamp variables */
        lp_Time_t nrTime = lp_Time_s( 0 );
        lp_Time_t nrtSec = lp_Time_s( 0 );
        lp_Time_t nrtUse = lp_Time_s( 0 );

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* CSPS query structure variables */
        lp_Orient_t nrOrient;

        /* Rotation matrix variables */
        lp_Real_t nrMatrix[3][3] = { { 0.0 } };

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--input"        , "-i" ), argv,   nriPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"       , "-o" ), argv,   nroPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--path"         , "-p" ), argv,   nrcPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--orient-tag"   , "-k" ), argv,   nroTag  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--orient-mod"   , "-s" ), argv,   nroMod  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--second"       , "-w" ), argv,   nrsSec  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--micro-second" , "-u" ), argv,   nrsUse  , LC_STRING );
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

                    /* Create orientation structure */
                    nrOrient = lp_query_orientation_create( nrcPath, nroTag, nroMod );

                    /* Timestamp analysis */
                    sscanf( nrsSec, "%" lp_Time_i, & nrtSec );
                    sscanf( nrsUse, "%" lp_Time_i, & nrtUse );

                    /* Compose timestamp */
                    nrTime = lp_timestamp_compose( nrtSec, nrtUse );

                    /* Query orientation */
                    lp_query_orientation( & nrOrient, nrTime );

                    /* Assign matrix coefficients */
                    nrMatrix[0][0] = nrOrient.qrfxx;
                    nrMatrix[0][1] = nrOrient.qrfxy;
                    nrMatrix[0][2] = nrOrient.qrfxz;
                    nrMatrix[1][0] = nrOrient.qrfyx;
                    nrMatrix[1][1] = nrOrient.qrfyy;
                    nrMatrix[1][2] = nrOrient.qrfyz;
                    nrMatrix[2][0] = nrOrient.qrfzx;
                    nrMatrix[2][1] = nrOrient.qrfzy;
                    nrMatrix[2][2] = nrOrient.qrfzz;

                    /* Release query structure */
                    lp_query_orientation_delete( & nrOrient );

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

