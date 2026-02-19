#include "Utilities.h"

/**
 * Partition the vector.
 * 
 * @param values: vector
 * @param start: start index
 * @param end: end index
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
int Utilities::partition(std::vector<unsigned char>& values, int start, int end){
    unsigned char pivot = values[end];
    int i = start - 1;
    for(int j = start; j < end; j++){
        if(values[j] <= pivot){
            i++;
            std::swap(values[i], values[j]);
        }
    }
    std::swap(values[i+1], values[end]);
    return i+1;
}

/**
 * Quick select median value.
 * 
 * @param values: vector
 * @param start: start index
 * @param end: end index
 * @param medianIndex: median index
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
unsigned char Utilities::QuickSelectMedian(std::vector<unsigned char>& values, int start, int end, int medianIndex){
    if(start == end){
        return values[start];
    }
    int pivotIndex = partition(values, start, end);
    if(pivotIndex == medianIndex){
        return values[pivotIndex];
    }else if(pivotIndex < medianIndex){ // search right
        return QuickSelectMedian(values, pivotIndex+1, end, medianIndex);
    }else{ // search left
        return QuickSelectMedian(values, start, pivotIndex-1, medianIndex);
    }
}