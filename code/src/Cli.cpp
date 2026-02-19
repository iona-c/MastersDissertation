#include "Cli.h"




Cli::Cli(){
    clearScreen();
    std::string input = 
R"(
                Image Filters, Projections and Slices
---------------------------------------------------------------------





            
            Please enter input path:)";
    display(input);
    std::cin >> this->path;
        std::string end = 
    R"(





    )";
    display(end);
    this->image = Image(this->path);
}

Cli::Cli(const std::string path){
    this->path = path;
}

void Cli::display(std::string& out){
    refreshVar();
    std::cout<<out;
}

void Cli::clearScreen(){
    #ifdef _WIN32
    // Windows system
    std::system("cls");
    #else
    // Unix-like (Linux, MacOS) system
    std::system("clear");
    #endif
}
void Cli::promtInput(){
    
}

void Cli::start(){
    clearScreen();
    display(StartPage);
    int ret = parseFileType();
    clearScreen();
    switch (ret){
        case 0:
            oss <<COL_BLUE<< "Folder:" <<path<<COL_NORMAL;
            display(options3D);
            parseOptions3D();
            break;
        case 1:
            oss <<COL_BLUE<< "Images:"<<path<<COL_NORMAL;
            display(filters2D);
            parseFilterType2d();
            break;
        default:
            std::cerr<<"[ERROR] Should not be here";
            throw std::runtime_error("[ERROR] Should not get here - start()");
    }

    if(ret==1){
        while(!exit){
            clearScreen();
            display(filters2D);
            parseFilterType2d();
        }
    } else if(ret==0){
        while(!exit){
            if(!sliced){
                clearScreen();
                display(options3D);
                parseOptions3D();
            } else if(sliced){
                clearScreen();
                display(filters2D);
                parseFilterType2d();
            }
        }
    }
}
void Cli::initiateVolume(){
    int start;
    int end;
    try{
        std::cout<<"   Enter 0 to load all"<<std::endl;
        std::cout<<"Enter start number to load:"<<std::endl<<"==>";
        std::cin>>start;

        if(start == 0){
            std::cout<<"Images loading..."<<std::endl;
            this->vol = Volume(path, -1,-1);
            return;
        }
        std::cout<<"Enter end number to load:"<<std::endl<<"==>";
        std::cin>>end;
        this->vol = Volume(path, start, end);
    }catch(const std::exception& e){
        exit = true; 
    }
    image.w = vol.w;
    image.h = vol.h;

}

int Cli::parseFileType(){
    int selected; 
    std::cin>> selected;

    switch (selected){
        case 1:
            this->image = Image(this->path);
            return 1;
            break;
        case 2:
            initiateVolume(); 
            return 0;;
            break;
        default:
            std::cerr<<"[LOG] Incorrect selection" <<std::endl;
            throw std::runtime_error("[EXCEPTION] Incorrect selection for file type");
    }
}

void Cli::cliSnP(){
    float noise;
    std::cout<<"Enter Noise:\n==>";
    std::cin >> noise;
    cf.apply(cf.saltNpepper, image, noise);
}

void Cli::cliThres(){
    clearScreen();
    refreshVar();
    display(thresholdPage);
    int select;
    std::cin>>select;

    int thres; 
    std::cout<<"Enter threshold number:\n==>";
    std::cin>>thres;
    switch (select){
        case 1:;
            cf.apply(cf.thHSL, image, thres);
            oss << " -> " << COL_BLUE << "Filter: Threshold HSL" << COL_NORMAL;
            break;
        case 2:
            cf.apply(cf.thHSV, image,thres);
            oss << " -> " << COL_BLUE << "Filter: Threshold HSV" << COL_NORMAL;
            break;
        case 3:
            cf.apply(cf.thGREY, image, thres);
            oss << " -> " << COL_BLUE << "Filter: Threshold Grey" << COL_NORMAL;
            break;
        default:
            std::cout<<"[LOG] Enter correct Threshold number"<<std::endl;
            throw std::runtime_error("[EXCEPTION] Incorrect selection");
    }
}

void Cli::cliHist(){
    clearScreen();
    refreshVar();
    std::cout<<histEq<<std::endl;
    int selected;
    std::cin>> selected;
    switch (selected){
        case 1:
            cf.apply(cf.histHSL, image);
            break;
        case 2:
            cf.apply(cf.histHSV, image);
            break;
        case 3:
            cf.apply(cf.histGREY, image);
        default:
            std::cout<< "[LOG] Incorrect selection" << std::endl;
            throw std::runtime_error("[EXCEPTION] Incorrect selection"); 
    }
}

void Cli::cliBrigt(){
    std::cout<<"==>Enter auto or manual:\n==>";
    std::string mode;
    std::cin >> mode;
    cf.apply(cf.Brightness, image, mode);
}

