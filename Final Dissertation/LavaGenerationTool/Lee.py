# Description: This script contains the implementation of the Lee Filter for SAR speckle reduction and edge preservation.
import numpy as np
def LeeFilter(Im, n, ENL, functionMode):
    """
    This function is a Lee Filter implementation for SAR speckle reduction and edge preservation,
    based on the MATLAB implementation of Kristian Sørensen, Eigil Lippert  and Simon Lupemba (DTU Space).
    Ad Astra
    """  
    dim = Im.shape  # dimensions of input matrix
    deadpixel = 0
    sigma_v = np.sqrt(1 / ENL)
    n = int(np.floor(n/2))

    # Local values with out mask
    E_norm = localMean(Im, n)
    Var_y_norm = localVariance(Im,E_norm, n)
    Var_x_norm = (Var_y_norm - E_norm**2*sigma_v**2)/(1+sigma_v**2)
    k_norm = Var_x_norm/Var_y_norm 
    k_norm[k_norm<0]=0
    CFAR_lee = CFAR(k_norm)
    threshold = np.mean(CFAR_lee)

    # Local values with mask
    Mask = compute_masks(Im, n, functionMode)
    Mask[k_norm<threshold] = -1  
    E = localMeanMask(Im, n, Mask)
    Var_y = localVarianceMask(Im,E, n,Mask)
    # Weighting 
    Var_x = (Var_y - E**2*sigma_v**2)/(1+sigma_v**2)
    k = Var_x/Var_y 
    k[k<0]=0
    #New image generation
    Im = E + k*(Im-E)
    Im[dim[0] - n:, :] = deadpixel
    Im[:n, :] = deadpixel
    Im[:, dim[1] - n:] = deadpixel
    Im[:, :n] = deadpixel

    Im = Im[(n):(-n),(n):(-n)]
    
    return Im
 

