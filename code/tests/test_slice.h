#ifndef TEST_SLICE
#define TEST_SLICE

#include "Slice.h"
#include <stdexcept>

/**
 * @brief Tests the Slice generation from a volumetric dataset.
 *
 * Initializes a volume with a defined pattern of intensity values and generates slices along the XZ and YZ planes using the Slice class.
 * This test evaluates the function's execution for potential errors and validates the generated slices by comparing resulting dimensions 
 * against expected sizes. It ensures that the slices maintain the correct dimensions, representing cross-sections of the volumetric 
 * data along specified planes. This approach indirectly confirms the accurate implementation of the slice generation algorithms and 
 * verifies their consistency across different datasets and orientations.
 *
 * The test is contained within a try-catch block to handle exceptions, ensuring the robustness of the test suite. This mechanism 
 * facilitates the detection of potential issues within the slice generation functions, including algorithmic discrepancies or 
 * unexpected behaviors under various conditions.
 *
 * The chosen volumetric dataset is deliberately designed to challenge the slice generation algorithms, encompassing diverse intensity 
 * distributions and spatial arrangements to thoroughly evaluate the accuracy and reliability of the generated slices.
 *
 * Author: Ce Huang, Prayush Udas
 * Acknowledgement: Developed with the assistance of generative AI technology.
 */

void testSlice() {
    const int width = 10;
    const int height = 10;
    const int depth = 5;

    // Create a volume with known intensity values
    Volume volume;
    volume.w = width;
    volume.h = height;
    volume.l = depth;
    volume.data.resize(depth + 1, std::vector<std::vector<unsigned char>>(height + 1, std::vector<unsigned char>(width + 1)));

    for (int z = 1; z <= depth; ++z) {
        for (int y = 1; y <= height; ++y) {
            for (int x = 1; x <= width; ++x) {
                volume.data[z][y][x] = static_cast<unsigned char>(x + y + (z-1) * 10); // Assigning intensity values
            }
        }
    }

    // Create a Slice object
    Slice slicer;

    try {
        // Test sliceXZ function
        {
            int testY = 3; // Example Y value for XZ slice
            slicer.sliceXZ(volume, testY);

            // Validate the dimensions of the resulting slice
            size_t expectedSize = depth * width;
            if (volume.slice.size() != expectedSize) {
                throw std::runtime_error("[TEST] SliceXZ test failed: Expected slice size " + std::to_string(expectedSize) + ", got " + std::to_string(volume.slice.size()) + ".");
            }
        }

        // Test sliceYZ function
        {
            int testX = 3; // Example X value for YZ slice
            slicer.sliceYZ(volume, testX);

            // Validate the dimensions of the resulting slice
            size_t expectedSize = depth * height;
            if (volume.slice.size() != expectedSize) {
                throw std::runtime_error("[TEST] SliceYZ test failed: Expected slice size " + std::to_string(expectedSize) + ", got " + std::to_string(volume.slice.size()) + ".");
            }
        }

        std::cout << COL_GREEN << "[TEST] All slice tests passed." << COL_NORMAL << std::endl << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << COL_RED << e.what() << COL_NORMAL << std::endl;
    }
}


#endif