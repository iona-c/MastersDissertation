#ifndef CLI
#define CLI

#include <iostream>
#include <string>
#include <sstream>
#include <format>
#include "Volume.h"
#include "Image.h"
#include "ColourFilter.h"
#include "EdgeDetection.h"
#include "Projection.h"
#include "Slice.h"
#include "Blur.h"
#include "stringColours.h"



/**
 * The Cli (Command Line Interface) class is designed to facilitate the interaction between the user and
 * the image/volume processing functionalities. It provides a text-based interface for users to input
 * commands and options, enabling them to apply various filters and transformations to images and volumes.
 * The class integrates different processing tools and maintains the state necessary for user interaction
 * and data manipulation.
 *
 * Attributes:
 *   path (std::string): The path to the input image or volume file.
 *   dest (std::string): The destination path for saving processed images or volumes.
 *   vol (Volume): An object to hold and manipulate volume data.
 *   image (Image): An object to hold and manipulate image data.
 *   oss (std::ostringstream): A stream used for building strings for display or output.
 *   cf (ColourFilter): An instance of ColourFilter for applying color filters to images.
 *   blur (Blur): An instance of Blur for applying blur effects to images.
 *   ed (EdgeDetection): An instance of EdgeDetection for applying edge detection to images.
 *   proj (Projection): An instance of Projection for handling projections in volume data.
 *   slice (Slice): An object for slicing operations on volumes.
 *   Various std::string attributes for storing different pages or sections of the CLI interface.
 *   exit (bool): A flag to indicate whether to exit the CLI loop.
 *   sliced (bool): A flag indicating if a slice operation has been performed on the volume.
 *
 * Constructors:
 *   Cli():
 *     Initializes a new instance of the Cli class with default values.
 *
 *   Cli(const std::string path):
 *     Initializes a new instance of the Cli class and sets the path for the input file.
 *     @param path The path to the input file.
 *
 * Public Methods:
 *   Various public methods are provided to navigate through the CLI, parse user input, apply processing
 *   operations, and manage the state and flow of the CLI application. These methods include functionalities
 *   for displaying the interface, initiating volume or image processing, parsing user inputs for different
 *   types of filters and operations, and saving the processed outputs.
 */

class Cli{
    private:
    std::string path;
    std::string dest;
    Volume vol;
    Image image;
    std::ostringstream oss;

    //Filters
    ColourFilter cf;
    Blur blur;
    EdgeDetection ed;
    Projection proj;
    Slice slice;
    
    std::string StartPage; 

    std::string filters2D;
    std::string histEq;
    std::string options3D;
    std::string filter3D;
    std::string slice3D;
    std::string projection3D;
    std::string lastPage;
    std::string thresholdPage;

    bool exit = false;
    bool sliced = false;
    public:
        Cli();
        Cli(const std::string path);
        void display(std::string& out);
        void start();
        void initiateVolume();
        int parseFileType();
        void parseFilterType2d();
        void parseFilterType3d();
        void clearScreen();
        void promtInput();
        void refreshVar();
        void parseOptions3D();
        void parseSlice3D();

        void parseSliceYZ();
        void parseSliceXZ();
        void parseProjection3D();

        void cliSnP();
        void cliThres();
        void cliHist();
        void cliBrigt();
        void cliMBlur(int typ);
        void cliGBlur(int typ);
        void cliBBlur();
        void cliSaveImg();
        void cliSaveVol();
};

#endif