def compute_masks(Image, n, functionMode):
    # Get dimensions of Image
    dimensions = Image.shape

    # Initialize the M matrices with zeros
    M11, M12, M13, M21, M22, M23, M31, M32, M33 = 0, 0, 0, 0, 0, 0, 0, 0, 0

    # Iterate over k from -n to n
    for k in range(-n, n+1):

        # Horizontal displacements:

        # If k is less than 0, move elements to the left and add zeros on the left
        if k < 0:
            Eh = np.pad(Image[:, :dimensions[1]-abs(k)], ((0, 0), (abs(k), 0)), 'constant', constant_values=0)

        # If k is greater than 0, move elements to the right and add zeros on the right
        elif k > 0:
            Eh = np.pad(Image[:, k:dimensions[1]], ((0, 0), (0, k)), 'constant', constant_values=0)

        # If k is 0, keep Eh as the original Image
        else:
            Eh = Image

        # Iterate over i from -n to n
        for i in range(-n, n+1):

            # Vertical displacements:

            # If i is less than 0, move elements up and add zeros above
            if i < 0:
                Ev = np.pad(Eh[:dimensions[0]-abs(i), :], ((abs(i), 0), (0, 0)), 'constant', constant_values=0)

            # If i is greater than 0, move elements down and add zeros below
            elif i > 0:
                Ev = np.pad(Eh[i:dimensions[0], :], ((0, i), (0, 0)), 'constant', constant_values=0)

            # If i is 0, keep Ev as the current Eh
            else:
                Ev = Eh

            # Assign the pixels to the correct box as defined by Lee in the article
            if k <= -n+2:
                if i <= -n+2:
                    M11 += Ev
                if abs(i) <= 1:
                    M21 += Ev
                if i >= n-2:
                    M31 += Ev

            if abs(k) <= 1:
                if i <= -n+2:
                    M12 += Ev
                if abs(i) <= 1:
                    M22 += Ev
                if i >= n-2:
                    M32 += Ev

            if k >= n-2:
                if i <= -n+2:
                    M13 += Ev
                if abs(i) <= 1:
                    M23 += Ev
                if i >= n-2:
                    M33 += Ev

    # Create an array for masks filled with -1
    Mask = np.full(dimensions, -1)
    return Mask

    # Define functionMode (e.g., 'd' or 'r')

    if functionMode == 'd':  # difference

        # Assign masks based on the maximum difference
        Mask[np.logical_and.reduce((
            np.abs(M23 - M21) >= np.abs(M13 - M31),
            np.abs(M23 - M21) >= np.abs(M12 - M32),
            np.abs(M23 - M21) >= np.abs(M11 - M33)
        ))] = 0

        Mask[np.logical_and(Mask == 0, np.abs(M23 - M22) > np.abs(M21 - M22))] = 4

        Mask[np.logical_and.reduce((
            np.abs(M13 - M31) >= np.abs(M23 - M21),
            np.abs(M13 - M31) >= np.abs(M12 - M32),
            np.abs(M13 - M31) >= np.abs(M11 - M33)
        ))] = 1

        Mask[np.logical_and(Mask == 1, np.abs(M13 - M22) > np.abs(M31 - M22))] = 5

        Mask[np.logical_and.reduce((
            np.abs(M12 - M32) >= np.abs(M23 - M21),
            np.abs(M12 - M32) >= np.abs(M13 - M31),
            np.abs(M12 - M32) >= np.abs(M11 - M33)
        ))] = 2

        Mask[np.logical_and(Mask == 2, np.abs(M12 - M22) > np.abs(M32 - M22))] = 6

        Mask[np.logical_and.reduce((
            np.abs(M11 - M33) >= np.abs(M23 - M21),
            np.abs(M11 - M33) >= np.abs(M13 - M31),
            np.abs(M11 - M33) >= np.abs(M12 - M32)
        ))] = 3

        Mask[np.logical_and(Mask == 3, np.abs(M11 - M22) > np.abs(M33 - M22))] = 7

    if functionMode == 'r':

        # Make sure not to divide by zero
        M11[M11 == 0] = -1
        M12[M12 == 0] = -1
        M31[M31 == 0] = -1
        M21[M21 == 0] = -1
        M22[M22 == 0] = -1
        M23[M23 == 0] = -1
        M31[M31 == 0] = -1
        M32[M32 == 0] = -1
        M33[M33 == 0] = -1

        # Assign masks based on the maximum ratio
        mask_condition = np.logical_and.reduce((
            np.maximum(M23, M21) / np.minimum(M23, M21) >= np.maximum(M13, M31) / np.minimum(M13, M31),
            np.maximum(M23, M21) / np.minimum(M23, M21) >= np.maximum(M12, M32) / np.minimum(M12, M32),
            np.maximum(M23, M21) / np.minimum(M23, M21) >= np.maximum(M11, M33) / np.minimum(M11, M33)
        ))
        Mask[mask_condition] = 0

        mask_condition = np.logical_and(Mask == 0, np.maximum(M23, M22) / np.minimum(M23, M22) > np.maximum(M21, M22) / np.minimum(M21, M22))
        Mask[mask_condition] = 4

        mask_condition = np.logical_and.reduce((
            np.maximum(M13, M31) / np.minimum(M13, M31) >= np.maximum(M23, M21) / np.minimum(M23, M21),
            np.maximum(M13, M31) / np.minimum(M13, M31) >= np.maximum(M12, M32) / np.minimum(M12, M32),
            np.maximum(M13, M31) / np.minimum(M13, M31) >= np.maximum(M11, M33) / np.minimum(M11, M33)
        ))

        return Mask
    
