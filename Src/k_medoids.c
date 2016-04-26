#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>


#ifdef IRIS
    #define nf 4
    #define nd 150
    #define k 3
    #define filename "../Data/iris.data"
    #define desc "IRIS"
#endif

#ifdef CARS
    #define nf 18
    #define nd 846
    #define k 4
    #define filename "../Data/cars.data"
    #define desc "CARS"
#endif

#ifdef SOYBEAN
    #define nf 35
    #define nd 47
    #define k 4
    #define filename "../Data/soybean-small.data"
    #define desc "SOYBEAN"
#endif

//Data Structure for a datapoint.
//It has an array of features as well as a string representing its classification type
struct DataPoint {
    double features[nf];
    char classification[20];
};

//Data Structure for a cluster
//It has a pointer to its medoid as well as an array of pointers to each datapoint in the cluster
struct Cluster {
    int numDatapoints;
    struct DataPoint *medoid;
    struct DataPoint *datapoints[nd];
};

struct DataPoint datapoints[nd];
struct Cluster clusters[k];
double **matrix;
double *v_scores;
double *rowsums;

void readDatapoints() {
    char *array[nf + 1];
    double features[nf];
    char *tokens;

    FILE *file;
    char * line = NULL;
    int lineNumber = 0;
    int i = 0;
    size_t len = 0;
    ssize_t read;
    
    file = fopen(filename, "r");
    if(file){
        while((read = getline(&line,&len,file)) != -1){
            tokens = strtok(line,",");
            
            while(tokens != NULL){
                array[i++] = tokens;
                tokens = strtok(NULL,",");
            }

            for(i=0;i<nf;i++){
                features[i] = atof(array[i]);
            }
            
            memcpy(datapoints[lineNumber].features,features,sizeof(double) * nf);
            strcpy(datapoints[lineNumber].classification,strtok(array[nf],"\n"));
            lineNumber++;
            i = 0;
        }
        fclose(file);
    } else {
        printf("File doesn't exist.\n");
    }
}

double distance(struct DataPoint dp1, struct DataPoint dp2) {
    int i;
    double sum = 0.0;
    for(i = 0; i < nf; i++){
        sum += pow(dp1.features[i] - dp2.features[i],2.0);
    }
    return sqrt(sum);
}

int indexOfLargestElement(double* values, int length){
    int i, largeIndex;
    largeIndex = 0;
    double largeValue = values[0];
    for (i = 1;i < length;i++){
        if(values[i] > largeValue){
            largeIndex = i;
            largeValue = values[i];
        }
    }
    return largeIndex;
}

int indexOfSmallestElement(double* values, int length){
    int i, smallIndex;
    smallIndex = 0;
    double smallValue = values[0];
    for (i = 1;i < length;i++){
        if(values[i] < smallValue){
            smallIndex = i;
            smallValue = values[i];
        }
    }
    return smallIndex;
}


void getLowestElements(double* v_scores, int* buffer){
    int i,largeIndex;
    double largeValue;
    double* buffer_values = malloc(k * sizeof(double));
    for(i=0;i<k;i++){
        buffer[i] = i;
        buffer_values[i] = v_scores[i];
    }
    for(i=k;i<nd;i++){
        largeIndex = indexOfLargestElement(buffer_values,k);
        largeValue = buffer_values[largeIndex];
        if(v_scores[i] < largeValue){
            buffer[largeIndex] = i;
            buffer_values[largeIndex] = v_scores[i];
        }
    }

    free(buffer_values);
}


