#ifndef EDGE
#define EDGE
#include "Filter.h"

/**
 * The EdgeDetection class is a specialized form of Filter that implements various edge detection algorithms.
 * Edge detection is a fundamental tool in image processing and computer vision, used to identify points in
 * an image where the brightness changes sharply or has discontinuities. This class supports several common
 * edge detection methods, including Sobel, Prewitt, Scharr, and Roberts Cross.
 *
 * Enums:
 *   type: Defines the types of edge detection algorithms available.
 *     - Sobel: Represents the Sobel edge detection operator.
 *     - Prewitt: Represents the Prewitt edge detection operator.
 *     - Scharr: Represents the Scharr edge detection operator.
 *     - RobertsCross: Represents the Roberts Cross edge detection operator.
 *
 * Public Method:
 *   void apply(Image& image, type method):
 *     Applies the specified edge detection algorithm to the given image.
 *     @param image A reference to the Image object on which the edge detection will be applied.
 *     @param method The edge detection method to apply, specified as a value from the EdgeDetection::type enum.
 *
 * Private Method:
 *   void _EdgeDetect(Image& image, type method):
 *     A helper method that performs the actual edge detection algorithm on the image based on the specified method.
 *     @param image A reference to the Image object on which the edge detection is performed.
 *     @param method The edge detection method to apply, specified as a value from the EdgeDetection::type enum.
 *
 *   void apply():
 *     An overridden method from the Filter base class, made private to prevent its direct invocation without specifying
 *     the edge detection type.
 * @author Prayush Udas
 */

class EdgeDetection : public Filter{
    public:
        enum type {
            Sobel,
            Prewitt,
            Scharr,
            RobertsCross
        };
        void apply(Image& image, type method);
    private:
        void _EdgeDetect(Image& image, type method);
        void apply(){};
};

#endif