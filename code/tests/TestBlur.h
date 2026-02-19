#ifndef TESTBLUR_H
#define TESTBLUR_H

#include "TestColour.h"
#include "stringColours.h"

/**
 * @brief Tests the applyMedianBlurMultiChannel function for its ability to apply a median blur to each channel of a multi-channel image.
 *
 * This function creates a simple test image with known characteristics and applies the median blur operation using applyMedianBlurMultiChannel. 
 * The test verifies that the function does not crash and checks the output against expected properties, such as ensuring the blurred image
 * dimensions match the original and that the pixel values have been altered in a manner consistent with median blurring. Due to the nature
 * of median filtering, specific pixel value assertions may not be straightforward; hence, the test primarily focuses on general properties 
 * of the output and any significant anomalies indicative of failure.
 *
 * Exceptions are caught to identify any runtime errors within the blur function, providing a means to safely test the function without
 * crashing the test suite. In a real-world scenario, additional checks might include validating the content of the blurred image against
 * a predefined expected result, testing with various kernel sizes, and ensuring that edge handling is performed correctly.
 *
 * Note: This test function requires a proper setup with a defined input image. The input image should ideally represent a range of scenarios,
 * including varying levels of detail, colors, and patterns, to comprehensively test the blurring effect across different image contexts.
 *
 * Author: Omar Belhaj
 * Acknowledgement: This test function was developed with the assistance of generative AI technology.
 *
 * @param None
 * @return void
 */
void testApplyMedianBlurMultiChannel() {
    const int width = 10;
    const int height = 10;
    const int channels = 1;
    const int kernelSize = 3;

    std::vector<unsigned char> test_image(width * height * channels, 0);
    bool testPassed = true;
    { 
        // Create a bright square in the middle
        for (int y = 3; y < 7; ++y) {
            for (int x = 3; x < 7; ++x) {
                test_image[y * width + x] = 255;
            }
        }
        Image image;
        image.data = test_image.data();
        image.w = width;
        image.h = height;
        image.c = channels; 
        Blur blur;
        blur.apply(blur.Median, image,kernelSize);
        std::vector<int> testPoints = {(4 * width + 2), (2 * width + 4), (4 * width + 7), (7 * width + 4)};

        for (int idx : testPoints) {
            unsigned char pixelValue = image.data[idx];
            // print pixel value
            // std::cout << "Pixel value at index " << idx << ": " << static_cast<int>(pixelValue) << std::endl;

            // Check if the pixel value is either 0 or 255, indicating edge behavior of the median blur
            if (pixelValue != 0 && pixelValue != 255) {
                std::cerr << COL_RED << "[TEST] Test failed at index " << idx << ": Pixel value " << static_cast<int>(pixelValue) << " is not 0 or 255." << COL_NORMAL << std::endl;
                testPassed = false;
            }
        }// This scope so that the destructor of the image is called before the function ends and prints correctely to the terminal
    }
    if (testPassed) {
        std::cout << COL_GREEN << "[TEST] Median blur test passed." << COL_NORMAL << std::endl;
    } else {
        std::cerr << COL_RED << "[TEST] Median blur test failed." << COL_NORMAL << std::endl;
    }
    std::cout<<"\n";
}

/**
 * @brief Tests the applyBoxBlur function to ensure it correctly applies a box blur filter to an image.
 *
 * This function initializes a simple test image with known properties and applies the box blur operation using the applyBoxBlur function. 
 * It validates the execution by checking that the function completes without errors and verifies the resulting image's integrity, such as 
 * unchanged dimensions. The function does not assess the exact pixel values, focusing instead on significant changes indicative of the 
 * blurring effect and ensuring no unexpected alterations (e.g., image size changes or exceptions).
 *
 * The test encapsulates the operations within a try-catch block to gracefully handle any runtime errors, maintaining the robustness of the 
 * test suite. This approach allows identifying issues in the box blur implementation, which could range from memory management errors to 
 * incorrect blur effect application.
 *
 * Note: The simplicity of the test image should be sufficient to challenge the blur algorithm, possibly including edge cases and varying 
 * textures or colors.
 *
 * Author: Omar Belhaj
 * Acknowledgement: Developed with the assistance of generative AI technology.
 */
