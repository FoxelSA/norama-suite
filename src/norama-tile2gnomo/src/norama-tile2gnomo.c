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

    # include "norama-tile2gnomo.h"

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

        /* Image parameters variables */
        int ngOwidth  = 0;
        int ngOheight = 0;

        /* Equirectangular tile variables */
        int   ngFullWidth  = 0;
        int   ngFullHeight = 0;
        int   ngEqrPosX    = 0;
        int   ngEqrPosY    = 0;
        float ngEqrCenX    = 0.0;
        float ngEqrCenY    = 0.0;

        /* Interpolation method variables */
        inter_Method_t ngInter = inter_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular", "-e" ), argv,   ngIpath     , __STDP_STRING );
        stdp( stda( argc, argv,  "--rectilinear"    , "-r" ), argv,   ngOpath     , __STDP_STRING );
        stdp( stda( argc, argv,  "--width"          , "-w" ), argv, & ngOwidth    , __STDP_INT    );
        stdp( stda( argc, argv,  "--height"         , "-t" ), argv, & ngOheight   , __STDP_INT    );
        stdp( stda( argc, argv,  "--full-width"     , "-f" ), argv, & ngFullWidth , __STDP_INT    );
        stdp( stda( argc, argv,  "--full-height"    , "-g" ), argv, & ngFullHeight, __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-position-x" , "-x" ), argv, & ngEqrPosX   , __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-position-y" , "-y" ), argv, & ngEqrPosY   , __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-center-x"   , "-c" ), argv, & ngEqrCenX   , __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--eqr-center-y"   , "-d" ), argv, & ngEqrCenY   , __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation"  , "-i" ), argv,   ngMethod    , __STDP_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( ngMethod, "bilinear" ) == 0 ) ngInter = inter_bilinearf;
        if ( strcmp( ngMethod, "bicubic"  ) == 0 ) ngInter = inter_bicubicf;
        if ( strcmp( ngMethod, "bipentic" ) == 0 ) ngInter = inter_bipenticf;

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) ) {

            /* Display usage */
            fprintf( stdout, NG_HELP );

        } else {

            /* Import input image */
            ngIimage = cvLoadImage( ngIpath, CV_LOAD_IMAGE_COLOR );

            /*  Verify input image reading */
            if ( ngIimage != NULL ) {

                /* Initialize output image structure */
                ngOimage = cvCreateImage( cvSize( ngOwidth, ngOheight ), IPL_DEPTH_8U , ngIimage->nChannels );

                /* Verify output image creation */
                if ( ngOimage != NULL ) {

                    /* Equirectangular tile gnomonic reprojection */
                    gnomonic_ttg(

                        ( inter_C8_t * ) ngIimage->imageData,
                        ngIimage->width, 
                        ngIimage->height, 
                        ngIimage->nChannels, 
                        ( inter_C8_t * ) ngOimage->imageData, 
                        ngOimage->width, 
                        ngOimage->height,
                        ngOimage->nChannels,
                        ngFullWidth,
                        ngFullHeight,
                        ngEqrPosX,
                        ngEqrPosY,
                        ngEqrCenX + ngEqrPosX,
                        ngEqrCenY + ngEqrPosY,
                        ngInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( ngOpath, ngOimage, NULL ) == 0 ) {

                        /* Display message */
                        fprintf( stdout, "Error : Unable to write gnomonic image\n" );

                    }

                /* Display message */
                } else { fprintf( stdout, "Error : Unable to create gnomonic image\n" ); }

            /* Display message */
            } else { fprintf( stdout, "Error : Unable to read equirectangular tile image\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

