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

    # include "norama-stitch.h"

/*
    Source - Software main function
 */

    int main ( int argc, char ** argv ) {

        /* Mapping size variables */
        long nrMapW = 0;
        long nrMapH = 0;

        /* Exportation options variables */
        int nrOption = -1;

        /* Stitching variables */
        char * nriPosx = NULL;
        char * nriPosy = NULL;

        /* Image path variables */
        char * nriPath = NULL;
        char * nraPath = NULL;
        char * nroPath = NULL;

        /* Image count variables */
        long nrSize = 0;

        /* Stitching array variables */
        long * nraPosx = NULL;
        long * nraPosy = NULL;

        /* Image allocation variables */
        IplImage ** nriImage = NULL;
        IplImage  * nroImage = NULL;

        /* Search in parameters */
        lc_stdp( lc_stda( argc, argv, "--width"     , "-W" ), argv, & nrMapW  , LC_LONG   );
        lc_stdp( lc_stda( argc, argv, "--height"    , "-H" ), argv, & nrMapH  , LC_LONG   );
        lc_stdp( lc_stda( argc, argv, "--export"    , "-q" ), argv, & nrOption, LC_INT    );
        lc_stdp( lc_stda( argc, argv, "--input"     , "-i" ), argv, & nriPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--alpha"     , "-a" ), argv, & nraPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--output"    , "-o" ), argv, & nroPath , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--x-position", "-x" ), argv, & nriPosx , LC_STRING );
        lc_stdp( lc_stda( argc, argv, "--y-position", "-y" ), argv, & nriPosy , LC_STRING );

        /* Software swicth */
        if ( lc_stda( argc, argv, "--help", "-h" ) || ( argc <= 1 ) ) {

            /* Display usage */
            fprintf( stdout, NR_HELP );

        } else {

            /* Consistency verification and element count */
            if ( ( nrSize = nr_stitch_consistency( argc, argv, nriPath, nraPath, nriPosx, nriPosy ) ) > 0 ) {

                /* Create image allocation */
                nroImage = cvCreateImage( cvSize( nrMapW, nrMapH ), IPL_DEPTH_8U, 3 );

                /* Verify allocation creation */
                if ( nroImage != NULL ) {

                    /* Create input image array */
                    if ( ( nriImage = nr_stitch_ilist_create( nriPath, nrSize ) ) != NULL ) {

                        /* Create x-position array */
                        if ( ( nraPosx = nr_stitch_nlist_create( nriPosx, nrSize ) ) != NULL ) {
                        
                            /* Create y-position array */
                            if ( ( nraPosy = nr_stitch_nlist_create( nriPosy, nrSize ) ) != NULL ) {

                                /* Stitching procedure */
                                nr_stitch( nrSize, nriImage, nraPosx, nraPosy, nroImage );

                            /* Display message */
                            } else { fprintf( LC_ERR, "Error : Unable to import y coordinates\n" ); }

                        /* Display message */
                        } else { fprintf( LC_ERR, "Error : Unable to import x coordinates\n" ); }

                    /* Display message */
                    } else { fprintf( LC_ERR, "Error : Unable to import source image(s)\n" ); }

                    /* Delete input image array */
                    nr_stitch_ilist_delete( nrSize, nriImage );

                    /* Export output image */
                    if ( lc_imwrite( nroPath, nroImage, nrOption ) == 0 ) {

                        /* Display message */
                        fprintf( LC_ERR, "Error : Unable to write output image\n" );

                    }

                    /* Release image memory */
                    cvReleaseImage( & nroImage );

                /* Display message */
                } else { fprintf( LC_ERR, "Error : Unable to create output image\n" ); }

            /* Display message */
            } else { fprintf( LC_ERR, "Error : Stitching parameter not consistant\n" ); }

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

/*
    Source - 
 */

    int nr_stitch( long nrSize, IplImage ** nriImage, long * nrPosx, long * nrPosy, IplImage * nroImage ) {

        /* Image information variables */
        long nriWidth   = 0;
        long nriHeight  = 0;
        long nriChannel = 0;
        long nriLength  = 0;
        long nroWidth   = nroImage->width;
        long nroHeight  = nroImage->height;
        long nroChannel = nroImage->nChannels;
        long nroLength  = nroImage->widthStep;

        /* Parsing variables */
        long nrParse = 0;
        long nrLayer = 0;

        /* Element access variables */
        long nrEx = 0;
        long nrEy = 0;
        long nrGx = 0;
        long nrGy = 0;

        /* Fusion array variables */
        float * nrfWeight = NULL;
        float * nrfColors = NULL;

        /* Buffer variables */
        float nrwBuffer = 0.0;
        float nrcBuffer = 0.0;

        /* Returned value variables */
        int nrReturn = 0;

        /* Allocating weights array */
        if ( ( nrfWeight = ( float * ) malloc( nroWidth * nroHeight * sizeof( float ) ) ) != NULL ) {

            /* Allocating colormetric array */
            if ( ( nrfColors = ( float * ) malloc( nroWidth * nroHeight * sizeof( float ) ) ) != NULL ) {

                /* Reset weights array */
                memset( nrfWeight, 0, nroWidth * nroHeight * sizeof( float ) );

                /* Creating weights array */
                for ( nrParse = 0; nrParse < nrSize; nrParse ++ ) {

                    /* Extract source image information */
                    nriWidth   = ( * ( nriImage + nrParse ) )->width;
                    nriHeight  = ( * ( nriImage + nrParse ) )->height;
                    nriChannel = ( * ( nriImage + nrParse ) )->nChannels;
                    nriLength  = ( * ( nriImage + nrParse ) )->widthStep;

                    /* Parsing image elements - y */
                    for ( nrEy = 0; nrEy < nriHeight; nrEy ++ ) {

                        /* Parsing image elements - x */
                        for ( nrEx = 0; nrEx < nriWidth; nrEx ++ ) {

                            /* Compose global mapping coordinates */
                            nrGx = ( nrEx + nroWidth  + * ( nrPosx + nrParse ) ) % nroWidth;
                            nrGy = ( nrEy + nroHeight + * ( nrPosy + nrParse ) ) % nroHeight;

                            /* Verify alpha availability */
                            if ( ( * ( nriImage + nrParse ) )->nChannels == 4 ) {

                                /* Add alpha weight */
                                * ( nrfWeight + ( nrGy * nroWidth ) + nrGx ) += ( ( float ) * ( ( * ( nriImage + nrParse ) )->imageData + nriLength * nrEy + nrEx * nriChannel + 3 ) ) / 255.0f;

                            } else {

                                /* Add standard weight */
                                * ( nrfWeight + ( nrGy * nroWidth ) + nrGx ) += 1.0;

                            }

                        }

                    }

                }

                /* Parsing color layers */
                for ( nrLayer = 0; nrLayer < 3; nrLayer ++ ) {

                    /* Reset colorimetric array */
                    memset( nrfColors, 0, nroWidth * nroHeight * sizeof( float ) );

                    /* Creating layer weighted array */
                    for ( nrParse = 0; nrParse < nrSize; nrParse ++ ) {

                        /* Extract source image information */
                        nriWidth   = ( * ( nriImage + nrParse ) )->width;
                        nriHeight  = ( * ( nriImage + nrParse ) )->height;
                        nriChannel = ( * ( nriImage + nrParse ) )->nChannels;
                        nriLength  = ( * ( nriImage + nrParse ) )->widthStep;

                        /* Parsing image elements - y */
                        for ( nrEy = 0; nrEy < nriHeight; nrEy ++ ) {

                            /* Parsing image elements - x */
                            for ( nrEx = 0; nrEx < nriWidth; nrEx ++ ) {

                                /* Compose global mapping coordinates */
                                nrGx = ( nrEx + nroWidth  + * ( nrPosx + nrParse ) ) % nroWidth;
                                nrGy = ( nrEy + nroHeight + * ( nrPosy + nrParse ) ) % nroHeight;

                                /* Verify alpha availability */
                                if ( ( * ( nriImage + nrParse ) )->nChannels == 4 ) {

                                    /* Extract elements */
                                    nrwBuffer = ( ( float ) * ( ( * ( nriImage + nrParse ) )->imageData + nriLength * nrEy + nrEx * nriChannel + 3       ) ); 
                                    nrcBuffer = ( ( float ) * ( ( * ( nriImage + nrParse ) )->imageData + nriLength * nrEy + nrEx * nriChannel + nrLayer ) ); 

                                    /* Add weighted component */
                                    * ( nrfColors + ( nrGy * nroWidth ) + nrGx ) += ( nrcBuffer / 255.0f ) * ( nrwBuffer / 255.0f );

                                } else {

                                    /* Extract elements */
                                    nrcBuffer = ( ( float ) * ( ( * ( nriImage + nrParse ) )->imageData + nriLength * nrEy + nrEx * nriChannel + nrLayer ) );

                                    /* Add unweighted component */
                                    * ( nrfColors + ( nrGy * nroWidth ) + nrGx ) += ( nrcBuffer / 255.0f );

                                }

                            }

                        }

                    }

                    /* Parsing output mapping - y */
                    for ( nrGy = 0; nrGy < nroHeight; nrGy ++ ) {

                        /* Parsing output mapping - x */
                        for ( nrGx = 0; nrGx < nroWidth; nrGx ++ ) {

                            /* Extract element colorimetric sum and weight */
                            nrcBuffer = * ( nrfColors + ( nrGy * nroWidth ) + nrGx );
                            nrwBuffer = * ( nrfWeight + ( nrGy * nroWidth ) + nrGx );

                            /* Computing layer element */
                            * ( nroImage->imageData + nroLength * nrGy + nrGx * nroChannel + nrLayer ) = ( unsigned char ) ( 255.0f * ( nrcBuffer / nrwBuffer ) );

                        }

                    }

                }

            /* Update returned value */
            } else { nrReturn = 1; }

        /* Update returned value */
        } else { nrReturn = 1; }

        /* Return status value */
        return( nrReturn );

    }

/*
    Source - 
 */

    long nr_stitch_consistency(

        int                argc,
        char **            argv,
        char const * const nriPath, 
        char const * const nraPath, 
        char const * const nriPosx, 
        char const * const nriPosy

    ) {

        /* Consistency variables */
        long nriPathElement = nr_stitch_count( nriPath );
        long nraPathElement = nr_stitch_count( nraPath );
        long nriPosxElement = nr_stitch_count( nriPosx );
        long nriPosyElement = nr_stitch_count( nriPosy );

        /* Verify stitching mode */
        if ( lc_stda( argc, argv, "--elphel", "-E" ) ) {

            /* Return consistency error */
            return( 0 );

        } else {

            /* Consistency verification */
            if ( ( ( nriPathElement == nraPathElement ) || ( nraPathElement == 0 ) ) && ( nriPathElement == nriPosxElement ) && ( nriPathElement == nriPosyElement ) ) {

                /* Return number of element to stitch */
                return( nriPathElement );

            } else {

                /* Return consistency error */
                return( 0 );

            }

        }

    }

/*
    Source - 
 */

    IplImage ** nr_stitch_ilist_create( char * nriPath, long nrSize ) {

        /* List token variables */
        char * nripPath = nriPath;

        /* Parsing variables */
        long nrParse = 0;

        /* Returned variables */
        IplImage ** nrReturn = 0;

        /* Allocating images array */
        if ( ( nrReturn = ( IplImage ** ) malloc( nrSize * sizeof( void * ) ) ) != NULL ) {

            /* Initialize array */
            for ( nrParse = 0; nrParse < nrSize; nrParse ++ ) * ( nrReturn + nrParse ) = NULL;

            /* Parsing input lists */
            for ( nrParse = 0; nrParse < nrSize; nrParse ++ ) {

                /* Search tokens end */
                nriPath = nr_stitch_next( nripPath );

                /* Import source image */
                * ( nrReturn + nrParse ) = cvLoadImage( nripPath, CV_LOAD_IMAGE_UNCHANGED );

                /* Swap previous token initial reference */
                nripPath = nriPath + 1;

            }
        
        }

        /* Return success */
        return( nrReturn );

    }

    void nr_stitch_ilist_delete( long nrSize, IplImage ** nrArray ) {

        /* Parsing variables */
        long nrParse = 0;

        /* Verify array allocation */
        if ( nrArray != NULL ) {

            /* Parsing array elements */
            for ( nrParse = 0; nrParse < nrSize; nrParse ++ ) {

                /* Unload array element */
                cvReleaseImage( nrArray + nrParse );

            }

            /* Unallocate array memory */
            free( nrArray );

        }

    }

/*
    Source - 
 */

    long * nr_stitch_nlist_create( char * nriNumber, long nrSize ) {

        /* List token variables */
        char * nripNumber = nriNumber;

        /* Parsing variables */
        long nrParse = 0;

        /* Returned variables */
        long * nrReturn = NULL;

        /* Allocating number array */
        if ( ( nrReturn = ( long * ) malloc( nrSize * sizeof( long ) ) ) != NULL ) {

            /* Parsing list */
            for( nrParse = 0; nrParse < nrSize; nrParse ++ ) {

                /* Search token limit */
                nriNumber = nr_stitch_next( nripNumber );

                /* Import token value */
                * ( nrReturn + nrParse ) = atol( nripNumber );

                /* Jumping on next token */
                nripNumber = nriNumber + 1;

            }

        }

        /* Return allocated array */
        return( nrReturn );

    }

    void nr_stitch_nlist_delete( long * nrArray ) {

        /* Verify array allocation */
        if ( nrArray != NULL ) {

            /* Unallocate array memory */
            free( nrArray );

        }

    }

/*
    Source - 
 */

    long nr_stitch_count( char const * const nrList ) {

        /* Parsing variables */
        long nrParse = 0;

        /* Returned value variables */
        long nrReturn = 0;

        /* Verify string pointer */
        if ( nrList != NULL ) {

            /* Verify string length */
            if ( strlen( nrList ) > 0 ) {

                /* Parsing list string */
                while ( * ( nrList + nrParse ) != '\0' ) {

                    /* Searching for character */
                    if ( * ( nrList + ( nrParse ++ ) ) == ',' ) nrReturn ++;

                }

                /* Return count */
                return( nrReturn + 1 );

            } else {

                /* Return count */
                return( 0 );

            }

        } else {

            /* Return count */
            return( 0 );

        }

    }

/*
    Source - 
 */

    char * nr_stitch_next( char * nrList ) {

        /* Verify string pointer */
        if ( nrList != NULL ) {

            /* Search for next separation token */
            while ( ( * ( nrList ) != '\0' ) && ( * ( nrList ) != ',' ) ) nrList ++;

            /* Force terminal zero */
            * ( nrList ) = '\0';

        }

        /* Return next element */
        return( nrList );

    }

