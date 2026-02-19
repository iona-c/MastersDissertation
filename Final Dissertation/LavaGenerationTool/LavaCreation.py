import seaborn as sns
import matplotlib.pyplot as plt
import random
from polygenerator import (
    random_polygon,
    random_star_shaped_polygon,
    random_convex_polygon,
)
from scipy.special import comb
import numpy as np
import segmentation
import skimage as ski
from bresenham import bresenham
import segmentation
from skimage.segmentation import flood, flood_fill
from shapely.geometry import Point, Polygon
from perlin_noise import PerlinNoise

# this FUcntion was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib


def random_poly(size, num_points,radar_img,visualise):
# this Fucntion was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib
    """
    Generates a random polygon, scales it, and optionally visualizes it on a radar image.
    
    Parameters:
    size (int): The scaling factor for the polygon's size.
    num_points (int): The number of vertices in the polygon.
    radar_img (ndarray): The radar image on which the polygon is to be plotted.
    visualise (bool): If True, the polygon will be visualized on the radar image.
    
    Returns:
    list: A list of the new coordinates of the polygon's vertices.
    """
    #generating the random polygon
    polygon = random_polygon(num_points=num_points)
    #finding a random point in the radar image
    start_pt =(np.random.randint(0,radar_img.shape[0]-size),np.random.randint(0,radar_img.shape[1]-size))
    #scaling the polygon shape to fit into the radar image using starting pt and the size variable
    new_coords =[]
    for i in range(len(polygon)):
        first =polygon[i][0]*size+start_pt[0]
        second=polygon[i][1]*size+start_pt[1]
        new_coords.append([first,second])
    new_coords.append(new_coords[0])
    #plotting the shape
    if visualise:
        xs, ys = zip(*new_coords)
        sns.heatmap(radar_img)
        plt.plot(ys, xs, "w-", linewidth=0.8)
    return xs, ys

#
# LAVA SHAPE CREATION WITH BERNSTEIN CURVE AND PERLIN NOISE
#
NOISE_AMPLITUDE = 25

# Bernstein polynomial function
bernstein = lambda n, k, t: comb(n, k) * t**k * (1.-t)**(n-k)

# Derivative of the Bernstein polynomial function
dt_bernstein = lambda n, k, t: (
    comb(n, k) * (
        (k * t**(k - 1) * (1.-t)**(n - k) if t != 0 else 0) - # this if statement deals with the divide by zero error when t = 0
        ((n - k) * t**k * (1.-t)**(n - k - 1) if t != 1 else 0) # this if statement deals with the divide by zero error when t = 1
    )
)

def noisy_bezier(points, num=100,NOISE_AMPLITUDE=35,noise_freq=5):
    degree = len(points) - 1
    t_values = np.linspace(0, 1, num=num)
    curve = []
    
    perlin_noise = PerlinNoise(octaves=noise_freq)

    for t in t_values:
        position = bernstein(degree, np.arange(degree + 1), t) @ points
        dt = dt_bernstein(degree, np.arange(degree + 1), t) @ points
        if np.all(dt == 0):
            continue
        normalised_tangent = dt/np.linalg.norm(dt)
        normal = np.array([normalised_tangent[1], -normalised_tangent[0]])

        position += normal * NOISE_AMPLITUDE * perlin_noise(t)

        curve.append(position)

    return np.array(curve)

class Segment():
# this Function was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib
    def __init__(self, p1, p2, angle1, angle2, **kw):
        self.p1 = p1; self.p2 = p2
        self.angle1 = angle1; self.angle2 = angle2
        self.numpoints = kw.get("numpoints", 100)
        r = kw.get("r", 0.3)
        d = np.sqrt(np.sum((self.p2-self.p1)**2))
        self.r = r*d
        self.p = np.zeros((4,2))
        self.p[0,:] = self.p1[:]
        self.p[3,:] = self.p2[:]
        self.calc_intermediate_points(self.r)

    def calc_intermediate_points(self,r):
        self.p[1,:] = self.p1 + np.array([self.r*np.cos(self.angle1),
                                    self.r*np.sin(self.angle1)])
        self.p[2,:] = self.p2 + np.array([self.r*np.cos(self.angle2+np.pi),
                                    self.r*np.sin(self.angle2+np.pi)])
        self.curve = noisy_bezier(self.p,self.numpoints)


