#ifndef COLOUR_FILTER
#define COLOUR_FILTER
#include "Filter.h"


class HSL {
    public:
        float h, s, v;
        float r,g,b;
        void RGBtoHSL(float& fR, float& fG, float& fB, float& fH, float& fS, float& fL);
        void HSLtoRGB(float fH, float fS, float fL, float& fR, float& fG, float& fB);
};

class HSV {
    public:
        float h, s, v;
        float r,g,b;
        void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV);
        void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b);
};


class ColourFilter : public Filter, public HSL, public HSV{
    private:
        void _AutoAdjustBrightness(Image& image, int average=128);
        void _ManualAdjustBrightness(Image& image);
        void BrightnessFilter(Image& image, std::string mode);
        void saltNpepperFilter(Image& image, double noisePercentage);
        void GrayscaleFilter(Image& image);
        void thresholdHSL(Image& image, int threshold);
        void thresholdHSV(Image& image,  int threshold);
        void thresholdGREY(Image& image, int threshold);
        void histogramEqHSL(Image& image);
        void histogramEqHSV(Image& image);
        void histogramEqGrayscale(Image& image);
    public:
        enum cf{ 
            saltNpepper,
            GrayScale,
            Brightness,
            thHSL,
            thHSV,
            thGREY,
            histHSL,
            histHSV,
            histGREY
        };
        void apply(){};
        void apply(cf filter, Image& image);
        void apply(cf filter, Image& image, std::string mode);
        void apply(cf filter, Image& image, double noisePercentage);
        void apply(cf filter, Image& image, int threshold);
            
};

#endif