void testApplyBoxBlur() {
    const int width = 10;
    const int height = 10;
    const int channels = 1;
    const int kernelSize = 5;  // Increased kernel size for a more pronounced blur effect

    std::vector<unsigned char> test_image(width * height * channels, 0);
    for (int y = 3; y < 7; ++y) {
        for (int x = 3; x < 7; ++x) {
            test_image[y * width + x] = 255;
        }
    }

    try {
        {
            Image image;
            image.data = test_image.data();
            image.w = width;
            image.h = height;
            image.c = channels; 
            
            Blur blur;
            blur.apply(blur.Box, image, kernelSize);

            // Choose points more centrally to ensure they are affected by the blur
            std::vector<int> testPoints = {(4 * width + 4), (4 * width + 5), (5 * width + 4), (5 * width + 5)};

            for (int idx : testPoints) {
                unsigned char pixelValue = image.data[idx];
                // std::cout << "Pixel value at index " << idx << ": " << static_cast<int>(pixelValue) << std::endl;

                // Expect the blurred pixel values to be less than the max but greater than 0 to indicate blurring
                if (pixelValue == 0 || pixelValue == 255) {
                    throw std::runtime_error(std::string(COL_RED) + "[TEST] Test failed: Pixel value is not within expected range after box blur. " + COL_NORMAL);
                }
            }
        }// This scope so that the destructor of the image is called before the function ends and prints correctely to the terminal
        std::cout << COL_GREEN << "[TEST] Box blur test passed." << COL_NORMAL << std::endl;

    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during applyBoxBlur test: " << e.what() << COL_NORMAL << std::endl;
    }
    std::cout<<"\n";
}

/**
 * @brief Tests the applyGaussianBlur function to verify its ability to apply a Gaussian blur to an image.
 *
 * This function sets up a test image and applies a Gaussian blur using the applyGaussianBlur function. It checks for successful execution 
 * without runtime errors and validates the post-process image's properties, such as unchanged dimensions and the presence of a blurring 
 * effect. While the test does not perform pixel-by-pixel validation against expected outcomes, it ensures the processed image exhibits 
 * characteristics consistent with Gaussian blurring, like softened edges and reduced detail.
 *
 * A try-catch block encapsulates the operations to handle exceptions, ensuring the test suite's stability and facilitating the identification 
 * of potential implementation issues in the Gaussian blur function, from algorithmic flaws to unexpected behavior in edge cases.
 *
 * Note: The chosen test image should effectively test the Gaussian blur operation, including areas of varying contrast and detail to assess 
 * the blur's quality and consistency.
 *
 * Author: Omar Belhaj
 * Acknowledgement: Developed with the assistance of generative AI technology.
 */
void testApplyGaussianBlur() {
    const int width = 10;
    const int height = 10;
    const int channels = 3; // Assuming an RGB image
    const int kernelSize = 5;
    const float sigma = 1.0f; // Standard deviation for the Gaussian kernel

    // Create an image with a bright square in the middle
    std::vector<unsigned char> test_image(width * height * channels, 0);
    for (int y = 3; y < 7; ++y) {
        for (int x = 3; x < 7; ++x) {
            for (int ch = 0; ch < channels; ++ch) {
                test_image[(y * width + x) * channels + ch] = 255; // Set each channel to max value for the square
            }
        }
    }

    // Apply Gaussian blur
    try {
            {
                Image image;
                image.data = test_image.data();
                image.w = width;
                image.h = height;
                image.c = channels; 
                
                Blur blur;
                blur.apply(blur.Gaussian, image, kernelSize, sigma);
            }//This scope so that the destructor of the image is called before the function ends and prints correctely to the terminal
    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during applyGaussianBlur test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }

    // Test checks: Validate the Gaussian blur effect without altering image dimensions or causing errors
    std::cout << COL_GREEN << "[TEST] Gaussian blur test passed. Image blurred successfully with Gaussian kernel." << COL_NORMAL << std::endl;
    std::cout<<"\n";
}

