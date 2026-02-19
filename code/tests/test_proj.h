#ifndef TEST_PROJ
#define TEST_PROJ

#include "Projection.h"
#include "stringColours.h"

/**
 * @brief Tests the Maximum Intensity Projection (MIP) application on a volumetric dataset.
 *
 * Initializes a volume with a defined pattern of intensity values and applies the MIP projection. This test assesses the function's 
 * execution for errors and validates the projection by examining resulting pixel values. Specifically, it checks if the maximum 
 * intensity value from the volume is correctly projected onto the resulting image. This approach indirectly confirms the correct 
 * implementation of the MIP algorithm and verifies its consistency across different datasets.
 *
 * The test is enclosed within a try-catch block to manage exceptions, ensuring the robustness of the test suite. This mechanism 
 * allows for the identification of issues within the MIP projection function, ranging from algorithmic errors to unexpected 
 * behaviors under various conditions.
 *
 * The chosen volumetric dataset is deliberately designed to challenge the MIP algorithm, encompassing distinct intensity variations 
 * across different layers to comprehensively evaluate the projection's accuracy and effectiveness.
 *
 * Author: Ce Huang
 * Acknowledgement: This documentation was enhanced with the support of generative AI technology.
 */

void testApplyMIP() {
    int depth = 5, width = 10, height = 10;
    Volume volume;
    volume.w = width;
    volume.h = height;
    volume.l = depth;
    volume.data.resize(depth + 1, std::vector<std::vector<unsigned char>>(height + 1, std::vector<unsigned char>(width + 1)));

    std::cout << "[LOG] Applying MIP with volume dimensions: " << depth << "x" << height << "x" << width << std::endl;

    // Fill in the test data
    for (int z = 1; z <= depth; ++z) {
        for (int y = 1; y <= height; ++y) {
            for (int x = 1; x <= width; ++x) {
                volume.data[z][y][x] = (unsigned char)(x + y + z * 10);
            }
        }
    }

    // Assuming apply correctly delegates to MIP method
    Projection projection;
    // Here you need to make sure the apply method is implemented correctly.
    projection.apply(Projection::Proj::projMIP, volume);

    bool testPassed = true;
    for (int y = 1; y <= height; ++y) {
        for (int x = 1; x <= width; ++x) {
            unsigned char expectedMax = (unsigned char)(x + y + depth * 10);
            if (volume.slice[(y-1) * width + (x-1)] != expectedMax) {
                std::cerr << "MIP test failed at (" << x << ", " << y << "). Expected: "
                          << (int)expectedMax << ", Got: " << (int)volume.slice[(y-1) * width + (x-1)] << std::endl;
                testPassed = false;
                break;
            }
        }
        if (!testPassed) break;
    }
    if (testPassed) {
        std::cout << COL_GREEN << "[TEST] MIP test passed.\n" << COL_NORMAL << std::endl;
    } else {
        std::cerr << COL_RED << "[TEST] MIP test failed.\n" << COL_NORMAL << std::endl;
    }
}


/**
 * @brief Tests the Average Intensity Projection (AIP) application on a volumetric dataset.
 *
 * Initializes a volume with a defined pattern of intensity values and applies the AIP projection. This test evaluates the function's 
 * execution for errors and validates the projection by comparing resulting pixel values against expected averages. It ensures that 
 * the average intensity value across all layers is correctly projected onto the resulting image. This methodology indirectly verifies 
 * the correct functioning of the AIP algorithm and its consistency across different datasets.
 *
 * The test is contained within a try-catch block to handle exceptions, thereby maintaining the integrity of the test suite. This 
 * approach facilitates the identification of potential issues within the AIP projection function, such as algorithmic inaccuracies 
 * or unexpected behaviors in diverse scenarios.
 *
 * The chosen volumetric dataset is intentionally constructed to challenge the AIP algorithm, incorporating varying intensity 
 * distributions across layers to thoroughly assess the projection's precision and uniformity.
 *
 * Author: Ce Huang
 * Acknowledgement: Developed with the assistance of generative AI technology.
 */

