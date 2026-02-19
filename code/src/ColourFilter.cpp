#include "ColourFilter.h"

/**
 * Applies a specified color filter to an image.
 * This function supports various color filters, but currently, it is implemented to apply only the grayscale filter. 
 * When the grayscale filter is selected, it transforms the input image into grayscale by calling the `GrayscaleFilter` method
 * on the `image`. The transformation is applied in-place, altering the original image data.
 *
 * @param filter An enumeration value of type `cf` that specifies the color filter to apply. The supported filters include 
 * `cf::GrayScale`. Future versions may include additional filters.
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 *
 * Note: The function modifies the input image in-place, replacing its original data with the filtered output.
 * Depending on the applied filter, the output image's color properties may change (e.g., to grayscale).
 */
void ColourFilter::apply(cf filter, Image& image){
    switch (filter){ 
        case cf::GrayScale:
            GrayscaleFilter(image);
            std::cout<<"[LOG] Applying Grayscale"<<std::endl;
        case cf::histHSL:
            histogramEqHSL(image);
            std::cout<<"[LOG] Applying Histogram Equaliser using HSL"<< std::endl;
            break;
        case cf::histHSV:
            histogramEqHSV(image);
            std::cout<<"[LOG] Applying Histogram Equaliser using HSV" << std::endl;
        case cf::histGREY:
            histogramEqGrayscale(image);
            std::cout<<"[LOG] Applying Histogram Equaliser using Grayscale" << std::endl;
        default:
            std::cout<< "[ERROR] Please apply correct filter for ColourFilter"<< std::endl;
            break;
    }
}

/**
 * Applies a brightness adjustment to an image based on a specified mode.
 * This function allows for the adjustment of the image's brightness. Currently, it supports a single filter type
 * for brightness adjustment. The actual adjustment is carried out by the `AdjustBrightness` method, which modifies
 * the brightness of the input image according to the provided `mode`.
 *
 * @param filter An enumeration value of type `cf` that specifies the color filter to apply. Currently, only
 * `cf::Brightness` is supported, which triggers a brightness adjustment on the image.
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 * @param mode A `std::string` specifying the mode of brightness adjustment. The interpretation of this parameter
 * is dependent on the implementation of the `AdjustBrightness` method. It could, for example, indicate the direction
 * and magnitude of the brightness change (e.g., "increase" or "decrease").
 *
 * Note: The function modifies the input image in-place, adjusting its brightness according to the specified mode.
 * The exact behavior and supported modes are determined by the implementation of `AdjustBrightness`.
 */
void ColourFilter::apply(cf filter, Image& image, std::string mode){
    switch (filter){
        case cf::Brightness:
            BrightnessFilter(image, mode);
            std::cout<<"[Log] Applying Brightness"<<std::endl;
            break;
        default:
            std::cout<< "[ERROR] Please apply correct filter for ColourFilter"<< std::endl;
            break;
    }
}

/**
 * Applies a specified color filter to an image, with additional parameters based on the filter type.
 * This implementation supports the salt-and-pepper noise filter, which adds noise to the image by randomly
 * setting pixels to black or white, simulating the salt and pepper effect. The intensity of this effect is
 * controlled by the `noisePercentage` parameter, which dictates the percentage of the image pixels that will
 * be affected by the noise.
 *
 * @param filter An enumeration value of type `cf` that specifies the color filter to apply. The supported filter 
 * in this implementation is `cf::saltNpepper`, which applies salt-and-pepper noise to the image.
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 * @param noisePercentage A double value that represents the percentage of the image pixels to be affected by
 * the salt-and-pepper noise. The value should be between 0 and 100, where 0 means no noise, and 100 means
 * that all pixels will be affected.
 *
 * Note: The function modifies the input image in-place, replacing its original data with the noise-filtered output.
 * The resulting image will have randomly distributed black and white pixels in proportion to the specified noise percentage.
 */
void ColourFilter::apply(cf filter, Image& image, double noisePercentage){
    switch (filter){
        case cf::saltNpepper:
            saltNpepperFilter(image, noisePercentage);
            std::cout<<"[LOG] Applying Salt N Pepper filter" << std::endl;
            break;
        default:
            std::cout<< "[ERROR] Please apply correct filter for ColourFilter"<< std::endl;
            break;
    }
}