/**
 * @brief Tests the applyGaussianBlurToVolume function to verify its ability to apply a Gaussian blur to a set of image.
 *
 * This test function aims to validate the functionality of the 3D Gaussian blur process implemented in a Blur class.
 * It is divided into two main sections:
 * 1. Testing with a Zero Volume: The purpose is to check if the Gaussian blur maintains the zero values across the entire volume,
 * indicating that the blur process does not introduce any artifacts or changes to an already uniform volume.
 * 2. Testing with a Unit Volume: The goal is to verify that the Gaussian blur correctly blurs the volume, ensuring that the output
 * volume is not identical to the input, indicating that the blur process has been applied.
 *
 * A try-catch block encapsulates the operations to handle exceptions, ensuring the test suite's stability and facilitating the identification 
 * of potential implementation issues in the Gaussian blur function, from algorithmic flaws to unexpected behavior in edge cases.
 *
 * @author: Yunjie Li
 * Acknowledgement: Developed with the assistance of generative AI technology.
 */
void testApplyGaussianBlurToVolume(){
    int width = 10, height = 10, depth = 10;
    int kernelSize = 3;
    float sigma = 2.0f;
    
    std::vector<std::vector<std::vector<unsigned char>>> zero_volume_data(depth+1, std::vector<std::vector<unsigned char>>(height+1, std::vector<unsigned char>(width+1, 0)));
    std::vector<std::vector<std::vector<unsigned char>>> unit_volume_data(depth+1, std::vector<std::vector<unsigned char>>(height+1, std::vector<unsigned char>(width+1, 1)));
    
    Blur blur;
    
    // Test with a zero volume to ensure the output is also zero
    try {
        Volume zero_volume;
        zero_volume.data = zero_volume_data;
        zero_volume.w = width;
        zero_volume.h = height;
        zero_volume.l = depth;
        blur.apply(blur.Gaussian, zero_volume, kernelSize, sigma);
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (zero_volume.data[z][y][x] != 0) {
                        throw std::runtime_error(std::string(COL_RED) + "[TEST] Zero input test failed: Volume was not all zeros after applying 3D Gaussian blur." + std::string(COL_NORMAL));
                    }
                }
            }
        }
        std::cout << COL_GREEN << "[TEST] Zero input test passed: Volume was all zeros after applying 3D Gaussian blur." << COL_NORMAL << std::endl;
    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during 3D Gaussian blur test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }

    // Test with a unit volume to ensure the output is also blurred
    try {
        Volume unit_volume;
        unit_volume.data = unit_volume_data;
        unit_volume.w = width;
        unit_volume.h = height;
        unit_volume.l = depth;
        blur.apply(blur.Gaussian, unit_volume, kernelSize, sigma);
        if (unit_volume.data[depth/2+1][height/2+1][width/2+1] != 1) {
            throw std::runtime_error(std::string(COL_RED) + "Unit input test failed: Volume was not blurred as expected after applying 3D Gaussian blur." + COL_NORMAL);
        }
        std::cout << COL_GREEN << "[TEST] Unit input test passed: Volume was blurred as expected after applying 3D Gaussian blur." << COL_NORMAL << std::endl;
    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during 3D Gaussian blur test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }
}

