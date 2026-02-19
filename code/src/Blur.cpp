#include "Blur.h"
#include "stb_image.h"
#include "stb_image_write.h"

/**
 * Applies a specified blur filter to an Image object without sigma parameter. This function
 * can apply Median or Box blur based on the provided filter type. Gaussian blur requires a sigma
 * parameter and is not applied by this function.
 * 
 * @param filter The type of blur filter to apply.
 * @param image The image to apply the blur filter on.
 * @param kernelSize The size of the kernel used for blurring.
 * 
 * @author Prayush Udas
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::apply(type filter, Image& image, int kernelSize) {
    switch (filter){
        case type::Median:
            applyMedianBlurMultiChannel(image, kernelSize);
            std::cout << "[LOG] Applying Medium Blur" <<std::endl;
            break;
        case type::Box: 
            applyBoxBlur(image, kernelSize);
            std::cout << "[LOG] Applying Box Blur"<< std::endl;
            break;
        case type::Gaussian:
            std::cout << "[LOG] Input a sigma" << std::endl;
        default:
            std::cout << "[ERROR] Wrong arguments" << std::endl;
            break;
    }
}

/**
 * Applies a Gaussian blur filter to an Image object with a specified sigma value. This function
 * specifically handles the Gaussian blur, requiring both kernel size and sigma value.
 * 
 * @param filter The type of blur filter to apply, expected to be Gaussian.
 * @param image The image to apply the Gaussian blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param sigma The sigma value for the Gaussian kernel.
 * 
 * @author Prayush Udas
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::apply(type filter, Image& image, int kernelSize, float sigma) {
    switch (filter){
        case type::Gaussian:
            applyGaussianBlur(image, kernelSize, sigma);
            std::cout << "[LOG] Applying Gaussian Blur" << std::endl;
            break;
        case type::Box:
        case type::Median:
        default:
            std::cout << "[ERROR] Wrong arguments" << std::endl;
            break;
    }
}

/**
 * Applies a median blur filter to a single channel of an Image. The function modifies a
 * provided result buffer with the blurred pixel values.
 * 
 * @param image The image whose channel is to be blurred.
 * @param channelNum The channel number to apply the median blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param result The buffer where the result is to be stored.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::_applyMedianBlurChannel(Image& image,int channelNum, int kernelSize, unsigned char* result){
    int height = image.h;
    int width = image.w;
    int totalChannels = image.c;
    int channel = channelNum;
    int offset = kernelSize/2;
    std::vector<unsigned char> window;
    // Iterate over each pixel in the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            window.clear();

            // Collect the pixel values within the kernel window, handling edges by replication
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = std::min(std::max(x + kx, 0), width - 1);
                    int ny = std::min(std::max(y + ky, 0), height - 1);
                    window.push_back(image.data[(ny * width + nx) * totalChannels + channel]);
                }
            }

            // Sort the window to find the median
            int n = window.size();
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (window[j] > window[j + 1]) {
                        // Swap window[j] and window[j + 1]
                        unsigned char temp = window[j];
                        window[j] = window[j + 1];
                        window[j + 1] = temp;
                    }
                }
            }

            // Pick the middle element as the median
            unsigned char medianValue = window[window.size() / 2];

            // Set the median value as the new pixel value for this channel
            result[(y * width + x) * totalChannels + channel] = medianValue;
        }
    }

}

/**
 * Applies a median blur filter to all channels of an Image.
 * 
 * @param image The image to apply the median blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::applyMedianBlurMultiChannel(Image& image, int kernelSize){
    unsigned char* result = new unsigned char[image.w * image.h* image.c];
    //Apply the median blur separately for each channel
    for (int ch = 0; ch < image.c; ++ch){
        _applyMedianBlurChannel(image,ch,kernelSize, result);
    }
    
    std::copy(result, result + image.w * image.h * image.c, image.data);
}

/**
 * Applies a box blur filter to a single channel of an Image. The function modifies a
 * provided result buffer with the blurred pixel values.
 * 
 * @param image The image whose channel is to be blurred.
 * @param channel The channel number to apply the box blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param result The buffer where the result is to be stored.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::_applyBoxBlurToChannel(Image& image, int channel, int kernelSize, unsigned char* result){
    int height = image.h;
    int width = image.w;
    int numChannels = image.c;

    int offset = kernelSize / 2;
    int kernelArea = kernelSize * kernelSize;
    // Use dynamic memory allocation for rowSum to handle large images
    int* rowSum = new int[width];

    for (int y = 0; y < height; ++y) {
        // Reset rowSum for each row
        for (int i = 0; i < width; ++i) {
            rowSum[i] = 0;
        }

        // Initial row sum for the first kernel window
        for (int ky = -offset; ky <= offset; ++ky) {
            int ny = (y + ky < 0) ? 0 : (y + ky >= height ? height - 1 : y + ky);
            for (int x = 0; x < width; ++x) {
                rowSum[x] += image.data[(ny * width + x) * numChannels + channel];
            }
        }

        for (int x = 0; x < width; ++x) {
            int sum = 0;

            // Sum the row sums within the kernel window
            for (int kx = -offset; kx <= offset; ++kx) {
                int nx = (x + kx < 0) ? 0 : (x + kx >= width ? width - 1 : x + kx);
                sum += rowSum[nx];
            }

            // Calculate the average and assign it as the new pixel value
            result[(y * width + x) * numChannels + channel] = sum / kernelArea;

            // Update rowSum for the next pixel, if not at the end of the row
            if (x < width - 1) {
                for (int ky = -offset; ky <= offset; ++ky) {
                    int removeY = (y + ky < 0) ? 0 : (y + ky >= height ? height - 1 : y + ky);
                    int addY = (y + ky < 0) ? 0 : (y + ky >= height ? height - 1 : y + ky);
                    rowSum[x] -= image.data[(removeY * width + x) * numChannels + channel];
                    rowSum[x] += image.data[(addY * width + x + 1) * numChannels + channel];
                }
            }
        }
    }
    delete[] rowSum;
}

 /**
 * Applies a box blur filter to all channels of an Image.
 * 
 * @param image The image to apply the box blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::applyBoxBlur(Image& image,  int kernelSize){
    unsigned char* result = new unsigned char[image.w * image.h* image.c];
    int numChannels = image.c;
    for (int channel = 0; channel < numChannels; ++channel) {
        _applyBoxBlurToChannel(image ,channel, kernelSize, result);
    }
    std::copy(result, result + image.w * image.w * numChannels, image.data);
    delete[] result;
}

/**
 * Generates a Gaussian kernel with a specified size and sigma value.
 * 
 * @param kernelSize The size of the Gaussian kernel to be generated.
 * @param sigma The sigma value for the Gaussian kernel.
 * @return A 2D vector representing the Gaussian kernel.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
std::vector<std::vector<float>> Blur::_generateGaussianKernel(int kernelSize, float sigma){
    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize));
    float sum = 0.0f;
    int offset = kernelSize / 2;

    for (int x = -offset; x <= offset; ++x) {
        for (int y = -offset; y <= offset; ++y) {
            float exponent = -(x * x + y * y) / (2 * sigma * sigma);
            float value = exp(exponent) / (2 * M_PI * sigma * sigma);
            kernel[x + offset][y + offset] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

/**
 * Applies a Gaussian blur filter to a single channel of an Image. The function modifies a
 * provided result buffer with the blurred pixel values using a precomputed Gaussian kernel.
 * 
 * @param image The image whose channel is to be blurred.
 * @param kernel The Gaussian kernel to use for blurring.
 * @param channel The channel number to apply the Gaussian blur on.
 * @param result The buffer where the result is to be stored.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI
 */
