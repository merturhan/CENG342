// Mert Urhan
// Hocam maalesef ki ödevi tamamlayamadım, sürekli olarak "signal 11 (Segmentation fault)" hatası alıyorum

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <hellomake.h>
#include <mpi.h>

int main(int argc, char *argv[]){
	//myFunc();

	if(argc != 4){
		printf("Error! Give four arguments\n");
		printf("schoolNum schoolNum fileName");
		return 1;
	}

	int largeSize = atoi(argv[1]) % 10000;
	int smallSize = (atoi(argv[2]) % 100) + 100;
	
	int x= atoi(argv[1]);
	int y= atoi(argv[2]);
	char *fileName = argv[3];

	long int seed = 19050111067;
	double *vector;
	double *vectorr;
	double **largeMatrix;
	double **smallMatrix;

	double start, end;
	int i, j;

	int num_procs, rank;

	//Large Matrix
	largeMatrix = (double **)malloc(largeSize * sizeof(double *));
	for(i=0; i<largeSize; i++){
		largeMatrix[i] = (double *)malloc(largeSize * sizeof(double));
	}
	srand(seed);
	for(i=0; i<largeSize; i++){
		for(j=0; j<largeSize; j++){
			largeMatrix[i][j] = (rand()) % 100 + 1.0;
		}
	}
	printf("%dx%d matrix defined and filled with random numbers", largeSize, largeSize);
	vector = (double *)malloc(largeSize * sizeof(double *));
	for(i=0; i<largeSize; i++){
		vector[i] = (rand()) % 100 + 1.0;
	}
	printf("Random vector created for matrix multiplication\n");



	//Small Matrix
	smallMatrix = (double **)malloc(smallSize * sizeof(double *));
	for(i=0; i<smallSize; i++){
		smallMatrix[i] = (double *)malloc(smallSize * sizeof(double));
	}
	srand(seed);
	for(i=0; i<smallSize; i++){
		for(j=0; j<smallSize; j++){
			smallMatrix[i][j] = (rand()) % 100 + 1.0;
		}
	}
	printf("%dx%d matrix defined and filled with random numbers", smallSize, smallSize);
	vectorr = (double *)malloc(smallSize * sizeof(double *));
	for(i=0; i<smallSize; i++){
		vector[i] = (rand()) % 100 + 1.0;
	}
	printf("Random vector created for matrix multiplication\n");
	



	FILE *fp = fopen(fileName, "w");

	if(fp == NULL){
		printf("%s couldn't find", fileName);
	}

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *result, *partialResult;
	result = (double*)malloc(largeSize * sizeof(double));
    partialResult = (double*)malloc(largeSize * sizeof(double));

 	// Broadcast the vector to all processes
	MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

 	// Compute the partial result for each process
	MPI_Bcast(vector, largeSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	for (i = rank; i < largeSize; i += num_procs) {
        partialResult[i] = 0.0;
        for (j = 0; j < largeSize; j++) {
            partialResult[i] += *largeMatrix[i * largeSize + j] * vector[j];
        }
    }

	// Sum the partial results from all processes
    MPI_Reduce(partialResult, result, largeSize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Stop the timer
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

	// Print the result and the time taken
    if (rank == 0) {
        printf("Result:\n");
        for (i = 0; i < largeSize; i++) {
            fprintf(fp, "%.lf\n", result[i]);
        }
        printf("%d Core Used, Time taken: %f seconds\n", num_procs, end - start);
    }
    free(largeMatrix);
    free(vector);
    free(result);
    free(partialResult);
	MPI_Finalize();



    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *resultt, *partialResultt;
	resultt = (double*)malloc(smallSize * sizeof(double));
    partialResultt = (double*)malloc(smallSize * sizeof(double));
    
    // Broadcast the vector to all processes
	MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

 	// Compute the partial result for each process
	MPI_Bcast(vectorr, smallSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	for (i = rank; i < smallSize; i += num_procs) {
        partialResultt[i] = 0.0;
        for (j = 0; j < smallSize; j++) {
            partialResultt[i] += *smallMatrix[i * smallSize + j] * vectorr[j];
        }
    }

	// Sum the partial results from all processes
    MPI_Reduce(partialResultt, resultt, smallSize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Stop the timer
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

	// Print the result and the time taken
    if (rank == 0) {
        printf("Result:\n");
        for (i = 0; i < smallSize; i++) {
            fprintf(fp, "%.lf\n", resultt[i]);
        }
        printf("%d Core Used, Time taken: %f seconds\n", num_procs, end - start);
    }
    free(smallMatrix);
    free(vectorr);
    free(resultt);
    free(partialResultt);
	MPI_Finalize();


	/*
	double result = 0;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			result = matrix[i][j] *  vector[j];
		}
		fprintf(fp, "%.lf\n", result);
		result = 0;
	}
	*/

	printf("\nMultiplied and wrote to %s txt\n\n", fileName);
	fclose(fp);


	return 0;

}