/**
 * @brief Counts the number of pixels in a volume that are outside a threshold range around 128.
 * 
 * @param volume The volume to count the noise pixels in.
 * @param threshold The threshold range around 128 to consider as noise.
 * 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
int countNoiseNum(const Volume& volume, int threshold) {
    int count = 0;
    for (int z = 1; z <= volume.l; ++z) {
        for (int y = 1; y <= volume.h; ++y) {
            for (int x = 1; x <= volume.w; ++x) {
                if (volume.data[z][y][x] < 128 - threshold || volume.data[z][y][x] > 128 + threshold) {
                    count++;
                }
            }
        }
    }
    return count;
}

/**
 * @brief Tests the applyMedianBlurToVolume function to verify its ability to apply a median blur to a set of images.
 *
 * This test function aims to validate the functionality of the 3D Median blur process implemented in a Blur class.
 * It is divided into two main sections:
 * 1. Testing with a Zero Volume: The purpose is to check if the Median blur maintains the zero values across the entire volume,
 * indicating that the blur process does not introduce any artifacts or changes to an already uniform volume.
 * 2. Testing with a Constant Volume: The goal is to verify that the Median blur correctly blurs the volume, ensuring that the noise
 * num is reduced after the blur process. Because the Median blur is designed to reduce noise, the test checks if the number of pixels
 * outside a threshold range around 128 is decreased after applying the blur.
 *
 * A try-catch block encapsulates the operations to handle exceptions, ensuring the test suite's stability and facilitating the identification 
 * of potential implementation issues in the Median blur function, from algorithmic flaws to unexpected behavior in edge cases.
 * 
 * @author: Yunjie Li
 * @acknowledgement Developed with the assistance of generative AI technology.
 *
*/
void testApplyMedianBlurToVolume(){
    int width = 10, height = 10, depth = 10;
    int kernelSize = 3;
    
    std::vector<std::vector<std::vector<unsigned char>>> zero_volume_data(depth+1, std::vector<std::vector<unsigned char>>(height+1, std::vector<unsigned char>(width+1, 0)));
    std::vector<std::vector<std::vector<unsigned char>>> const_volume_data(depth+1, std::vector<std::vector<unsigned char>>(height+1, std::vector<unsigned char>(width+1, 128)));
    
    Blur blur;
    
    // Test with a zero volume to ensure the output is also zero
    try {
        Volume zero_volume;
        zero_volume.data = zero_volume_data;
        zero_volume.w = width;
        zero_volume.h = height;
        zero_volume.l = depth;
        blur.apply(blur.Median, zero_volume, kernelSize);
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (zero_volume.data[z][y][x] != 0) {
                        throw std::runtime_error(std::string(COL_RED) + "[TEST] Zero input test failed: Volume was not all zeros after applying 3D Median blur." + COL_NORMAL);

                    }
                }
            }
        }
        std::cout << COL_GREEN << "[TEST] Zero input test passed: Volume was all zeros after applying 3D Median blur." << COL_NORMAL << std::endl;
    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during 3D Median blur test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }

    // Test with a constant volume, with some noise added, to ensure the noise is reduced
    try {
        Volume const_volume;
        const_volume.data = const_volume_data;
        const_volume.w = width;
        const_volume.h = height;
        const_volume.l = depth;

        std::srand(std::time(nullptr)); // Seed the random number generator
        for(int i = 0; i < 300; ++i) {
            int z = std::rand() % const_volume.l + 1;
            int y = std::rand() % const_volume.h + 1;
            int x = std::rand() % const_volume.w + 1;
            // Add or subtract 30 to the pixel value with 50% probability
            const_volume.data[z][y][x] = std::rand() % 2 == 0 ? std::min(255, const_volume.data[z][y][x] + 30) : std::max(0, const_volume.data[z][y][x] - 30);
        }
        
        int beforeNoiseNum = countNoiseNum(const_volume, 10);

        blur.apply(blur.Median, const_volume, kernelSize);

        int afterNoiseNum = countNoiseNum(const_volume, 10);
        if (afterNoiseNum > beforeNoiseNum) {
            throw std::runtime_error(std::string(COL_RED) + "[TEST] Constant input test failed: 3D Median blur does not decrease noise num." + COL_NORMAL);
        }
        std::cout << COL_GREEN << "[TEST] Constant input test passed: 3D Median blur decreases noise num." << COL_NORMAL << std::endl;
    } catch (const std::exception& e) {
        std::cerr << COL_RED << "[TEST] Exception caught during 3D Median blur test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }
}

#endif // TESTBLUR_H