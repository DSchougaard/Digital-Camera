
/********************************************************************************
 * CCD Header
 ********************************************************************************/

#ifndef __CCD_H
#define __CCD_H

#include "types.h"

/* get the width of the next image to show */
WORD ccd_get_width();

/* get the width of the next image to show */
WORD ccd_get_height();

/* capture an image */
void ccd_capture_image();

/* reset internal pixel pointer */
void ccd_reset_pointer();

/* get one pixel of the captured image */
BYTE ccd_get_pixel();

/* get four pixels of the captured image */
WORD ccd_get_pixels();

#endif /* __CCD_H */
/*
 * ccd.h
 *
 *  Created on: 14/09/2011
 *      Author: Daniel
 */

#ifndef CCD_H_
#define CCD_H_



#endif /* CCD_H_ */
