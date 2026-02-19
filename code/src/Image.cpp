#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/**
 * Default constructor that initializes an empty Image object.
* Outputs a log message indicating that an empty Image instance is created.
* @author Prayush Udas
*/
Image::Image(){
      std::cout << "[LOG][ImageConst] Empty Image instance created" << std::endl; 
}

/**
 * Parameterized constructor that loads an image from the specified file path.
 * The image data is loaded into the 'data' member, and the dimensions and channel
 * count are set accordingly.
 * 
 * @param path The file path of the image to load.
 * Outputs a log message indicating the dimensions and channel count of the loaded image.
 * @author Prayush Udas
 */
Image::Image(std::string path){
      this->path = path;
      data = stbi_load(path.c_str(), &this->w, &this->h, &this->c, 0);
      std::cout << "[LOG][ImageConst] Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl; 
 }    
/**
 * Destructor that frees the allocated image data if it exists.
 * Outputs a log message indicating that the Image object is destructed.
 * @author Prayush Udas
 */
Image::~Image(){
      if(!this->data){
            stbi_image_free(data);
      }
      std::cout<<"[LOG][ImageDes] Image destructed" << std::endl;
}

/**
 * Saves the image data to the specified file path in PNG format.
 * 
 * @param path The file path where the image should be saved.
 * Outputs a log message indicating the result of the save operation.
 * @author Prayush Udas
 */
void Image::save(std::string path){
    int success = stbi_write_png(path.c_str(), w, h, c, data, 0);
    if(!success){
    std::cerr << "[LOG][ImageSave] Error in saving file" << std::endl; 
    }
    std::cout << "[LOG][ImageSave] " << this->path << " saved as " <<  path<< std::endl;
}