void Cli::cliMBlur(int typ){
    int kernelSize;
    std::cout<<"Enter kernel size:\n==>";
    std::cin >> kernelSize;
    if (typ ==1){
        blur.apply(blur.Median, image, kernelSize);
    } else if(typ ==2){
        blur.apply(blur.Median, vol, kernelSize);
    }
}
void Cli::cliGBlur(int typ){
    int kernelSize;
    float sigma;
    std::cout<<"Enter kernel size:\n==>";
    std::cin >> kernelSize;
    std::cout<<"Enter simga:\n==>";
    std::cin >> sigma;
    if (typ == 1){
        blur.apply(blur.Gaussian, image, kernelSize, sigma);
    } else if(typ ==2){
        blur.apply(blur.Gaussian, vol, kernelSize, sigma);
    }
}

void Cli::cliBBlur(){
    int kernelSize;
    std::cout<<"Enter kernel size:\n==>";
    std::cin >> kernelSize;
    blur.apply(blur.Box, image, kernelSize);

}

void Cli::cliSaveImg(){
    this->exit = true;
    oss<<std::endl;
    std::cout<<"Enter a file name to save to:\n==>";
    std::cin>>this->dest;
    image.save(this->dest);
}
void Cli::parseFilterType2d(){
    int selected; 
    std::cin>> selected;

    switch (selected){
        case 0:
            oss <<std::endl;
            cliSaveImg();
            clearScreen();
            display(lastPage);
            break;
        case 1:
            oss <<" -> " << COL_BLUE << "Filter: SaltNPepper" << COL_NORMAL;
            cliSnP();
            break;
        case 2:
            oss <<" -> " << COL_BLUE << "Filter: Grayscale" << COL_NORMAL;
            cf.apply(cf.GrayScale, image); 
            break;
        case 3:
            //oss <<" -> " << COL_BLUE << "Filter: Threshold" << COL_NORMAL;
            cliThres();
            break;
        case 4:
            oss <<" -> " << COL_BLUE << "Filter: Histogram Eq" << COL_NORMAL;
            cliHist();
            break;
        case 5:
            oss <<" -> " << COL_BLUE << "Filter: Brightness" << COL_NORMAL;
            cliBrigt();
            break;
        case 6:
            oss <<" -> " << COL_BLUE << "Blur: Median" << COL_NORMAL;
            cliMBlur(1);
            break;
        case 7:
            oss <<" -> " << COL_BLUE << "Blur: Box" << COL_NORMAL;
            cliBBlur();
            break;
        case 8:
            oss <<" -> " << COL_BLUE << "Blur: Gaussian" << COL_NORMAL;
            cliGBlur(1);
            break;
        case 9:
            oss <<" -> " << COL_BLUE << "Edge Detection: Sobel" << COL_NORMAL;
            ed.apply(image, ed.Sobel);
            break;
        case 10:
            oss <<" -> " << COL_BLUE << "Edge Detection: Prewitt" << COL_NORMAL;
            ed.apply(image, ed.Prewitt);
            break;
        case 11:
            oss <<" -> " << COL_BLUE << "Edge Detection: Scharr" << COL_NORMAL;
            ed.apply(image, ed.Scharr);
            break;
        case 12:
            oss <<" -> " << COL_BLUE << "Edge Detection: Robert Cross" << COL_NORMAL;
            ed.apply(image, ed.RobertsCross);
            break;
        default:
            std::cerr<<"[LOG] Incorrect selection" <<std::endl;
            throw std::runtime_error("[EXCEPTION] Incorrect selection");
    }   
}

void Cli::parseFilterType3d(){
    int selected; 
    std::cin>> selected;
    
    switch (selected){
        case 1:
            oss <<" -> " << COL_CYAN << "Blur: Gaussian" << COL_NORMAL;
            cliGBlur(2);
            break;
        case 2:
            oss <<" -> " << COL_CYAN << "Blur: Median" << COL_NORMAL;
            cliMBlur(2);
            break;
        default:
            std::cerr<<"[ERROR] Should never reach this in parseFilterType3d"<<std::endl;
            break;
    }
 
}

void Cli::cliSaveVol(){
    if(sliced){
        image.save(dest);
    }else{
        vol.save(dest);
    }
}

void Cli::parseOptions3D(){
   int selected;
   std::cin>>selected; 
   switch (selected){
        case 0:
            oss<<std::endl;
            cliSaveVol();
            clearScreen();
            display(lastPage);
            exit = true;
        case 1:
            clearScreen();  
            display(filter3D);
            parseFilterType3d();
            break;
        case 2:
            clearScreen();
            display(projection3D);
            parseProjection3D();
            sliced = true;
            break;
        case 3:
            clearScreen();
            display(slice3D);
            parseSlice3D();
            sliced = true;
            break;
        default:
            std::cerr<<"[ERROR] Should not be here" << std::endl;
   }
}

