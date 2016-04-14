#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct DataPoint {
    int numFeatures;
    double* features;
    char* classification;
} dp;

void readDatapoints(char* filename, dp* datapoints,int numDatapoints, int numFeatures) {
    //struct DataPoint *datapoints;
    //datapoints = (dp*) malloc(sizeof(dp) * numDatapoints);
}



int main(){
    FILE *file;
    char * line = NULL;
    int lineNumber = 0;
    int numDatapoints = 150;
    int numFeatures = 4;
    size_t len = 0;
    ssize_t read;

    struct DataPoint *datapoints;
    datapoints = (dp*) malloc(sizeof(dp) * numDatapoints);

    file = fopen("../Data/iris.data", "r");
    if(file){
        while((read = getline(&line,&len,file)) != -1){

            //Create a new Struct representing a datapoint and put it into the array.
            struct DataPoint dp1;
            datapoints[lineNumber] = dp1;

            int i = 0;
            char *tokens = strtok(line,",");
            char *array[numFeatures];
            double features[numFeatures - 1];

            while(tokens != NULL){
                array[i++] = tokens;
                tokens = strtok(NULL,",");
            }

            for(i=0;i<numFeatures;i++){
                features[i] = atof(array[i]);
            }
            
            datapoints[lineNumber].features = features;
            datapoints[lineNumber].classification = strtok(array[numFeatures],"\n");
            lineNumber++;
        }
        fclose(file);
    } else {
        printf("File doesn't exist.\n");
    }
    
    int i;
    for(i = 0;i<numDatapoints;i++){
       printf("%s\n", datapoints[i].classification);
    }
}