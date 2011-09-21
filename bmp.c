#include <stdio.h>
#include <stdlib.h>

#include "includes/bmp.h"

/* see lecture notes for more information on pragma pack directive */
#pragma pack(push, 1)

/*
 *   BITMAP FILE: See http://en.wikipedia.org/wiki/BMP_file_format
 *   ___________________________________________________________
 *  |          |          |            |                        |
 *  |   file   |   info   |  Palette   |       Pixel data       |     
 *  |  header  |  header  | (optional) |                        |
 *  |__________|__________|____________|________________________|
 *  start of file 									  end of file
 * 
 *  - Lines must be word-aligned!
 *  
 * 
 */

/**********************************************************************
 * Bitmap File Header
 **********************************************************************/
typedef struct {
	HALFWORD 	BfType;  		    /* Must be 0x4D42 				  */
	WORD	 	BfSize;				/* Size of the file in bytes 	  */
	HALFWORD	BfReserved1;		/* Should be 0 					  */
	HALFWORD	BfReserved2;		/* Should be 0 					  */
	WORD		BfOffBits;			/* Offset of image data in file   */
} BITMAPFILEHEADER;

/**********************************************************************
 * Bitmap Information Header
 **********************************************************************/
typedef struct {	
	WORD 		BiSize;				/* Size of this structure 		  */
	WORD		BiWidth;			/* Width of the image in bytes    */
	WORD		BiHeight;			/* Height of the image in bytes   */
	HALFWORD	BiPlanes;			/* Should be 1 					  */
	HALFWORD	BiBitCount;			/* Bit count (..) 				  */
	WORD		BiCompression;		/* Compression used 			  */
	WORD		BiSizeImage;		/* Size of the image in bytes 	  */
	WORD		BiXPelsPerMeter; 	/* Pixels per meter, X			  */
	WORD		BiYPelsPerMeter;	/* Pixels per meter, Y 			  */
	WORD		BiClrUsed;			/* number of colors used 		  */
	WORD		BiClrImportant;		/* number of important colors 	  */
} BITMAPINFOHEADER;

typedef struct {
	BYTE	red;
	BYTE	green;
	BYTE 	blue;
} PIXEL;

typedef struct {
	BYTE	rgbBlue;
	BYTE 	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
} RBGQUAD;

/* add here other structs you want to pack */

#pragma pack(pop)

BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;