void Cli::parseSliceXZ(){
    std::cout<<"Input Index to slice at:\n==>";
    int selected;
    std::cin>>selected;
    slice.sliceXZ(vol, selected);
    image.w = vol.w;
    image.h = vol.l;
    image.c = 1;
    image.data = new unsigned char[vol.slice.size()];
    std::copy(vol.slice.begin(), vol.slice.end(), image.data);
}

void Cli::parseSliceYZ(){
    std::cout<<"Input Index to slice at:\n==>";
    int selected;
    std::cin>>selected;
    slice.sliceYZ(vol, selected);
    image.w = vol.w;
    image.h = vol.l;
    image.c = 1;
    image.data = new unsigned char[vol.slice.size()];
    std::copy(vol.slice.begin(), vol.slice.end(), image.data);
}

void Cli::parseSlice3D(){
    int selected;
    std::cin>>selected;
    switch (selected){
        case 1: 
            oss<< " -> " << COL_CYAN<<" Slice : Y-Z" << COL_NORMAL;
            parseSliceYZ(); 
            break;
        case 2:
            oss<< " -> " << COL_CYAN<<" Slice : X-Z" << COL_NORMAL;
            parseSliceXZ();
            break;
        default:
            std::cerr<<"[ERROR] Should not be here in parseSlice3D" <<std::endl;
    }
}

void Cli::parseProjection3D(){
    int selected;
    std::cin>>selected;
    switch (selected){
        case 1:
            oss<<" -> " << COL_CYAN << "Proj : MIP" << COL_NORMAL;
            proj.apply(proj.projMIP, vol);
            break;
        case 2:
            oss<<" -> " << COL_CYAN << "Proj : AIP" << COL_NORMAL;
            proj.apply(proj.projAIP, vol);
            break;
        case 3:
            oss<<" -> " << COL_CYAN << "Proj : MinIP" << COL_NORMAL;
            proj.apply(proj.projMinIP, vol);
            break;
        default:
            std::cerr<<"[ERROR] Should not be here"<<std::endl;
    }
    std::copy(vol.slice.begin(), vol.slice.end(), image.data);
}

void Cli::refreshVar(){
    StartPage =  std::format(
R"(
    Image Filters, Projections and Slices
---------------------------------------------------------------------



        Select 2D or 3D
            [1] 2D
            [2] 3D





{}
==>)", oss.str());

    filters2D = std::format(
R"(
---------------------------------------------------------------------



        Select Filter:
            Colour Filters:
                [1] Salt and Pepper
                [2] Grayscale
                [3] Threshold
                [4] Histogram equalisation
                [5] Brightness
            Blur:
                [6] Median Blur
                [7] Box Blur
                [8] Gaussian blur
            Edge Detection:
                [9] Sobel
               [10] Prewitt
               [11] Scharr
               [12] Roberts' Cross
        Save:
            [0] Save and Quit

Filters Applied:
{}
==>)",oss.str());
    histEq = std::format(
R"(
---------------------------------------------------------------------




        Select Histogram Equaliser type:
            [1] Histogram Equaliser using HSL 
            [2] Histogram Equaliser using HSV
            [3] Histogram Equaliser using Grayscale 




Filters Applied:
{}
==>)", oss.str());

    thresholdPage = std::format(
R"(
---------------------------------------------------------------------




        Select threshold type:
            [1] HSL
            [2] HSV
            [3] GRAY




Filters Applied:
{}
==>)", oss.str());

    lastPage = std::format(
R"(
---------------------------------------------------------------------





        {}Thank you for using the application!{}
        
        Saved to: {} 

        Filters Applied: 
        {}            
            






==>)",COL_GREEN,COL_NORMAL,this->dest, oss.str());

    options3D = std::format(
R"(
---------------------------------------------------------------------




        Select 3D operation:
            [1] Filters
            [2] Orthographic Projections
            [3] Slice


        Save:
            [0] Save and Quit

Applied Filters:
{}
==>)", oss.str());

    filter3D = std::format(
R"(
---------------------------------------------------------------------




        Select 3D Filter:
            [1] 3D Gaussian Blur
            [2] 3D Median Blur





Applied Filters:
{}
==>)", oss.str()); 

    slice3D = std::format(
R"(
---------------------------------------------------------------------




        Select slice axis:
            [1] Slice Y-Z
            [2] Slice X-Z





Applied Filters:
{}
==>)", oss.str()); 

    projection3D = std::format(
R"(
---------------------------------------------------------------------




        Select slice axis:
            [1] MIP
            [2] AIP
            [3] MinIP




Applied Filters:
{}
==>)", oss.str()); 
}