/**
 * Applies a specified color filter to an image, with a threshold parameter that modifies the filter effect.
 * This function supports various filters, including HSL and HSV thresholding, grayscale thresholding, and 
 * salt-and-pepper noise, each utilizing the `threshold` parameter differently based on the filter type.
 *
 * - For `thHSL` and `thHSV`, the threshold determines the cutoff value for the hue, saturation, or lightness/value
 *   components, converting the image into a binary representation based on the threshold criteria.
 * - For `thGREY`, it applies a grayscale thresholding, where pixels are turned either black or white depending on
 *   their brightness relative to the threshold value.
 * - The `saltNpepper` filter interprets the threshold as the percentage of pixels to affect, similar to its usage
 *   with a double parameter in other methods.
 *
 * @param filter An enumeration value of type `cf` that specifies the color filter to apply. Supported filters include
 * `cf::thHSL`, `cf::thHSV`, `cf::thGREY`, and `cf::saltNpepper`. Incorrect filter usage prompts a message advising to
 * use a correct filter.
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 * @param threshold An integer value used as a threshold for the filter effect. Its interpretation varies with the
 * filter being applied. For thresholding operations, it determines the cutoff level; for noise filters, it indicates
 * the percentage of pixels to be affected.
 *
 * Note: The function modifies the input image in-place, replacing its original data with the filtered output. The
 * effect of the filter, and consequently the appearance of the resulting image, depends on the chosen filter and
 * the threshold value.
 */
void ColourFilter::apply(cf filter, Image& image, int threshold){
    switch (filter){
        case cf::thHSL:
            thresholdHSL(image, threshold);
            std::cout<<"[LOG] Performing Threshold HSL" << std::endl;
            break;
        case cf::thHSV:
            thresholdHSV(image, threshold);
            std::cout<<"[LOG] Performing Threshold HSV" << std::endl;
            break;
        case cf::thGREY:
            thresholdGREY(image, threshold);
            std::cout<< "[LOG] Performing Threshold GREY" << std::endl;
            break;
        case cf::saltNpepper:
            saltNpepperFilter(image, threshold);
            std::cout<<"[LOG] Applying Salt N Pepper filter"<<std::endl;
        case cf::Brightness:
        case cf::GrayScale: 
        default:
            std::cout<< "[ERROR] Please apply correct filter for ColourFilter"<< std::endl;
            break;
    }
}

/**
 * Applies "salt and pepper" noise to an image at a specified noise percentage. This function iterates over each pixel in the image and, based on the noise percentage, randomly determines whether to convert the pixel to either pure black ("pepper") or pure white ("salt"). This effect is applied uniformly across all channels of a pixel, affecting the image in place.
 *
 * Parameters:
 * @param image A reference to an Image object that will be modified in-place. The image should have its width (`w`),
 * height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`) is stored in a flat,
 * linear array in row-major order, with each pixel consisting of `c` consecutive values (channels).
 * @param noisePercentage A double value representing the percentage of the image's total pixels that will be modified
 * to create the noise effect. This value should be between 0.0 (no noise) and 100.0 (maximum noise), and it determines
 * the density of the salt and pepper noise.
 *
 * Preconditions:
 * - The `image.data` array must be allocated and filled with the initial pixel data.
 * - The `noisePercentage` should be a valid double value between 0 and 100 (inclusive).
 *
 * Postconditions:
 * - The `image.data` array will be modified in place, with a certain percentage of its pixels turned to black or white, as per the specified `noisePercentage`.
 * - The original pixel data will be overwritten where noise is applied.
 *
 * Side Effects:
 * - The function seeds the random number generator (`std::srand`) at the beginning of each call, which affects the state of the global random number generator used by `std::rand`.
 * - Due to the random nature of the noise application, multiple calls to this function with the same `noisePercentage` will likely result in different noise patterns.
 * 
 * @author: Prayush Udas
 * @acknowledgement: This documentation was enhanced with the support of generative AI technology.
 */
void ColourFilter::saltNpepperFilter(Image& image, double noisePercentage) {
    int totalPixels = image.w * image.h;
    int channels = image.c;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Apply noise to each pixel in the image
    for (int i = 0; i < totalPixels; ++i) { // Loop through each pixel
        // Randomly decide if the current pixel should be noisy, based on the noise percentage
        if (std::rand() % 100 < noisePercentage) {
            int dataIndex = i * channels;

            // Decide whether to make it 'salt' (white) or 'pepper' (black)
            bool isSalt = std::rand() % 2 == 0;
            unsigned char colorValue = isSalt ? 255 : 0;

            // Apply the noise color to all channels of the pixel
            for (int ch = 0; ch < channels; ++ch) {
                image.data[dataIndex + ch] = colorValue;
            }
        }
    }
}




