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

    # include "norama-rotate.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Rotation angles variables */
        double nrAzim = 0.0;
        double nrElev = 0.0;
        double nrRoll = 0.0;

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
        stdp( stda( argc, argv, "--input"        , "-n" ), argv,   nriPath , NR_STRING );
        stdp( stda( argc, argv, "--output"       , "-o" ), argv,   nroPath , NR_STRING );
        stdp( stda( argc, argv, "--interpolation", "-i" ), argv,   nrMethod, NR_STRING );
        stdp( stda( argc, argv, "--azimuth"      , "-a" ), argv, & nrAzim  , NR_DOUBLE );
        stdp( stda( argc, argv, "--elevation"    , "-e" ), argv, & nrElev  , NR_DOUBLE );
        stdp( stda( argc, argv, "--roll"         , "-r" ), argv, & nrRoll  , NR_DOUBLE );
        stdp( stda( argc, argv, "--threads"      , "-t" ), argv, & nrThread, NR_INT    );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

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
                        nr_rotate_method( nrMethod ),
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
    Source - Interpolation method by string
 */

    li_Method_t nr_rotate_method( char const * const nrTag ) {

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

