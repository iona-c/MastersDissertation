#include <algorithm>
#include "EdgeDetection.h"
#include "ColourFilter.h"

/**
 * Applies edge detection to an image using the specified method.
 * This function first converts the input image to grayscale, then applies one of several
 * edge detection algorithms based on the `method` parameter. Supported edge detection methods include Sobel, Prewitt,
 * Scharr, and Roberts Cross, each having its own way of calculating the gradient of the image intensity to find edges.
 * The result is a new image where edges are highlighted, and the intensity of each pixel represents the edge magnitude.
 *
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 * @param method An enumeration value of type `type` that specifies the edge detection algorithm to apply.
 * The supported methods are `type::Sobel`, `type::Prewitt`, `type::Scharr`, and `type::RobertsCross`.
 *
 * Note: The function modifies the input image in-place, replacing its original data with the edge-detected output.
 * The output image will be in grayscale, regardless of the original image's color depth.
 * @author Jiawei Wang-jcw23
 */
void EdgeDetection::_EdgeDetect(Image& image, type method) {
    ColourFilter cc;
    cc.apply(cc.GrayScale, image); 
    int gx[3][3], gy[3][3];

    switch (method) {
        case type::Sobel:
            // Sobel Gx
            gx[0][0] = -1; gx[0][1] = 0; gx[0][2] = 1;
            gx[1][0] = -2; gx[1][1] = 0; gx[1][2] = 2;
            gx[2][0] = -1; gx[2][1] = 0; gx[2][2] = 1;
            // Sobel Gy
            gy[0][0] = -1; gy[0][1] = -2; gy[0][2] = -1;
            gy[1][0] = 0;  gy[1][1] = 0;  gy[1][2] = 0;
            gy[2][0] = 1;  gy[2][1] = 2;  gy[2][2] = 1;
            std::cout<<"[Log] Applying Sobel Edge detection"<< std::endl;
            break;
        
        case type::Prewitt:
            // Prewitt Gx
            gx[0][0] = -1; gx[0][1] = 0; gx[0][2] = 1;
            gx[1][0] = -1; gx[1][1] = 0; gx[1][2] = 1;
            gx[2][0] = -1; gx[2][1] = 0; gx[2][2] = 1;
            // Prewitt Gy
            gy[0][0] = -1; gy[0][1] = -1; gy[0][2] = -1;
            gy[1][0] = 0;  gy[1][1] = 0;  gy[1][2] = 0;
            gy[2][0] = 1;  gy[2][1] = 1;  gy[2][2] = 1;
            std::cout<<"[Log] Applying Prewitt Edge detection"<< std::endl;
            break;

        case type::Scharr:
            // Scharr Gx
            gx[0][0] = -3;  gx[0][1] = 0; gx[0][2] = 3;
            gx[1][0] = -10; gx[1][1] = 0; gx[1][2] = 10;
            gx[2][0] = -3;  gx[2][1] = 0; gx[2][2] = 3;

            // Scharr Gy
            gy[0][0] = -3; gy[0][1] = -10; gy[0][2] = -3;
            gy[1][0] = 0;  gy[1][1] = 0;   gy[1][2] = 0;
            gy[2][0] = 3;  gy[2][1] = 10;  gy[2][2] = 3;

            std::cout<<"[Log] Applying Scharr Edge detection"<< std::endl;
            break;
        
        case type::RobertsCross:
            // Roberts Cross Gx
            gx[0][0] = 1; gx[0][1] = 0;
            gx[1][0] = 0; gx[1][1] = -1;
            // Roberts Cross Gy
            gy[0][0] = 0; gy[0][1] = 1;
            gy[1][0] = -1; gy[1][1] = 0;
            std::cout<<"[Log] Applying Robers Cross Edge detection"<< std::endl;
            break;
    }

    int width = image.w;
    int height = image.h;
    int channels = image.c;
;
    std::unique_ptr<unsigned char[]> output(new unsigned char[width * height * channels]);
    std::fill(output.get(), output.get() + width * height * channels, 0);

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {

            float sumX = 0;
            float sumY = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int p = (y + ky) * width + (x + kx);
                    sumX += gx[ky + 1][kx + 1] * image.data[p];
                    sumY += gy[ky + 1][kx + 1] * image.data[p];
                }
            }
            if (method == type::Scharr) {
                sumX = std::max(0.0f, sumX);
                sumY = std::max(0.0f, sumY);
            }
            float sum = sqrt(sumX * sumX + sumY * sumY);
            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            for (int c = 0; c < channels; c++) {
                output[(y * width + x) * channels + c] = (unsigned char)sum;
            }
        }
    }
    if (method == type::RobertsCross) {
        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                // Calculate gradient magnitudes for Roberts' Cross
                float gx = (float)(image.data[(y * width + x) * channels] - image.data[((y + 1) * width + (x + 1)) * channels]);
                float gy = (float)(image.data[((y + 1) * width + x) * channels] - image.data[(y * width + (x + 1)) * channels]);
                float magnitude = sqrt(gx * gx + gy * gy);

                // Clamp and set the output pixel value
                int outputIndex = (y * width + x) * channels;
                unsigned char intensity = (unsigned char)(std::min(std::max((int)magnitude, 0), 255));
                for (int c = 0; c < channels; c++) {
                    output[outputIndex + c] = intensity;
                }
            }
        }
    
    }
    std::copy(output.get(), output.get() + width * height * channels, image.data);
}

void EdgeDetection::apply(Image& image, type method){
    _EdgeDetect(image, method);
}
