#ifndef SLICE_H
#define SLICE_H

#include "Image.h"
#include <vector>
#include <string>
#include "Volume.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <functional>

class Slice {
public:
    // Constructor declared as public so that Slice objects can be created from outside the class

    // Generate slice in the XZ plane
    void sliceXZ(Volume& volume, int y);
    void sliceYZ(Volume& volume, int x);

private:
    std::vector<Image> images; // Store loaded images
    int width, height; // Width and height of the image
};

#endif // SLICE_H