def localMean(Image, n):
    
    # Assuming Image and n are defined
    deadpixel = 0

    # Get dimensions of Image
    dimensions = Image.shape

    # Calculate the number of pixels N
    N = (2 * n + 1) ** 2

    # Initialize variables
    E_sum = np.zeros(dimensions)
    E = np.zeros(dimensions)

    # Iterate over k from -n to n
    for k in range(-n, n + 1):

        # Horizontal displacements:

        # If k is less than 0, move elements to the left and add zeros on the left
        if k < 0:
            Eh = np.pad(Image[:, :dimensions[1] - abs(k)], ((0, 0), (abs(k), 0)), 'constant')

        # If k is greater than 0, move elements to the right and add zeros on the right
        elif k > 0:
            Eh = np.pad(Image[:, k:dimensions[1]], ((0, 0), (0, k)), 'constant')

        # If k is 0, keep Eh as the original Image
        else:
            Eh = Image

        # Iterate over i from -n to n
        for i in range(-n, n + 1):

            # Vertical displacements:

            # If i is less than 0, move elements up and add zeros above
            if i < 0:
                Ev = np.pad(Eh[:dimensions[0] - abs(i), :], ((abs(i), 0), (0, 0)), 'constant')

            # If i is greater than 0, move elements down and add zeros below
            elif i > 0:
                Ev = np.pad(Eh[i:dimensions[0], :], ((0, i), (0, 0)), 'constant')

            # If i is 0, keep Ev as the current Eh
            else:
                Ev = Eh

            # Add weights to E_sum
            E_sum = 1 / N * Ev + E_sum

    # Convert to uint8 to mimic behavior of setting pixels to 0
    if deadpixel == 1:
        E[:n, :] = 0
        E[dimensions[0] - n:, :] = 0
        E[:, :n] = 0
        E[:, dimensions[1] - n:] = 0

    # E_sum contains the result
    E = E_sum

    return E

def localVariance(Im, localMean, n):
    # Assuming Im and n are defined and localMean is the mean of the current pixel
    dim = Im.shape
    N = (2 * n + 1) ** 2
    V_sum = np.zeros(dim)
    V = np.zeros(dim)

    # Iterate over k from -n to n
    for k in range(-n, n + 1):

        # Horizontal displacements:

        # If k is less than 0, move elements to the left and add zeros on the left
        if k < 0:
            Vh = np.pad(Im[:, :dim[1] - abs(k)], ((0, 0), (abs(k), 0)), 'constant')

        # If k is greater than 0, move elements to the right and add zeros on the right
        elif k > 0:
            Vh = np.pad(Im[:, k:dim[1]], ((0, 0), (0, k)), 'constant')

        # If k is 0, keep Vh as the original Im
        else:
            Vh = Im

        # Iterate over i from -n to n
        for i in range(-n, n + 1):

            # Vertical displacements:

            # If i is less than 0, move elements up and add zeros above
            if i < 0:
                Vv = np.pad(Vh[:dim[0] - abs(i), :], ((abs(i), 0), (0, 0)), 'constant')

            # If i is greater than 0, move elements down and add zeros below
            elif i > 0:
                Vv = np.pad(Vh[i:dim[0], :], ((0, i), (0, 0)), 'constant')

            # If i is 0, keep Vv as the current Vh
            else:
                Vv = Vh

            # Calculate variance for each pixel
            V_sum = 1 / N * (Vv - localMean) ** 2 + V_sum

    # V_sum contains the result
    V = V_sum

    return V

def CFAR(img):

    procent = 1  # Set to 1 if nargin is 1

    r1 = [10.5100, 114.5100, 115.9800, 117.9800]  # water region
    r2 = [359.5100, 677.5100, 37.9800, 38.9800]  # bright field
    r3 = [500.5100, 743.5100, 67.9800, 75.9800]  # dark field

    # Crop regions and concatenate data
    data = np.concatenate([
        img[int(r1[1]):int(r1[1] + r1[3]), int(r1[0]):int(r1[0] + r1[2])].flatten(),
        img[int(r2[1]):int(r2[1] + r2[3]), int(r2[0]):int(r2[0] + r2[2])].flatten(),
        img[int(r3[1]):int(r3[1] + r3[3]), int(r3[0]):int(r3[0] + r3[2])].flatten()
    ])

    # Sort the data
    sorted_data = np.sort(data)

    # Calculate the index for the threshold
    len_data = len(sorted_data)
    index = round(len_data - len_data / 100 * procent)

    # Threshold value
    threshold = sorted_data[index]

    return threshold

