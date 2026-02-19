
#include "Volume.h"
#include "stb_image.h"
#include "stb_image_write.h"



/**
 * @brief Construct a new Volume:: Volume object. Load the images from the directory and store them in a 3D vector.
 * 
 * @param dirPath The directory path where the images are stored.
 * @param minIndex The minimum index of the images to be loaded. Optional.
 * @param maxIndex The maximum index of the images to be loaded. Optional.
 * 
 * @author Yunjie Li, Ce Huang, Prayush Udas
 * @acknowledgement This function was developed with the assistance of generative AI.
 */
Volume::Volume(const std::string dirPath, int minIndex, int maxIndex){
    size_t mindex = minIndex;
    size_t mxIndex = maxIndex;
    namespace fs = std::filesystem;
    std::map<std::string, bool> imageFiles; // store the image files, use map to avoid duplicates and sort the files

    try {
        if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                auto filename = entry.path().string();
                if (entry.path().extension() == ".png") {
                    imageFiles[filename] = true;
                }
            }
        } else {
            throw std::runtime_error("The directory does not exist or is not a directory.");
        }
    } catch (const fs::filesystem_error& err) {
        std::cerr << "[ERROR] Filesystem error: " << err.what() << "\n";
        std::exit(EXIT_FAILURE); // Exit the program on error
    } catch (const std::exception& err) {
        std::cerr << "[ERROR] General error: " << err.what() << "\n";
        std::exit(EXIT_FAILURE); // Exit the program on error
    }

    // Adjust the index range if mindex or mxIndex are set
    auto beginIt = (mindex > 0 && mindex <= imageFiles.size()) ? std::next(imageFiles.begin(), mindex - 1) : imageFiles.begin();
    auto endIt = (mxIndex > 0 && mxIndex <= imageFiles.size()) ? std::next(imageFiles.begin(), mxIndex) : imageFiles.end();

    // load the first image to get the size and channels
    if (beginIt != imageFiles.end()) {
        const auto& firstImageName = beginIt->first;
        unsigned char* imgData = stbi_load(firstImageName.c_str(), &w, &h, &c, 1); // load as grayscale image
        if (imgData == nullptr) {
            std::cerr << "[LOG] Failed to load the image." << std::endl;
            return;
        } else {
            std::cout << "[LOG] Volumes loaded with image size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
        }
        stbi_image_free(imgData);
    }

    // Adjust data vector size based on selected images
    int selectedImagesCount = std::distance(beginIt, endIt);
    data.resize(selectedImagesCount+1, std::vector<std::vector<unsigned char>>(h+1, std::vector<unsigned char>(w * c + 1)));


    // load images and store them in the 3D vector, index starts from 1
    // check if the size and channels are the same as the first image
    int index = 1;
    for (auto it = beginIt; it != endIt; ++it) {
        const auto& filename = it->first;
        unsigned char* imgData = stbi_load(filename.c_str(), &w, &h, &c, 1); // Load as grayscale image
        if (imgData != nullptr) {
            for (int y = 1; y <= h; ++y) {
                for (int x = 1; x <= w * c; ++x) {
                    data[index][y][x] = imgData[((y-1) * w * c) + x-1];
                }
            }
            stbi_image_free(imgData);
        } else {
            std::cerr << "[ERROR] Failed to load image: " << filename << std::endl;
        }
        ++index;
    }
    std::cout << "[LOG] Selected images loaded." << std::endl;

    this->l = this->data.size()-1;
}

/**
 * Save the output images after projection to the specified directory.
 * 
 * @param filename The filename of the output image.
 * 
 * @author Prayush Udas
 */
void Volume::save(const std::string& filename){
    std::string fname;
    if(sliced){
        stbi_write_png(filename.c_str(), this->w, this->h, 1, this->slice.data(), 0);
    }else{
        for (size_t imgIndex = 0; imgIndex < data.size(); ++imgIndex){    
            fname = std::format("{}/VolImage_{}.png", filename, imgIndex);
            stbi_write_png(fname.c_str(), this->w, this->h, 1, data[imgIndex].data(), 0);
        }
    }
}

Volume::Volume() : w(10), h(10), c(3), l(5) {
   std::cout<<"[LOG] Volumes loaded with size " << w << " x " << h << " with " << c << " channel(s)."<<std::endl;
}

