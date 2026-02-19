######################################################################
# --------------------------------------------------------
# Created by Gerard Gallardo i Peres
# ---------------------------------------------------------
# Objective: Function to read Magellan Full Resolution Mosaicked
# Image Data records (F-MIDR), as well as compressed versions (C-MIDR)
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
from setup import cnf
from setup import chd
import math
from scipy.signal import savgol_filter

def read_mgn_SAR_img(path,cnf_data,table):

    path = "20240528T081310603689\cartOrder\magellan_midr\data\fmidr\fmidr_05s054_201.img"
    #cnf_data = vars(cnf.cnf())
    #table = vars(chd.table_muhleman_c1())

    # Matrix initialization

    pi = np.pi
    Data_info = {}

    Data_img_original = []
    Data_img_original_aux = []

    Data_img_dB_corrected = []
    Data_img_dB_corrected_aux = []

    Data_img_corrected = []
    Data_img_corrected_aux = []

    lat_matrix = []
    lat_matrix_aux2 = []
    lon_matrix = []
    lon_matrix_aux = []
    lon_matrix_aux2 = []

    inc_matrix_aux_2 = []
    inc_matrix_aux = []
    inc_matrix = []

    # SAR data initialization

    lbl1 = path.find("c1")
    lbl2 = path.find("mg_")

    if lbl1 != -1:
        lbl_aux = 'c1f'
        Data_info["type"] = "C1-MIDR"
        Data_info["mosaic"] = path[lbl2+8:lbl2+15]
    else:
        lbl_aux = 'ff'
        Data_info["type"] = "F-MIDR"
        Data_info["mosaic"] = path[lbl2+8:lbl2+15]

    Data_info["disk"] = path[lbl2:lbl2+7]

    # generate polyfits (9th degree) of the entire table values, before iteration
    lats = table["lat"]
    incs = table["inc"]
    factors = table["factor"]
    inc_interp = np.polyfit(lats,incs,9) 
    fact_interp = np.polyfit(lats,factors,9)

    for i in range(1, cnf_data["mgn"].framelet_num+1):

        if i < 10:
            fid=open(path+lbl_aux+"0"+str(i)+".img","rb")
        else:
            fid=open(path+lbl_aux+str(i)+".img","rb")


        headerendpos_ascii=fid.tell()
        header = str(fid.readline().rstrip())
        #header=fid.fgetl()

        lat_init_idx = header.find("LAT_UL")
        length_aux_idx = header.find("LON_UL")
        lat_UL = float(header[(lat_init_idx+7):(length_aux_idx-3)])
        lat_end_idx = header.find("LAT_LR")
        length_aux_idx = header.find("LON_LR")
        lat_LR = float(header[(lat_end_idx+7):(length_aux_idx-3)])

        lon_init_idx = header.find("LON_UL")
        length_aux_idx = header.find("LAT_LL")
        lon_UL = float(header[(lon_init_idx+7):(length_aux_idx-3)])
        lon_end_idx = header.find("LON_LR")
        length_aux_idx = header.find("PROJ_LON")
        lon_LR = float(header[(lon_end_idx+7):(length_aux_idx-3)])

        Data_info["Middle_lat"] = lat_LR + (lat_UL - lat_LR)/2
        Data_info["Middle_lon"] = lon_LR + (lon_UL - lon_LR)/2

        PIXSIZ_idx = header.find("PIXSIZ")
        length_idx_aux = header.find("PRODUCT")
        PIXSIZ_SAR = float(header[(PIXSIZ_idx+7):(length_idx_aux-2)])

        Data_info["PIXSIZ"] = PIXSIZ_SAR

        if cnf_data["mgn"].SAR_projection == "sinus":

            PROJSAMP_idx = header.find("PROJSAMP")
            if header[PROJSAMP_idx+9] == '0':
                PROJSAMP = 0    
            else:
                PROJSAMP = float(header[(PROJSAMP_idx+9):(PROJSAMP_idx+14)])

            SPECLINE_idx = header.find("SPECLINE")

            if header[SPECLINE_idx+9] == '0':
                SPECLINE = 0   
            else:
                SPECLINE = float(header[(SPECLINE_idx+9):(SPECLINE_idx+15)])

            PROJ_LON_idx = header.find("PROJ_LON")
            PROJ_LON = float(header[(PROJ_LON_idx+9):(SPECLINE_idx-3)])

            d = 6051000/PIXSIZ_SAR

            x_aux = np.linspace(1,1024,1024)
            y_aux = np.linspace(1,1024,1024)


            lon_matrix_aux = np.zeros((1024,1024))
            lat_col = (y_aux.transpose() + 0.5 -SPECLINE)/-d*180/pi
            lat_matrix_aux = np.tile(lat_col,(1024,1))
            for k in range(0,len(x_aux)):
                lon_matrix_aux[:,k] = ((x_aux[k]+0.5-PROJSAMP)/(np.cos(lat_col*pi/180)*d)*180/pi + PROJ_LON)
            lon_matrix_aux = np.where(lon_matrix_aux < 0, 360 + lon_matrix_aux, lon_matrix_aux)
                #lon_row = lon_row.reshape(1,len(lon_row))
                #if k == 0:
                    #lon_matrix_aux = lon_row
                #else:
                    #lon_matrix_aux = np.concatenate((lon_matrix_aux,lon_row),axis=0)
                #for k in range(0,len(x_aux)):
                    #if lon_row[0,k-1].any() < 0:
                        #lon_row[0,k-1] = 360 + lon_row[0,k-1]

        if cnf_data["mgn"].SAR_projection == "linear":

            lat_col = np.flip(np.linspace(lat_LR,lat_UL,1024))
            lat_matrix_aux = np.tile(lat_col,(1024,1))

            lon_row = np.flip(np.linspace(lon_LR,lon_UL,1024))
            lon_matrix_aux = np.tile(lon_row,(1024,1))

        reading_pos = fid.seek(1050624 - 1024*1024) # file length - the size of the image in uint8 bytes
        image = np.fromfile(fid,"uint8",1048576,"")
        image = image.reshape(1024,1024)

        #inc_col = np.zeros(1024)
        #factor_col = np.zeros(1024)
        #for jj in range(0,1024):
        
            #matrix_aux = np.absolute(lat_matrix_aux[0,jj] - table["lat"])
            #idx_aux_lat = matrix_aux.argmin() 
            #if table["lat"][idx_aux_lat] > lat_matrix_aux[0,jj]:
                #interp_factor = (lat_matrix_aux[0,jj] - table["lat"][idx_aux_lat - 1])/(table["lat"][idx_aux_lat] - table["lat"][idx_aux_lat - 1])
                #inc_col[jj] = table["inc"][idx_aux_lat] + (table["inc"][idx_aux_lat] - table["inc"][idx_aux_lat - 1])*interp_factor
                #factor_col[jj] = table["factor"][idx_aux_lat] + (table["factor"][idx_aux_lat] - table["factor"][idx_aux_lat - 1])*interp_factor
            #else:
                #interp_factor = (table["lat"][idx_aux_lat + 1] - lat_matrix_aux[0,jj])/(table["lat"][idx_aux_lat + 1] - table["lat"][idx_aux_lat])
                #inc_col[jj] = table["inc"][idx_aux_lat] + (table["inc"][idx_aux_lat + 1] - table["inc"][idx_aux_lat])*interp_factor
                #factor_col[jj] = table["factor"][idx_aux_lat] + (table["factor"][idx_aux_lat + 1] - table["factor"][idx_aux_lat])*interp_factor          

        #inc_interp = np.polyfit(lat_col,inc_col,7) # fit polynomial of degree 7
        #fact_interp = np.polyfit(lat_col,factor_col,7) # fit polynomial of degree 7
        #factor_matrix = np.tile(factor_col,(1024,1))
        #inc_matrix_aux = np.tile(factor_col,(1024,1))
        #inc_matrix_aux = 0.00008*lat_matrix_aux**3 - 0.0127*lat_matrix_aux**2 + 0.1825*lat_matrix_aux + 45.665 # Quantitative Analysis of Venus Radar Backscatter Data in ArcGIS, 2005 First, the best-fit approximation used creates an error that is everywhere < 2%, use for low latitudes
        #inc_matrix_aux = inc_interp[0]*lat_matrix_aux**(7) + inc_interp[1]*lat_matrix_aux**(6) +inc_interp[2]*lat_matrix_aux**(5) +inc_interp[3]*lat_matrix_aux**(4) + inc_interp[4]*lat_matrix_aux**(3) + inc_interp[5]*lat_matrix_aux**(2) + inc_interp[6]*lat_matrix_aux + inc_interp[7] #Use for high latitudes (away from equator)
        #factor_matrix = fact_interp[0]*lat_matrix_aux**(7) + fact_interp[1]*lat_matrix_aux**(6) +fact_interp[2]*lat_matrix_aux**(5) +fact_interp[3]*lat_matrix_aux**(4) + fact_interp[4]*lat_matrix_aux**(3) + fact_interp[5]*lat_matrix_aux**(2) + fact_interp[6]*lat_matrix_aux + fact_interp[7] #Quantitative Analysis of Venus Radar Backscatter Data in ArcGIS, 2005 First, the best-fit approximation used creates an error that is everywhere < 2%.

        inc_matrix_aux = inc_interp[0]*lat_matrix_aux**(9) + inc_interp[1]*lat_matrix_aux**(8) + inc_interp[2]*lat_matrix_aux**(7) + inc_interp[3]*lat_matrix_aux**(6) + inc_interp[4]*lat_matrix_aux**(5) + inc_interp[5]*lat_matrix_aux**(4) + inc_interp[6]*lat_matrix_aux**(3) + inc_interp[7]*lat_matrix_aux**(2) + inc_interp[8]*lat_matrix_aux + inc_interp[9]
        factor_matrix = fact_interp[0]*lat_matrix_aux**(9) + fact_interp[1]*lat_matrix_aux**(8) +fact_interp[2]*lat_matrix_aux**(7) +fact_interp[3]*lat_matrix_aux**(6) + fact_interp[4]*lat_matrix_aux**(5) + fact_interp[5]*lat_matrix_aux**(4) + fact_interp[6]*lat_matrix_aux**(3) + fact_interp[7]*lat_matrix_aux**(2) + fact_interp[8]*lat_matrix_aux + fact_interp[9]
    

        # if table["type"] == "left-looking":
        #     inc_matrix_aux = 0.00008*lat_matrix_aux**(3) - 0.0127*lat_matrix_aux**(2) + 0.1825*lat_matrix_aux + 45.665 #Quantitative Analysis of Venus Radar Backscatter Data in ArcGIS, 2005 First, the best-fit approximation used creates an error that is everywhere < 2%.
        # if table["type"] == "right-looking":
        #     inc_matrix_aux = np.tile(inc_col,(1024,1))
            
        image_sig0_aux = 10**(0.1*(-20+(image-1)/5))
        image_sig0_aux2 = np.multiply(image_sig0_aux,(0.0118*np.cos((inc_matrix_aux+0.5)*pi/180)))
        image_sig0_aux3 = (np.sin((inc_matrix_aux+0.5)*pi/180)+0.111*np.cos((inc_matrix_aux+0.5)*pi/180))**3
        image_sig0 = np.divide(image_sig0_aux2,image_sig0_aux3)
        image_sig0_dB = -20+(image-1)/5 + factor_matrix
        #image_sig0_dB = 10*np.log10(image_sig0)

        #lat_matrix_aux = lat_matrix_aux.transpose()
        lon_matrix_aux = lon_matrix_aux.transpose()
        #inc_matrix_aux = inc_matrix_aux.transpose()
        image = image.transpose()
        image_sig0 = image_sig0.transpose()
        image_sig0_dB = image_sig0_dB.transpose()

        if i == 1 or i % 8 == 1:
            Data_img_original_aux = image
            Data_img_corrected_aux = image_sig0
            Data_img_dB_corrected_aux = image_sig0_dB
            lat_matrix_aux2 = lat_matrix_aux
            lon_matrix_aux2 = lon_matrix_aux
            inc_matrix_aux2 = inc_matrix_aux
        else:
            Data_img_original_aux = np.vstack((Data_img_original_aux,image))
            Data_img_corrected_aux = np.vstack((Data_img_corrected_aux,image_sig0))
            Data_img_dB_corrected_aux = np.vstack((Data_img_dB_corrected_aux,image_sig0_dB))
            lat_matrix_aux2 = np.vstack((lat_matrix_aux2,lat_matrix_aux))
            lon_matrix_aux2 = np.vstack((lon_matrix_aux2,lon_matrix_aux))
            inc_matrix_aux2 = np.vstack((inc_matrix_aux2,inc_matrix_aux))
    
        lon_matrix_aux = []


        size_aux = np.shape(Data_img_original_aux)
        if np.floor(size_aux[0]/1024) == 8 and i == 8:
                Data_img_original = Data_img_original_aux
                Data_img_corrected = Data_img_corrected_aux
                Data_img_dB_corrected = Data_img_dB_corrected_aux
                lat_matrix = lat_matrix_aux2
                lon_matrix = lon_matrix_aux2
                inc_matrix = inc_matrix_aux2    
                Data_img_original_aux = []
                Data_img_dB_corrected_aux = []
                Data_img_corrected_aux = []
                lat_matrix_aux2 = []
                lon_matrix_aux2 = []
                inc_matrix_aux2 = []
        elif np.floor(size_aux[0]/1024) == 8 and i != 8:
                Data_img_original = np.hstack((Data_img_original,Data_img_original_aux))
                Data_img_corrected = np.hstack((Data_img_corrected,Data_img_corrected_aux))
                Data_img_dB_corrected = np.hstack((Data_img_dB_corrected,Data_img_dB_corrected_aux))
                lat_matrix = np.hstack((lat_matrix,lat_matrix_aux2))
                lon_matrix = np.hstack((lon_matrix,lon_matrix_aux2))
                inc_matrix = np.hstack((inc_matrix,inc_matrix_aux2))
                Data_img_original_aux = []
                Data_img_dB_corrected_aux = []
                Data_img_corrected_aux = []
                lat_matrix_aux2 = []
                lon_matrix_aux2 = []
                inc_matrix_aux2 = []



        fid.close()


    Data_img_original = Data_img_original.transpose()
    Data_img_dB_corrected = Data_img_dB_corrected.transpose()
    Data_img_corrected = Data_img_corrected.transpose()
    lat_matrix = lat_matrix.transpose()
    lon_matrix = lon_matrix.transpose()
    inc_matrix = inc_matrix.transpose()
    lat_matrix = np.sort(lat_matrix, axis=1)
    lon_matrix = np.sort(lon_matrix, axis=0)

    inc_matrix = inc_matrix[np.flip(np.argsort(inc_matrix[:,0])),:] # Find a sorted order of the given labels

    if cnf_data["mgn"].SAR_projection == "sinus": 
        
        lon_matrix[lon_matrix < 0] = np.nan
        lon_matrix[lon_matrix > 360] = np.nan

    # In case no specific 1024x1024 framelet is selected, use the whole lat/lon array of the framelet
    #linearizing from the corners, to avoid the discontinuity at the edges of the framelets

    if cnf_data['mgn'].framelet_selection == 0:
    # lon_matrix = savgol_filter(lon_matrix, 101, 3,axis=1)
    # lat_matrix = savgol_filter(lat_matrix, 101, 3,axis=0)
        lat_matrix_aux = np.flip(np.linspace(lat_matrix[-1,-1],lat_matrix[0,0],7168))
        lat_matrix= np.tile(lat_matrix_aux,(8192,1)).T
        lon_matrix_aux = np.flip(np.linspace(lon_matrix[-1,-1],lon_matrix[0,0],8192))
        lon_matrix= np.tile(lon_matrix_aux,(7168,1))

    # Select a specific framelet within the loaded MIDR product, if required

    if cnf_data['mgn'].framelet_selection != 0:
        framelet_row = (cnf_data['mgn'].framelet_selection - 1) // 8
        framelet_col = (cnf_data['mgn'].framelet_selection - 1) - framelet_row * 8

        Data_img_original = Data_img_original[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
        Data_img_dB_corrected = Data_img_dB_corrected[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
        Data_img_corrected = Data_img_corrected[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
        inc_matrix = inc_matrix[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
        lat_matrix = lat_matrix[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
        lon_matrix = lon_matrix[
            framelet_row * 1024 : framelet_row * 1024 + 1024,
            framelet_col * 1024 : framelet_col * 1024 + 1024
        ]
    

    Data_info["corner_UL"] = [lat_matrix[0,0], lon_matrix[0,0]]
    Data_info["corner_LR"] = [lat_matrix[-1,-1], lon_matrix[-1,-1]]

    return Data_img_original, Data_img_corrected, Data_img_dB_corrected, lat_matrix, lon_matrix, inc_matrix, Data_info



    