BOOL bmp_open(char* file, IMAGE* image) {
	int i;
	float tempVal;

	/* note: "rb" means open for binary read */
	FILE* fp = fopen(file, "rb");

	if(fp == NULL) {							   
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}
		
	/* todo: process file */

	PIXEL pixel;


	fread(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	fread(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fread(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fread(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fread(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	fread(&bmih.BiSize, sizeof(bmih.BiSize), 1, fp);
	fread(&bmih.BiWidth, sizeof(bmih.BiWidth), 1, fp);
	image->Width = bmih.BiWidth;
	fread(&bmih.BiHeight, sizeof(bmih.BiHeight), 1, fp);
	image->Height = bmih.BiHeight;
	fread(&bmih.BiPlanes, sizeof(bmih.BiPlanes), 1, fp);
	fread(&bmih.BiBitCount, sizeof(bmih.BiBitCount), 1, fp);
	fread(&bmih.BiCompression, sizeof(bmih.BiCompression), 1, fp);
	fread(&bmih.BiSizeImage, sizeof(bmih.BiSizeImage), 1, fp);
	fread(&bmih.BiXPelsPerMeter, sizeof(bmih.BiXPelsPerMeter), 1, fp);
	fread(&bmih.BiYPelsPerMeter, sizeof(bmih.BiYPelsPerMeter), 1, fp);
	fread(&bmih.BiClrUsed, sizeof(bmih.BiClrUsed), 1, fp);
	fread(&bmih.BiClrImportant, sizeof(bmih.BiClrImportant), 1, fp);

	/*fread(&bmfh, sizeof(bmfh), 1, fp);
	fread(&bmih, sizeof(bmih), 1, fp);*/



	/*printf("----- File Header ----- \n");
	printf("BfType: %x \n",bmfh.BfType);
	printf("BfSize: %d\n", bmfh.BfSize);
	printf("BfReserved1: %d\n", bmfh.BfReserved1);
	printf("BfReserved2: %d\n", bmfh.BfReserved2);
	printf("BfOffBits: %d\n", bmfh.BfOffBits);

	for(i = 0; i < 3; i++) { printf("\n"); }

	printf("----- Info Header ----- \n");
	printf("BiSize: %d\n", bmih.BiSize);
	printf("BiWidth: %d\n", bmih.BiWidth);
	printf("BiHeight: %d\n", bmih.BiHeight);
	printf("BiPlanes: %d\n", bmih.BiPlanes);
	printf("BiBitCount: %d\n", bmih.BiBitCount);
	printf("BiCompression: %d\n", bmih.BiCompression);
	printf("BiSizeImage: %d\n", bmih.BiSizeImage);
	printf("BiXPelsPerMeter: %d\n", bmih.BiXPelsPerMeter);
	printf("BiYPelsPerMeter: %d\n", bmih.BiYPelsPerMeter);
	printf("BiClrUsed: %d\n", bmih.BiClrUsed);
	printf("BiClrImportant: %d\n", bmih.BiClrImportant);*/

	//int resolution = bmih.BiWidth*bmih.BiHeight;
	//if(resolution < 0) { resolution = resolution*-1; }
	for(i = 0; i < 512*512; i++){
		fread(&pixel.red, sizeof(pixel.red), 1, fp);
		fread(&pixel.green, sizeof(pixel.green), 1, fp);
		fread(&pixel.blue, sizeof(pixel.blue), 1, fp);
		tempVal = 0.3*pixel.red+0.59*pixel.green+0.11*pixel.blue;
		image->Pixels[i] = (BYTE)tempVal;
	}



	
	/* success */
	fclose(fp);
	return TRUE;
}

BOOL bmp_save(char* file, IMAGE* image) {
	//int i, j;
	int i;

	/* note: "wb" means open for binary write */
	FILE* fp = fopen(file, "wb");

	if(fp == NULL) {
		/* failed to open file, return failure */
		perror("Could not open file");
		return FALSE;
	}


	/* todo: store image to fp */
	fwrite(&bmfh.BfType, sizeof(bmfh.BfType), 1, fp);
	// take into account of new size
	/*int newSize = bmfh.BfSize/4;
	newSize += 256*4;
	bmfh.BfSize = newSize;*/
	fwrite(&bmfh.BfSize, sizeof(bmfh.BfSize), 1, fp);
	fwrite(&bmfh.BfReserved1, sizeof(bmfh.BfReserved1), 1, fp);
	fwrite(&bmfh.BfReserved2, sizeof(bmfh.BfReserved2), 1, fp);
	fwrite(&bmfh.BfOffBits, sizeof(bmfh.BfOffBits), 1, fp);

	fwrite(&bmih.BiSize, sizeof(bmih.BiSize), 1, fp);
	fwrite(&bmih.BiWidth, sizeof(bmih.BiWidth), 1, fp);
	fwrite(&bmih.BiHeight, sizeof(bmih.BiHeight), 1, fp);
	fwrite(&bmih.BiPlanes, sizeof(bmih.BiPlanes), 1, fp);
	bmih.BiBitCount = 8;
	fwrite(&bmih.BiBitCount, sizeof(bmih.BiBitCount), 1, fp);
	fwrite(&bmih.BiCompression, sizeof(bmih.BiCompression), 1, fp);
	fwrite(&bmih.BiSizeImage, sizeof(bmih.BiSizeImage), 1, fp);
	fwrite(&bmih.BiXPelsPerMeter, sizeof(bmih.BiXPelsPerMeter), 1, fp);
	fwrite(&bmih.BiYPelsPerMeter, sizeof(bmih.BiYPelsPerMeter), 1, fp);
	bmih.BiClrUsed = 256;
	fwrite(&bmih.BiClrUsed, sizeof(bmih.BiClrUsed), 1, fp);
	fwrite(&bmih.BiClrImportant, sizeof(bmih.BiClrImportant), 1, fp);
	
	/*RBGQUAD palette[256];
	for(i = 0; i < 256; i++){
		palette[i].rgbGreen = i;
		palette[i].rgbRed = i;
		palette[i].rgbBlue = i;
		palette[i].rgbReserved = 0;
	}

	for(i = 0; i < 256; i++){
		fwrite(&palette[i].rgbGreen, sizeof(palette[i].rgbGreen), 1, fp);
		fwrite(&palette[i].rgbRed, sizeof(palette[i].rgbRed), 1, fp);
		fwrite(&palette[i].rgbBlue, sizeof(palette[i].rgbBlue), 1, fp);
		fwrite(&palette[i].rgbReserved, sizeof(palette[i].rgbReserved), 1, fp);
	}*/

	BYTE RESERVED = 0;
	BYTE col = 0;
	for(i = 0; i < 256; i++){
		col = i;
		BYTE test[3] = { i, i, i };
		fwrite(&test, sizeof(BYTE), 3, fp);
		//fwrite(&col, sizeof(col), 1, fp);
		//fwrite(&col, sizeof(col), 1, fp);
		fwrite(&RESERVED, sizeof(RESERVED), 1, fp);
	}


	int resolution = bmih.BiWidth*bmih.BiHeight;
	if(resolution < 0) { resolution = resolution*-1; }

	/*for(i = 0; i < 512*512; i++){
		fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
		//fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
		//fwrite(&image->Pixels[i], sizeof(image->Pixels[i]), 1, fp);
	}*/

	fwrite(image->Pixels, sizeof(image->Pixels[0]), 512*512, fp);

	fclose(fp);
	return TRUE;
}

