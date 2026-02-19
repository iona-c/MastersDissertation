import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import Lee
import math
from scipy.signal import savgol_filter
from scipy.ndimage import median_filter
import skimage as ski
from sklearn import mixture, metrics
from sklearn.cluster import KMeans
from skimage.segmentation import flood, flood_fill

def anti_pepper(array,visualise):
    """ This function takes in a array of the radar image.
        A heatmap of the origional image and gaussian and lee filtered image is plotted
        It returns two arrays, the denoised Gaussian and Lee filtered
    """
    gaus_lava =ski.filters.gaussian(array,sigma=5)


    #lee filter
    y =array.shape[0]
    x=array.shape[1]
    y1 =int(np.round(y/2))
    x1=int(np.round(x/2))
    crop =array[y1:y1+100,x1:x1+100]
    sigma =np.var(crop)
    mew =np.mean(crop)
    ENL =(mew**2)/sigma
    lee_img =Lee.LeeFilter(array,n=5,ENL=ENL,functionMode='d')
    

    if visualise:
        fig, axes =plt.subplots(1,3, figsize =(25,7))
        axes[0] =sns.heatmap(array,ax =axes[0])
        axes[0].set_title('Origional')
        axes[1] =sns.heatmap(gaus_lava, ax=axes[1])
        axes[1].set_title('Guassian')
        axes[2].set_title('Lee filter')
        axes[2]=sns.heatmap(lee_img)


    return gaus_lava, lee_img



def classification(array, n_clusters, visualise):
    """
    Classifies the input array into clusters using Gaussian Mixture Model and KMeans clustering, and optionally visualizes the results.

    Parameters:
    -----------
    array : np.ndarray
        The input 2D array to be classified.
    n_clusters : int
        The number of clusters to form.
    visualise : bool
        Whether to visualize the results using heatmaps and KDE plots.

    Returns:
    --------
    clustered : np.ndarray
        The array with the same shape as input, where each element is replaced by its cluster label from Gaussian Mixture Model.
    classy : np.ndarray
        The array with the same shape as input, where each element is replaced by its cluster label from KMeans.
    lava_cluster_kmeans : int
        The cluster label corresponding to lava-like values from KMeans.
    lava_cluster_gm : int
        The cluster label corresponding to lava-like values from Gaussian Mixture Model.
    """
    # Gaussian Mixture Classification
    flat = array.flatten()
    flat2d = flat.reshape(-1, 1)
    gmm = mixture.GaussianMixture(n_components=n_clusters, random_state=42)
    gmm.fit(flat2d)
    clusters = gmm.predict(flat2d)
    df = pd.DataFrame({'Data': flat2d.ravel(), 'Cluster': clusters})
    gmm_clustered = df['Cluster'].values
    gmm_clustered = gmm_clustered.reshape(array.shape[0], array.shape[1])

    # KMeans Classification
    kmeans = KMeans(n_clusters=n_clusters, random_state=0, n_init="auto").fit(flat2d)
    klabels = kmeans.labels_
    kcenters = kmeans.cluster_centers_.flatten()
    kmean_clustered = klabels.reshape(array.shape[0], array.shape[1])
    #Selecting the lava cluster
    for i in range(len(flat2d)):
        if -9.5<flat2d[i]<-8.5:
            lava_cluster_kmeans =klabels[i]
            break
    for i in range(len(flat2d)):    
        if -9.5<df['Data'][i]<-8.5:
            lava_cluster_gm =df['Cluster'][i]
            break

 
    if visualise:
        fig, axes = plt.subplots(2, 2, figsize=(15, 15))
        # plotting the mixed gaussian
        sns.kdeplot(data=df, x='Data', hue='Cluster', fill=True, palette='viridis', ax=axes[0, 0])
        sns.kdeplot(flat, ax=axes[0, 0])
        axes[0, 0].set_title('GMM Histogram of the clusters')
        sns.heatmap(gmm_clustered, cmap='mako', ax=axes[0, 1])
        axes[0, 1].set_title('GMM Classified Image')
        
        # plotting the Kmeans
        for i in range(n_clusters):
            sns.kdeplot(flat2d[klabels == i].flatten(), label=f'Cluster {i}', ax=axes[1, 0])
        
        for center in kcenters:
            axes[1, 0].axvline(center, ls='--')

        sns.kdeplot(array.flatten(), label='Original', ax=axes[1, 0])
        sns.heatmap(kmean_clustered,cmap='mako', ax=axes[1, 1])
        axes[1, 0].set_title('Kmeans Histogram of the clusters')
        axes[1, 1].set_title('Kmeans Classified Image')
        axes[1,0].legend()
        plt.show()
    
    return gmm_clustered, kmean_clustered, lava_cluster_kmeans, lava_cluster_gm


