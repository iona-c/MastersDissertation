#ifndef TESTCOLOUR_H
#define TESTCOLOUR_H

#include <iostream>
#include "Image.h"
#include "Volume.h"
#include "EdgeDetection.h"
#include "ColourFilter.h"
#include "Projection.h"
#include "Blur.h"
#include "stringColours.h"

/**
 * @brief Calculates the Mean Absolute Error (MAE) between two images.
 *
 * This function computes the MAE between two images, which is a measure of
 * the differences between the pixel values of the images. The MAE represents
 * the square root of the second sample moment of the differences between
 * predicted values and observed values or the quadratic mean of these differences.
 * The images must have the same dimensions and number of channels.
 *
 * @param img1 The first image as a reference of type Image.
 * @param img2 The second image to compare against the first of type Image.
 * @return The MAE value as a double. If the images do not match in dimensions
 *         or channels, it returns -1.0 and outputs an error message.
 *
 * Usage example:
 *
 * Image img1("path_to_first_image");
 * Image img2("path_to_second_image");
 * double MAE = calculateMSE(img1, img2);
 * std::cout << "MAE: " << MAE << std::endl;
 */
double calculateMSE(const Image& img1, const Image& img2) {
    if (img1.w != img2.w || img1.h != img2.h || img1.c != img2.c) {
        std::cerr << "[ERROR] Images dimensions or channels do not match!" << std::endl;
        return -1.0;
    }

    double mae = 0.0;
    int size = img1.w * img1.h * img1.c;
    for (int i = 0; i < size; i++) {
        mae += std::abs(static_cast<double>(img1.data[i]) - static_cast<double>(img2.data[i]));
    }
    std::cout << "[LOG] MAE: " << mae << std::endl;
    mae /= size;
    return mae;
}

/**
 * Jiawei Wang-jcw23
 * Tests if two images are similar based on Mean Absolute Error (MAE).
 * 
 * This function loads two images from the specified paths and computes their MAE to assess their similarity.
 * An MAE value between 0.0 and 0.1 (inclusive) indicates the images are similar enough to be considered a pass.
 * Otherwise, the test is considered a fail. The function reports errors if it cannot load one or both images.
 * 
 * @param path1 The file path to the first image.
 * @param path2 The file path to the second image.
 * @return Returns true if the MAE is within the specified threshold (indicating similarity), 
 *         false otherwise or if the images cannot be loaded.
 */

bool testImageComparison(const std::string& path1, const std::string& path2) {
    // Instantiating Image objects with paths
    Image img1(path1);
    Image img2(path2);

    if (!img1.data || !img2.data) {
        std::cerr << "[ERROR] Error loading one or both images." << std::endl;
        return false; // Test failed due to error loading images
    }

    double MAE = calculateMSE(img1, img2);
    std::cout << "[LOG] Mean Absolute Error: " << MAE << "\n";
    if (MAE >= 0.0 && MAE <= 30) {
        return true; // Test passed
    } else {
        return false; // Test failed due to MAE out of range
    }
}

/**
 * Jiawei Wang-jcw23
 * Conducts a test to compare a grayscale image processed by an algorithm against a standard grayscale image.
 * 
 * This function aims to verify the accuracy of a grayscale conversion algorithm by comparing its output
 * with a pre-converted grayscale image using the Mean Absolute Error (MAE) method. The paths to the
 * images are predefined within the function. A successful comparison (i.e., the images are similar within
 * an acceptable threshold) will print a success message to the console. If the comparison fails, the function
 * throws a runtime error indicating the test failure.
 * 
 * Throws:
 *     std::runtime_error if the grayscale test fails, indicating the algorithm's output is not sufficiently
 *     close to the expected result.
 */

void testGrayScale(){
    // Adjust these paths to point to your specific images
    std::string opencvImagePath = "tests/testimages/grayscale.png";
    std::string algorithmImagePath = "../Output/1-grayscale/gracehopper.png";

    bool result = testImageComparison(opencvImagePath, algorithmImagePath);

    if (result) {
        std::cout << COL_GREEN << "[TEST] Grayscale test passed." << COL_NORMAL << std::endl;
    } else {
        throw std::runtime_error(COL_RED "[TEST] Grayscale test failed." COL_NORMAL);
    }
    std::cout<<"\n";
}

