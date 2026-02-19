#ifndef TESTEDGEDETECTION_H
#define TESTEDGEDETECTION_H

#include "TestColour.h"
#include "stringColours.h"

/**
 * Jiawei Wang-jcw23
 * Performs a test to compare an image processed with the Sobel edge detection algorithm against a reference image.
 * 
 * This function is designed to validate the accuracy of a Sobel edge detection implementation by comparing its
 * output with a known correct result. The comparison uses the Mean Absolute Error (MAE) between the two images
 * to determine similarity. Paths to the test and the algorithm output images are hardcoded within the function.
 * A successful test (images similar within an acceptable threshold) results in a console message indicating
 * success; otherwise, a runtime error is thrown, indicating the test's failure.
 * 
 * Throws:
 *     std::runtime_error if the Sobel edge detection test fails, signaling that the algorithm's output does not
 *     closely match the expected outcome.
 */
void testSobel(){
    // Adjust these paths to point to your specific images
    std::string opencvImagePath = "tests/testimages/sobel_edge_detection.png";
    std::string algorithmImagePath = "../Output/7-edgedetection/sobel/gracehopper.png";

    bool result = testImageComparison(opencvImagePath, algorithmImagePath);

    if (result) {
        std::cout << COL_GREEN << "[TEST] Sobel test passed." << COL_NORMAL << std::endl;
    } else {
        throw std::runtime_error(COL_RED "[TEST] Sobel test failed." COL_NORMAL);
    }
    std::cout<<"\n";
}

/**
 * Jiawei Wang-jcw23
 * Performs a test to compare an image processed with the Prewitt edge detection algorithm against a reference image.
 * 
 * This function is designed to validate the accuracy of a Prewitt edge detection implementation by comparing its
 * output with a known correct result. The comparison uses the Mean Absolute Error (MAE) between the two images
 * to determine similarity. Paths to the test and the algorithm output images are hardcoded within the function.
 * A successful test (images similar within an acceptable threshold) results in a console message indicating
 * success; otherwise, a runtime error is thrown, indicating the test's failure.
 * 
 * Throws:
 *     std::runtime_error if the Prewitt edge detection test fails, signaling that the algorithm's output does not
 *     closely match the expected outcome.
 */
void testPrewitt(){
    // Adjust these paths to point to your specific images
    std::string opencvImagePath = "tests/testimages/prewitt_edge_detection.png";
    std::string algorithmImagePath = "../Output/7-edgedetection/prewitt/gracehopper.png";

    bool result = testImageComparison(opencvImagePath, algorithmImagePath);

    if (result) {
        std::cout << COL_GREEN << "[TEST] Prewitt test passed." << COL_NORMAL << std::endl;
    } else {
        throw std::runtime_error(std::string(COL_RED) + "[TEST] Prewitt test failed." + std::string(COL_NORMAL));
    }
    std::cout<<"\n";
}

/**
 * Jiawei Wang-jcw23
 * Performs a test to compare an image processed with the Scharr edge detection algorithm against a reference image.
 * 
 * This function is designed to validate the accuracy of a Scharr edge detection implementation by comparing its
 * output with a known correct result. The comparison uses the Mean Absolute Error (MAE) between the two images
 * to determine similarity. Paths to the test and the algorithm output images are hardcoded within the function.
 * A successful test (images similar within an acceptable threshold) results in a console message indicating
 * success; otherwise, a runtime error is thrown, indicating the test's failure.
 * 
 * Throws:
 *     std::runtime_error if the Scharr edge detection test fails, signaling that the algorithm's output does not
 *     closely match the expected outcome.
 */
void testScharr(){
    // Adjust these paths to point to your specific images
    std::string opencvImagePath = "tests/testimages/scharr_edge_detection.png";
    std::string algorithmImagePath = "../Output/7-edgedetection/scharr/gracehopper.png";

    bool result = testImageComparison(opencvImagePath, algorithmImagePath);

    if (result) {
        std::cout << COL_GREEN << "[TEST] Scharr test passed." << COL_NORMAL << std::endl;
    } else {
        throw std::runtime_error(COL_RED "[TEST] Scharr test failed." COL_NORMAL);
    }
    std::cout<<"\n";
}

/**
 * Jiawei Wang-jcw23
 * Performs a test to compare an image processed with the Roberts Cross edge detection algorithm against a reference image.
 * 
 * This function is designed to validate the accuracy of a Roberts Cross edge detection implementation by comparing its
 * output with a known correct result. The comparison uses the Mean Absolute Error (MAE) between the two images
 * to determine similarity. Paths to the test and the algorithm output images are hardcoded within the function.
 * A successful test (images similar within an acceptable threshold) results in a console message indicating
 * success; otherwise, a runtime error is thrown, indicating the test's failure.
 * 
 * Throws:
 *     std::runtime_error if the Roberts Cross edge detection test fails, signaling that the algorithm's output does not
 *     closely match the expected outcome.
 */
void testRobertsCross(){
    // Adjust these paths to point to your specific images
    std::string opencvImagePath = "tests/testimages/roberts_cross_edge_detection.png";
    std::string algorithmImagePath = "../Output/7-edgedetection/robertscross/gracehopper.png";

    bool result = testImageComparison(opencvImagePath, algorithmImagePath);

    if (result) {
        std::cout << COL_GREEN << "[TEST] RobertsCross test passed." << COL_NORMAL << std::endl;
    } else {
        throw std::runtime_error(COL_RED "[TEST] RobertsCross test failed." COL_NORMAL);
    }
    std::cout<<"\n";
}
#endif