//
// Lagrange Polynomial Interpolation
// icallend (github)
// icallend@cs.brown.edu
// 3/15/2011
// main.c
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#define	 BINARYLENGTH	8

void constantConversion(int terms[], int termSize, int *coToReturn);
void toBinary(int val, int *binToReturn);
int sumIntArr(int arr[], int arrSize);

int DEBUG = 0;

typedef struct pt {
	int x;
	int y;
} pt;

main(int argc, char *argv[]) {
	
	int xterm = 0, yterm = 0, ptsSize = 0, c;
	int i, h;
	
	while(--argc > 0 && (*++argv)[0]=='-'){
		while(c = *++argv[0]){
			switch(c){
				case 'g':
					DEBUG = 1;
					break;
				default:
					printf("Illegal option \'%c\' in runtime arguments\n",c);
					abort();
					break;
			}
		}
	}
	
	printf("\n\n**Lagrange Polynomial Interpolation**\n");
	system("date");
	printf( (DEBUG>0) ? "DEBUG MODE\n\n" : "\n" );
	
	printf("\nPlease input the number of points to be entered:\n");
	scanf("%d",&ptsSize);
	ptsSize = ptsSize;
	pt pts[ptsSize];
	
	printf("\nPlease input a set of %d points,\nwith x and y values delineated by a comma\nand each set delineated by a carriage return:\n", ptsSize);
	for(i=0;i<ptsSize;i++){
		scanf("%d,%d",&xterm,&yterm);
		pts[i].x = xterm;
		pts[i].y = yterm;
	}

	if(DEBUG)printf("\nptssize %d",ptsSize);
	int arrayToPass[ptsSize-1], coefArray[ptsSize];
	int subtract;
	if(DEBUG)printf("\nsizeof arrayToPass %d",sizeof(arrayToPass));
	if(DEBUG)printf("\nsizeof coefarray %d",sizeof(coefArray));
	float a = 1, finalC[ptsSize], eqc[ptsSize][ptsSize]; // array of equation constants[# of equations][# constants]


	for(i = 0; i<ptsSize; i++){
		coefArray[i] = pts[i].x;
		if(DEBUG)printf("\nx%d: %d",i,coefArray[i]);
		finalC[i] = 0.0;
	}
	
	for(i = 0; i<ptsSize; i++){
		subtract = 0;
		eqc[i][0] = 1.0;
		
		for(h = 1; h<ptsSize;h++){
			eqc[i][h] = 0.0;
		}
		
		for(h = 0; h<ptsSize; h++){
			if(h == i){
				subtract = 1;
			}else{
				eqc[i][0] *= (pts[i].x - pts[h].x);
				switch(subtract){
					case 0:
						arrayToPass[h] = pts[h].x;
						break;
					case 1:
						arrayToPass[h-1] = pts[h].x;
						break;
				}
			}
		}
		
		if(DEBUG){
			for(h=0;h<ptsSize-1;h++){
				printf("\narrayToPass[%d]: %d",h,arrayToPass[h]);
			}
		}
		
		int *currentCoes=(int *)malloc(sizeof(int[ptsSize]));
		constantConversion(arrayToPass, ptsSize-1, currentCoes);
		
		for(h=1;h<ptsSize;h++){
			eqc[i][h] = *(currentCoes+h);
		}
		
		free(currentCoes);
		
		a = eqc[i][0];
		eqc[i][0] = (pts[i].y)/a;
		
		for(h = 1; h<ptsSize; h++){
			eqc[i][h] *= eqc[i][0];
		}
		
		if(DEBUG){
		for(h = 0; h<ptsSize; h++){
			printf("%d-%d: %f\n",i,h,eqc[i][h]);
		}
		}
	}
	
	if(DEBUG)printf("\nout of main for loop\n");
	
	for(i=0;i<ptsSize;i++){
		for(h=0; h<ptsSize; h++){
			if(DEBUG)if(i==2)printf("\n\ncurrent eqc value: %f",eqc[h][i]);
			if(DEBUG)printf("\ntransfering eqc[%d][%d] to finalC[%d]",h,i,i);
			finalC[i] += eqc[h][i];
		}
	}
	
	if(DEBUG)printf("\nout of transfer for loop\n");

	printf("\n\nConstants:\n");
	for(i = 0; i < ptsSize; i++){
		if( (i%2 == 1) && (finalC[i]!=0) )finalC[i] *= -1;
		putchar(65+i);
		printf(": %f\n", finalC[i]);
	}
	
	printf("\n");

	return 0;
}


