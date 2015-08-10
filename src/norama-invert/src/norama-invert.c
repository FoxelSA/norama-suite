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

    # include "norama-invert.h"

/*
    Source - Software main function
 */

    int main ( int argc, char ** argv ) {

        /* Image initialization variables */
        unsigned char nrRed   = 0;
        unsigned char nrGreen = 0;
        unsigned char nrBlue  = 0;

        /* Projection float variables */
        double nrApper  = 0.0;
        double nrSightX = 0.0;
        double nrSightY = 0.0;
        double nrAzim   = 0.0;
        double nrHead   = 0.0;
        double nrElev   = 0.0;
        double nrRoll   = 0.0;
        double nrFocal  = 0.0;
        double nrPixel  = 0.0;

        /* Projection integer variables */
        int nreWidth   = 0;
        int nreHeight  = 0;
        int nrmWidth   = 0;
        int nrmHeight  = 0;
        int nrmCornerX = 0;
        int nrmCornerY = 0;

        /* Exportation options variables */
        int nrOption = -1;

        /* Parallel processing variables */
        int nrThread = 1;

        /* Interpolation tag variables */
        char * nrMethod = NULL;

        /* Image path variables */
        char * nriPath = NULL;
        char * nroPath = NULL;
        char * nriSeed = NULL;

        /* Image allocations variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--aperture"     , "-u" ), argv, & nrApper   , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--sight-x"      , "-x" ), argv, & nrSightX  , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--sight-y"      , "-y" ), argv, & nrSightY  , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--azimuth"      , "-a" ), argv, & nrAzim    , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--heading"      , "-d" ), argv, & nrHead    , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--elevation"    , "-e" ), argv, & nrElev    , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--roll"         , "-r" ), argv, & nrRoll    , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--focal"        , "-f" ), argv, & nrFocal   , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--pixel"        , "-p" ), argv, & nrPixel   , LC_DOUBLE );
        lc_stdp( lc_stda( argc, argv, "--red"          , "-R" ), argv, & nrRed     , LC_UCHAR  );
        lc_stdp( lc_stda( argc, argv, "--green"        , "-G" ), argv, & nrGreen   , LC_UCHAR  );
        lc_stdp( lc_stda( argc, argv, "--blue"         , "-B" ), argv, & nrBlue    , LC_UCHAR  );
        lc_stdp( lc_stda( argc, argv, "--eqr-width"    , "-k" ), argv, & nreWidth  , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--eqr-height"   , "-l" ), argv, & nreHeight , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--map-width"    , "-W" ), argv, & nrmWidth  , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--map-height"   , "-H" ), argv, & nrmHeight , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--tile-x"       , "-X" ), argv, & nrmCornerX, LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--tile-y"       , "-Y" ), argv, & nrmCornerY, LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread  , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--export"       , "-q" ), argv, & nrOption  , LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--input"        , "-i" ), argv, & nriPath   , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"       , "-o" ), argv, & nroPath   , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--seed"         , "-s" ), argv, & nriSeed   , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--interpolation", "-n" ), argv, & nrMethod  , LC_STRING );

        /* Software swicth */
        if ( lc_stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Import input image */
            nriImage = cvLoadImage( nriPath, lc_stda( argc, argv, "--force-rgb", "-F" ) ? CV_LOAD_IMAGE_COLOR : CV_LOAD_IMAGE_UNCHANGED );

            /*  Verify input image reading */
            if ( nriImage != NULL ) {

                /* Check for image seed */
                if ( strlen( nriSeed ) == 0 ) {

                    /* Create image allocation */
                    nroImage = cvCreateImage( cvSize( nreWidth, nreHeight ), IPL_DEPTH_8U , nriImage->nChannels );

                } else {

                    /* Load image seed */
                    nroImage = cvLoadImage( nriSeed, CV_LOAD_IMAGE_UNCHANGED );

                }

                /* Verify allocation creation */
                if ( nroImage != NULL ) {

                    /* Image initialization swicth */
                    if ( lc_stda( argc, argv, "--clear", "-C" ) ) {

                        /* Initialize image background */
                        cvSet( nroImage, CV_RGB( nrRed, nrGreen, nrBlue ), NULL );

                    }

                    /* Select projection model */
                    if ( lc_stda( argc, argv, "--generic", "-N" ) ) {

                        /* Projection - generic */
                        lg_gtt_genericp(

                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            nrSightX,
                            nrSightY,
                            nrmWidth,
                            nrmHeight,
                            nrmCornerX,
                            nrmCornerY,
                            nrAzim  * ( LG_PI / 180.0 ),
                            nrElev  * ( LG_PI / 180.0 ),
                            nrRoll  * ( LG_PI / 180.0 ),
                            nrFocal,
                            nrPixel,
                            lc_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( lc_stda( argc, argv, "--elphel", "-E" ) ) {

                        /* Projection - elphel-specific */
                        lg_gtt_elphelp(

                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            nrSightX,
                            nrSightY,
                            nrmWidth,
                            nrmHeight,
                            nrmCornerX,
                            nrmCornerY,
                            nrRoll * ( LG_PI / 180.0 ),
                            nrAzim * ( LG_PI / 180.0 ),
                            nrElev * ( LG_PI / 180.0 ),
                            nrHead * ( LG_PI / 180.0 ),
                            nrPixel,
                            nrFocal,
                            lc_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( lc_stda( argc, argv, "--center", "-T" ) ) {

                        /* Projection - center-specific */
                        lg_gtt_centerp(

                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            nrmWidth,
                            nrmHeight,
                            nrmCornerX,
                            nrmCornerY,
                            nrAzim * ( LG_PI / 180.0 ),
                            nrElev * ( LG_PI / 180.0 ),
                            nrRoll * ( LG_PI / 180.0 ),
                            nrFocal,
                            nrPixel,
                            lc_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( lc_stda( argc, argv, "--complete", "-P" ) ) {

                        /* Projection - aperture-specific */
                        lg_gte_apperturep( 

                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            nrAzim  * ( LG_PI / 180.0 ),
                            nrElev  * ( LG_PI / 180.0 ),
                            nrRoll  * ( LG_PI / 180.0 ),
                            nrApper * ( LG_PI / 180.0 ),
                            lc_method( nrMethod ),
                            nrThread

                        );

                    }

                    /* Export output image */
                    if ( lc_imwrite( nroPath, nroImage, nrOption ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write output image\n" );

                    }

                    /* Release image memory */
                    cvReleaseImage( & nroImage );

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create output image or read output image seed\n" ); }

                /* Release image memory */
                cvReleaseImage( & nriImage );

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

