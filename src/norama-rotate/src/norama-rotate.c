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

        /* Image variables */
        IplImage * ngIimage = NULL;
        IplImage * ngOimage = NULL;

        /* Image path variables */
        char ngIpath[256] = "input.png";
        char ngOpath[256] = "output.png";

        /* Interpolation descriptor variables */
        char ngMethod[256] = "bicubic";

        /* Rotation angles variables */
        float ngAngleX = 0.0;
        float ngAngleY = 0.0;
        float ngAngleZ = 0.0;

        /* Interpolation method variables */
        interp ngInter = gnomonic_interp_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--input"        , "-n" ), argv,   ngIpath , __STDP_STRING );
        stdp( stda( argc, argv,  "--output"       , "-o" ), argv,   ngOpath , __STDP_STRING );
        stdp( stda( argc, argv,  "--angle-x"      , "-x" ), argv, & ngAngleX, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--angle-y"      , "-y" ), argv, & ngAngleY, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--angle-z"      , "-z" ), argv, & ngAngleZ, __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation", "-i" ), argv,   ngMethod, __STDP_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( ngMethod, "bilinear" ) == 0 ) ngInter = gnomonic_interp_bilinearf;
        if ( strcmp( ngMethod, "bicubic"  ) == 0 ) ngInter = gnomonic_interp_bicubicf;
        if ( strcmp( ngMethod, "bipentic" ) == 0 ) ngInter = gnomonic_interp_bipenticf;

        /* Convert angles to radian */
        ngAngleX *= - ( M_PI / 180.0 );
        ngAngleY *= - ( M_PI / 180.0 );
        ngAngleZ *= - ( M_PI / 180.0 );

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) ) {

            /* Display usage */
            fprintf( stdout, NG_HELP );

        } else {

            /* Import input image */
            ngIimage = cvLoadImage( ngIpath, CV_LOAD_IMAGE_COLOR );

            /*  Verify input image reading */
            if ( ngIimage != NULL ) {

                /* Create image allocation */
                ngOimage = cvCreateImage( cvSize( ngIimage->width, ngIimage->height ), IPL_DEPTH_8U , ngIimage->nChannels );

                /* Verify allocation creation */
                if ( ngOimage != NULL ) {

                    /* Position angles */
                    float ngAngH = 0.0, ngAngV = 0.0;

                    /* Positionning variables */
                    float ngPX = 0, ngPY = 0;

                    /* Parsing variable */
                    int ngX = 0, ngY = 0;

                    /* Rotation matrix */
                    float ngMatrix[3][3] = {

                        { 
                            + cos( ngAngleZ ) * cos( ngAngleY ), 
                            + sin( ngAngleZ ) * cos( ngAngleX ) + cos( ngAngleZ ) * sin( ngAngleY ) * sin( ngAngleX ), 
                            + sin( ngAngleZ ) * sin( ngAngleX ) - cos( ngAngleZ ) * sin( ngAngleY ) * cos( ngAngleX ) 
                        },
                        { 
                            - sin( ngAngleZ ) * cos( ngAngleY ), 
                            + cos( ngAngleZ ) * cos( ngAngleX ) - sin( ngAngleZ ) * sin( ngAngleY ) * sin( ngAngleX ), 
                            + cos( ngAngleZ ) * sin( ngAngleX ) + sin( ngAngleZ ) * sin( ngAngleY ) * cos( ngAngleX ) 
                        },
                        { 
                            + sin( ngAngleY ), 
                            - cos( ngAngleY ) * sin( ngAngleX ), 
                            + cos( ngAngleY ) * cos( ngAngleX ) 
                        }

                    };

                    /* Sphere point vectors */
                    float ngVectori[3] = { 0.0, 0.0, 0.0 };
                    float ngVectorf[3] = { 0.0, 0.0, 0.0 };

                    /* Processing loop on y */
                    for ( ngY = 0; ngY < ngIimage->height; ngY ++ ) {

                        /* Processing loop on x */
                        for ( ngX = 0; ngX < ngIimage->width; ngX ++ ) {

                            /* Retrive position angles from pixels */
                            ngAngH = ( ( ( float ) ngX / ( ngIimage->width  - 1 ) ) * 2.0 ) * M_PI;
                            ngAngV = ( ( ( float ) ngY / ( ngIimage->height - 1 ) ) - 0.5 ) * M_PI;

                            /* Retrieve initial vector on sphere */
                            ngVectori[0] = cos( ngAngH ) * cos( ngAngV );
                            ngVectori[1] = sin( ngAngH ) * cos( ngAngV );
                            ngVectori[2] = sin( ngAngV );

                            /* Apply rotation transform */
                            ngVectorf[0] = ngMatrix[0][0] * ngVectori[0] + ngMatrix[0][1] * ngVectori[1] + ngMatrix[0][2] * ngVectori[2];
                            ngVectorf[1] = ngMatrix[1][0] * ngVectori[0] + ngMatrix[1][1] * ngVectori[1] + ngMatrix[1][2] * ngVectori[2];
                            ngVectorf[2] = ngMatrix[2][0] * ngVectori[0] + ngMatrix[2][1] * ngVectori[1] + ngMatrix[2][2] * ngVectori[2];

                            /* Retrieve rotated position angles - horizontal */
                            ngAngH = ngVectorf[0] / sqrt( ngVectorf[0] * ngVectorf[0] + ngVectorf[1] * ngVectorf[1] );

                            /* Case study */
                            if ( ngAngH >= 1.0 ) {

                                /* Assign horizontal angle */
                                ngAngH = 0.0;

                            } else if ( ngAngH <= - 1.0 ) {

                                /* Assign horizontal angle */
                                ngAngH = M_PI;

                            } else {

                                /* Case study */
                                if ( ngVectorf[1] < 0.0 ) {

                                    /* Assign horizontal angle */
                                    ngAngH = 2.0 * M_PI - acos( ngAngH );

                                } else {

                                    /* Assign horizontal angle */
                                    ngAngH = acos( ngAngH );

                                }

                            }

                            /* Retrieve rotated position angles - vertical */
                            ngAngV = asin( ngVectorf[2] );

                            /* Retrieve pixel components */
                            ngPX = ( ngAngH / ( 2.0 * M_PI ) ) * ( ngOimage->width  - 1 );
                            ngPY = ( ( ngAngV / M_PI ) + 0.5 ) * ( ngOimage->height - 1 );

                            /* Assign pixel value */
                            * ( ngOimage->imageData + ngOimage->nChannels * ( ngOimage->width * ngY + ngX )     ) = 
                            ngInter( ( unsigned char * ) ngIimage->imageData, ngIimage->width, ngIimage->height, ngIimage->nChannels, 0, ngPX, ngPY );
                            * ( ngOimage->imageData + ngOimage->nChannels * ( ngOimage->width * ngY + ngX ) + 1 ) = 
                            ngInter( ( unsigned char * ) ngIimage->imageData, ngIimage->width, ngIimage->height, ngIimage->nChannels, 1, ngPX, ngPY );
                            * ( ngOimage->imageData + ngOimage->nChannels * ( ngOimage->width * ngY + ngX ) + 2 ) = 
                            ngInter( ( unsigned char * ) ngIimage->imageData, ngIimage->width, ngIimage->height, ngIimage->nChannels, 2, ngPX, ngPY );

                        }

                    }

                    /* Export output image */
                    if ( cvSaveImage( ngOpath, ngOimage, NULL ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write output image\n" );

                    }

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create output image\n" ); }

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read input image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