void constantConversion(int terms[], int termSize, int *coToReturn){
	int i=0, j=0;
	int cArray[termSize+1];
	
	for(i=0; i<termSize+1;i++){
		cArray[i] = 0;
	}
	
	if(DEBUG)printf("\nsizeof(terms): %d",sizeof(terms));
	if(DEBUG)printf("\ntermsize: %d",termSize);
	if(DEBUG)printf("\ntwo to the termsize: %d",(int)pow(2,termSize));
	
	for(i=0;i<((int)pow(2,termSize));i++){
	  if(DEBUG)printf("\n\nforLoopCalled");
	  int placesArr[termSize], tempProd = 1;
	  if(DEBUG)printf("\ni: %d",i);

	  if(DEBUG)printf("\npreMalloc");
	  
	  int *places=(int *)malloc(10*sizeof(int[BINARYLENGTH]));
	  
	  if(DEBUG)printf("\npostMalloc");
	  
	  toBinary(i, places);
	  
	  if(DEBUG)printf("\npostToBinary");
	  
	  for(j=termSize;j>0;j--){
	  	placesArr[j-1] = *(places+BINARYLENGTH-j);
	  	if(DEBUG)printf("\nplacesArr[%d]: %d",j-1,placesArr[j-1]);
	  }
	  
	  if(DEBUG)printf("\nout of foor loop");
	  
	  free(places);
	  
	  if(DEBUG)printf("\npostFree(places)\n");
	  
	  for(j=0;j<termSize;j++){
		if(placesArr[j]!=0){
			if(DEBUG)printf("|%d-%d",j,terms[j]);
			tempProd *= terms[j];
			if(DEBUG)printf("/%d",tempProd);
		}
	  }
	  int sumIntArrPlacesArr = sumIntArr(placesArr, termSize);
	  if(DEBUG)printf("\nsumIntArrPlacesArr val: %d",sumIntArrPlacesArr);
	  cArray[sumIntArrPlacesArr] += tempProd;
	  if(DEBUG)printf("\ncArray: %d",cArray[sumIntArrPlacesArr]);
	}
	
	if(DEBUG)printf("\n\n");
	
	for(i=0;i<termSize+1;i++){
		if(DEBUG){
			printf("coeficciantMethod ");
			putchar(65+i);
			printf(": %d\n",cArray[i]);
		}
		*(coToReturn+i) = cArray[i];
	}
	
	if(DEBUG)printf("\nOUT OF COEFICCIANT METHOD!!!\n\n");
	
}

void toBinary(int val, int *binToReturn){ 
	if(DEBUG)printf("\nbinary called");
    int count = 0, x = 0, y = 0;
	if(DEBUG)printf("\npost binary malloc");
	for(y = 7; y >= 0; y--) {
		if(DEBUG){
			printf("\nin binary forloop (iteration ");
			(y>0) ? printf("-%d)",y) : printf(" %d)",y);
		}
		
		x = val / (1 << y);
		val = val - x * (1 << y);
		*(binToReturn+count) = x;
		count++;
	}
	if(DEBUG)printf("\nout of binary forloop");
}

int sumIntArr(int arr[], int arrSize){
	if(DEBUG)printf("\nsumIntArr called");
	int temp = 0, i = 0;
	if(DEBUG)printf("\nsizeof arr: %d",arrSize);
	for(i=0;i<arrSize;i++){
		if(DEBUG)printf("\nin sumintarr forloop (iteration %d)",i);
	    temp += arr[i];
	}
	if(DEBUG)printf("\nout of sumintarr forloop");
	if(DEBUG)printf("\ntemp: %d",temp);
	return temp;
}