def localMeanMask(Image, n, Mask):
    # Define masks as in Lee's article
    Mask0 = np.concatenate((np.zeros((n * 2 + 1, n)), np.ones((n * 2 + 1, n + 1))), axis=1)
    Mask1 = np.ones((2 * n + 1, 2 * n + 1))
    for r in range(2, 2 * n + 2):
        Mask1[r - 1, :] = np.concatenate((np.zeros(r - 1), np.ones(2 * n + 2 - r)))
    Mask2 = np.concatenate((np.ones((n + 1, n * 2 + 1)), np.zeros((n, n * 2 + 1))), axis=0)
    Mask3 = np.fliplr(Mask1)
    Mask4 = np.fliplr(Mask0)
    Mask5 = np.flipud(Mask3)
    Mask6 = np.flipud(Mask2)
    Mask7 = np.fliplr(Mask5)

    # Initialize variables
    dimensions = Image.shape  # Dimensions of the input matrix.
    E_sum = np.zeros(dimensions)  # Empty array for mean values.

    # For each iteration, move the array one step in the given direction.
    # Calculate the current mean for each iteration and add to E_sum.
    # Iterations work from left to right in the array.
    for k in range(-n, n + 1):
        # Horizontal displacements:
        # Move element to the left, adding zeros on the left if k is on the
        # left of the center array (-n).
        if k < 0:
            Eh = np.pad(Image[:, :dimensions[1] - abs(k)], ((0, 0), (abs(k), 0)), 'constant', constant_values=0)
        # Move element to the right, adding zeros on the right.
        elif k > 0:
            Eh = np.pad(Image[:, k:], ((0, 0), (0, k)), 'constant', constant_values=0)
        # When reaching the center element, do nothing.
        else:
            Eh = Image

        for i in range(-n, n + 1):
            # Vertical displacements:
            # Move elements up, adding zeros above.
            if i < 0:
                Ev = np.pad(Eh[:dimensions[0] - abs(i), :], ((abs(i), 0), (0, 0)), 'constant', constant_values=0)
            # Move elements down, adding zeros below.
            elif i > 0:
                Ev = np.pad(Eh[i:, :], ((0, i), (0, 0)), 'constant', constant_values=0)
            # When reaching the center element, do nothing.
            else:
                Ev = Eh

            x = k + n
            y = i + n

            E_sum[Mask == 0] = Mask0[y, x] / np.sum(np.sum(Mask0)) * Ev[Mask == 0] + E_sum[Mask == 0]
            E_sum[Mask == 1] = Mask1[y, x] / np.sum(np.sum(Mask1)) * Ev[Mask == 1] + E_sum[Mask == 1]
            E_sum[Mask == 2] = Mask2[y, x] / np.sum(np.sum(Mask2)) * Ev[Mask == 2] + E_sum[Mask == 2]
            E_sum[Mask == 3] = Mask3[y, x] / np.sum(np.sum(Mask3)) * Ev[Mask == 3] + E_sum[Mask == 3]
            E_sum[Mask == 4] = Mask4[y, x] / np.sum(np.sum(Mask4)) * Ev[Mask == 4] + E_sum[Mask == 4]
            E_sum[Mask == 5] = Mask5[y, x] / np.sum(np.sum(Mask5)) * Ev[Mask == 5] + E_sum[Mask == 5]
            E_sum[Mask == 6] = Mask6[y, x] / np.sum(np.sum(Mask6)) * Ev[Mask == 6] + E_sum[Mask == 6]
            E_sum[Mask == 7] = Mask7[y, x] / np.sum(np.sum(Mask7)) * Ev[Mask == 7] + E_sum[Mask == 7]

            # If the area is under threshold
            E_sum[Mask == -1] = 1 / (2 * n + 1) ** 2 * Ev[Mask == -1] + E_sum[Mask == -1]

    return E_sum

# Example usage:
# E = local_mean_mask(Image, 4, Mask)