def get_curve(points, **kw):
# this FUcntion was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib
    segments = []
    for i in range(len(points)-1):
        seg = Segment(points[i,:2], points[i+1,:2], points[i,2],points[i+1,2],**kw)
        segments.append(seg)
    curve = np.concatenate([s.curve for s in segments])
    return segments, curve

def ccw_sort(p):
# this FUcntion was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib
    d = p-np.mean(p,axis=0)
    s = np.arctan2(d[:,0], d[:,1])
    return p[np.argsort(s),:]

def get_bezier_curve(a, rad=0.2, edgy=0):
# this FUcntion was based off of code from 
# https://stackoverflow.com/questions/50731785/create-random-shape-contour-using-matplotlib
    """ given an array of points *a*, create a curve through
    those points. 
    *rad* is a number between 0 and 1 to steer the distance of
          control points.
    *edgy* is a parameter which controls how "edgy" the curve is,
           edgy=0 is smoothest."""
    p = np.arctan(edgy)/np.pi+.5
    a = ccw_sort(a)
    a = np.append(a, np.atleast_2d(a[0,:]), axis=0)
    d = np.diff(a, axis=0)
    ang = np.arctan2(d[:,1],d[:,0])
    f = lambda ang : (ang>=0)*ang + (ang<0)*(ang+2*np.pi)
    ang = f(ang)
    ang1 = ang
    ang2 = np.roll(ang,1)
    ang = p*ang1 + (1-p)*ang2 + (np.abs(ang2-ang1) > np.pi )*np.pi
    ang = np.append(ang, [ang[0]])
    a = np.append(a, np.atleast_2d(ang).T, axis=1)
    s, c = get_curve(a, r=rad, method="var")
    x,y = c.T
    return x,y, a

METERS_PER_PIXEL = 75

def get_located_points(array, new_lava_position,size_meters=40000, n=5, mindst=None, rec=0):
    """
    location given as a set of coordinates (x, y)
    Generate `n` random points centered around a specified location, ensuring they are at 
    least `mindst` distance apart, and scale them according to the given `size_meters`.

    Parameters
    ----------
    array : np.ndarray
        A 2D numpy array representing the boundaries within which to generate random points.
    new_lava_position : tuple of (float, float)
        A tuple containing the (x, y) coordinates around which to center the generated points.
    size_meters : float, optional
        The size in meters to scale the points after generating them in the unit square (default is 40,000).
    n : int, optional
        The number of random points to generate (default is 5).
    mindst : float, optional
        The minimum distance that each point should have from the others. If not provided, it is computed 
        as `0.7 / n`.
    rec : int, optional
        The recursion depth, used internally to limit the number of recursive calls (default is 0).

    Returns
    -------
    np.ndarray
        A 2D numpy array of shape (n, 2) containing the generated points' (x, y) coordinates.
    """
    scale =np.ceil(np.sqrt(size_meters/METERS_PER_PIXEL))**2
    location_x, location_y =new_lava_position #getting the x and y position 
    
    mindst = mindst or .7 / n
    
    a1 = np.random.rand(n, 1) * scale + (location_y-(0.5*scale))
    a2 = np.random.rand(n, 1) * scale + (location_x-(0.5*scale))
 

    a = np.hstack((a2, a1))
    
  
    d = np.sqrt(np.sum(np.diff(ccw_sort(a), axis=0) ** 2, axis=1))
    
    if np.all(d >= mindst) or rec >= 200:
        return a
    else:
        return get_located_points(array, n=n, scale=scale, mindst=mindst, rec=rec + 1)
    
