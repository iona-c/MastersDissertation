######################################################################
# --------------------------------------------------------
# Created by Gerard Gallardo i Peres
# ---------------------------------------------------------
# Objective: Store configuration information such as flags,
# variables and any other setup information that might be
# needed for the desired run.
#
#
# ----------------------------------------------------------
# Authors:   Gerard Gallardo / Imperial College London
#
# Reviewer:  --   / 
# Last rev.: --   / 
#
######################################################################
            #######            #######   #############################
###      ##########    ###############   #############################
###      ##########    ###############   #############################
###      ##########    ###############   #############################
            #######            #######           #####################
######################################################################

import numpy as np

class cnf:
        def __init__(self):
                

### General configuration ###


        # Datasets 1 [Magellan, NovaSAR, TerraSAR, SRTM]

                self.SAR_dataset = [1, 0, 0, 0] 
        
        # Datasets 2 [Magellan, NovaSAR, TerraSAR, SRTM]

                self.SAR_dataset_2 = [0, 0, 0, 0]

        # Datasets [Magellan gtdr, Magellan stereo]

                self.topo_dataset = [1, 0] 

        # Ancillary Datasets [Magellan rsm slopes, Magellan emissivity, Magellan reflectivity]

                self.ancillary_dataset = [0, 0, 0] 

        # Select analysis sections

                self.analysis = [1, 0, 0, 0]  

        # Multilooking flags for each dataset

                self.multilooking = [1, 0, 0, 0] #1 or 0 if applicable

        # Horizontal filtering dimensions according to dataset

                self.data_horizontal_multilooking = [3, 1, 11, 12]

        # Vertical filtering dimensions according to dataset

                self.data_vertical_multilooking = [3, 1, 11, 16]

        # Horizontal filtering dimensions when simulating VenSAR or VISAR

                self.simulation_horizontal_multilooking = 7 

        # Vertical filtering dimensions when simulating VenSAR or VISAR

                self.simulation_vertical_multilooking = 7

        # Select type of filter

                self.filter = "lee" # "boxcar" or "lee", etc

        # Select Venus mission simulation

                self.simulation = "none" # "veritas", "envision" or "none" if i is a magellan cycle comparison

        # simulated change patch polygon, in case of simulation. If only 1 change, assign change_2 to 0.

                self.change_1 = np.array([[467, 453, 432, 418, 399, 364, 346, 354, 370, 333, 339, 377, 375, 363, 375, 396, 387, 420, 447, 456, 477], [704, 678, 661, 639, 614, 597, 603, 609, 616, 631, 637, 652, 682, 698, 711, 728, 743, 773, 775, 764, 729]])
                self.change_2 = np.array([[473, 483, 508, 546, 563, 600, 641, 622, 563, 559, 532, 504, 481], [400, 424, 447, 460, 477, 504, 500, 480, 447, 419, 392, 374, 362]])
                # Change_f43_mg0029_f45s218_q_1_1024_1_1024_c1 = np.array([[467, 453, 432, 418, 399, 364, 346, 354, 370, 333, 339, 377, 375, 363, 375, 396, 387, 420, 447, 456, 477], [704, 678, 661, 639, 614, 597, 603, 609, 616, 631, 637, 652, 682, 698, 711, 728, 743, 773, 775, 764, 729]])
                # Change_f43_mg0029_f45s218_q_1_1024_1_1024_c2 = np.array([[473, 483, 508, 546, 563, 600, 641, 622, 563, 559, 532, 504, 481], [400, 424, 447, 460, 477, 504, 500, 480, 447, 419, 392, 374, 362]])
                # Change_f30_mg0004_f25n351_q_1_400_1_400 = [60 51 59 49 70 66 74 83 86 79 94 98 112 114 135 137 129 118 125 133 144 137 119 124 99 90 81 74 78, 229 234 237 247 256 270 277 283 292 300 305 310 312 321 320 315 307 303 299 293 286 272 269 263 261 264 258 251 245]
                # Change_f30_mg0004_f25n351_q_624_1024_624_1024 = [215 218 227 244 265 271 289 331 315 318 310 318 326 329 322 307 289 275 271 260 241 222 211 195, 101 103 92 108 92 106 100 79 95 101 109 123 135 149 164 170 173 160 174 163 147 155 138 121]
                # Change_f43_mg0029_f45s218_q_1_1024_1_1024_c1 = [467 453 432 418 399 364 346 354 370 333 339 377 375 363 375 396 387 420 447 456 477, 704 678 661 639  614 597 603 609 616 631 637 652 682 698 711 728 743 773 775 764 729]
                # Change_f43_mg0029_f45s218_q_1_1024_1_1024_c2 = [473 483 508 546 563 600 641 622 563 559 532 504 481, 400 424 447 460 477 504 500 480 447 419 392 374 362]
                # Change_f33_mg_0013_f65n126_q_1_1024_1_1024_c1 = [400 422 433 445 462 453 425, 137 140 152 169 161 147 136]
                # Change_f33_mg_0013_f65n126_q_1_1024_1_1024_c2 = [780 791 804 804 817 808 788 770 758 748 721 704 713 728 713 697 696 716, 771 755 743 727 720 711 705 709 700 682 680 691 703 715 732 751 762 774]
                # Change_f33_mg_0013_f65n126_q_1_1024_1_1024_c3 = [947 932 904 901 928 961 962 924 897 865 861 868 864 865 887, 736 719 701 694 683 678 662 656 649 655 673 696 709 727 739]
                # Change_f48_mg_0056_f40n046_q_1_1024_1_1024_c1 = [371 394 413 434 456 474 496 482 485 477 471 477 465 436 413 385 374 339 307 297 276 283 298, 664 679 694 715 739 747 769 726 706 682 644 627 599 606 600 605 583 567 583 612 633 656 666]
                # Change_f48_mg_0056_f40n046_q_1_1024_1_1024_c2 = [279 255 237 210 214 186 147 110 124 101 89 108 144 162 193 218 204, 654 656 616 601 565 546 560 590 623 645 688 714 699 681 691 694 709]
                # Change_mg_0033_c100n197 = [3568 3563 3351 3253 3174 2956 2799 2611 2492 2157 2008 1693 1406 1120 973 715 784 834 1120 1367 1801 2245 2621 2996 3193 3381, 1837 1744 1523 1419 1276 1243 1177 1221 1160 1050 1077 923 853 890 834 1204 1380 1540 1617 1545 1534 1656 1793 1936 2019 2124]

        # Select wether to show inspect/plots results or not

                self.show_plots = 1 # 1 or 0

        # Select colormap for results

                self.cmaps = ['Greys_r', 'terrain', 'turbo', 'hsv', 'jet', 'hot_r'] # [SAR images, topography, slope angle, slope aspect, local incidence angles, lats/lons]


        # define intra classes

                self.mgn = self.mgn()
                self.novasar = self.novasar()
                self.terrasar = self.terrasar()
                self.test_statistic = self.test_statistic()




