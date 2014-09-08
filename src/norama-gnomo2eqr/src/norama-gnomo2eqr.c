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

    # include "norama-gnomo2eqr.h"

/*
    Source - Entry point
 */

    int main ( int argc, char ** argv ) {

        /* Image variables */
        IplImage * nrIImage = NULL;
        IplImage * nrMImage = NULL;
        IplImage * nrOImage = NULL;

        /* Image path variables */
        char nrIPath[256] = { 0 };
        char nrMPath[256] = { 0 };
        char nrEPath[256] = { 0 };
        char nrOPath[256] = { 0 };

        /* Interpolation descriptor variables */
        char nrMethod[256] = { 0 };

        /* Gnomonic projection variables */
        float nrNadirHor = 0.0;
        float nrNadirVer = 0.0;
        float nrApperHor = 45.0;
        float nrApperVer = 45.0;

        /* Interpolation method variables */
        li_Method_t nrInter = inter_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular"     , "-e" ), argv,   nrEPath   , NR_STRING );
        stdp( stda( argc, argv,  "--mask"                , "-m" ), argv,   nrMPath   , NR_STRING );
        stdp( stda( argc, argv,  "--rectilinear"         , "-r" ), argv,   nrIPath   , NR_STRING );
        stdp( stda( argc, argv,  "--output"              , "-o" ), argv,   nrOPath   , NR_STRING );
        stdp( stda( argc, argv,  "--nadir-horizontal"    , "-u" ), argv, & nrNadirHor, NR_FLOAT  );
        stdp( stda( argc, argv,  "--nadir-vertical"      , "-v" ), argv, & nrNadirVer, NR_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-horizontal", "-a" ), argv, & nrApperHor, NR_FLOAT  );
        stdp( stda( argc, argv,  "--apperture-vertical"  , "-b" ), argv, & nrApperVer, NR_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation"       , "-i" ), argv,   nrMethod  , NR_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( nrMethod, "bilinear" ) == 0 ) nrInter = li_bilinearf;
        if ( strcmp( nrMethod, "bicubic"  ) == 0 ) nrInter = li_bicubicf;
        if ( strcmp( nrMethod, "bipentic" ) == 0 ) nrInter = li_bipenticf;
        if ( strcmp( nrMethod, "biheptic" ) == 0 ) nrInter = li_bihepticf;

        /* Convert angles to radian */
        nrNadirHor *= ( LG_PI / 180.0 );
        nrNadirVer *= ( LG_PI / 180.0 );
        nrApperHor *= ( LG_PI / 180.0 );
        nrApperVer *= ( LG_PI / 180.0 );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Import input image */
            nrIImage = cvLoadImage( nrIPath, CV_LOAD_IMAGE_COLOR     );
            nrMImage = cvLoadImage( nrMPath, CV_LOAD_IMAGE_GRAYSCALE );
            nrOImage = cvLoadImage( nrEPath, CV_LOAD_IMAGE_COLOR     );

            /*  Verify input image reading */
            if ( ( nrIImage != NULL ) && ( nrMImage != NULL ) && ( nrOImage != NULL ) ) {

                /* Verify rectilinear image and mask consistency */
                if ( ( nrIImage->width == nrMImage->width ) && ( nrIImage->height == nrMImage->height ) ) {

                    /* Gnomonic reprojection */
                    lg_gte_blend(

                        ( inter_C8_t * ) nrOImage->imageData,
                        nrOImage->width, 
                        nrOImage->height, 
                        nrOImage->nChannels, 
                        ( inter_C8_t * ) nrIImage->imageData,
                        ( inter_C8_t * ) nrMImage->imageData,
                        nrIImage->width, 
                        nrIImage->height,
                        nrIImage->nChannels,
                        nrNadirHor,
                        nrNadirVer,
                        nrApperHor,
                        nrApperVer,
                        nrInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( nrOPath, nrOImage, NULL ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write output image\n" );

                    }

                } else { fprintf( stdout, "Error : Rectilinear image and its mask have different size\n" ); }

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image(s)\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    Source - Arguments common handler
 */

    int stda( int argc, char ** argv, const char * const ltag, const char * const stag ) {

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

    void stdp( int argi, char ** argv, void * param, int type ) {

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