def get_random_points(array, n=5, size_meters=40000, mindst=None, rec=0):
    """
    Generate `n` random points within a unit square, ensuring they are at least `mindst` distance apart,
    and then scale them according to the given `size_meters`.

    Parameters:
    ----------
    array : np.ndarray
        A 2D numpy array representing the boundaries within which to generate random points.
    n : int, optional
        The number of random points to generate (default is 5).
    size_meters : float, optional
        The size in meters to scale the points after generating them in the unit square (default is 40,000).
    mindst : float, optional
        The minimum distance that each point should have from the others. If not provided, it's computed 
        as `0.7 / n`.
    rec : int, optional
        The recursion depth, used internally to limit the number of recursive calls (default is 0).

    Returns:
    -------
    np.ndarray
        A 2D numpy array of shape (n, 2) containing the generated points' (x, y) coordinates.
    """
    d1, d2 = array.shape
    
    mindst = mindst or .7 / n
    scale =np.ceil(np.sqrt(size_meters/METERS_PER_PIXEL))**2
    # Generate random points ensuring they stay within the array boundaries
    margin1 = d1-scale
    margin2 = d2-scale
    
    a1 = np.random.rand(n, 1) * scale + random.uniform(0,margin1) #y coords
    a2 = np.random.rand(n, 1) * scale + random.uniform(0,margin2) #x coords
    print('margines',margin2,margin1)

    a = np.hstack((a2, a1))
    
    # Calculate distances between consecutive points
    d = np.sqrt(np.sum(np.diff(ccw_sort(a), axis=0) ** 2, axis=1))
    
    if np.all(d >= mindst) or rec >= 200:
        return a
    else:
        return get_random_points(array, n=n, scale=scale, mindst=mindst, rec=rec + 1)
    

    
