#ifndef BLUR
#define BLUR

#include "Filter.h"
#include "Utilities.h"

/**
 * @file Blur.h
 * @brief The Blur class provides various blurring effects for images and volumes.
 * 
 * @details This class supports Median, Box, and Gaussian blur types. Each blur type
 * can be applied to either an Image or a Volume. The Gaussian blur method also supports
 * specifying the sigma value for the Gaussian kernel.
 * 
 * @note The Volume related methods are declared but not implemented.
 * 
 * @author Prayush Udas
 */
class Blur : public Filter{ 
    
    public: 
        enum type{
            Median,
            Box,
            Gaussian
        };
        void apply(type filter, Image& image, int kernelSize);
        void apply(type filter, Image& image, int kernelSize, float sigma);
        void apply(type filter, Volume& volume, int kernelSize);
        void apply(type filter, Volume& volume, int kernelSize, float sigma);
    private:
        void applyMedianBlurMultiChannel(Image& image , int kernelSize);
        void applyBoxBlur(Image& image, int kernelSize);
        void applyGaussianBlur(Image& image, int kernelSize, float sigma);
        void _applyBoxBlurToChannel(Image& image, int kernelSize, int channelNum, unsigned char* result);
        void _applyMedianBlurChannel(Image& image,int channel, int kernelSize, unsigned char* result);
        void _applyGaussianBlurToChannel(Image& image, const std::vector<std::vector<float>>& kernel, int channel, unsigned char* result);
        std::vector<std::vector<float>> _generateGaussianKernel(int kernelSize, float sigma);
        
        void applyGaussianBlurToVolume(Volume& volume, int kernelSize, float sigma);
        void applyMedianBlurToVolume(Volume& volume, int kernelSize);
        double _Gaussian2DKernel(const int& x, const int& y, const float& sigma);
        std::vector<double> _Gaussian3DKernel(const int&size, const float& sigma);
        unsigned char _CalculateWeightedAverage(const int& z, const int& y, const int& x, const std::vector<double>& weights, const int& size, const Volume& volume);
        unsigned char _CalculateMedianValue(const int& z, const int& y, const int& x, const int& size, const Volume& volume);
        void apply(){}
};

#endif