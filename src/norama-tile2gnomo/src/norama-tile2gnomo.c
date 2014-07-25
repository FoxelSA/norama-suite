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
        IplImage * nrIImage = NULL;
        IplImage * nrOImage = NULL;

        /* Image path variables */
        char nrIPath[256] = { 0 };
        char nrOPath[256] = { 0 };

        /* Interpolation descriptor variables */
        char nrMethod[256] = { 0 };

        /* Image parameters variables */
        int nrOWidth  = 0;
        int nrOHeight = 0;

        /* Equirectangular tile variables */
        int   nrFullWidth  = 0;
        int   nrFullHeight = 0;
        int   nrEqrPosX    = 0;
        int   nrEqrPosY    = 0;
        float nrEqrCenX    = 0.0;
        float nrEqrCenY    = 0.0;

        /* Interpolation method variables */
        inter_Method_t nrInter = inter_bicubicf;

        /* Search in parameters */
        stdp( stda( argc, argv,  "--equirectangular", "-e" ), argv,   nrIPath     , __STDP_STRING );
        stdp( stda( argc, argv,  "--rectilinear"    , "-r" ), argv,   nrOPath     , __STDP_STRING );
        stdp( stda( argc, argv,  "--width"          , "-w" ), argv, & nrOWidth    , __STDP_INT    );
        stdp( stda( argc, argv,  "--height"         , "-t" ), argv, & nrOHeight   , __STDP_INT    );
        stdp( stda( argc, argv,  "--full-width"     , "-f" ), argv, & nrFullWidth , __STDP_INT    );
        stdp( stda( argc, argv,  "--full-height"    , "-g" ), argv, & nrFullHeight, __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-position-x" , "-x" ), argv, & nrEqrPosX   , __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-position-y" , "-y" ), argv, & nrEqrPosY   , __STDP_INT    );
        stdp( stda( argc, argv,  "--eqr-center-x"   , "-c" ), argv, & nrEqrCenX   , __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--eqr-center-y"   , "-d" ), argv, & nrEqrCenY   , __STDP_FLOAT  );
        stdp( stda( argc, argv,  "--interpolation"  , "-i" ), argv,   nrMethod    , __STDP_STRING );

        /* Sepcify interpolation method */
        if ( strcmp( nrMethod, "bilinear" ) == 0 ) nrInter = inter_bilinearf;
        if ( strcmp( nrMethod, "bicubic"  ) == 0 ) nrInter = inter_bicubicf;
        if ( strcmp( nrMethod, "bipentic" ) == 0 ) nrInter = inter_bipenticf;

        /* Software swicth */
        if ( stda( argc, argv, "--help", "-h" ) ) {

            /* Display usage */
            fprintf( stdout, NG_HELP );

        } else {

            /* Import input image */
            nrIImage = cvLoadImage( nrIPath, CV_LOAD_IMAGE_COLOR );

            /*  Verify input image reading */
            if ( nrIImage != NULL ) {

                /* Initialize output image structure */
                nrOImage = cvCreateImage( cvSize( nrOWidth, nrOHeight ), IPL_DEPTH_8U , nrIImage->nChannels );

                /* Verify output image creation */
                if ( nrOImage != NULL ) {

                    /* Equirectangular tile gnomonic reprojection */
                    gnomonic_ttg(

                        ( inter_C8_t * ) nrIImage->imageData,
                        nrIImage->width, 
                        nrIImage->height, 
                        nrIImage->nChannels, 
                        ( inter_C8_t * ) nrOImage->imageData, 
                        nrOImage->width, 
                        nrOImage->height,
                        nrOImage->nChannels,
                        nrFullWidth,
                        nrFullHeight,
                        nrEqrPosX,
                        nrEqrPosY,
                        nrEqrCenX + nrEqrPosX,
                        nrEqrCenY + nrEqrPosY,
                        nrInter

                    );

                    /* Export output image */
                    if ( cvSaveImage( nrOPath, nrOImage, NULL ) == 0 ) {

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