VALUE_OF_POLYGON_PIXELS=100
class PolygonFill:
    """
    A class used to handle operations related to filling a polygon on a radar image.

    This class allows for the creation and manipulation of a polygon based on specified
    X and Y coordinates. The polygon can be rasterized, points joined to form edges,
    internal points identified, and the polygon filled on a radar image.

    Attributes
    ----------
    radar_img : np.ndarray
        The radar image on which the polygon will be filled.
    polygon_Xcoords : list or np.ndarray
        X-coordinates of the vertices of the polygon.
    polygon_Ycoords : list or np.ndarray
        Y-coordinates of the vertices of the polygon.
    sq : np.ndarray
        A zero array representing the size of the polygon.
    x_min : int
        Minimum X-coordinate value of the polygon.
    y_min : int
        Minimum Y-coordinate value of the polygon.
    x : np.ndarray
        Rasterized X-coordinates of the polygon.
    y : np.ndarray
        Rasterized Y-coordinates of the polygon.
    poly_bool : np.ndarray
        A boolean array representing the polygon after applying Gaussian filtering.
    internal_pt : tuple
        Coordinates of a point that is inside the polygon.
    """
    def __init__(self, radar_img, polygon_Xcoords, polygon_Ycoords):
        """
        Initializes the PolygonFill class with the radar image and polygon coordinates.

        Parameters
        ----------
        radar_img : np.ndarray
            The radar image on which the polygon will be filled.
        polygon_Xcoords : list or np.ndarray
            X-coordinates of the vertices of the polygon.
        polygon_Ycoords : list or np.ndarray
            Y-coordinates of the vertices of the polygon.
        """
        self.radar_img = radar_img
        self.polygon_Xcoords = polygon_Xcoords
        self.polygon_Ycoords = polygon_Ycoords
        self.sq = None   #sq is the zero array with the size of polygon
        self.x_min = None
        self.y_min = None
        self.x = None
        self.y = None
        self.poly_bool = None
        self.internal_pt = None

    def rasterize_points(self):
        """
        Rasterizes the polygon points based on the provided X and Y coordinates.

        This method converts the floating-point polygon coordinates to integers and 
        determines the bounding box for the polygon. It initializes a zero array for 
        the polygon and populates it with pixel values representing the polygon edges.

        Returns
        -------
        None
        """
        x = np.rint(self.polygon_Xcoords).astype(int)
        y = np.rint(self.polygon_Ycoords).astype(int)
        self.x = x
        self.y = y
        x_min, x_max = min(x), max(x)
        y_min, y_max = min(y), max(y)
        self.x_min = x_min
        self.y_min = y_min
        sq = np.zeros((y_max - y_min + 1, x_max - x_min + 1))
        for i in range(len(x)):
            t = x[i] - x_min
            v = y[i] - y_min
            sq[v, t] = VALUE_OF_POLYGON_PIXELS
        self.sq = sq

    def join_points(self):
        """
        Connects the rasterized polygon points to form edges using the Bresenham algorithm.

        This method generates the edges of the polygon by joining consecutive points using
        the Bresenham line-drawing algorithm. It ensures all edges are connected and 
        applies a Gaussian filter to create a smooth polygonal shape.

        Returns
        -------
        None
        """
        assert len(self.x) == len(self.y)
        ypixel_list = []
        xpixel_list = []

        for i in range(len(self.x) - 1):
            X0 = int(self.x[i])
            Y0 = int(self.y[i])
            X1 = int(self.x[i + 1])
            Y1 = int(self.y[i + 1])

            bres = bresenham(X0, Y0, X1, Y1)
            B = list(bres)
            bx, by = zip(*B)
            assert len(B) == len(bx) == len(by)

            if i == 0:
                for j in range(len(B)):
                    ypixel_list.append(by[j])
                    xpixel_list.append(bx[j])
            else:
                for j in range(len(B) - 1):
                    ypixel_list.append(by[j + 1])
                    xpixel_list.append(bx[j + 1])

        finalx = int(self.x[-1])
        finaly = int(self.y[-1])
        firstx = int(self.x[0])
        firsty = int(self.y[0])
        bres = bresenham(finalx, finaly, firstx, firsty)
        B = list(bres)
        bx, by = zip(*B)
        for j in range(len(B) - 1):
            ypixel_list.append(by[j + 1])
            xpixel_list.append(bx[j + 1])

        assert len(xpixel_list) == len(ypixel_list)
        assert min(ypixel_list) == self.y_min
        assert min(xpixel_list) == self.x_min

        for i in range(len(xpixel_list)):
            t = xpixel_list[i] - self.x_min
            v = ypixel_list[i] - self.y_min
            self.sq[v, t] = VALUE_OF_POLYGON_PIXELS*10

        poly_arr = ski.filters.gaussian(self.sq, sigma=1.5)
        poly_bool = (poly_arr > 10).astype(np.bool_)
        self.poly_bool = poly_bool

    def find_internal_pt(self):
        """
        Adjusts the mean point of given x and y coordinates until it is inside the polygon formed by the coordinates.

        This function calculates the mean point (mx, my) of the provided x and y coordinates. It then checks if this point
        lies inside the polygon formed by the provided coordinates. If the point is not inside the polygon, the function 
        iteratively adjusts the point by adding a random value (between -5 and 5) to either the x or y coordinate until 
        the point lies inside the polygon.

        Parameters:
        x (list or numpy array): The x-coordinates of the polygon vertices.
        y (list or numpy array): The y-coordinates of the polygon vertices.

        Returns:
        tuple: Adjusted point coordinates (x, y) that lie inside the polygon.
        """
        mx = np.mean(self.x)
        my = np.mean(self.y)
        point_coords = [mx, my]
        point = Point(point_coords)
        poly_tuples = [(self.x[i], self.y[i]) for i in range(len(self.y))]
        polygon = Polygon(poly_tuples)
        is_inside = polygon.contains(point)

        while not is_inside:
            coin = np.random.randint(0, 2)
            added_value = np.random.randint(-5, 6)
            if coin == 0:
                point_coords[0] += added_value
            elif coin == 1:
                point_coords[1] += added_value
            point = Point(point_coords)
            is_inside = polygon.contains(point)

        self.internal_pt = point_coords

    def fill_polygon(self):
        """
        Fills the polygon on the radar image using the internal point identified.

        This method uses a flood-fill algorithm to fill the polygon area on the radar image
        starting from the internal point found by `find_internal_pt`. It applies a Gaussian 
        filter to smooth the filled polygon and returns the processed radar image.

        Returns
        -------
        np.ndarray
            A numpy array representing the radar image with the filled polygon.
        """
        filled_poly = flood_fill(self.poly_bool, (int(self.internal_pt[1] - self.y_min), int(self.internal_pt[0] - self.x_min)), 1)
        radar = self.radar_img.copy()
        for i in range(filled_poly.shape[0]):
            for j in range(filled_poly.shape[1]):
                if filled_poly[i, j] == 1:
                    radar[i + self.y_min, j + self.x_min] = 10
        radar = (radar > 1).astype(np.float64)
        rad = ski.filters.gaussian(radar, sigma=1)
        return rad



def cropping_img(image,grid_size):
    """
    Crops an image to the largest size that is a multiple of the specified grid size.

    This function takes an input image and crops it such that the resulting image dimensions
    are the largest multiples of the given grid size that fit within the original image dimensions.
    The cropped image will have dimensions of (no_y_img * grid_size, no_x_img * grid_size),
    where `no_y_img` and `no_x_img` are the number of grid sizes that fit in the image's height
    and width, respectively.

    Parameters
    ----------
    image : np.ndarray
        The input image as a numpy array.
    grid_size : int
        The size of the grid to which the image should be cropped. This value determines
        the dimensions of the cropped image, ensuring it is a multiple of `grid_size`.

    Returns
    -------
    np.ndarray
        The cropped image with dimensions that are the largest multiples of `grid_size` 
        fitting within the original image dimensions.
    """
    y_len =image.shape[0]
    x_len =image.shape[1]

    no_y_img =int(np.floor(y_len/grid_size))
    no_x_img =int(np.floor(x_len/grid_size))
    print('size of new image', no_y_img*grid_size,',',no_x_img*grid_size)
    img_crop =image[:(no_y_img*grid_size),:(no_x_img*grid_size)]
    assert (no_y_img * 128 == img_crop.shape[0]) and (no_x_img * 128 == img_crop.shape[1])
    return img_crop

