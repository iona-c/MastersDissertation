#ifndef VOLUME
#define VOLUME

#include <iostream>
#include <vector> 
#include <map>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <format>
#include "Image.h"

/**
 * The Volume class represents a 3D volume, typically used in medical imaging or scientific visualization,
 * where the volume is composed of a stack of 2D slices or a 3D array of data. This class provides
 * functionalities to load a volume from a file, access its data, and save manipulated or processed volumes.
 *
 * Attributes:
 *   path (std::string): The file path from which the volume was loaded or where it will be saved.
 *   w (int): Width of the volume, i.e., the number of pixels in each row of a slice.
 *   h (int): Height of the volume, i.e., the number of pixels in each column of a slice.
 *   c (int): Number of channels in the volume's data (e.g., 1 for grayscale, 3 for RGB).
 *   l (int): The number of slices in the volume.
 *   sliced (bool): Indicates whether the volume has been sliced.
 *   data (std::vector<std::vector<std::vector<unsigned char>>>): The raw data of the volume stored as a 3D vector.
 *   slice (std::vector<unsigned char>): The processed data after applying a projection or slicing operation.
 *
 * Constructors:
 *   Volume():
 *     Initializes an empty Volume object with default parameters.
 *
 *   Volume(std::string path):
 *     Loads volume data from the specified file path. The loaded data populates the attributes of the Volume object.
 *     @param path A string representing the file path of the volume to be loaded.
 *
 *   Volume(std::string path, int minIndex=-1, int maxIndex=-1):
 *     Loads volume data from a specified path and applies slicing based on the given indices.
 *     @param path A string representing the file path of the volume.
 *     @param minIndex The starting index for slicing the volume (inclusive). Default is -1, indicating the start of the volume.
 *     @param maxIndex The ending index for slicing the volume (inclusive). Default is -1, indicating the end of the volume.
 *
 * Method:
 *   void save(const std::string& path):
 *     Saves the volume or processed slice to the specified file path.
 *     @param path A string representing the file path where the volume or slice will be saved.
 * 
 * @author Prayush Udas
 */

class Volume{
    public:
        std::string path;
        int w, h, c, l;
        bool sliced = false; 
        Volume();
        std::vector<std::vector<std::vector<unsigned char>>> data;
        //result is processed data after applying MIP, AIP, MinIP
        Volume(std::string path);
        // result is processed data after applying MIP, AIP, MinIP
        std::vector<unsigned char> slice; 
        Volume(std::string path, int minIndex=-1, int maxIndex=-1);
        void save(const std::string& path);
};


#endif