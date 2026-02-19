#ifndef FILTER
#define FILTER

#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include "Image.h"
#include "Volume.h"


/**
 * The Filter class serves as an abstract base class for various image and volume filtering operations.
 * It defines a common interface that all specific filter classes must implement. The primary purpose
 * of this class is to establish a contract for filter operations, ensuring that all derived filter
 * classes provide a consistent implementation of the apply method, which is used to execute the
 * filter operation on an image or volume.
 *
 * Public Methods:
 *   virtual void apply() = 0:
 *     An abstract method that must be implemented by derived classes to apply the filter
 *     operation. The specific behavior of this method will vary depending on the filter type
 *     implemented in the derived class.
 *
 *   virtual ~Filter():
 *     A virtual destructor that ensures derived classes can have their destructors called
 *     correctly, allowing for proper resource cleanup when a Filter object is deleted.
 */

class Filter{
    
    public:
        virtual void apply() = 0;
        virtual ~Filter() = default;

    private:

};
#endif