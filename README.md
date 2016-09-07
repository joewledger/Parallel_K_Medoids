##Parallel K-Medoids

This project implements a parallelizable version of the K-Medoids algorithm. In particular, it implements the [Park-Jun K-medoids algorithm](http://www.sciencedirect.com/science/article/pii/S095741740800081X).

K-medoids is a clustering algorithm that attempts to partition a dataset of n points into k clusters. Each cluster has a datapoint marked as the medoid of the cluster. The algorithm attempts to minimize the sum of pairwise distances between each datapoint and the medoid of the cluster that it is in.

###Algorithm
1) Select initial medoids (discussed in detail in the final report document).

2) Update medoids by finding the datapoint minimizing the sum of pairwise distances from itself to all other points in the cluster (for each cluster).

3) Assign objects to medoids.

4) Repeat steps 2 and 3 until the sum of distances from each datapoint to their medoid no longer changes.

###Code
The code is written in C with OpenMP and OpenACC directives that can be applied using a compiler flag. I applied parallelization to each of the steps 1-3 seperately, and then compared the runtime of the non-parallelized, OpenMP, and OpenACC code. I tested the code on three datasets (all included in the Data/ directory).

To run the code yourself, run either the run.sh shell script (for non-parallelized K-medoids), the omp.slurm script (for OpenMP parallelized K-medoids), or the acc.slurm script (for OpenACC parallelized K-medoids).

###Results

The following graphs show the results of my experiments on the 3 datasets I tested on (Cars, Iris, and Soybean).

![cars](https://cloud.githubusercontent.com/assets/6250320/18315765/287332bc-74e6-11e6-8876-80de07c24d8a.png)
![iris](https://cloud.githubusercontent.com/assets/6250320/18315766/2b143868-74e6-11e6-8202-de3e376a3afa.png)
![soybean](https://cloud.githubusercontent.com/assets/6250320/18315767/2cf9d692-74e6-11e6-8c05-8a9da4d9a599.png)

###Conclusion
OpenACC reduced the runtime needed for the algorithm to complete significantly on all datasets. OpenMP also gave decent, but more sporadic improvements over the non-parallelized code.
