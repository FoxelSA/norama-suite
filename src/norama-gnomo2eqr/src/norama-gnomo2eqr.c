/*
 * feature suite - Panorama tools suite
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

        /* Argument reading index */
        int        ngArgs       = 1;

        /* Image variables */
        IplImage * ngIimage     = NULL;
        IplImage * ngMimage     = NULL;
        IplImage * ngOimage     = NULL;

        /* Image path variables */
        char       ngIpath[256] = "input.png";
        char       ngMpath[256] = "mask.png";
        char       ngEpath[256] = "eqr.png";
        char       ngOpath[256] = "output.png";

        /* Gnomonic projection variables */
        double     ngNadir_hor  = 0.0;
        double     ngNadir_ver  = 0.0;
        double     ngApper_hor  = 40.0 * ( GNOMONIC_PI / 180.0 );
        double     ngApper_ver  = 40.0 * ( GNOMONIC_PI / 180.0 );

        /* Interpolation method variables */
        interp      ngInter      = gnomonic_interp_bicubicf;

        /* Argument reading */
        while ( ngArgs < argc ) {

            /* Verify help */
            if ( strcmp( argv[ngArgs], "--help" ) == 0 ) {

                /* Display help */
                ng_gnomo2eqr_usage();

                /* Return to system */
                return( EXIT_SUCCESS );

            } else {

                /* Verify argument format */
                if ( ( ngArgs+1 ) < argc ) {

                    /* Read argument and parameter */
                    if ( strcmp( argv[ngArgs], "--gnomo"     ) == 0 ) strcpy( ngIpath, argv[ngArgs+1] );
                    if ( strcmp( argv[ngArgs], "--mask"      ) == 0 ) strcpy( ngMpath, argv[ngArgs+1] );
                    if ( strcmp( argv[ngArgs], "--equirect"  ) == 0 ) strcpy( ngEpath, argv[ngArgs+1] );
                    if ( strcmp( argv[ngArgs], "--output"    ) == 0 ) strcpy( ngOpath, argv[ngArgs+1] );
                    if ( strcmp( argv[ngArgs], "--nadir-hor" ) == 0 ) ngNadir_hor = atof( argv[ngArgs+1] ) * ( GNOMONIC_PI / 180.0 );
                    if ( strcmp( argv[ngArgs], "--nadir-ver" ) == 0 ) ngNadir_ver = atof( argv[ngArgs+1] ) * ( GNOMONIC_PI / 180.0 );
                    if ( strcmp( argv[ngArgs], "--apper-hor" ) == 0 ) ngApper_hor = atof( argv[ngArgs+1] ) * ( GNOMONIC_PI / 180.0 );
                    if ( strcmp( argv[ngArgs], "--apper-ver" ) == 0 ) ngApper_ver = atof( argv[ngArgs+1] ) * ( GNOMONIC_PI / 180.0 );
                    if ( strcmp( argv[ngArgs], "--interpol"  ) == 0 ) {

                        /* Read parameter */
                        if ( strcmp( argv[ngArgs+1], "bilinear" ) == 0 ) ngInter = gnomonic_interp_bilinearf;
                        if ( strcmp( argv[ngArgs+1], "bicubic"  ) == 0 ) ngInter = gnomonic_interp_bicubicf;
                        if ( strcmp( argv[ngArgs+1], "bipentic" ) == 0 ) ngInter = gnomonic_interp_bipenticf;

                    }
                

                } else {

                    /* Display message */
                    fprintf( stdout, "Error : Arguments and parameters format\n" );

                    /* Return to system */
                    return( EXIT_SUCCESS );

                }

            }

            /* Increment reading */
            ngArgs += 2;

        }

        /* Import input image */
        ngIimage = cvLoadImage( ngIpath, CV_LOAD_IMAGE_COLOR     );
        ngMimage = cvLoadImage( ngMpath, CV_LOAD_IMAGE_GRAYSCALE );
        ngOimage = cvLoadImage( ngEpath, CV_LOAD_IMAGE_COLOR     );

        /*  Verify input image reading */
        if ( ( ngIimage != NULL ) && ( ngMimage != NULL ) && ( ngOimage != NULL ) ) {

            /* Gnomonic reprojection */
            gnomonic_gte_blend(

                ( unsigned char * ) ngOimage->imageData,
                ngOimage->width, 
                ngOimage->height, 
                ngOimage->nChannels, 
                ( unsigned char * ) ngIimage->imageData,
                ( unsigned char * ) ngMimage->imageData,
                ngIimage->width, 
                ngIimage->height,
                ngIimage->nChannels,
                ngNadir_hor,
                ngNadir_ver,
                ngApper_hor,
                ngApper_ver,
                ngInter

            );

            /* Export output image */
            if ( cvSaveImage( ngOpath, ngOimage, NULL ) == 0 ) {

                /* Display message */
                fprintf( stdout, "Error : Unable to write output image\n" );

            }

        /* Display message */
        } else { fprintf( stdout, "Error : Unable to read input image(s)\n" ); }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

    void ng_gnomo2eqr_usage ( void ) {

        /* Display help */
        printf( "NAME\n\n\tnorama-gnomo2eqr - Inverted gnomonic projection\n\n"                  );
        printf( "SYNOPSIS\n\n\tnorama-gnomo2eqr [OPTIONS]...\n\n"                                );
        printf( "DESCRIPTION\n\n\tReproject the gnomonic image into equirectangular mapping\n\n" );
        printf( "\t--gnomo \tPath to the gnomonic image\n"                                       );
        printf( "\t--equirect\tPath to equirectangular image\n"                                  );
        printf( "\t--mask  \tPath to gnomonic greyscale mask image\n"                            );
        printf( "\t--output\tPath to the output equirectangular result image\n"                  );
        printf( "\t--nadir-hor\tHorizontal angular position of the line of sight [0,2PI[\n"      );
        printf( "\t--nadir-ver\tVertical angular position of the line of sight [-PI/2,+PI/2]\n"  );
        printf( "\t--apper-hor\tHalf horizontal angular apperture of the gnomonic projection\n"  );
        printf( "\t--apper-ver\tHalf vertical angular apperture of the gnomonic projection\n"    );
        printf( "\t--interpol\tInterpolation method : bilinear/bicubic/bipentic\n\n"             );
        printf( "AUTHOR\n\n\tWritten by Nils Hamel <nils.hamel@foxel.ch>\n\n"                    );
        printf( "COPYRIGHT\n\n\tCopyright (c) 2013-2014 FOXEL SA - http://foxel.ch\n\t"          );
        printf( "GNU AGPLv3 - Please read <http://foxel.ch/license> for more information\n\n"    );

    }