/**
 * Jiawei Wang-jcw23
 * Applies a grayscale filter to an RGB or RGBA image in place.
 *
 * This function converts an RGB or RGBA image to grayscale using the luminance
 * method, where the grayscale value Y is calculated as a weighted sum of the
 * red, green, and blue components of each pixel acfording to the formula
 * Y = 0.2126R + 0.7152G + 0.0722B. The weights reflect the human eye's
 * sensitivity to these colors. This method ensures a perceptually acfurate
 * grayscale representation of the original image.
 *
 * @param image a pointer to the image data. This array contains the image pixels
 *             as a flat sequence of channel values. For an RGB image, the channel
 *             values are in the order R, G, B, and optionally A for RGBA images.
 *
 * Note: This function modifies the image data in place. Therefore, the original
 * image data will be altered after this function is called.
 */

// Function to apply grayscale filter
void ColourFilter::GrayscaleFilter(Image& image) {
    int channels = image.c; 
    int height = image.h;
    int width = image.w;
    if(channels < 3) {
        std::cerr << "[ERROR] Image does not have enough channels (RGB) to convert to grayscale." << std::endl;
        return;
    }

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int i = (y * width + x) * channels;
            unsigned char r = image.data[i];
            unsigned char g = image.data[i + 1];
            unsigned char b = image.data[i + 2];
            unsigned char grayscale = static_cast<unsigned char>(0.2126 * r + 0.7152 * g + 0.0722 * b);
            image.data[y * width + x] = grayscale;
            image.data[i + 1] = grayscale;
            if(channels == 4) { // Preserve the alpha channel if present
                image.data[i + 3] = image.data[i + 3];
            }
        }
    }
    image.c = 1;
}

