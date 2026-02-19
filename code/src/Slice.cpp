#include "Slice.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <functional>

#include "stb_image.h"

#include "stb_image_write.h"

/**
 * @brief Generates a YZ slice from a specified X position within a volumetric dataset.
 *
 * This function constructs a two-dimensional image slice by traversing through the volume along the YZ plane at a given X coordinate.
 * It extracts a slice that represents a cross-section of the volume, which can be particularly useful for analyzing structural
 * and spatial relationships within the dataset. This method enables the visualization of the internal composition of the volume
 * from a side view, enhancing the understanding of its three-dimensional structure.
 *
 * The generated slice is stored in the `result` vector of the `Volume` instance, ready for further processing or visualization.
 *
 * @param volume The volumetric dataset from which the slice is extracted.
 * @param x The X coordinate position at which the YZ slice is to be taken.
 * 
 * @author: Ce Huang, Prayush Udas
 * Acknowledgement: Documentation aided by generative AI technology.
 */

void Slice::sliceYZ(Volume& volume, int x){
    std::vector<unsigned char> slice(volume.h * volume.l);
    for (int z = 0; z < volume.l; ++z) {
        for (int y = 0; y < volume.h; ++y) {
            slice[z * volume.h + y] = volume.data[z][y][x];
        }
    }
    volume.slice = slice;
    volume.sliced = true;
}


/**
 * @brief Creates an XZ slice from a specified Y position within a volumetric dataset.
 *
 * By iterating over the volume along the XZ plane at a particular Y coordinate, this function produces a slice that reflects
 * a cross-sectional view of the volume. Such slices are invaluable for examining the volume's structural properties from a
 * top-down perspective, allowing for a detailed analysis of the spatial distribution of features within the dataset.
 *
 * Upon completion, the slice is made available in the `result` vector of the `Volume` instance, facilitating its use in subsequent
 * visualization or analytical tasks.
 *
 * @param volume The volumetric dataset from which the slice is generated.
 * @param y The Y coordinate position at which the XZ slice is to be extracted.
 * 
 * @author: Ce Huang, Prayush Udas
 * Acknowledgement: Documentation aided by generative AI technology.
 */

void Slice::sliceXZ(Volume& volume, int y){
    std::vector<unsigned char> slice(volume.l * volume.w);
    for (int z = 0; z < volume.l; ++z) {
        for (int x = 0; x < volume.w; ++x) {
            slice[z * volume.w + x] = volume.data[z][y][x];
        }
    }
    volume.slice = slice;
    volume.sliced = true;
}