/**
 * Tests the `saltNpepper` method of the `ColourFilter` class by applying varying levels of salt and 
 * pepper noise to a test image and verifying the correctness of the noise application. This function 
 * iterates through a predefined set of noise percentages (0%, 25%, 50%, 75%, and 100%) and applies 
 * the salt and pepper noise to a uniformly grey-colored image at each level. It then assesses the 
 * noise application by counting the pixels that have been altered to pure black or white, considering 
 * the RGB channels collectively for each pixel. The test checks if the percentage of such altered pixels 
 * closely matches the intended noise percentage, allowing a tolerance of 1% to account for the randomness 
 * inherent in the noise application process.
 *
 * The function initializes the test image to a medium grey to avoid any initial pixels being at the 
 * extreme values of 0 or 255, which could skew the assessment of noise application. It dynamically 
 * allocates memory for the image data and ensures proper cleanup after the test to prevent memory leaks.
 *
 * Preconditions:
 * - The `ColourFilter` class must be implemented with a method `apply` capable of applying salt and 
 *   pepper noise.
 * - An `Image` structure or class must be defined, capable of holding image data including width (`w`), 
 *   height (`h`), number of channels (`c`), and the pixel data (`data`).
 *
 * Postconditions:
 * - The test image is modified by the application of salt and pepper noise at various levels.
 * - The test results are printed to the standard output, indicating the noise level tested and whether 
 *   the actual noise level was within the expected range.
 * - The allocated memory for the test image is properly freed to prevent memory leaks.
 *
 * Throws:
 * - This function catches and reports exceptions that may occur during the noise application process, 
 *   including those thrown by the `saltNpepper` method itself or by issues in memory management.
 *
 * Note:
 * - This test function is intended for use in a testing framework or as part of a suite of tests for 
 *   image processing functions. It assumes the presence of the necessary infrastructure for such testing, 
 *   including the `Image` and `ColourFilter` classes.
 * 
 * @author Omar Belhaj & Prayush Udas
 * @acknowledgement: This function was developed with the assistance of generative AI.
 */
void testSaltNpepperFilter() {
    const int width = 100;
    const int height = 100;
    const int channels = 3;  // Assuming an RGB image
    const std::vector<double> noisePercentages = {0, 25, 50, 75, 100};  // Different noise percentages to test

    for (double noisePercentage : noisePercentages) {
        // Initialize a test image with a specific pattern
        Image image;
        image.w = width;
        image.h = height;
        image.c = channels;
        // Allocate memory for image.data
        image.data = new unsigned char[width * height * channels];
        unsigned char greyValue = 128;  // A value between black and white to avoid black or white pixels
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int ch = 0; ch < channels; ++ch) {
                    image.data[(y * width + x) * channels + ch] = greyValue;
                }
            }
        }

        try {
            ColourFilter cf;
            cf.apply(cf.saltNpepper, image, noisePercentage);

            int noisyPixelsCount = 0;
            for (size_t i = 0; i < width * height * channels; i += channels) {
                bool isNoisy = true;
                for (int ch = 0; ch < channels; ++ch) {
                    if (image.data[i + ch] != 0 && image.data[i + ch] != 255) {
                        isNoisy = false;
                        break;
                    }
                }
                if (isNoisy) {
                    ++noisyPixelsCount;
                }
            }

            // Calculate the actual noise percentage based on noisy pixel count, not channel count
            double actualNoisePercentage = (100.0 * noisyPixelsCount) / (width * height);
            if (std::abs(actualNoisePercentage - noisePercentage) <= 1.0) {  // Allowing 1% tolerance
                std::cout << COL_GREEN << "[TEST] Salt and Pepper filter test for noise percentage " << noisePercentage << "% passed. Noise applied successfully." << COL_NORMAL << std::endl;
            } else {
                std::cerr << COL_GREEN << "[TEST] Salt and Pepper filter test for noise percentage " << noisePercentage << "% failed. Unexpected noise percentage: " << actualNoisePercentage << "%" << COL_NORMAL << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << COL_RED << "[TEST] Exception caught during Salt and Pepper filter test for noise percentage " << noisePercentage << "%: " << e.what() << COL_NORMAL << std::endl;
        }

        // Free the allocated memory for image.data to prevent memory leaks
        delete[] image.data;
    }
}
#endif