/**
 * Adjusts the brightness of an image based on a specified mode.
 * This function supports two modes of brightness adjustment: "auto" and "manual". Before any adjustments are made,
 * the input mode string is transformed to lowercase to ensure case-insensitive comparison. The "auto" mode triggers
 * an automatic brightness adjustment process, where the image's brightness is adjusted based on a predefined algorithm
 * or target average brightness level. The "manual" mode allows for custom brightness adjustments, potentially requiring
 * user input or specific parameters not detailed in this snippet. If an invalid mode is specified, an error message is
 * logged, indicating the unsupported mode selection.
 *
 * @param image A reference to an Image object that will be modified in-place. It is expected that the Image object
 * has its dimensions (width, height) and color channel information properly initialized and contains the pixel data
 * in a format that the subsequent brightness adjustment functions can process.
 * @param mode A string specifying the mode of brightness adjustment. This should be either "auto" for automatic
 * adjustments or "manual" for adjustments that might require additional parameters or user input. The comparison
 * is case-insensitive.
 *
 * Note: The actual adjustment logic is performed in the `_AutoAdjustBrightness` or `_ManualAdjustBrightness` functions,
 * which need to be correctly implemented and accessible within this class. This function primarily acts as a dispatcher
 * based on the selected mode. Error handling for invalid mode input is included, but detailed implementation of the
 * adjustment algorithms is assumed to be encapsulated within the called functions.
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void ColourFilter::BrightnessFilter(Image& image, std::string mode){
    // transform mode to lowercase
    std::transform(mode.begin(), mode.end(), mode.begin(),
                [](unsigned char c) -> unsigned char { return std::tolower(c); });
        
    if (mode == "auto" || mode == "manual") {
        if (mode == "auto") {
            std::cout << "[LOG] Auto mode selected!" << std::endl;
            _AutoAdjustBrightness(image);
        } else if (mode == "manual") {
            std::cout << "[LOG] Manual mode selected!" << std::endl;
            _ManualAdjustBrightness(image);
        }
    } else {
        std::cerr << "[ERROR] Invalid mode for Brightness. Please enter auto or manual!" << std::endl;
    }
}


/**
 * Automatically adjusts the brightness of an image to reach a desired average pixel value.
 * This function computes the current average pixel value of the image and compares it to the
 * specified target average. It then adjusts all pixels in the image to decrease or increase
 * the overall brightness accordingly, ensuring that the modified pixel values remain within
 * the valid range (0-255).
 *
 * @param image A reference to an Image object that will be modified in-place. The image should have its
 * width (`w`), height (`h`), and channel count (`c`) properly set. It is assumed that the image data (`data`)
 * is stored in a flat, linear array in row-major order, with each element representing a pixel's intensity
 * value (for grayscale images) or channel intensity (for color images).
 * @param average An integer specifying the desired average pixel value for the adjusted image. This value
 * should be between 0 and 255, inclusive. The function will modify the brightness of the image to attempt
 * to reach this average value.
 *
 * Note: The function modifies the input image in-place, adjusting the brightness of each pixel. The
 * adjustment is performed uniformly across the entire image based on the difference between the current
 * and desired average pixel values. The result is an image with altered brightness but preserved
 * contrast and color balance (for color images).
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void ColourFilter::_AutoAdjustBrightness(Image& image, int average) {
    int totalValue = 0;
    int pixelCount = 0;
    if (image.c == 4){
        pixelCount = image.w * image.h * 3;
        for (int i = 0; i < image.w * image.h * image.c; i+=4) {
            for (int j = 0; j < 3; j++) {
                totalValue += image.data[i + j];
            }
        }
    } else {
        pixelCount = image.w * image.h * image.c;
        for (int i = 0; i < image.w * image.h * image.c; i++) {
            totalValue += image.data[i];
        }
    }

    int averageValue = totalValue / pixelCount;
    int difference = average - averageValue; // difference between the average value and the desired average value

    // Adjust brightness
    if (image.c == 4){
        pixelCount = image.w * image.h * 3;
        for (int i = 0; i < image.w * image.h * image.c; i+=4) {
            for (int j = 0; j < 3; j++) {
                int value = image.data[i+j] + difference;
                image.data[i+j] = std::min(255, std::max(0, value));
            }
        }
    } else {
        pixelCount = image.w * image.h * image.c;
        for (int i = 0; i < image.w * image.h * image.c; i++) {
            int value = image.data[i] + difference;
            image.data[i] = std::min(255, std::max(0, value));
        }
    }
}

/**
 * Manually adjusts the brightness of an image based on user input.
 * This function prompts the user to specify how much they would like to adjust the brightness of the image.
 * The brightness adjustment can be any integer value between -255 and 255, inclusive. Positive values increase
 * the brightness, while negative values decrease it. The function ensures that the adjusted brightness values
 * of pixels remain within the valid range of 0 to 255. If the user inputs an invalid value (non-integer or outside
 * the allowed range), they are prompted to enter a new value until a valid one is provided.
 *
 * @param image A reference to an Image object that will be modified in-place. The image object must have its
 * dimensions (width, height) and color channels (c) properly initialized, and contain the pixel data in a format
 * that allows direct manipulation. Each pixel's color information is assumed to be stored sequentially in the
 * `image.data` array.
 *
 * Note: This function directly interacts with the user through standard input and output streams (`std::cin` and
 * `std::cout`). It repeatedly prompts the user until a valid brightness adjustment value is entered. The brightness
 * adjustment is applied uniformly to all pixels in the image. Since the function uses recursive calls to handle
 * invalid input, excessively invalid attempts might lead to a stack overflow. However, for typical use cases, this
 * behavior facilitates user correction of input errors. 
 * @author Yunjie Li
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
void ColourFilter::_ManualAdjustBrightness(Image& image) {
    std::cout << "\n==>Enter brightness number?" << std::endl;
     
    int brightness;
    std::cin >> brightness;
    
    // Check if input is valid
    if (std::cin.fail()) {
        std::cout << "[ERROR] Invalid input! Please enter a number!" << std::endl;
        std::cin.clear(); // clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the input
        _ManualAdjustBrightness(image);
        return;
    } else if (brightness < -255 || brightness > 255) {
        std::cout << "[ERROR] Invalid input! Please enter a number between -255 and 255!" << std::endl;
        _ManualAdjustBrightness(image);
        return;
    }
    
    // Adjust brightness
    if (image.c == 4){
        for (int i = 0; i < image.w * image.h * image.c; i+=4) {
            for (int j = 0; j < 3; j++) {
                int value = image.data[i+j] + brightness;
                image.data[i+j] = std::min(255, std::max(0, value));
            }
        }
    } else {
        for (int i = 0; i < image.w * image.h * image.c; i++) {
            int value = image.data[i] + brightness;
            image.data[i] = std::min(255, std::max(0, value));
        }
    }
}

/**
 * Converts an RGB color value to its HSV (Hue, Saturation, Value) representation.
 * This function takes the RGB color components as inputs and calculates their HSV equivalents based on the
 * color's hue, saturation, and value. The conversion algorithm accounts for the different cases where the
 * maximum RGB component is either red, green, or blue, adjusting the hue (H) accordingly. Saturation (S) and
 * value (V) are computed based on the maximum and minimum RGB components.
 *
 * @param fR Reference to the red component of the RGB color, in the range [0, 1].
 * @param fG Reference to the green component of the RGB color, in the range [0, 1].
 * @param fB The blue component of the RGB color, in the range [0, 1].
 * @param fH Reference to the hue component of the HSV color, in degrees [0, 360].
 * @param fS Reference to the saturation component of the HSV color, in the range [0, 1].
 * @param fV Reference to the value component of the HSV color, in the range [0, 1].
 *
 * Note: The function modifies the hue (fH), saturation (fS), and value (fV) parameters in place. It assumes
 * that the RGB inputs are in the range [0, 1]. If the hue calculation results in a negative value, it is
 * adjusted by adding 360 to ensure that the hue lies within the [0, 360] range. This function provides
 * a direct way to convert from RGB to HSV color space, facilitating tasks that require color manipulation
 * or analysis in the HSV space.
 */
