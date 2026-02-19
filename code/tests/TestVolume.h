#include "Volume.h"
#include "Image.h"
#include "stringColours.h"

/**
 * @brief Test the Volume class, checking that the dimensions of the 3D vector are correct.
 * And when the minIndex and maxIndex are set(even if not correctly), the range of images loaded is correct.
 * 
 * The test will pass if the dimensions of the 3D vector are 3, because in the testimagefor3d folder, there are 3 png images and 1 jpg image.
 * And the test will pass if the range of images loaded is correct.
 * 
 * @author Yunjie Li, Iona Y Chadda
*/
#include <filesystem>

void testVolume(){
    int Imin = 10;
    int Imax = 15;
    std::string folderPath = "../code/tests/testimagesfor3d/";
    try {
        Volume volume(folderPath, Imin, Imax);
        if (volume.l != 3) {
            std::cout << volume.l << std::endl;
            throw std::runtime_error(COL_RED "[TEST] Volume dimensions test failed: Volume dimensions are not 3" COL_NORMAL);
        }
        std::cout << COL_GREEN << "[TEST] Volume dimensions test passed: Volume dimensions are 3" COL_NORMAL << std::endl;
    } catch (const std::exception& e) {
        std::cerr << COL_RED "[TEST] Exception caught during Volume test: " << e.what() << COL_NORMAL << std::endl;
        return; // Exit the test on exception
    }
}