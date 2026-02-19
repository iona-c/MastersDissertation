#ifndef IMAGES
#define IMAGES


#include <iostream>
#include <memory>
#include <string>

/**
 * The Image class represents an image with attributes to store its dimensions and pixel data.
 * It provides functionalities to load an image from a file, save the image to a file, and handle
 * the memory allocation and deallocation associated with image data.
 *
 * Attributes:
 *   w (int): The width of the image in pixels.
 *   h (int): The height of the image in pixels.
 *   c (int): The number of color channels in the image (e.g., 3 for RGB).
 *   data (unsigned char*): A pointer to the raw image data, stored in a linear array.
 *   path (std::string): The file path of the image, used for loading and saving.
 *
 * Constructors:
 *   Image(): Initializes an empty image with width, height, and channels set to 0 and data pointer to nullptr.
 *   Image(std::string path): Initializes an image by loading it from the specified file path.
 *
 * Destructor:
 *   ~Image(): Handles the deallocation of the image data to prevent memory leaks.
 *
 * Member function:
 *   void save(std::string path): Saves the image to a specified file path, inferring the format from the file extension.
 *
 * @author Prayush Udas
 */
class Image{
  
  public:
    int w, h, c; 
    unsigned char* data; 
    std::string path;
    Image();
    Image(std::string path);
    ~Image(); 
    void save(std::string path);
    
};
#endif 