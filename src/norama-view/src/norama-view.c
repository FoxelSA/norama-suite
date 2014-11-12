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

    # include "norama-view.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Image path variables */
        char nriPath[256] = { 0 };

        /* Parallel processing variables */
        int nrThread = 1;

        /* Window name variables */
        char nrName[256] = "norama-view";

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nrdImage = NULL;

        /* Mouse control variables */
        nr_Mouse nrMouse = { 0.0, 0.0, LG_PI / 2.0 };

        /* Search in parameters */
        stdp( stda( argc, argv, "--input"  , "-i" ), argv,   nriPath , NR_STRING );
        stdp( stda( argc, argv, "--threads", "-t" ), argv, & nrThread, NR_INT    );

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
                nrdImage = cvCreateImage( cvSize( 1920, 1200 ), IPL_DEPTH_8U , nriImage->nChannels );

                /* Verify allocation creation */
                if ( nrdImage != NULL ) {

                    /* Create display window */
                    cvNamedWindow( nrName, CV_WINDOW_NORMAL );

                    /* Setting window on full screen */
                    cvSetWindowProperty( nrName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );

                    /* Define window mouse event callback function */
                    cvSetMouseCallback( nrName, & ( nr_norama_view_mouse ), & ( nrMouse ) );

                    while ( 1 == 1 ) {

                        /* Compute gnomonic projection */
                        lg_etg_apperturep(

                            ( inter_C8_t * ) nriImage->imageData,
                            nriImage->width,
                            nriImage->height,
                            nriImage->nChannels,
                            ( inter_C8_t * ) nrdImage->imageData,
                            nrdImage->width,
                            nrdImage->height,
                            nrdImage->nChannels,
                            nrMouse.msAzim,
                            nrMouse.msElev,
                            0,
                            nrMouse.msAppe,
                            li_bilinearf,
                            4

                        );

                        /* Display image on screen */
                        cvShowImage( nrName, nrdImage );

                        /* Exit condition */
                        if ( ( unsigned char ) cvWaitKey( 4 ) == 27 ) break;

                    }

                    /* Destroy display window */
                    cvDestroyWindow( nrName );

                    /* Release image memory */
                    cvReleaseImage( & nrdImage );

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create display buffer\n" ); }

                /* Release image memory */
                cvReleaseImage( & nriImage );

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    Source - Mouse callback
 */

    void nr_norama_view_mouse( int event, int x, int y, int flag, void * userdata ) {

        static int nrMemX = 0;
        static int nrMemY = 0;
        static int nrMemS = 0;

        static double nrAzim = 0.0;
        static double nrElev = 0.0;
        static double nrAppe = 0.0;

        nr_Mouse * nrMouse = ( nr_Mouse * ) userdata;

        if ( event == CV_EVENT_LBUTTONDOWN ) {

            if ( nrMemS == 0 ) {

                nrMemX = x;
                nrMemY = y;

                nrAzim = nrMouse->msAzim;
                nrElev = nrMouse->msElev;

                nrMemS = 1;

            }

        } else if ( event == CV_EVENT_RBUTTONDOWN ) {

            if ( nrMemS == 0 ) {

                nrMemY = y;

                nrAppe = nrMouse->msAppe;

                nrMemS = 2;

            }

        } else if ( ( event == CV_EVENT_LBUTTONUP ) || ( event == CV_EVENT_RBUTTONUP ) ) {

            nrMemS = 0;

        } else if ( event == CV_EVENT_MOUSEWHEEL ) {

            nrMouse->msAppe *= 1.2;

        } else if ( event == CV_EVENT_MOUSEHWHEEL ) {

            nrMouse->msAppe *= 0.9;

        } else {

            if ( nrMemS == 1 ) {

                nrMouse->msAzim = nrAzim - ( ( x - nrMemX ) * ( LG_PI / 180.0 ) * 0.05 );
                nrMouse->msElev = nrElev + ( ( y - nrMemY ) * ( LG_PI / 180.0 ) * 0.05 );

            } else if ( nrMemS == 2 ) {

                nrMouse->msAppe = nrAppe + ( ( y - nrMemY )  * ( LG_PI / 180.0 ) * 0.10 );

            }

        }

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

