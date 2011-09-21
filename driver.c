/*
 * driver.c
 *
 *  Created on: 07/09/2011
 *      Author: Daniel Schougaard
 */
#include <stdio.h>

#include "includes/bmp.h"
#include "includes/filter.h"

int main(){
	int i;
	int MAX = 1;

	IMAGE img;


	bmp_open("example24.bmp" ,&img);
	for( i = 0; i < MAX; i ++){
		applyFilterSimple(&img, 7, 10);
	}
	/*for( i = 0; i < 3*MAX; i++){
		applyFilterSimple(&img, 1,2);
	}*/


	printf("Filter Applied\n");
	bmp_save("filter_example24.bmp", &img);
	printf("Process completed!\n");


	return 0;
}


