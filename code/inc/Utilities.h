#ifndef UTILITIES
#define UTILITIES
#include <vector>

class Utilities{
    public:
        static int partition(std::vector<unsigned char>& values, int start, int end);
        static unsigned char QuickSelectMedian(std::vector<unsigned char>& values, int start, int end, int medianIndex);
};

#endif