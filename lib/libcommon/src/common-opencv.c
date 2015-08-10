/*
 * norama-suite libcommon - norama-suite common library
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

    # include "common-opencv.h"

/*
    Source - Image exportation methods
 */

    int lc_imwrite_jpeg( char * lcPath, IplImage * lcImage, int lcQuality ) {

        /* Option array variables */
        int lcOptions[2] = { CV_IMWRITE_JPEG_QUALITY, ( lcQuality > 100 ) ? 100: ( ( lcQuality < 0 ) ? 0 : lcQuality ) };

        /* Image exportation */
        return( cvSaveImage(  lcPath, lcImage, lcOptions ) );

    }

    int lc_imwrite_png( char * lcPath, IplImage * lcImage, int lcCompression ) {

        /* Option array variables */
        int lcOptions[2] = { CV_IMWRITE_PNG_COMPRESSION, ( lcCompression > 9 ) ? 9 : ( ( lcCompression < 0 ) ? 0 : lcCompression ) };

        /* Image exportation */
        return( cvSaveImage(  lcPath, lcImage, lcOptions ) );

    }