def localVarianceMask(Im, localMeanMask, n, Mask):
    # Define masks as in Lee's article
    Mask0 = np.concatenate((np.zeros((n * 2 + 1, n)), np.ones((n * 2 + 1, n + 1))), axis=1)
    Mask1 = np.ones((2 * n + 1, 2 * n + 1))
    for r in range(2, 2 * n + 2):
        Mask1[r - 1, :] = np.concatenate((np.zeros(r - 1), np.ones(2 * n + 2 - r)))
    Mask2 = np.concatenate((np.ones((n + 1, n * 2 + 1)), np.zeros((n, n * 2 + 1))), axis=0)
    Mask3 = np.fliplr(Mask1)
    Mask4 = np.fliplr(Mask0)
    Mask5 = np.flipud(Mask3)
    Mask6 = np.flipud(Mask2)
    Mask7 = np.fliplr(Mask5)

    # Initialize values
    dim = Im.shape  # Dimensions of the input matrix.
    V_sum = np.zeros(dim)  # Empty array for variance values.

    for k in range(-n, n + 1):
        # Horizontal displacements:
        # Move element to the left, adding zeros on the left if k is on the
        # left of the center array (-n).
        if k < 0:
            Vh = np.pad(Im[:, :dim[1] - abs(k)], ((0, 0), (abs(k), 0)), 'constant', constant_values=0)
        # Move element to the right, adding zeros on the right.
        elif k > 0:
            Vh = np.pad(Im[:, k:], ((0, 0), (0, k)), 'constant', constant_values=0)
        # When reaching the center element, do nothing.
        else:
            Vh = Im

        for i in range(-n, n + 1):
            # Vertical displacements:
            # Move elements up, adding zeros above.
            if i < 0:
                Vv = np.pad(Vh[:dim[0] - abs(i), :], ((abs(i), 0), (0, 0)), 'constant', constant_values=0)
            # Move elements down, adding zeros below.
            elif i > 0:
                Vv = np.pad(Vh[i:, :], ((0, i), (0, 0)), 'constant', constant_values=0)
            # When reaching the center element, do nothing.
            else:
                Vv = Vh

            x = k + n
            y = i + n

            V_sum[Mask == 0] = (Mask0[y, x] / np.sum(np.sum(Mask0))) * (Vv[Mask == 0] - localMeanMask[Mask == 0])**2 + V_sum[Mask == 0]
            V_sum[Mask == 1] = (Mask1[y, x] / np.sum(np.sum(Mask1))) * (Vv[Mask == 1] - localMeanMask[Mask == 1])**2 + V_sum[Mask == 1]
            V_sum[Mask == 2] = (Mask2[y, x] / np.sum(np.sum(Mask2))) * (Vv[Mask == 2] - localMeanMask[Mask == 2])**2 + V_sum[Mask == 2]
            V_sum[Mask == 3] = (Mask3[y, x] / np.sum(np.sum(Mask3))) * (Vv[Mask == 3] - localMeanMask[Mask == 3])**2 + V_sum[Mask == 3]
            V_sum[Mask == 4] = (Mask4[y, x] / np.sum(np.sum(Mask4))) * (Vv[Mask == 4] - localMeanMask[Mask == 4])**2 + V_sum[Mask == 4]
            V_sum[Mask == 5] = (Mask5[y, x] / np.sum(np.sum(Mask5))) * (Vv[Mask == 5] - localMeanMask[Mask == 5])**2 + V_sum[Mask == 5]
            V_sum[Mask == 6] = (Mask6[y, x] / np.sum(np.sum(Mask6))) * (Vv[Mask == 6] - localMeanMask[Mask == 6])**2 + V_sum[Mask == 6]
            V_sum[Mask == 7] = (Mask7[y, x] / np.sum(np.sum(Mask7))) * (Vv[Mask == 7] - localMeanMask[Mask == 7])**2 + V_sum[Mask == 7]

            # If area is under threshold
            V_sum[Mask == -1] = (1 / (2 * n + 1)**2) * (Vv[Mask == -1] - localMeanMask[Mask == -1])**2 + V_sum[Mask == -1]

    V = V_sum

    return V