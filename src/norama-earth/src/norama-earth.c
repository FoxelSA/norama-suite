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

        /* Exportation options variables */
        int nrOption = -1;

        /* Parallel processing variables */
        int nrThread = 1;

        /* Interpolation descriptor variables */
        char * nrMethod = NULL;

        /* Image path variables */
        char * nriPath = NULL;
        char * nroPath = NULL;
        char * nrcPath = NULL;

        /* CSPS switch variables */
        char * nrcTag = NULL;
        char * nrcMod = NULL;
        char * nroTag = NULL;
        char * nroMod = NULL;

        /* Timestamp variables */
        lp_Time_t nrtSec = lp_Time_s( 0 );
        lp_Time_t nrtUse = lp_Time_s( 0 );

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* Rotation matrix variables */
        lp_Real_t nrMatrix[3][3] = { { 0.0 } };

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--input"        , "-a" ), argv, & nriPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"       , "-b" ), argv, & nroPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--path"         , "-p" ), argv, & nrcPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--cam-tag"      , "-c" ), argv, & nrcTag  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--cam-mod"      , "-m" ), argv, & nrcMod  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--imu-tag"      , "-i" ), argv, & nroTag  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--imu-mod"      , "-s" ), argv, & nroMod  , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--second"       , "-u" ), argv, & nrtSec  , LC_ULLONG );
        lc_stdp( lc_stda( argc, argv, "--micro-second" , "-v" ), argv, & nrtUse  , LC_ULLONG );
        lc_stdp( lc_stda( argc, argv, "--interpolation", "-n" ), argv, & nrMethod, LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread, LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--export"       , "-q" ), argv, & nrOption, LC_INT    );

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
                    if ( nr_earth_matrix( nrcPath, nrcTag, nrcMod, nroTag, nroMod, nrtSec, nrtUse, nrMatrix ) == LC_TRUE ) {

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
                        if ( lc_imwrite( nroPath, nroImage, nrOption ) == 0 ) {

                            /* Display message */
                            fprintf( LC_ERR, "Error : Unable to write output image\n" );

                        }

                    }

                    /* Release image memory */
                    cvReleaseImage( & nroImage );

                /* Display message */
                } else { fprintf( LC_ERR, "Error : Unable to create output image\n" ); }

                /* Release image memory */
                cvReleaseImage( & nriImage );

            /* Display message */
            } else { fprintf( LC_ERR, "Error : Unable to read input image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    Source - CSPS interface methods
 */

    int nr_earth_matrix( 

        lp_Char_t const * const nrPath, 
        lp_Char_t const * const nrCamTag, 
        lp_Char_t const * const nrCamMod, 
        lp_Char_t const * const nrIMUTag, 
        lp_Char_t const * const nrIMUMod, 
        lp_Time_t const         nrSecond,
        lp_Time_t const         nrMicro,
        lp_Real_t               nrMatrix[3][3]

    ) {

        /* Query structure variables */
        lp_Orient_t  nrOrient;
        lp_Trigger_t nrTrigger;

        /* Returned value variables */
        int nrReturn = LC_FALSE;

        /* Create query structure */
        nrTrigger = lp_query_trigger_create( nrPath, nrCamTag, nrCamMod );

        /* Verify structure state */
        if ( lp_query_trigger_state( & nrTrigger ) == LP_FALSE ) {

            /* Display message */
            fprintf( LC_ERR, "Error : unable to create query structure on trigger\n" );

        } else {

            /* Create query structure */
            nrOrient = lp_query_orientation_create( nrPath, nrIMUTag, nrIMUMod );

            /* Verify structure state */
            if ( lp_query_orientation_state( & nrOrient ) == LP_FALSE ) {

                /* Display message */
                fprintf( LC_ERR, "Error : unable to create query structure on orientation\n" );

            } else {

                /* Query master/synchronization time-link */
                lp_query_trigger_bymaster( & nrTrigger, lp_timestamp_compose( nrSecond, nrMicro ) );

                /* Verify query status */
                if ( lp_query_trigger_status( & nrTrigger ) == LP_FALSE ) {

                    /* Display message */
                    fprintf( LC_ERR, "Error : unable to query trigger timestamp\n" );

                } else {

                    /* Query orientation by timestamp */
                    lp_query_orientation( & nrOrient, nrTrigger.qrSynch );

                    /* Verify query status */
                    if ( lp_query_orientation_status( & nrOrient ) == LP_FALSE ) {

                        /* Display message */
                        fprintf( LC_ERR, "Error : unable to query orientation\n" );

                    } else {

                        /* Rotation matrix method */
                        lp_query_orientation_matrix( & nrOrient, nrMatrix );

                        /* Update status */
                        nrReturn = LC_TRUE;

                    }

                }

                /* Release query structure */
                lp_query_orientation_delete( & nrOrient );

            }

            /* Release query structure */
            lp_query_trigger_delete( & nrTrigger );

        }

        /* Return status */
        return( nrReturn );

    }