def booleing(thresh, array,thresh_equal=False):
    """
    Splits the image into 1 or 0 depending on the threshold, to make a mask of the values = to 1

    Parameters:
    -----------
    array : np.ndarray
        The input 2D array to be converted to a mask.
    thresh : int or list of int
        the values of which the image is thresholded.
    thresh_equal : bool
        this is made true if you want to split the array into whether or not a pixel has the specific value of the given threshold

    Returns:
    --------
    bin_lava : np.ndarray
        the mask of the origional array, made black and white
    """
    flat_GL = array.flatten().tolist()
    bin_lava = []
    if thresh_equal:
        for i in flat_GL:
            if i==thresh:
                bin_value = 1
            else:
                bin_value = 0
            bin_lava.append(bin_value)
    else:
        for i in flat_GL:
            if isinstance(thresh, list) and len(thresh) == 2:
                if thresh[0] < i < thresh[1]:
                    bin_value = 1
                else:
                    bin_value = 0
            else:
                if i > thresh:
                    bin_value = 1
                else:
                    bin_value = 0
            bin_lava.append(bin_value)
    
    bin_lava = np.array(bin_lava)
    bin_lava = bin_lava.reshape(array.shape)
    return bin_lava


def cropping_img(image,grid_size):
    """
    crops the image to fit a perfect number of squares of the given grid_size.
    
    Parameters:
    -----------
    array : np.ndarray
        The input 2D array to be cropped.
    grid_size : int
        The size that the grid squares would be

    Returns:
    --------
    img_crop : np.ndarray
        array cropped to be the size that fits the size of grid squares
    """
    y_len =image.shape[0]
    x_len =image.shape[1]

    no_y_img =int(np.floor(y_len/grid_size))
    no_x_img =int(np.floor(x_len/grid_size))
    print('size of new image', no_y_img*grid_size,',',no_x_img*grid_size)
    img_crop =image[:(no_y_img*grid_size),:(no_x_img*grid_size)]
    print(img_crop.shape)
    assert (no_y_img * grid_size == img_crop.shape[0]) and (no_x_img * grid_size == img_crop.shape[1])
    return img_crop


def list_of_mini_img(img,img_size):
    """
    splits the large array into squares and puts them in a list
    
    Parameters:
    -----------
    img : np.ndarray
        the image that will be split
    img_size : int
        The size that the grid squares would be

    Returns:
    --------
    list_of_imgs: list
        a list of pn.ndarray of the size img_size
    """
    #checking that the image size is a whole number of squares
    assert (img.shape[0]%img_size ==0) and (img.shape[1]%img_size==0)
    list_of_imgs =[]
    for i in range(0,img.shape[0],img_size):
        for j in range(0,img.shape[1],img_size):
            mini=img[i:i+img_size,j:j+img_size]
            list_of_imgs.append(mini)
    return list_of_imgs

def lava_seperate(Gaus_array, lee_array, orig_array):
    """
    This function plots the different histograms of the pixel values
    this is a visualising function

    Parameters:
    -----------
    Gaus_array : np.ndarray
        the image array but with a Gaussian noise removing filter applied
    lee_array : np.ndarray
        the image array but with a Lee noise removing filter applied
    orig_array : np.ndarray
        the radar image
    """
    Gaus =Gaus_array.flatten()
    lee =lee_array.flatten()
    orig =orig_array.flatten()
    sns.kdeplot(lee, label='lee_filter')
    sns.kdeplot(Gaus, label='gaussian')
    sns.kdeplot(orig, label='origional')
    plt.title('kd plot of pixel values')
    plt.grid(True)
    plt.legend()
    plt.show()


def sorting_imgs(bin_samples, raw_samples,thresh0,thresh1,img_size):
    """
    sorts the list of image squares into ones that are covering the lava flow and the ones that arent
    by working if the bin_samples are pixels of 0 or 1 in the grid shape. It also shows the histogram 
    of the sum of each square with gives the proportion that sqaure is covering the lava areas
    
    Parameters:
    -----------
    bin_samples : np.ndarray
        the image that will be split
    raw_samples : np.ndarray
        the image that will be split
    thresh0 : int
        the minimal number of 0 valued pixels in the grid to be classed as a 0 square
    thresh1 : int
        the minimal number of 1 valued pixels in the grid to be classed as a 1 square
    img_size : int
        The size that the grid squares would be

    Returns:
    --------
    sample_score0: list
        a list of pn.ndarray of the size img_size, where the pixel values are 0
    sample_score1: list
        a list of pn.ndarray of the size img_size, where the pixel values are 1
    lava_samplesO: list
        a list of pn.ndarray of the squares which are covering the lava flow areas
        which is speficied for the images that are in sample_score1
    """
    #seperating the images into lists of areas that cover only the lava and other bits that just cover the ground
    sample_score0 =[]
    sample_score1 =[]
    for i in range(len(bin_samples)):
        sum =np.sum(bin_samples[i])
        if sum==0:
            sample_score0.append((sum, i))
        else:
            ave =sum/(img_size*img_size) #remember brackets
            if ave <= thresh0:
                sample_score0.append((ave,i))
            elif ave > thresh1:
                sample_score1.append((ave, i))

    #plotting the average score of the images
    vals =(i[0] for i in sample_score1)
    vals =list(vals)
    zeros =(i[0] for i in sample_score0)
    zeros =list(zeros)
    #making the plot
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.hist(zeros, bins=30, color='blue', alpha=0.7)
    plt.title('Histogram of Zeros')
    plt.xlabel('Value')
    plt.ylabel('Frequency')
    plt.subplot(1, 2, 2)
    plt.hist(vals, bins=30, color='green', alpha=0.7)
    plt.title('Histogram of Vals')
    plt.xlabel('Value')
    plt.ylabel('Frequency')
    plt.tight_layout()
    plt.show()

    #collecting the origional image lava samples
    indexes =(i[1] for i in sample_score1)
    indexes =list(indexes)
    lava_samplesO =[]
    for i in indexes:
        lava_samplesO.append(raw_samples[i])
        
    return sample_score0, sample_score1, lava_samplesO

