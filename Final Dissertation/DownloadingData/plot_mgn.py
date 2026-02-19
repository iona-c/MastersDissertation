
######################################################################
# --------------------------------------------------------
# Created by Gerard Gallardo i Peres
# ---------------------------------------------------------
# Objective: Produce and save the desired series of figures to support
# analysis of PhD resources
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

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap
mpl.rcParams.update(mpl.rcParamsDefault)
import cnf
cnf_data = vars(cnf.cnf())

def plot_mgn_SAR(data, dB, inf, path, filename,save=0,datatype="detected"):
    """
    This function plots sigma0 images (in dB) of Magellan SAR imagery (any cycle)
    Ad Astra
    """

    plt.rcParams["figure.figsize"] = (75,15)
    fig1 = plt.imshow(data,cmap=cnf_data['cmaps'][0], extent=[inf["corner_UL"][1], inf["corner_LR"][1],inf["corner_UL"][0], inf["corner_LR"][0]], origin='lower')
    ax = plt.gca()
    ax.set_ylim(ax.get_ylim()[::-1])
    ax.set_aspect('equal')
    ax.set_xlabel('Longitude')
    ax.set_ylabel('Latitude')
    cbar = plt.colorbar(fig1, shrink=0.65)
    
    if dB:
        cbar.set_label('[dB]')
        if datatype == "detected":
            ax.set_title("Magellan muhleman-corrected backscatter dB image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "multilook":
            ax.set_title("Magellan muhleman-corrected multilook backscatter dB image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "VISAR":
            ax.set_title("VISAR-simulated from Magellan muhleman-corrected backscatter dB image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "VenSAR":
            ax.set_title("VenSAR-simulated from Magellan muhleman-corrected backscatter dB image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        else:
            print("Error: data type not recognised")
    else:
        cbar.set_label('[linear]')
        if datatype == "detected":
            ax.set_title("Magellan muhleman-corrected backscatter image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "multilook":
            ax.set_title("Magellan muhleman-corrected multilook backscatter image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "VISAR":
            ax.set_title("VISAR-simulated from Magellan muhleman-corrected backscatter image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        elif datatype == "VenSAR":
            ax.set_title("VenSAR-simulated from Magellan muhleman-corrected backscatter image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])
        else:
            print("Error: data type not recognised")

    if cnf_data["show_plots"] == 1:
        plt.show(fig1)
    if save:
        plt.savefig(path + filename + "_" + inf["disk"][0:2] + "_" + inf["disk"][3:6] + "_mosaic_" + inf["mosaic"] + '.png', bbox_inches='tight', dpi=500)

    plt.close()

def plot_mgn_lons(data, inf, path, filename,save=0):
    """
    This function plots latitude images of SAR imagery
    Ad Astra
    """

    plt.rcParams["figure.figsize"] = (75,15)
    fig2 = plt.imshow(data,cmap=cnf_data['cmaps'][5], extent=[inf["corner_UL"][1], inf["corner_LR"][1],inf["corner_UL"][0], inf["corner_LR"][0]], origin='lower')
    ax = plt.gca()
    ax.set_ylim(ax.get_ylim()[::-1])
    ax.set_aspect('equal')
    #ax.set_xlabel('Longitude')
    #.set_ylabel('Latitude')
    cbar = plt.colorbar(fig2, shrink=0.65)
    cbar.set_label('Longitude [deg]')
    ax.set_title("Magellan longitude image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])

    if cnf_data["show_plots"] == 1:
        plt.show(fig2)
    if save:
        plt.savefig(path + filename + "_" + inf["disk"][0:2] + "_" + inf["disk"][3:6] + "_mosaic_" + inf["mosaic"] + "_longitude_" + '.png', bbox_inches='tight', dpi=500)

    plt.close()
    
def plot_mgn_lats(data, inf, path, filename,save=0):
    """
    This function plots latitude images of SAR imagery
    Ad Astra
    """

    plt.rcParams["figure.figsize"] = (75,15)
    fig3 = plt.imshow(data,cmap=cnf_data['cmaps'][5], extent=[inf["corner_UL"][1], inf["corner_LR"][1],inf["corner_UL"][0], inf["corner_LR"][0]], origin='lower')
    ax = plt.gca()
    ax.set_ylim(ax.get_ylim()[::-1])
    ax.set_aspect('equal')
    #ax.set_xlabel('Longitude')
    #.set_ylabel('Latitude')
    cbar = plt.colorbar(fig3, shrink=0.65)
    cbar.set_label('Latitude [deg]')
    ax.set_title("Magellan latitude image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])

    if cnf_data["show_plots"] == 1:
        plt.show(fig3)

    if save:
        plt.savefig(path + filename + "_" + inf["disk"][0:2] + "_" + inf["disk"][3:6] + "_mosaic_" + inf["mosaic"] + "_latitude_" + '.png', bbox_inches='tight', dpi=500)

    plt.close()
    
def plot_mgn_inc(data, inf, path, filename,save=0):
    """
    This function plots latitude images of SAR imagery
    Ad Astra
    """

    plt.rcParams["figure.figsize"] = (75,15)
    fig4 = plt.imshow(data,cmap=cnf_data['cmaps'][0], extent=[inf["corner_UL"][1], inf["corner_LR"][1],inf["corner_UL"][0], inf["corner_LR"][0]], origin='lower')
    ax = plt.gca()
    ax.set_ylim(ax.get_ylim()[::-1])
    ax.set_aspect('equal')
    ax.set_xlabel('Longitude')
    ax.set_ylabel('Latitude')
    cbar = plt.colorbar(fig4, shrink=0.65)
    cbar.set_label('Incidence angle [deg]')
    ax.set_title("Magellan incidence angle image from disk "+ inf["disk"] + " mosaic " + inf["mosaic"])

    if cnf_data["show_plots"] == 1:
        plt.show(fig4)
    #plt.show(fig4)
    if save:
        plt.savefig(path + filename + "_" + inf["disk"][0:2] + "_" + inf["disk"][3:6] + "_mosaic_" + inf["mosaic"] + "_incidence_" + '.png', bbox_inches='tight', dpi=500)

    plt.close()