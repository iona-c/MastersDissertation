#include "main.h"
#include "Slice.h"
#include "stb_image.h"
#include "stb_image_write.h"




/*
Example Usage: 
  1) Load image/volume with Image/Volume class
        Image image = Image(argv[1]) 
          - pass file as command line argument  
          - Loads image into image.data inside Image instance

  2) Create specific Filter class:
        - Filter classes: ColourFilter, Blur, EdgeDetection, Projection
        Blur blur;

  3) Call specific apply with filter instance:
        blur.apply(blue.Box, image, 5);

          - this example is for Box Blur
          - All processed images are automatically saved back into
            image.data inside image instance
          - Each apply takes an enumerate value that represents the 
              type of function to be applied
                - See inside class for the available types
  
  4) Chain the filters:
        ColourFilter gs;
        grayscale.apply(gs.GrayScale); // apply grayscale
        gs.apply(gs.saltNpepper, 10.0); // apply saltNpepper  

  5) Save the image:
        image.save("Example_after.png");

          - Will be saved to the path specified
          - Automatically uses image.data to save the data into the
              specified location
  @author Pryush Udas
*/

int main() {
  // Comment it out when not using CLI
  // if(argc == 1){
  //  std::cout << "Pass in a image as parameter" << std::endl; 
  //  std::cout<< "./main <Image path>" << std::endl;
  //  std::cout<< "\tExample: ./main Images/gracehopper.png"<<std::endl;
  //  return 0;
  // }
  
  // Loading image
  //   std::cout<<argv[1]<<std::endl;
  //  Image image = Image(argv[1]); 

  // -- Images Example
  //  Creating specific filter
  //  Blur blur;
  //  blur.apply(blur.Box, image, 5);

  // Chaning filters 
  //  ColourFilter cf;
  //  cf.apply(cf.GrayScale, image);
  //  cf.apply(cf.saltNpepper, image, 50.0);

  //saving the processed image
  // image.save("Example_after.png");

  //-- For Volumes
  //  Volume vol = Volume("Scans/confuciusornis", 100, 200);
  //  Slice slice;
  //  slice.sliceYZ(vol,400);
  //  vol.save("Example_Volume_save.png");

  Cli cli;
  cli.start();
  return 0;  
}
