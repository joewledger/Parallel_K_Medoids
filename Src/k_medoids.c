#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define nf 4
#define nd 150
#define k 5

struct DataPoint {
    double features[nf];
    char classification[20];
};

struct Cluster {
    int numDatapoints;
    struct DataPoint *medoid;
    struct DataPoint *datapoints[nd];
};

struct DataPoint datapoints[nd];
struct Cluster clusters[k];

char *tokens;
char *array[nf + 1];
double features[nf];


void readDatapoints(char* filename) {
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

void cluster() {
    return 0;
}

void saveClustersToFile() {
    return 0;
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

int main(){  
    
    readDatapoints("../Data/iris.data");
    printDatapoints();
    cluster();
    saveClustersToFile();
}