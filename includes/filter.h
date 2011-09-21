/*
 * filter.h
 *
 *  Created on: 14/09/2011
 *      Author: Daniel
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "types.h"
#include "bmp.h"

void applyFilter(IMAGE *image);
void applySharpenFilter(IMAGE *image, int selection, int debug);
void applyFilterSimple(IMAGE *image, int selection, int debug);
void setTranslationArray(int translationArray[3][3], int index);
BYTE pixelWeight(int poss, BYTE mirrorArray[514][514], BYTE filter[3][3]);

#endif /* FILTER_H_ */