def crop_to_match(smaller_array, larger_array):
    # Get the dimensions of both arrays
    small_shape = smaller_array.shape
    large_shape = larger_array.shape
    
    # Calculate the starting indices to crop the larger array
    start_idx = [(large_dim - small_dim) // 2 for small_dim, large_dim in zip(small_shape, large_shape)]
    
    # Calculate the ending indices to crop the larger array
    end_idx = [start + small_dim for start, small_dim in zip(start_idx, small_shape)]
    
    # Slice the larger array to the same shape as the smaller array
    cropped_array = larger_array[start_idx[0]:end_idx[0], start_idx[1]:end_idx[1]]
    
    return cropped_array


def find_backscatter(origional_array,cluster_array,coordinate, visualise=False):
    """
    Finds and calculates the average backscatter value of a specified lava flow in a radar image.

    Parameters:
    original_array (np.ndarray): The original radar image array.
    cluster_array (np.ndarray): The clustered image array where different clusters are represented by unique values.
    coordinate (tuple): The (x, y) coordinate used to start the flood fill operation.

    Returns:
    float: The average backscatter value of the pixels in the specified cluster.
    float: The standard deviation of the backscatter value of the pixels in the specified cluster
    """
    flooded =flood_fill(cluster_array,(coordinate[1],coordinate[0]),-30)#-30 is chosen as it wont be already in use
    #the coordinates in fllod_fill is y,x not x,y as it follows numpy not cartiesian
    lava_values =[]
    if origional_array.shape[0]*origional_array.shape[1] > cluster_array.shape[0]*cluster_array.shape[1]:
        new_origional =crop_to_match(cluster_array,origional_array)
        origional_array=new_origional
    elif origional_array.shape[0]*origional_array.shape[1] < cluster_array.shape[0]*cluster_array.shape[1]:
        new_cluster =crop_to_match(cluster_array,origional_array)
        cluster_array=new_cluster
    else:
        pass
    
    assert origional_array.shape == cluster_array.shape
    for i in range(origional_array.shape[0]):
        for j in range(origional_array.shape[1]):
            if flooded[i,j]==-30:
                pix_val=origional_array[i,j]
                lava_values.append(pix_val)
            else:
                pass
    print('Number of lava pixels: ',len(lava_values))
    print('Average backscatter value ',np.mean(lava_values))
    print('standard deviation',np.std(lava_values))
    if visualise:
        fig, axes =plt.subplots(1,2, figsize =(17,7))
        axes[0]=sns.heatmap(flooded, ax=axes[0])
        axes[0].plot(coordinate[0],coordinate[1], marker='*', markersize=10)
        axes[0].set_title('Image of Selected Lava flow')
        axes[1]=sns.kdeplot(lava_values, ax=axes[1])
        axes[1].set_title('The Lava Flow Pixel Values')
    else:
        pass
    mask = (flooded==-30)
    return np.mean(lava_values), np.std(lava_values), lava_values, mask

def calculate_enl(db_image, region_mask):
    """
    Calculate the Equivalent Number of Looks (ENL) for a radar image in dB scale.

    Parameters:
    - db_image: 2D NumPy array, the radar image with backscatter coefficients in dB.
    - region_mask: 2D NumPy array (same size as db_image), boolean mask where True indicates the homogeneous region.

    Returns:
    - ENL value for the specified region.
    """

    # Convert the dB image to linear scale
    linear_image = 10 ** (db_image / 10)

    # Select the region defined by the mask
    homogeneous_region = linear_image[region_mask]

    # Calculate the mean and variance of the selected region
    mean_intensity = np.mean(homogeneous_region)
    variance_intensity = np.var(homogeneous_region)

    # Calculate the ENL
    enl = (mean_intensity ** 2) / variance_intensity

    return enl
    