void Blur::_applyGaussianBlurToChannel(Image& image,  const std::vector<std::vector<float>>& kernel, int channel, unsigned char* result){
    int height = image.h;
    int width = image.w;
    int numChannels = image.c;
    int offset = kernel.size() / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sum = 0.0f;

            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = x + kx;
                    int ny = y + ky;

                    // Clamping nx and ny
                    nx = (nx < 0) ? 0 : (nx >= width) ? width - 1 : nx;
                    ny = (ny < 0) ? 0 : (ny >= height) ? height - 1 : ny;

                    sum += kernel[ky + offset][kx + offset] * image.data[(ny * width + nx) * numChannels + channel];
                }
            }

            // Clamping the result
            int newValue = static_cast<int>(sum);
            newValue = (newValue < 0) ? 0 : (newValue > 255) ? 255 : newValue;

            result[(y * width + x) * numChannels + channel] = static_cast<unsigned char>(newValue);
        }
    }
}

/**
 * Applies a Gaussian blur filter to all channels of an Image. This function utilizes the
 * _generateGaussianKernel and _applyGaussianBlurToChannel helper functions to perform the
 * blurring operation.
 * 
 * @param image The image to apply the Gaussian blur on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param sigma The sigma value for the Gaussian kernel.
 * 
 * @author Omar Belhaj
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::applyGaussianBlur(Image& image, int kernelSize, float sigma) {
    int numChannels = image.c;
    
    std::vector<std::vector<float>> kernel = _generateGaussianKernel(kernelSize, sigma);
    unsigned char* result = new unsigned char[image.w * image.h* image.c];
    for (int channel = 0; channel < numChannels; ++channel) {
        _applyGaussianBlurToChannel(image, kernel, channel, result);
    }
    std::copy(result, result + image.w*image.h*image.c, image.data);
    delete[] result;
}

/**
 * Placeholder function for applying a gaussian blur filter to a Volume.
 * 
 * @param filter The type of blur filter to apply.
 * @param volume The volume to apply the blur filter on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param sigma The sigma value for the Gaussian kernel.
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::apply(type filter, Volume& volume, int kernelSize, float sigma){
    switch (filter){
        case type::Gaussian:
            std::cout <<"[LOG] Applying Guassian Blur To Volume" << std::endl;
            applyGaussianBlurToVolume(volume, kernelSize, sigma);
            break;
        default:
            std::cerr << "[LOG] Wrong arguments" << std::endl;
            break;
    }
}

/**
 * Placeholder function for applying a median blur filter to a Volume.
 * 
 * @param filter The type of blur filter to apply.
 * @param volume The volume to apply the blur filter on.
 * @param kernelSize The size of the kernel used for blurring.
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::apply(type filter, Volume& volume, int kernelSize){
    switch (filter){
        case type::Median:
            std::cout << "[LOG] Applying Median Blur To Volume" << std::endl;
            applyMedianBlurToVolume(volume, kernelSize);
            break;
        default:
            std::cerr << "[ERROR] Wrong arguments" << std::endl;
            break;
    }
}

/**
 * Apply median blur to volume.
 * 
 * @param volume The volume to apply the blur filter on.
 * @param kernelSize The size of the kernel used for blurring.
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::applyMedianBlurToVolume(Volume& volume, int kernelSize){
    std::cerr << "[LOG] Median Filter " << kernelSize << "x" << kernelSize << "x" << kernelSize << " is Processing..." << std::endl;
    std::vector<std::vector<std::vector<unsigned char>>> blurData(volume.l + 1, std::vector<std::vector<unsigned char>>(volume.h + 1, std::vector<unsigned char>(volume.w + 1)));

    for (int z = 1; z <= volume.l; ++z) {
        for (int y = 1; y <= volume.h; ++y) {
            for (int x = 1; x <= volume.w; ++x) {
                blurData[z][y][x] = _CalculateMedianValue(z, y, x, kernelSize, volume); // copy data, ignore index 0
            }
        }
    }
    volume.data.swap(blurData);
    std::cout << "[LOG] 3D Median done." << std::endl;
}

/**
 * Apply gaussian blur to volume.
 * 
 * @param volume The volume to apply the blur filter on.
 * @param kernelSize The size of the kernel used for blurring.
 * @param sigma The sigma value for the Gaussian kernel.
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void Blur::applyGaussianBlurToVolume(Volume& volume, int kernelSize, float sigma) {
    std::vector<double> weights = _Gaussian3DKernel(kernelSize, sigma); // generate 3D Gaussian kernel
    std::cerr << "[LOG] Gaussian Filter " << kernelSize << "x" << kernelSize << "x" << kernelSize << " is Processing..." << std::endl;
    std::vector<std::vector<std::vector<unsigned char>>> blurData(volume.l + 1, std::vector<std::vector<unsigned char>>(volume.h + 1, std::vector<unsigned char>(volume.w + 1)));

    for (int z = 1; z <= volume.l; ++z) {
        for (int y = 1; y <= volume.h; ++y) {
            for (int x = 1; x <= volume.w; ++x) {
                blurData[z][y][x] = _CalculateWeightedAverage(z, y, x, weights, kernelSize, volume); // copy data, ignore index 0
            }
        }
    }
    volume.data.swap(blurData);
    std::cout << "[LOG] 3D Gaussian done." << std::endl;
}

/**
 * Generate 2D Gaussian kernel.
 * 
 * @param x: x coordinate
 * @param y: y coordinate
 * @param sigma: sigma value
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
double Blur::_Gaussian2DKernel(const int& x, const int& y, const float& sigma) {
    return (1 / (2 * M_PI * sigma * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

/**
 * Generate 3D Gaussian kernel.
 * 
 * @param size: kernel size
 * @param sigma: sigma value
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
std::vector<double> Blur::_Gaussian3DKernel(const int&size, const float& sigma) {
    std::vector<double> kernel(size * size * size, 0);
    double sum = 0;

    // generate 3D Gaussian kernel
    int halfSize = size / 2;
    for (int z = 0; z < size; ++z) {
        for (int y = -halfSize; y <= halfSize; ++y) {
            for (int x = -halfSize; x <= halfSize; ++x) {
                double value = _Gaussian2DKernel(x, y, sigma);
                kernel[(z * size + (y + halfSize)) * size + (x + halfSize)] = value;
                sum += value;
            }
        }
    }
    // normalize the kernel
    for (double& val : kernel) {
        val /= sum;
    }
    return kernel;
}

/**
 * Calculate weighted average, used for Gaussian filter.
 * 
 * @param z: z coordinate
 * @param y: y coordinate
 * @param x: x coordinate
 * @param weights: weights
 * @param size: kernel size
 * @param volume: volume data
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
unsigned char Blur::_CalculateWeightedAverage(const int& z, const int& y, const int& x, const std::vector<double>& weights, const int& size, const Volume& volume) {
    double sum = 0.0;
    double totalWeights = 0.0;
    int index = 0; // used to access weights
    
    int halfSize = size / 2;
    for (int dz = -halfSize; dz <= halfSize; ++dz) {
        for (int dy = -halfSize; dy <= halfSize; ++dy) {
            for (int dx = -halfSize; dx <= halfSize; ++dx) {
                int nz = z + dz;
                int ny = y + dy;
                int nx = x + dx;
                
                // check if it is within the boundary, if not, consider it as 0
                if (nz > 0 && nz <= volume.l && ny > 0 && ny <= volume.h && nx > 0 && nx <= volume.w) {
                    sum += volume.data[nz][ny][nx] * weights[index];
                    totalWeights += weights[index];
                }
                
                ++index;
            }
        }
    }
    // return the weighted average, clamped to 0-255
    return static_cast<unsigned char>(std::clamp(sum / totalWeights, 0.0, 255.0));
}

/**
 * Calculate median value, used for Median filter.
 * 
 * @param z: z coordinate
 * @param y: y coordinate
 * @param x: x coordinate
 * @param size: kernel size
 * @param volume: volume data
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
unsigned char Blur::_CalculateMedianValue(const int& z, const int& y, const int& x, const int& size, const Volume& volume) {
    std::vector<unsigned char> values;

    int halfSize = size / 2;
    for (int dz = -halfSize; dz <= halfSize; ++dz) {
        for (int dy = -halfSize; dy <= halfSize; ++dy) {
            for (int dx = -halfSize; dx <= halfSize; ++dx) {
                int nz = z + dz;
                int ny = y + dy;
                int nx = x + dx;
                
                // check if it is within the boundary
                if (nz > 0 && nz <= volume.l && ny > 0 && ny <= volume.h && nx > 0 && nx <= volume.w) {
                    values.push_back(volume.data[nz][ny][nx]);
                }
            }
        }
    }

    int valuesSize = values.size();
    if (valuesSize % 2 == 0) {
        return (Utilities::QuickSelectMedian(values, 0, valuesSize-1, valuesSize/2-1) + Utilities::QuickSelectMedian(values, 0, valuesSize-1, valuesSize/2)) / 2;
    } else {
        return Utilities::QuickSelectMedian(values, 0, valuesSize-1, valuesSize/2);
    }
}