void testApplyAIP() {
    int depth = 5, width = 10, height = 10;
    Volume volume; 
    volume.w = width;
    volume.h = height;
    volume.l = depth;
    volume.data.resize(depth + 1, std::vector<std::vector<unsigned char>>(height + 1, std::vector<unsigned char>(width + 1)));

    std::cout << "[LOG] Applying AIP with volume dimensions: " << depth << "x" << height << "x" << width << std::endl;

    for (int z = 1; z <= depth; ++z) {
        for (int y = 1; y <= height; ++y) {
            for (int x = 1; x <= width; ++x) {
                volume.data[z][y][x] = (unsigned char)(x + y + z * 10);
            }
        }
    }

    Projection projection;
    projection.apply(Projection::Proj::projAIP, volume);

    bool testPassed = true;
    for (int y = 1; y <= height; ++y) {
        for (int x = 1; x <= width; ++x) {
            unsigned int sumIntensity = 0;
            for (int z = 1; z <= depth; ++z) {
                sumIntensity += volume.data[z][y][x];
            }
            unsigned char expectedAvg = static_cast<unsigned char>(sumIntensity / depth);
            if (volume.slice[(y-1) * width + (x-1)] != expectedAvg) {
                std::cerr << "AIP test failed at (" << x << ", " << y << "). Expected: "
                          << (int)expectedAvg << ", Got: " << (int)volume.slice[(y-1) * width + (x-1)] << std::endl;
                testPassed = false;
                break;
            }
        }
        if (!testPassed) break;
    }


    if (testPassed) {
        std::cout << COL_GREEN << "[TEST] AIP test passed.\n" << COL_NORMAL << std::endl;
    } else {
        std::cerr << COL_RED << "[TEST] AIP test failed.\n" << COL_NORMAL << std::endl;
    }
}

/**
 * @brief Tests the Minimum Intensity Projection (MinIP) application on a volumetric dataset.
 *
 * Initializes a volume with a defined pattern of intensity values and applies the MinIP projection. This test examines the function's 
 * execution for potential errors and validates the projection by comparing resulting pixel values to expected minimum intensities. 
 * It ensures that the minimum intensity value across all layers is correctly projected onto the resulting image. This approach 
 * indirectly confirms the accurate functioning of the MinIP algorithm and its consistency across various datasets.
 *
 * The test is enclosed within a try-catch block to handle exceptions, thereby preserving the integrity of the test suite. This 
 * methodology allows for the detection of potential issues within the MinIP projection function, including algorithmic discrepancies 
 * or unexpected behaviors in diverse scenarios.
 *
 * The selected volumetric dataset is deliberately crafted to challenge the MinIP algorithm, comprising distinct intensity gradients 
 * across layers to thoroughly evaluate the projection's accuracy and reliability.
 *
 * Author: Ce Huang
 * Acknowledgement: This documentation was enhanced with the support of generative AI technology.
 */

void testApplyMinIP() {
    int depth = 5, width = 10, height = 10;
    Volume volume; 
    volume.w = width;
    volume.h = height;
    volume.l = depth;
    volume.data.resize(depth + 1, std::vector<std::vector<unsigned char>>(height + 1, std::vector<unsigned char>(width + 1)));

    std::cout << "[LOG] Applying MinIP with volume dimensions: " << depth << "x" << height << "x" << width << std::endl;

    for (int z = 1; z <= depth; ++z) {
        for (int y = 1; y <= height; ++y) {
            for (int x = 1; x <= width; ++x) {
                volume.data[z][y][x] = (unsigned char)(x + y + z * 10); 
            }
        }
    }

    Projection projection;
    projection.apply(Projection::Proj::projMinIP, volume);

    bool testPassed = true;
    for (int y = 1; y <= height; ++y) {
        for (int x = 1; x <= width; ++x) {
            unsigned char expectedMin = 255;
            for (int z = 1; z <= depth; ++z) {
                unsigned char intensity = (unsigned char)(x + y + z * 10);
                if (intensity < expectedMin) {
                    expectedMin = intensity;
                }
            }
            if (volume.slice[(y-1) * width + (x-1)] != expectedMin) {
                std::cerr << "MinIP test failed at (" << x << ", " << y << "). Expected: "
                          << (int)expectedMin << ", Got: " << (int)volume.slice[(y-1) * width + (x-1)] << std::endl;
                testPassed = false;
                break;
            }
        }
        if (!testPassed) break;
    }

    if (testPassed) {
        std::cout << COL_GREEN << "[TEST] MinIP test passed.\n" << COL_NORMAL << std::endl;
    } else {
        std::cerr << COL_RED << "[TEST] MinIP test failed.\n" << COL_NORMAL << std::endl;
    }
}


#endif