def list_of_mini_img(img):
    """
    Splits a given image into smaller 128x128 pixel tiles.

    This function divides an input image into non-overlapping 128x128 pixel tiles. It assumes
    that the input image dimensions are divisible by 128 without any remainder.

    Parameters
    ----------
    img : np.ndarray
        A 2D or 3D numpy array representing the input image, where the shape must be divisible
        by 128 along both the height and width.

    Returns
    -------
    list of np.ndarray
        A list containing the 128x128 pixel tiles extracted from the input image.

    Raises
    ------
    AssertionError
        If the dimensions of `img` are not divisible by 128.
    """
    assert (img.shape[0]%128 ==0) and (img.shape[1]%128==0)
    list_of_imgs =[]
    for i in range(0,img.shape[0],128):
        for j in range(0,img.shape[1],128):
            mini=img[i:i+128,j:j+128]
            list_of_imgs.append(mini)
    return list_of_imgs

#this function will return the number of squares that are needed to fill the image
def new_sort(shape_tile_array):
    """
    Determines the number of non-empty tiles in an array and returns their indices.

    This function iterates through a list of tile arrays and counts how many tiles
    have a sum greater than 1. It also records the indices of these non-empty tiles.

    Parameters
    ----------
    shape_tile_array : list of np.ndarray
        A list of numpy arrays representing tiles. Each tile is expected to be a 2D array.

    Returns
    -------
    tuple
        A tuple containing the count of non-empty tiles (int) and a list of their indices (list of int).
    """
    indexes =[]
    count=0
    for i in range(len(shape_tile_array)):
        if np.sum(shape_tile_array[i]) > 1:
            indexes.append(i)
            count+=1
        else:
            pass
    return count, indexes

#post proccessing functions

def adjust_values(values, desired_mean, desired_std):
    """
    Adjusts the values of an array to have a specified mean and standard deviation.

    This function normalizes the input array to a mean of 0 and standard deviation of 1,
    then scales and shifts it to achieve the desired mean and standard deviation.

    Parameters
    ----------
    values : np.ndarray
        A numpy array of numerical values to be adjusted.
    desired_mean : float
        The desired mean for the adjusted values.
    desired_std : float
        The desired standard deviation for the adjusted values.

    Returns
    -------
    np.ndarray
        The adjusted array with the specified mean and standard deviation.
    """

    current_mean = np.mean(values)
    current_std = np.std(values)
    normalized_values = (values - current_mean) / current_std
    adjusted_values = normalized_values * desired_std + desired_mean
    
    return adjusted_values

def reassemble_large_array(tiles, original_height, original_width):
    """
    Reassembles a large image from smaller 128x128 pixel tiles.

    This function takes a list of smaller tiles and reassembles them into a larger image
    of the specified original dimensions. It assumes the tiles are arranged row-wise
    from left to right.

    Parameters
    ----------
    tiles : list of np.ndarray
        A list of 2D numpy arrays, each representing a 128x128 pixel tile.
    original_height : int
        The height of the original image before it was split into tiles.
    original_width : int
        The width of the original image before it was split into tiles.

    Returns
    -------
    np.ndarray
        A numpy array representing the reassembled large image with the original dimensions.
    """

    # Calculate the number of tiles along each dimension
    tiles_per_row = original_width // 128
    tiles_per_col = original_height // 128
    
    # Initialize an empty array with the original dimensions
    large_array = np.zeros((original_height, original_width))
    
    # Iterate through the list of tiles and place them into the large array
    for idx, tile in enumerate(tiles):
        row_idx = (idx // tiles_per_row) * 128
        col_idx = (idx % tiles_per_row) * 128
        large_array[row_idx:row_idx+128, col_idx:col_idx+128] = tile
    
    return large_array




