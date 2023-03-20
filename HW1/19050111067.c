#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <hellomake.h>

int main(int argc, char *argv[]){
	//myFunc();

	if(argc != 4){
		printf("Error! Give four arguments\n");
		printf("arraysize1 arraysize2 fileName");
		return 1;
	}
	
	int x= atoi(argv[1]);
	int y= atoi(argv[2]);
	char *fileName = argv[3];

	long int seed = 19050111067;
	double *vector;
	double **matrix;

	int i, j;

	matrix = (double **)malloc(x * sizeof(double *));

	for(i=0; i<x; i++){
		matrix[i] = (double *)malloc(y * sizeof(double));
	}

	srand(seed);

	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			matrix[i][j] = (rand()) % 100 + 1.0;
		}
	}
	printf("%dx%d matrix defined and filled with random numbers", x, y);


	vector = (double *)malloc(y * sizeof(double *));
	for(i=0; i<y; i++){
		vector[i] = (rand()) % 100 + 1.0;
	}
	printf("Random vector created for matrix multiplication\n");


	FILE *fp = fopen(fileName, "w");

	if(fp == NULL){
		printf("%s couldn't find", fileName);
	}

	double result = 0;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			result = matrix[i][j] *  vector[j];
		}
		fprintf(fp, "%.lf\n", result);
		result = 0;
	}

	printf("\nMultiplied and wrote to %s txt\n\n", fileName);
	fclose(fp);


	return 0;

}