void HSV::RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
    float fCMax = std::max({fR, fG, fB});
    float fCMin = std::min({fR, fG, fB});
    float fDelta = fCMax - fCMin;
  
  if(fDelta > 0) {
    if(fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if(fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if(fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }
    
    if(fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }
    
    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }
  
  if(fH < 0) {
    fH = 360 + fH;
  }}

/**
 * Converts an HSV (Hue, Saturation, Value) color value to its RGB (Red, Green, Blue) representation.
 * This function performs the conversion by first calculating the chroma (c), a secondary component (x),
 * and a match value (m) based on the input HSV values. These are then used to compute the RGB components
 * in a piecewise fashion, depending on the hue value's position within various ranges of the color wheel.
 *
 * @param h The hue component of the HSV color, in degrees [0, 360).
 * @param s The saturation component of the HSV color, in the range [0, 1].
 * @param v The value component of the HSV color, in the range [0, 1].
 * @param r Reference to the red component of the RGB color, which will be set by this function, in the range [0, 1].
 * @param g Reference to the green component of the RGB color, which will be set by this function, in the range [0, 1].
 * @param b Reference to the blue component of the RGB color, which will be set by this function, in the range [0, 1].
 *
 * Note: The function directly modifies the r, g, and b parameters to represent the RGB color equivalent of the
 * input HSV values. It accounts for the full circle of hue values by dividing the circle into six regions and
 * calculating the RGB components accordingly. The conversion algorithm ensures that the resulting RGB values
 * are within the correct range [0, 1], suitable for various applications requiring color space conversions.
 */
void HSV::HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
    float c = v * s;
    float x = c * (1 - std::fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;
    float rs, gs, bs;

    if(h >= 0 && h < 60) {
        rs = c; gs = x; bs = 0;
    } else if(h >= 60 && h < 120) {
        rs = x; gs = c; bs = 0;
    } else if(h >= 120 && h < 180) {
        rs = 0; gs = c; bs = x;
    } else if(h >= 180 && h < 240) {
        rs = 0; gs = x; bs = c;
    } else if(h >= 240 && h < 300) {
        rs = x; gs = 0; bs = c;
    } else {
        rs = c; gs = 0; bs = x;
    }

    r = (rs + m);
    g = (gs + m);
    b = (bs + m);
}

/**
 * Converts an RGB color value to its HSL (Hue, Saturation, Lightness) representation.
 * This function takes the RGB color components as inputs and calculates their HSL equivalents. The conversion
 * process involves finding the maximum and minimum values among the RGB components to determine the lightness
 * and the range between these values to calculate the hue and saturation. Hue is calculated differently depending
 * on which RGB component is the maximum. Lightness is the average of the maximum and minimum RGB values. Saturation
 * calculation takes into account the lightness and the range of RGB values.
 *
 * @param fR Reference to the red component of the RGB color, in the range [0, 1].
 * @param fG Reference to the green component of the RGB color, in the range [0, 1].
 * @param fB Reference to the blue component of the RGB color, in the range [0, 1].
 * @param fH Reference to the hue component of the HSL color, in degrees [0, 360].
 * @param fS Reference to the saturation component of the HSL color, in the range [0, 1].
 * @param fL Reference to the lightness component of the HSL color, in the range [0, 1].
 *
 * Note: The function modifies the hue (fH), saturation (fS), and lightness (fL) parameters in place. The RGB inputs
 * are expected to be in the range [0, 1]. The hue component is adjusted to ensure it remains within the [0, 360] range.
 * This function provides a direct way to convert from RGB to HSL color space, which can be useful for color manipulation
 * or analysis tasks that benefit from the perceptual qualities of the HSL representation.
 */
void HSL::RGBtoHSL(float& fR, float& fG, float& fB, float& fH, float& fS, float& fL) {
    float fCMax = std::max({fR, fG, fB});
    float fCMin = std::min({fR, fG, fB});
    float fDelta = fCMax - fCMin;

    // Calculate Hue
    if(fDelta == 0) {
        fH = 0;
    } else if(fCMax == fR) {
        fH = 60 * fmod(((fG - fB) / fDelta), 6);
        if(fH < 0) {
            fH += 360;
        }
    } else if(fCMax == fG) {
        fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if(fCMax == fB) {
        fH = 60 * (((fR - fG) / fDelta) + 4);
    }

    // Calculate Lightness
    fL = (fCMax + fCMin) / 2;

    // Calculate Saturation
    if(fDelta == 0) {
        fS = 0;
    } else {
        fS = fDelta / (1 - std::abs(2 * fL - 1));
    }
}

/**
 * Converts an HSL (Hue, Saturation, Lightness) color value to its RGB (Red, Green, Blue) representation.
 * This function calculates the RGB components based on the input HSL values using the conversion algorithm
 * that takes into account the chroma (C), the secondary component (X), and a match value (M) to adjust
 * the lightness. The hue component (H) is first normalized to a range of 0 to 6 to simplify the calculation
 * of RGB components. Depending on the segment of the hue (H'), different formulas are applied to calculate
 * the preliminary RGB values (before adjusting for lightness).
 *
 * @param fH The hue component of the HSL color, in degrees [0, 360).
 * @param fS The saturation component of the HSL color, in the range [0, 1].
 * @param fL The lightness component of the HSL color, in the range [0, 1].
 * @param fR Reference to the red component of the RGB color, which will be set by this function, in the range [0, 1].
 * @param fG Reference to the green component of the RGB color, which will be set by this function, in the range [0, 1].
 * @param fB Reference to the blue component of the RGB color, which will be set by this function, in the range [0, 1].
 *
 * Note: The function modifies the fR, fG, and fB parameters to represent the RGB color equivalent of the input HSL
 * values. It employs a piecewise method to ensure the RGB values are calculated accurately across the different hue
 * ranges. The conversion ensures that the resulting RGB values are within the correct range [0, 1], suitable for
 * various applications requiring color space conversions. The adjustment for lightness is applied at the end of the
 * conversion process, ensuring that the final RGB values reflect both the hue and lightness levels specified by the
 * input HSL values.
 */
void HSL::HSLtoRGB(float fH, float fS, float fL, float& fR, float& fG, float& fB) {
    float fC = (1 - std::fabs(2 * fL - 1)) * fS;
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - std::fabs(fmod(fHPrime, 2) - 1));
    float fM = fL - fC / 2;
    
    if(0 <= fHPrime && fHPrime < 1) {
        fR = fC;
        fG = fX;
        fB = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        fR = fX;
        fG = fC;
        fB = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        fR = 0;
        fG = fC;
        fB = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        fR = 0;
        fG = fX;
        fB = fC;
    } else if(4 <= fHPrime && fHPrime < 5) {
        fR = fX;
        fG = 0;
        fB = fC;
    } else if(5 <= fHPrime && fHPrime < 6) {
        fR = fC;
        fG = 0;
        fB = fX;
    } else {
        fR = 0;
        fG = 0;
        fB = 0;
    }
    
    // Adjust the lightness
    fR += fM;
    fG += fM;
    fB += fM;
}

/**
 * Applies a binary threshold filter to a grayscale image.
 * This function converts the input image into a binary image based on a specified threshold. Each pixel in the
 * image is compared against the threshold: pixels with a grayscale intensity above the threshold are set to white,
 * while those below are set to black. This creates a stark contrast between areas of the image, emphasizing the
 * boundary between lighter and darker regions. The function is specifically designed for images with a single
 * channel (grayscale), and the threshold is applied uniformly across the entire image.
 *
 * @param image A reference to an Image object representing the grayscale image to be processed. The image should
 * have its width (`w`), height (`h`), and channel count (`c`) properly set. It is expected that `c` is 1 for
 * grayscale images. The pixel data (`data`) should be stored in a flat, linear array in row-major order.
 * @param threshold An integer specifying the threshold value in the range [0, 255]. Pixels with a grayscale
 * intensity greater than this threshold will be set to white (255), and those below will be set to black (0).
 *
 * Note: The function modifies the input image in-place, replacing its original pixel data with the thresholded
 * binary output. The intended use is for grayscale images, and behavior for images with more than one color
 * channel (e.g., RGB images) may not be as expected, as it only considers the first channel of the image data.
 */
void ColourFilter::thresholdGREY(Image& image, int threshold) {
    float thresh =threshold/255.0f;
    for (int i = 0; i < image.w * image.h; i++) {
        float grey = image.data[i * image.c] / 255.0f;
        float newgrey;

        if (grey > thresh) {
            newgrey = 1.0;
        } else {
            newgrey = 0.0;
        }
        image.data[i * image.c] = static_cast<unsigned char>(newgrey * 255);
    }
}

/**
 * Applies a threshold filter based on the HSV (Hue, Saturation, Value) color space to an image.
 * This function converts each pixel from RGB to HSV, then applies a threshold to the value (brightness)
 * component. If the value component of a pixel exceeds the specified threshold, it is set to maximum brightness;
 * otherwise, it is set to minimum brightness. The result is an image with high-contrast regions based on the
 * original brightness levels, emphasizing areas above or below the given brightness threshold.
 *
 * @param image A reference to an Image object that will be modified in-place. The image object should have its
 * dimensions (width, height) and channel count (c) properly set. It is assumed that the image data (`data`) is
 * stored in a flat array, with pixel color components laid out sequentially in RGB order.
 * @param threshold An integer value specifying the threshold for the value (brightness) component in the HSV color space.
 * The threshold should be in the same scale as the RGB values (0-255), and it is internally normalized to [0, 1] for HSV
 * processing.
 *
 * Note: The function assumes that the input image is in RGB format and that each color channel is represented by
 * an 8-bit unsigned integer. The HSV conversion is used to isolate the brightness component of each pixel to apply
 * the thresholding operation, after which the pixels are converted back to RGB format. This method allows for selective
 * enhancement or suppression of details based on brightness, useful in various image processing applications.
 */
void ColourFilter::thresholdHSV(Image& image, int threshold){
    HSV hsv;
    float thresh =threshold/255.0f;
    for (int i = 0; i < image.w * image.h; i++) {
        float red = image.data[i * image.c] / 255.0f;
        float green = image.data[i*image.c+1]/255.0f;
        float blue =image.data[i*image.c+2] /255.0f;
        
        float h, s, v;
        hsv.RGBtoHSV(red, green, blue, h, s, v);
        float newV;
        if(v>thresh){
            newV = 1.0;
        }
        else{ newV=0.0;}
    float newR, newG, newB;
    hsv.HSVtoRGB(h,s,newV,newR,newG,newB);
    image.data[i*image.c] =static_cast<unsigned char>(newR * 255);
    image.data[i*image.c+1]=static_cast<unsigned char>(newG * 255);
    image.data[i*image.c+2]=static_cast<unsigned char>(newB * 255);
    }
}

/**
 * Applies a threshold filter based on the HSL (Hue, Saturation, Lightness) color space to an image.
 * This function converts each pixel from RGB to HSL, then applies a threshold to the lightness component.
 * If the lightness component of a pixel exceeds the specified threshold, it is set to maximum lightness
 * (white); otherwise, it is set to minimum lightness (black). This creates a high-contrast image that highlights
 * areas above the lightness threshold while darkening others, effectively segmenting the image based on lightness.
 *
 * @param image A reference to an Image object that will be modified in-place. The image should have its
 * dimensions (width and height) and color channels (c) properly initialized. Pixel data in the image is expected
 * to be stored in a flat array with RGB components for each pixel laid out sequentially.
 * @param threshold An integer specifying the threshold for the lightness component in the HSL color space.
 * The value should be within the typical RGB range of 0 to 255, and is internally normalized to [0, 1] for
 * processing in HSL space.
 *
 * Note: The function assumes the image is in RGB format and that each color component is represented by an
 * 8-bit unsigned integer. The thresholding operation based on lightness allows for isolating brighter or darker
 * parts of the image, which can be useful for various image processing tasks such as segmentation, highlighting
 * features, or preparing images for further analysis.
 */
void ColourFilter::thresholdHSL(Image& image, int threshold){
    HSL hsl;
    float thresh =threshold/255.0f;
    for (int i = 0; i < image.w * image.h; i++) {
        float red = image.data[i * image.c] / 255.0f;
        float green = image.data[i*image.c+1]/255.0f;
        float blue =image.data[i*image.c+2] /255.0f;
        
        float h, s, l;
        hsl.RGBtoHSL(red, green, blue, h, s, l);
        float newL;
        if(l>thresh){
            newL = 1.0;
        }
        else{ newL=0.0;}
    float newR, newG, newB;
    hsl.HSLtoRGB(h,s,newL,newR,newG,newB);
    image.data[i*image.c] =static_cast<unsigned char>(newR * 255);
    image.data[i*image.c+1]=static_cast<unsigned char>(newG * 255);
    image.data[i*image.c+2]=static_cast<unsigned char>(newB * 255);
    }

}


void ColourFilter::histogramEqHSL(Image& image){
    int w = image.w;
    int c = image.c;
    int h = image.h;
    unsigned char* data = image.data;
    
    std::vector<float> vChannel(w * h);

    // Convert RGB to HSV and store the V channel
    for (int i = 0; i < w * h; i++) {
        float r = data[i*c] / 255.0f;
        float g = data[i*c + 1] / 255.0f;
        float b = data[i*c + 2] / 255.0f;

        float h, s, v;
        RGBtoHSV(r, g, b, h, s, v);
        vChannel[i] = v;
    }

    // Histogram equalization core logic
    int histogram[256] = {0};
    for (float v : vChannel) {
        histogram[int(v * 255)]++;
    }

    float cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    std::transform(std::begin(cdf), std::end(cdf), std::begin(cdf), [&](float v) { return v / vChannel.size(); });

    for (float& v : vChannel) {
        v = cdf[int(v * 255)] * 255.0f / 255.0f;
    }

    // Convert processed V channel values back to RGB
    for (int i = 0; i < w * h; i++) {
        float h, s, v;
        float r = data[i*c] / 255.0f;
        float g = data[i*c + 1] / 255.0f;
        float b = data[i*c + 2] / 255.0f;
        RGBtoHSV(r, g, b, h, s, v);

        v = vChannel[i];

        float newR, newG, newB;
        HSVtoRGB(h, s, v, newR, newG, newB);

        data[i*c] = static_cast<unsigned char>(newR * 255);
        data[i*c + 1] = static_cast<unsigned char>(newG * 255);
        data[i*c + 2] = static_cast<unsigned char>(newB * 255);
    }
}

void ColourFilter::histogramEqHSV(Image& image){
    int w = image.w;
    int c = image.c;
    int h = image.h;
    unsigned char* data = image.data;
    std::vector<float> vChannel(w * h);

    // Convert RGB to HSV and store the V channel
    for (int i = 0; i < w * h; i++) {
        float r = data[i*c] / 255.0f;
        float g = data[i*c + 1] / 255.0f;
        float b = data[i*c + 2] / 255.0f;

        float h, s, v;
        RGBtoHSV(r, g, b, h, s, v);
        vChannel[i] = v;
    }

    // Histogram equalization core logic
    int histogram[256] = {0};
    for (float v : vChannel) {
        histogram[int(v * 255)]++;
    }

    float cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    std::transform(std::begin(cdf), std::end(cdf), std::begin(cdf), [&](float v) { return v / vChannel.size(); });

    for (float& v : vChannel) {
        v = cdf[int(v * 255)] * 255.0f / 255.0f;
    }

    // Convert processed V channel values back to RGB
    for (int i = 0; i < w * h; i++) {
        float h, s, v;
        float r = data[i*c] / 255.0f;
        float g = data[i*c + 1] / 255.0f;
        float b = data[i*c + 2] / 255.0f;
        RGBtoHSV(r, g, b, h, s, v);

        v = vChannel[i];

        float newR, newG, newB;
        HSVtoRGB(h, s, v, newR, newG, newB);

        data[i*c] = static_cast<unsigned char>(newR * 255);
        data[i*c + 1] = static_cast<unsigned char>(newG * 255);
        data[i*c + 2] = static_cast<unsigned char>(newB * 255);
    }
}

void ColourFilter::histogramEqGrayscale(Image& image) {
    int w = image.w;
    int c = image.c;
    int h = image.h;
    unsigned char* data = image.data; 
    
    if (!(c == 1)) {
        GrayscaleFilter(image); 
        return;
    }

    std::vector<float> grayscaleChannel(w * h);

    // Convert grayscale data to floating point numbers in the range [0, 1]
    for (int i = 0; i < w * h; ++i) {
        grayscaleChannel[i] = data[i] / 255.0f;
    }

    // Histogram equalization core logic
    int histogram[256] = {0};
    for (float v : grayscaleChannel) {
        histogram[int(v * 255)]++;
    }

    float cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    std::transform(std::begin(cdf), std::end(cdf), std::begin(cdf), [&](float v) { return v / grayscaleChannel.size(); });

    for (float& v : grayscaleChannel) {
        v = cdf[int(v * 255)] * 255.0f / 255.0f;
    }

    // Convert processed data back to integers in the range [0, 255]
    for (int i = 0; i < w * h; ++i) {
        data[i] = static_cast<unsigned char>(std::round(grayscaleChannel[i] * 255.0f));
    }
}