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

    # include "norama-direct.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Image initialization variables */
        unsigned char nrRed = 0;
        unsigned char nrGreen = 0;
        unsigned char nrBlue = 0;

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
        int nrrWidth   = 0;
        int nrrHeight  = 0;
        int nrmWidth   = 0;
        int nrmHeight  = 0;
        int nrmCornerX = 0;
        int nrmCornerY = 0;

        /* Parallel processing variables */
        int nrThread = 1;

        /* Interpolation tag variables */
        char nrMethod[256] = { 0 };

        /* Image path variables */
        char nriPath[256] = { 0 };
        char nroPath[256] = { 0 };
        char nriSeed[256] = { 0 };

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nroImage = NULL;

        /* Search in parameters */
        stdp( stda( argc, argv, "--apperture"    , "-u" ), argv, & nrApper   , NR_DOUBLE );
        stdp( stda( argc, argv, "--sight-x"      , "-x" ), argv, & nrSightX  , NR_DOUBLE );
        stdp( stda( argc, argv, "--sight-y"      , "-y" ), argv, & nrSightY  , NR_DOUBLE );
        stdp( stda( argc, argv, "--azimuth"      , "-a" ), argv, & nrAzim    , NR_DOUBLE );
        stdp( stda( argc, argv, "--heading"      , "-d" ), argv, & nrHead    , NR_DOUBLE );
        stdp( stda( argc, argv, "--elevation"    , "-e" ), argv, & nrElev    , NR_DOUBLE );
        stdp( stda( argc, argv, "--roll"         , "-r" ), argv, & nrRoll    , NR_DOUBLE );
        stdp( stda( argc, argv, "--focal"        , "-f" ), argv, & nrFocal   , NR_DOUBLE );
        stdp( stda( argc, argv, "--pixel"        , "-p" ), argv, & nrPixel   , NR_DOUBLE );
        stdp( stda( argc, argv, "--red"          , "-R" ), argv, & nrRed     , NR_UCHAR  );
        stdp( stda( argc, argv, "--green"        , "-G" ), argv, & nrGreen   , NR_UCHAR  );
        stdp( stda( argc, argv, "--blue"         , "-B" ), argv, & nrBlue    , NR_UCHAR  );
        stdp( stda( argc, argv, "--rect-width"   , "-k" ), argv, & nrrWidth  , NR_INT    );
        stdp( stda( argc, argv, "--rect-height"  , "-l" ), argv, & nrrHeight , NR_INT    );
        stdp( stda( argc, argv, "--map-width"    , "-W" ), argv, & nrmWidth  , NR_INT    );
        stdp( stda( argc, argv, "--map-height"   , "-H" ), argv, & nrmHeight , NR_INT    );
        stdp( stda( argc, argv, "--tile-x"       , "-X" ), argv, & nrmCornerX, NR_INT    );
        stdp( stda( argc, argv, "--tile-y"       , "-Y" ), argv, & nrmCornerY, NR_INT    );
        stdp( stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread  , NR_INT    );
        stdp( stda( argc, argv, "--input"        , "-i" ), argv,   nriPath   , NR_STRING );
        stdp( stda( argc, argv, "--output"       , "-o" ), argv,   nroPath   , NR_STRING );
        stdp( stda( argc, argv, "--seed"         , "-s" ), argv,   nriSeed   , NR_STRING );
        stdp( stda( argc, argv, "--interpolation", "-n" ), argv,   nrMethod  , NR_STRING );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Import input image */
            nriImage = cvLoadImage( nriPath, stda( argc, argv, "--force-rgb", "-F" ) ? CV_LOAD_IMAGE_COLOR : CV_LOAD_IMAGE_UNCHANGED );

            /*  Verify input image reading */
            if ( nriImage != NULL ) {

                /* Check for image seed */
                if ( strlen( nriSeed ) == 0 ) {

                    /* Create image allocation */
                    nroImage = cvCreateImage( cvSize( nrrWidth, nrrHeight ), IPL_DEPTH_8U , nriImage->nChannels );

                } else {

                    /* Load image seed */
                    nroImage = cvLoadImage( nriSeed, CV_LOAD_IMAGE_UNCHANGED );

                }

                /* Verify allocation creation */
                if ( nroImage != NULL ) {

                    /* Image initialization swicth */
                    if ( stda( argc, argv, "--clear", "-C" ) ) {

                        /* Initialize image background */
                        cvSet( nroImage, CV_RGB( nrRed, nrGreen, nrBlue ), NULL );

                    }

                    /* Select projection model */
                    if ( stda( argc, argv, "--generic", "-N" ) ) {

                        /* Projection - generic */
                        lg_ttg_genericp(

                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
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
                            nr_direct_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( stda( argc, argv, "--elphel" , "-E" ) ) {

                        /* Projection - elphel-specific */
                        lg_ttg_elphelp(

                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
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
                            nr_direct_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( stda( argc, argv, "--center" , "-T" ) ) {

                        /* Projection - center-specific */
                        lg_ttg_centerp(

                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            nrmWidth,
                            nrmHeight,
                            nrmCornerX,
                            nrmCornerY,
                            nrAzim * ( LG_PI / 180.0 ),
                            nrElev * ( LG_PI / 180.0 ),
                            nrRoll * ( LG_PI / 180.0 ),
                            nrFocal,
                            nrPixel,
                            nr_direct_method( nrMethod ),
                            nrThread

                        );

                    } else
                    if ( stda( argc, argv, "--complete", "-P" ) ) {

                        /* Projection - apperture-specific */
                        lg_etg_apperturep( 

                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            ( inter_C8_t * ) nroImage->imageData,
                            nroImage->width,
                            nroImage->height,
                            nroImage->nChannels,
                            nrAzim  * ( LG_PI / 180.0 ),
                            nrElev  * ( LG_PI / 180.0 ),
                            nrRoll  * ( LG_PI / 180.0 ),
                            nrApper * ( LG_PI / 180.0 ),
                            nr_direct_method( nrMethod ),
                            nrThread

                        );

                    }

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
    Source - Interpolation method by string
 */

    li_Method_t nr_direct_method( char const * const nrTag ) {

        /* Interpolation method variables */
        li_Method_t nrMethod = li_bicubicf;

        /* Switch on string tag */
        if ( strcmp( nrTag, "bilinearf" ) == 0 ) {

            /* Assign interpolation method */
            nrMethod = li_bilinearf;

        } else
        if ( strcmp( nrTag, "bicubicf" ) == 0 ) {

            /* Assign interpolation method */
            nrMethod = li_bicubicf;

        } else
        if ( strcmp( nrTag, "bipenticf" ) == 0 ) {

            /* Assign interpolation method */
            nrMethod = li_bipenticf;

        } else
        if ( strcmp( nrTag, "bihepticf" ) == 0 ) {

            /* Assign interpolation method */
            nrMethod = li_bihepticf;

        }

        /* Return selected method */
        return( nrMethod );

    }

/*
    Source - Arguments common handler
 */

    int stda( int argc, char ** argv, char const * const ltag, char const * const stag ) {

        /* Search for argument */
        while ( ( -- argc ) > 0 ) {

            /* Search for tag matching */
            if ( ( strcmp( argv[ argc ], ltag ) == 0 ) || ( strcmp( argv[ argc ], stag ) == 0 ) ) {

                /* Return pointer to argument parameter */
                return( argc + 1 );

            }

        /* Argument not found */
        } return( NR_NULL );

    }

/*
    Source - Parameters common handler
 */

    void stdp( int argi, char ** argv, void * const param, int const type ) {

        /* Index consistency */
        if ( argi == NR_NULL ) return;

        /* Select type */
        switch ( type ) {

            /* Specific reading operation - Integers */
            case ( NR_CHAR   ) : { * ( signed char        * ) param = atoi ( ( const char * ) argv[argi] ); } break;
            case ( NR_SHORT  ) : { * ( signed short       * ) param = atoi ( ( const char * ) argv[argi] ); } break;
            case ( NR_INT    ) : { * ( signed int         * ) param = atoi ( ( const char * ) argv[argi] ); } break;
            case ( NR_LONG   ) : { * ( signed long        * ) param = atol ( ( const char * ) argv[argi] ); } break;
            case ( NR_LLONG  ) : { * ( signed long long   * ) param = atoll( ( const char * ) argv[argi] ); } break;
            case ( NR_UCHAR  ) : { * ( unsigned char      * ) param = atol ( ( const char * ) argv[argi] ); } break;
            case ( NR_USHORT ) : { * ( unsigned short     * ) param = atol ( ( const char * ) argv[argi] ); } break;
            case ( NR_UINT   ) : { * ( unsigned int       * ) param = atol ( ( const char * ) argv[argi] ); } break;
            case ( NR_ULONG  ) : { * ( unsigned long      * ) param = atoll( ( const char * ) argv[argi] ); } break;
            case ( NR_ULLONG ) : { * ( unsigned long long * ) param = atoll( ( const char * ) argv[argi] ); } break;

            /* Specific reading operation - Floating point */
            case ( NR_FLOAT  ) : { * ( float              * ) param = atof ( ( const char * ) argv[argi] ); } break;
            case ( NR_DOUBLE ) : { * ( double             * ) param = atof ( ( const char * ) argv[argi] ); } break;

            /* Specific reading operation - String */
            case ( NR_STRING ) : { strcpy( ( char * ) param, ( const char * ) argv[argi] );  } break;

        };

    }

