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

        /* Window name variables */
        char nrName[256] = "norama-view";

        /* Keyevent variables */
        unsigned char nrEvent = 0;

        /* Parallel processing variables */
        int nrThread = NR_DFT_THREAD;

        /* Display variables */
        int nrWidth  = 0;
        int nrHeight = 0;

        /* Display scale variables */
        float nrScale = NR_DFT_SCALE;

        /* Image allocation variables */
        IplImage * nriImage = NULL;
        IplImage * nrdImage = NULL;

        /* Mouse control variables */
        nr_Mouse nrMouse = { 0.0, 0.0, NR_DFT_APPER, 0.0 };

        /* Search in parameters */
        stdp( stda( argc, argv, "--input"  , "-i" ), argv,   nriPath , NR_STRING );
        stdp( stda( argc, argv, "--threads", "-t" ), argv, & nrThread, NR_INT    );
        stdp( stda( argc, argv, "--width"  , "-x" ), argv, & nrWidth , NR_INT    );
        stdp( stda( argc, argv, "--height" , "-y" ), argv, & nrHeight, NR_INT    );
        stdp( stda( argc, argv, "--scale"  , "-s" ), argv, & nrScale , NR_FLOAT  );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Import input image */
            nriImage = cvLoadImage( nriPath, CV_LOAD_IMAGE_UNCHANGED );

            /*  Verify input image reading */
            if ( nriImage != NULL ) {

                /* Obtain screen resolution */
                if ( ( nrWidth == 0 ) || ( nrHeight == 0 ) ) nr_view_display( & nrWidth, & nrHeight, nrScale );

                /* Store display dimenstion */
                nrMouse.msWidth = nrWidth;

                /* Create image allocation */
                nrdImage = cvCreateImage( cvSize( nrWidth, nrHeight ), IPL_DEPTH_8U , nriImage->nChannels );

                /* Verify allocation creation */
                if ( nrdImage != NULL ) {

                    /* Create display window */
                    cvNamedWindow( nrName, CV_WINDOW_NORMAL );

                    /* Setting window on full screen */
                    cvSetWindowProperty( nrName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN );

                    /* Define window mouse event callback function */
                    cvSetMouseCallback( nrName, & ( nr_view_mouse ), & ( nrMouse ) );

                    /* Display pseudo-infinite loop */
                    while ( ( nrEvent = ( unsigned char ) cvWaitKey( 1 ) ) != NR_KEY_ESCAPE ) {

                        /* Boundaries management */
                        if ( nrMouse.msAppe > NR_MAX_APPER ) {

                            /* Re-range field-of-view */
                            nrMouse.msAppe = NR_MAX_APPER;

                        } else
                        if ( nrMouse.msAppe < NR_MIN_APPER ) {

                            /* Re-range field-of-view */
                            nrMouse.msAppe = NR_MIN_APPER;

                        }

                        /* Keyevent management */
                        if ( nrEvent == NR_KEY_F ) {

                            /* Reset field-of-view */
                            nrMouse.msAppe = NR_DFT_APPER;

                        } else
                        if ( nrEvent == NR_KEY_R ) {

                            /* Reset angular position */
                            nrMouse.msAzim = 0.0;
                            nrMouse.msElev = 0.0;

                            /* Reset field-of-view */
                            nrMouse.msAppe = NR_DFT_APPER;

                        }

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
                            0.0,
                            nrMouse.msAppe,
                            li_bilinearf,
                            nrThread

                        );

                        /* Display image on screen */
                        cvShowImage( nrName, nrdImage );

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

    void nr_view_mouse( int event, int x, int y, int flag, void * userdata ) {

        /* Mouse mode variables */
        static int nrMode = NR_MS_NONE;

        /* Mouse memory variables */
        static int nrMouseX = 0;
        static int nrMouseY = 0;

        /* Display viewpoint variables */
        static double nrAzim = 0.0;
        static double nrElev = 0.0;
        static double nrAppe = 0.0;

        /* Mouse handle structure variables */
        nr_Mouse * nrMouse = ( nr_Mouse * ) userdata;

        /* Check current mode */
        if ( nrMode == NR_MS_NONE ) {

            /* Check mouse event */
            if ( event == CV_EVENT_LBUTTONDOWN ) {

                /* Memorize event position */
                nrMouseX = x;
                nrMouseY = y;

                /* Memorize event state */
                nrAzim = nrMouse->msAzim;
                nrElev = nrMouse->msElev;

                /* Update mode */
                nrMode = NR_MS_MOVE;

            } else 
            if ( event == CV_EVENT_RBUTTONDOWN ) {

                /* Memorize event position */
                nrMouseY = y;

                /* Memorize event state */
                nrAppe = nrMouse->msAppe;

                /* Update mode */
                nrMode = NR_MS_CFOV;

            }

        } else
        if ( ( nrMode == NR_MS_MOVE ) && ( event == CV_EVENT_LBUTTONUP ) ) {

            /* Reset mode */
            nrMode = NR_MS_NONE;

        } else 
        if ( ( nrMode == NR_MS_CFOV ) && ( event == CV_EVENT_RBUTTONUP ) ) {

            /* Reset mode */
            nrMode = NR_MS_NONE;

        } else
        if ( nrMode == NR_MS_MOVE ) {

            /* Update angular position */
            nrMouse->msAzim = nrAzim - ( ( ( x - nrMouseX ) / nrMouse->msWidth ) * nrMouse->msAppe );
            nrMouse->msElev = nrElev + ( ( ( y - nrMouseY ) / nrMouse->msWidth ) * nrMouse->msAppe );

        } else
        if ( nrMode == NR_MS_CFOV ) {

            /* Update FOV value */
            nrMouse->msAppe = nrAppe - ( ( y - nrMouseY )  * ( LG_PI / 180.0 ) * 0.20 );

        }

    }

/*
    Source - Screen resolution
*/

    void nr_view_display( int * nrWidth, int * nrHeight, float nrScale ) {

        /* Setting default resolution */
        * nrWidth  = 1366;
        * nrHeight = 768;

        /* Create display handle */
        Display * nrDisplay = XOpenDisplay( NULL );

        /* Check handle creation */
        if ( nrDisplay != 0 ) {

            /* Create screen handle */
            Screen * nrScreen = DefaultScreenOfDisplay( nrDisplay );

            /* Check handle creation */
            if ( nrScreen != 0 ) {

                /* Setting screen resolution */
                * nrWidth  = ( int ) ( nrScreen->width  * nrScale );
                * nrHeight = ( int ) ( nrScreen->height * nrScale );

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