void selectInitialMedoids() {
    int i, j;
    v_scores = malloc(nd * sizeof(double));
    rowsums = malloc(nd * sizeof(double));
    for(i=0;i<nd;i++){
        rowsums[i] = 0.0;
        for(j=0;j<nd;j++){
            rowsums[i] += matrix[i][j];
        }
    }

    for(j=0;j<nd;j++){
        v_scores[j] = 0.0;
        for(i=0;i<nd;i++){
            v_scores[j] += (matrix[i][j] / rowsums[i]);
        }
    }

    int* lowestElements = malloc(k * sizeof(int));
    getLowestElements(v_scores,lowestElements);
    for(i = 0; i < k; i++){
        clusters[i].medoid = &datapoints[lowestElements[i]];
    }
    free(lowestElements);
    free(v_scores);
    free(rowsums);
}

void assignDataPointsToClusters(){
    int i,c;
    double* distances = malloc(k * sizeof(double));
    for(c=0;c<k;c++){
        clusters[c].numDatapoints = 0;
    }

    for(i = 0;i < nd; i++) {
        for(c = 0; c < k; c++){
            distances[c] = distance(datapoints[i], *clusters[c].medoid);
        }
        int close_index = indexOfSmallestElement(distances,k);
        clusters[close_index].datapoints[clusters[close_index].numDatapoints] = &datapoints[i];
        clusters[close_index].numDatapoints++;
    }
    free(distances);
}

//Update medoids by finding the datapoint that minimizes 
//the sum of pairwise distances from itself to all other datapoints in the cluster
void updateMedoids() {
    int i,j,c;
    for(c = 0; c < k;c++){
        int clusterSize = clusters[c].numDatapoints;
        double* distances = malloc(clusterSize * sizeof(double));
        #ifdef OMP
            #pragma omp parallel for private(j)
        #endif
        #ifdef ACC
            #pragma acc parallel for
        #endif
        for(i = 0; i < clusterSize; i++){
            distances[i] = 0.0;
            for(j = 0;j < clusterSize;j++){
                distances[i] += distance(*clusters[c].datapoints[i],*clusters[c].datapoints[j]);
            }
        }
        int medoidIndex = indexOfSmallestElement(distances,clusterSize);
        clusters[c].medoid = clusters[c].datapoints[medoidIndex];
        free(distances);
    }
}

double calculateTotalCost() {
    int i,c;
    double totalCost;
    #ifdef OMP
        #pragma omp parallel for private(i)
    #endif
    #ifdef ACC
        #pragma acc parallel for
    #endif
    for(c = 0;c < k;c++){
        for(i = 0; i < clusters[c].numDatapoints;i++){
            totalCost += distance(datapoints[i],*clusters[c].medoid);
        }
    }
    return totalCost;
}

void cluster() {
    int i,j;
    double c;
    double cost = INFINITY;
    matrix = malloc(nd*sizeof(double *));
    #ifdef OMP
        #pragma omp parallel for private(j)
    #endif
    #ifdef ACC
        #pragma acc parallel for
    #endif
    for(i=0;i<nd;i++){
        matrix[i] = malloc(nd*sizeof(double));
        for(j = 0; j < nd; j++){
            matrix[i][j] = distance(datapoints[i],datapoints[j]);
        } 
    }

    selectInitialMedoids();
    assignDataPointsToClusters();
    int safetyIterations = 0;
    while(cost - (c = calculateTotalCost()) > 1E-5){
        cost = c;
        updateMedoids();
        assignDataPointsToClusters();
        if(safetyIterations > 100){
            printf("Max iterations exceeded\n");
            break;
        }
        safetyIterations++;
    }

    for(i = 0; i < nd; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printDatapoints() {
    int i,j;
    for(i = 0; i < nd; i++){
        for(j = 0; j < nf; j++){
            printf("%f\t", datapoints[i].features[j]);
        }
        printf("%s\n", datapoints[i].classification);
    }
}

int main(int argc, char *argv[]){
    readDatapoints();
    int i;
    double start_time,stop_time;
    printf("%s\t",desc);
    for(i = 0;i < 10;i++){
        start_time = omp_get_wtime();
        cluster();
        stop_time = omp_get_wtime();
        printf("%f\t",stop_time - start_time);
    }
    printf("\n");
}