### Magellan configuration ###

        
        class mgn:
                def __init__(self):

        # Magellan SAR framelet number for each F/C MIDR

                        self.framelet_num = 56

        # Magellan framelet selection

                        self.framelet_selection = 0 # to select a specific framelet within the C1 or F MIDR product, set to 0 for the whole product

                # For Sif Mons framelet 30 is good (easy cases)
                # For Iduun Mons framelet 43 is good (difficult cases)
                # For tethis region framelet 33
        
        # Magellan slope framelet number (only applies to merc and sinus projections)

                        self.framelet_gsdr = 32

        # Magellan slopes product add borders in order for all image to be within the slopes image in terms of lat/lon

                        self.slopes_addborder_idxs = [-1, 1, -1, 1]

        # Magellan topography framelet number (only applies to merc and sinus projections)

                        self.framelet_gtdr = 32

        # Linear or Sinus projection in Magellan SAR images

                        self.SAR_projection = 'linear'

        # Linear or Nearest interpolation of NaNs in the aspect topo-to-sar interpolation step. Linear only recomended when dealing with small framelets/quadrants

                        self.aspect_interp = 'nearest'

        # Select whether the slopefield calculation from topography is from a manual implementation or using matlab gradientm. Cases 'manual' or 'gradientm'

                        self.slope_method = 'manual'

        # Enable slope filtering 

                        self.slope_filtering = 1

        # Choose 2-D window filter type for slope filtering

                        self.slope_filtertype = 'taylorwin'

        #     median         - Median filter
        #     bartlett       - Bartlett window.
        #     barthannwin    - Modified Bartlett-Hanning window. 
        #     blackman       - Blackman window.
        #     blackmanharris - Minimum 4-term Blackman-Harris window.
        #     bohmanwin      - Bohman window.
        #     chebwin        - Chebyshev window.
        #     flattopwin     - Flat Top window.
        #     gausswin       - Gaussian window.
        #     hamming        - Hamming window.
        #     hann           - Hann window.
        #     kaiser         - Kaiser window.
        #     nuttallwin     - Nuttall defined minimum 4-term Blackman-Harris window.
        #     parzenwin      - Parzen (de la Valle-Poussin) window.
        #     rectwin        - Rectangular window.
        #     taylorwin      - Taylor window.
        #     tukeywin       - Tukey window.
        #     triang         - Triangular window.

        # 2-D window filter size for slope and gtdr filtering rows cols

                        self.gxdr_window = [6, 6] #original 7x8

        # 2-D window filter size for stereo filtering rows cols

                        self.stereo_window = [9, 9]

        # Allow gtdr errors correction and filtering

                        self.topo_correction = 1  # either 1 or 0

        # Option method to correct topography errors: 'basic' or 'advanced'

                        self.topo_correction_option = 'advanced'

        # Allow stereo-derived filtering

                        self.stereo_correction = 1  # either 1 or 0

        # Select type of filter for gtdr/stereo analogously to for the slopes

                        self.topo_filtertype = 'hamming'

        # Select std filter thresholds

                        self.std_filter_thresholds = [250, 1000]

        # Use gxdr or stereo topography

                        self.topo = [1, 0]

        # Muhleman table 1 (leftlooking) or 2 (rightlooking)

                        self.table = 1

        # Projection used for gxdr georeferencing

                        self.gxdr_projection = 'mercator'

        # Type of DEM from stereo processing : 'high' (only stereo) or 'merge' (stereo + low-filtered gtdr)

                        # self.stereo_dem = 'merge'

        # Projection used for stereo georeferencing

                        self.stereo_projection = 'linear' # 'linear' or 'sinus'

        # Threshold for NaN association in identifying line artifacts in Magellan data

                        self.artifact_threshold = 0.007

        # Magellan quadrant of interest within framelet ([1 1024 1 1024]; if all the framelet)

                        self.quadrant = [0, 1023, 0, 1023] #whole framelet = [0, 1023, 0, 1023]

        # Magellan CD analysis ENL patch lat corner

                        self.quadrant_enl_patch_lat = -46.3742

                #Sif Mons: 25.1252.
                #Iduun Mons: -46.3742
                #Tethus regio: 64.6236
                #Nyx mons: 38.7686
                #Maat mons: 6.21675

        # Magellan CD analysis ENL patch lon corner

                        self.quadrant_enl_patch_lon = 217.086

                #Sif Mons: 351.625.
                #Iduun Mons:  217.086.
                #Tethus regio: 119.74.
                #Nyx mons: 49.5058
                #Maat mons: 190.358

        # ENL rectangle dimensions

                        self.ENL_box = [121, 143]

        # Margins (in pixels) to get rid of undesired border effects in slope field and local incidence angle

                        self.margins = 200 #200 pixels if using a full framelet is typically enough

        # Mode for topography-slopes processing: Mode 1 for interpolation as a last step, mode 2 for interpolation before slope generation

                        self.mode = 1

