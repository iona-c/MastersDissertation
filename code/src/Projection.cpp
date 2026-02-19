#include "Projection.h"
#include <algorithm>
#include <iostream>

/**
 * @brief Applies the Maximum Intensity Projection (MIP) to a volumetric dataset.
 *
 * This function iterates through the volumetric data, examining each voxel along the Z-axis for every (X, Y) position.
 * It selects the maximum intensity value found along this axis and projects it onto a two-dimensional image. The result
 * is an image highlighting the highest intensity regions throughout the volume, which is particularly useful for visualizing
 * structures in medical imaging and scientific data.
 *
 * The method efficiently traverses the volume data, ensuring that the maximum intensity value is accurately captured and
 * represented in the resultant projection. This approach guarantees that the critical features of the volume are emphasized,
 * aiding in the analysis and interpretation of complex datasets.
 *
 * Author: Ce Huang
 * Acknowledgement: This documentation was supported by generative AI technology.
 */

void Projection::MIP(Volume& volume) {
    std::vector<unsigned char> result(volume.w * volume.h, 0); // Initialize to record maximum intensity values
    // Iterate over each pixel
    for (int y = 1; y <= volume.h; ++y) {
        for (int x = 1; x <= volume.w; ++x) {
            unsigned char maxIntensity = 0;
            // Find maximum intensity value in z-direction
            for (int z = 1; z <= volume.l; ++z) {
                unsigned char intensity = volume.data[z][y][x]; // Single channel value
                if (intensity > maxIntensity) {
                    maxIntensity = intensity;
                }
            }
            // Write maximum intensity value into result image
            result[(y-1) * volume.w + x-1] = maxIntensity;
        }
    }
    volume.slice = result;
}


/**
 * @brief Implements the Average Intensity Projection (AIP) on a given volumetric dataset.
 *
 * AIP involves calculating the average intensity for each voxel across the Z-axis and projecting this average onto a 
 * two-dimensional image. This projection method is useful for highlighting the general structure within the volume by 
 * averaging out the intensity values, which can be beneficial in various applications, including visualization of 
 * biological tissues or materials science datasets.
 *
 * Through iterative computation over the entire dataset, this function ensures a faithful representation of the average
 * intensities. It mitigates the influence of outliers, thereby providing a clearer overview of the intrinsic properties
 * of the volume.
 *
 * Author: Ce Huang
 * Acknowledgement: Enhanced with insights from generative AI technology.
 */

void Projection::AIP(Volume& volume){
    std::vector<unsigned char> result(volume.w * volume.h, 0);

    // Iterate over each pixel
    for (int y = 1; y <= volume.h; ++y) {
        for (int x = 1; x <= volume.w; ++x) {
            unsigned int sumIntensity = 0;
            // Calculate sum of intensity values in z-direction
            for (int z = 1; z <= volume.l; ++z) {
                sumIntensity += volume.data[z][y][x]; // Accumulate intensity values
            }
            // Calculate average intensity value and write it into result image
            unsigned char avgIntensity = static_cast<unsigned char>(sumIntensity / volume.l);
            result[(y-1) * volume.w + (x-1)] = avgIntensity;
        }
    }
    volume.slice = result;
}


/**
 * @brief Conducts the Minimum Intensity Projection (MinIP) on a volumetric dataset.
 *
 * MinIP is the process of projecting the minimum intensity value found along the Z-axis for each (X, Y) coordinate
 * onto a two-dimensional plane. This projection technique is particularly adept at visualizing low-intensity structures
 * within the volume, such as airways or vessels in medical scans, by bringing forward the darkest areas in the dataset.
 *
 * By examining the entire depth of the volume, the method ensures that the areas of minimum intensity are accurately
 * highlighted in the resulting image. This facilitates the detection and analysis of features that may be overlooked
 * with other projection methods, offering a unique perspective on the volume's characteristics.
 *
 * Author: Ce Huang
 * Acknowledgement: Documentation aided by generative AI technology.
 */


void Projection::MinIP(Volume& volume) {
    std::vector<unsigned char> result(volume.w * volume.h, 255); // Initialize to maximum to find minimum
    // Iterate over each pixel
    for (int y = 1; y <= volume.h; ++y) {
        for (int x = 1; x <= volume.w; ++x) {
            unsigned char minIntensity = 255;
            // Find minimum intensity value in z-direction
            for (int z = 1; z <= volume.l; ++z) {
                unsigned char intensity = volume.data[z][y][x]; // Single channel value
                if (intensity < minIntensity) {
                    minIntensity = intensity;
                }
            }
            // Write minimum intensity value into result image
            result[(y-1) * volume.w + x-1] = minIntensity;
        }
    }
    volume.slice = result;
}


void Projection::apply(Proj method, Volume& volume) {
    switch (method) {
        case Proj::projMIP:
            MIP(volume);
            std::cout << "[LOG] Performing MIP" << std::endl;
            break;
        case Proj::projMinIP:
            MinIP(volume);
            std::cout << "[LOG] Performing MinIP" << std::endl;
            break;
        case Proj::projAIP:
            AIP(volume);
            std::cout << "[LOG] Performing AIP" << std::endl;
            break;
        default:
            std::cout << "[ERROR] Invalid projection method specified." << std::endl;
            break;
    }
    volume.sliced = true;
}



