#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "includes/filter.h"
#include "includes/types.h"
#include "includes/bmp.h"

/*
 * filter.c
 *
 *  Created on: 14/09/2011
 *      Author: Daniel
 */

void setTranslationArray(int translationArray[3][3], int index){
	translationArray[0][0] = index- (512 + 1);
	translationArray[0][1] = index - 512;
	translationArray[0][2] = index - (512 - 1);

	translationArray[1][0] = index - 1;
	translationArray[1][1] = index;
	translationArray[1][2] = index + 1;

	translationArray[2][0] = index + (512 - 1);
	translationArray[2][1] = index + 512;
	translationArray[2][2] = index + (512 + 1);
}


void applyFilterSimple(IMAGE *image, int selection, int debug){
	int i, j, h, x, debugCount, min, max;
	min = 0; max = 0;
	int correctedIValue;
	BYTE resultingValue;
	int filterSum, appliedFilterSum, testVal;

	float floatVal;

	BYTE tempArray[512*512];

	min = 0; max = 0;

	int filterOne[3][3] = { {0, 1, 0}, {1, 1, 1}, {0, 1, 0} };
	int filterTwo[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	int filterThree[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };
	int filterFour[3][3] = { {2, 0, 0}, {0, -1, 0}, {0, 0, -1} };
	int filterFive[3][3] = { {-1, -1, -1}, {-1, 4, -1}, {-1, -1, -1} };
	int filterSix[3][3] = { {-1, -1, 0}, {-1, 0, 1}, {0, 1, 1} };
	int filterSeven[3][3] = { {-1, -1, -1}, {-1, 4, -1}, {-1, -1, -1} };





	//BYTE chosenFilter[3][3];

	int chosenFilter[3][3];



	switch (selection) {
	case 1:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterOne[i][j];
			}
		}
		break;
	case 2:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterTwo[i][j];
			}
		}
		break;
	case 3:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterThree[i][j];
			}
		}
		break;
	case 4:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterFour[i][j];
			}
		}
		break;
	case 5:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterFive[i][j];
			}
		}
		break;
	case 6:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterSix[i][j];
			}
		}
		break;
	case 7:
		for(i = 0; i < 3; i++){
			for (j = 0; j < 3; j++) {
				chosenFilter[i][j] = filterSeven[i][j];
			}
		}
		break;
	default:
		break;
	}


	for(i = 0; i < 512*512; i++){
		tempArray[i] = image->Pixels[i];
	}


	for( i = 0; i < 512*512; i++){

		//int translationArray[3][3] = {{i- (512 + 1), i - 512,  i - (512 - 1)},{i-1, i, i+1},{i + (512 - 1), i + 512, i +(512+1)}};
		int translationArray[3][3];

		correctedIValue = i;


		// bottom
		if( i == 0 ){
			correctedIValue += (image->Width+1);
			// top
		}else if( i == image->Width-1 ){
			correctedIValue += image->Width-1;
			// top
		}else if( i == image->Width*image->Height-1){
			correctedIValue -= image->Width-1;
			// top
		}else if( i == image->Height*(image->Width-1)){
			correctedIValue -= image->Width+1;
			// top
		}else if( i < (image->Width-1)){
			//}else if(i < 512-1){
			correctedIValue += image->Width;
			// top
		}else if(i >= image->Height*image->Width-image->Width){

			correctedIValue -= image->Width;
			// left
		}else if(i%image->Width == 0){
			correctedIValue++;
			//right
		}else if((i+1)%image->Width == 0){
			correctedIValue = correctedIValue - 1;
		}

		setTranslationArray(translationArray, correctedIValue);

		filterSum = 0; appliedFilterSum = 0;

		for( j = 0; j < 3; j++){
			for( h = 0; h < 3; h++){
				appliedFilterSum = appliedFilterSum + ((int)tempArray[translationArray[j][h]])*chosenFilter[j][h];
				//printf("[%d; %d]: %d\n", j, h, appliedFilterSum);
				filterSum = filterSum + (int)chosenFilter[j][h];

			}

		}


		/*if(appliedFilterSum/filterSum < 0){
			resultingValue = 255+(appliedFilterSum/filterSum);
		}else{

		}*/


		/*if(filterSum == 0){
			filterSum = 1;
		}*/

		if( appliedFilterSum < 0 && filterSum > 0){
			appliedFilterSum *= -1;
		}else if( appliedFilterSum > 0 && filterSum < 0){
			filterSum *= -1;
		}


		testVal = appliedFilterSum/filterSum;

		if(testVal < min){
			min = testVal;
		}
		if(testVal > max){
			max = testVal;
		}






		if(testVal <= 0){
			resultingValue = (BYTE)0;
		}else if(testVal >= 255){
			resultingValue = (BYTE)255;
		}else{
			resultingValue = (BYTE)testVal;
		}

		/*if((appliedFilterSum/filterSum) <= 0){
			testVal = 0;
			resultingValue = (BYTE)testVal;
		}else if((appliedFilterSum/filterSum) >= 255){
			testVal = 255;
			resultingValue = (BYTE)testVal;
		}else{
			testVal = (appliedFilterSum/filterSum);

			resultingValue = (BYTE)testVal;

		}*/


		/*if(testVal < 0){
			printf("Applied: %d\nFilter: %d\ntessResult: %d\nResult: %d \n\n", appliedFilterSum, filterSum,testVal, resultingValue);
		}*/


		image->Pixels[i] = resultingValue;








	}

	printf("Min: %d\nMax: %d\n", min, max);




}