### Test statistics configuration ###


        class test_statistic:
                def __init__(self):

        # Test statistic histogram number of bins

                        self.hist_bins = 500

        # Factor on how different the mean of the artificial change 1 is with respect of the background

                        self.change_1_factor = 2

        # Factor on how different the mean of the artificial change 2 is with respect of the background.
        # If empty, assign to 0

                        self.change_2_factor = 2

        # Value of R_system considered to be the margin of R_1 (adjust according to number of looks)

                        #self.histogram_R1_margin_value = 0.001

        # Thresholding method to be used for change map: minimum_error, otsu or visual

                        self.threshold_method = 'minimum_error'

        # Selected threshold from visual method (not optimmized by any method)

                        self.threshold_value = 1.55

        # Flag to select whether to use ENL for Ls or choose own L1 and L2 values. 0 for preselected values, 1 for ENL values, 2 for optimization fit finding

                        self.ENL_flag = 0

        # Bounds for GB2 curve fitting in case LS optimization is selected to obtain L1 and L2

                        self.LSfit_bounds = [100,5]

        # L1 and L2 values if not ENLs

                        self.L_parameters = [70, 120] # [L1, L2]
        


### NovaSAR configuration ###

        class novasar:
                def __init__(self):

        # NovaSAR scene selector, corresponding to the pixel corners [az1 az2 sr1 sr2];

                        self.scene = [10700, 11600, 1200, 2400]
    
        # NovaSAR-1 patch coordinates

                        self.patch = [716, 866, 729, 809]


### TerraSAR-X configuration ###

        class terrasar:
                def __init__(self):

        # Select cols to read terrasar image (to crop and not read all image) 30253 is the maximum as it is the width of a stripmap image (2-6)

                        self.colnum = [1, 16000]

        # Select rows to read terrasar image (to crop and not read all image) 48218 is the maximum as it is the height of a stripmap image (2-6)

                        self.rownum = [1, 16000]

        # Name of the metadata file to read

                        self.metadata_filename = "TDX1_SAR__MGD_SE___SM_S_SRA_20180830T042304_20180830T042312"










