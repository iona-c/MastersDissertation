#include "test_main.h"

int main(){
    std::cout << COL_BLUE << "[TEST] Testing started..." << COL_NORMAL << std::endl;
    std::cout << COL_MAGENTA << "[TEST] Testing colour..." << COL_NORMAL << std::endl;
    testGrayScale();

    std::cout << COL_MAGENTA << "[TEST] Testing salt and pepper filter..." << COL_NORMAL << std::endl;
    testSaltNpepperFilter();
    
    std::cout << COL_MAGENTA << "[TEST] Testing blur..." << COL_NORMAL << std::endl;
    testApplyMedianBlurMultiChannel();
    testApplyBoxBlur();
    testApplyGaussianBlur();

    std::cout << COL_MAGENTA << "[TEST] Testing edge detection..." << COL_NORMAL << std::endl;
    testSobel();
    testPrewitt();
    testScharr();
    testRobertsCross();

    std::cout << COL_MAGENTA << "[TEST] Testing projection..." << COL_NORMAL << std::endl;
    testApplyMIP();
    testApplyAIP();
    testApplyMinIP();

    std::cout << COL_MAGENTA << "[TEST] Testing slicing..." << COL_NORMAL << std::endl;
    testSlice();



    std::cout << COL_BLUE << "[TEST] Testing Completed" << COL_NORMAL << std::endl;

}