void applySharpenFilter(IMAGE *image, int selection, int debug){
	int i, j, h, x, debugCount, min, max;
	float factor;
	min = 0; max = 0;
	int correctedIValue;
	BYTE resultingValue;
	int filterSum, appliedFilterSum, testVal;

	BYTE tempArray[512*512];
	float calcArray[512*512];

	int filterOne[3][3]  = { {-1, -1, -1}, {-1, 4, -1}, {-1, -1, -1} };
	int chosenFilter[3][3];
	for(i = 0; i < 3; i++){ for (j = 0; j < 3; j++) { chosenFilter[i][j] = filterOne[i][j]; } }
	for(i = 0; i < 512*512; i++){ tempArray[i] = image->Pixels[i]; }
	for( i = 0; i < 512*512; i++){
		int translationArray[3][3];
		correctedIValue = i;
		// bottom
		if( i == 0 ){
			correctedIValue += (image->Width+1);
			// top
		}else if( i == image->Width-1 ){
			correctedIValue += image->Width-1;
			// top
		}else if( i == image->Width*image->Height-1){
			correctedIValue -= image->Width-1;
			// top
		}else if( i == image->Height*(image->Width-1)){
			correctedIValue -= image->Width+1;
			// top
		}else if( i < (image->Width-1)){
			//}else if(i < 512-1){
			correctedIValue += image->Width;
			// top
		}else if(i >= image->Height*image->Width-image->Width){

			correctedIValue -= image->Width;
			// left
		}else if(i%image->Width == 0){
			correctedIValue++;
			//right
		}else if((i+1)%image->Width == 0){
			correctedIValue = correctedIValue - 1;
		}
		setTranslationArray(translationArray, correctedIValue);
		filterSum = 0; appliedFilterSum = 0;

		for( j = 0; j < 3; j++){
			for( h = 0; h < 3; h++){
				appliedFilterSum = appliedFilterSum + ((int)image->Pixels[translationArray[j][h]])*chosenFilter[j][h];
				//printf("[%d; %d]: %d\n", j, h, appliedFilterSum);
				filterSum = filterSum + (int)chosenFilter[j][h];
			}
		}

		if( appliedFilterSum < 0 && filterSum > 0){
			appliedFilterSum *= -1;
		}else if( appliedFilterSum > 0 && filterSum < 0){
			filterSum *= -1;
		}


		testVal = appliedFilterSum/filterSum;

		if(testVal < min){
			min = testVal;
		}
		if(testVal > max){
			max = testVal;
		}


		/*if(testVal <= 0){
			resultingValue = (BYTE)0;
		}else if(testVal >= 255){
			resultingValue = (BYTE)255;
		}else{
			resultingValue = (BYTE)testVal;
		}*/


		calcArray[i] = (float)testVal;
	}

	/*for( i = 0; i < 512*512; i++){
		calcArray[i] += min;
	}
	printf("Min: %d\nMax: %d\n", min, max);
	max += abs(min);
	factor = ((float)255/(float)max);
	printf("Factor: %f\n", factor);

	/*for( i = 0; i < 512*512; i++){
		calcArray[i] = calcArray[i]*factor;
		//printf("%d \n", calcArray[i]);
		image->Pixels[i] = (BYTE)calcArray[i];
	}*/


	for( i = 0; i < 512*512; i++ ){
		if(calcArray[i] <= 0){
			image->Pixels[i] = (BYTE)0;
		}else if(calcArray[i] >= 255){
			image->Pixels[i] = (BYTE)255;
		}else{
			image->Pixels[i] = (BYTE)testVal;
		}
	}





}





void applyFilter(IMAGE *image){
	// incremention variables
	int i,j;
	BYTE filterOne[3][3] = { {0, 0, 0}, {0, 1, 0}, {0, 0, 0} };





	// copy to the mirrorArray
	BYTE mirrorArray[(512+2)][(512+2)];
	for(i = 1; i < 512+1; i++){
		for(j = 1; j < 512+1; j++ ){
			mirrorArray[i][j] = image->Pixels[i+512*j];
		}
	}


	for(i = 1; i <= 512; i++){
		mirrorArray[i][0] = mirrorArray[i+1][0];
		mirrorArray[0][i] = mirrorArray[0][i+1];
		mirrorArray[513][i] = mirrorArray[513][i-1];
		mirrorArray[i][513] = mirrorArray[i-1][513];
	}
	mirrorArray[0][0] = mirrorArray[0+1][0+1];
	mirrorArray[0][513] = mirrorArray[1][513-1];
	mirrorArray[513][0] = mirrorArray[513-1][0];
	mirrorArray[513][513] = mirrorArray[513-1][513-1];



	for(i = 0; i < 512*512; i++){
		image->Pixels[i] = pixelWeight(i, mirrorArray, filterOne);
		/*todo check */

		/*if((i % 100) == 0){
			printf("%d :: %d\n", i, image->Pixels[i]);
		}*/
	}






}

BYTE pixelWeight(int poss, BYTE mirrorArray[514][514], BYTE filter[3][3]){
	int i,j;

	int x,y;

	float val, filterSum;

	BYTE carrySum;
	x = poss%512;
	y = poss-x*512;

	//printf("%d, %d\n", x, y);



	filterSum = 0;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			filterSum += filter[i][j];
		}
	}
	filterSum = 1/filterSum;

	for(i = -1; i < 2; i++){
		for(j = -1; j < 2; j++){
			carrySum = carrySum + mirrorArray[x+i][y+j]*filter[i+1][j+1];
		}
	}

	val = carrySum*filterSum;
	//printf("%d\n", val);

	return (BYTE) val;

}
