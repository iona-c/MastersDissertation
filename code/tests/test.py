import cv2
import numpy as np

# Load the image
image_path = '../Images/gracehopper.png'  # Replace with your image path
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)  # Convert to grayscale


# Sobel Edge Detection
# Sobel operator kernels
sobel_kernelx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype=int)
sobel_kernely = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]], dtype=int)

# Applying kernels to the grayscale image
sobel_x = cv2.filter2D(image, -1, sobel_kernelx)
sobel_y = cv2.filter2D(image, -1, sobel_kernely)
sobel_combined = cv2.bitwise_or(sobel_x, sobel_y)

# Scharr Edge Detection
# Scharr operator kernels
# Scharr operator kernels
# For x-gradient (Gx)
scharr_kernelx = np.array([[-3, 0, 3], [-10, 0, 10], [-3, 0, 3]], dtype=int)

# For y-gradient (Gy)
scharr_kernely = np.array([[-3, -10, -3], [0, 0, 0], [3, 10, 3]], dtype=int)

# Applying kernels to the grayscale image
scharr_x = cv2.filter2D(image, -1, scharr_kernelx)
scharr_y = cv2.filter2D(image, -1, scharr_kernely)
scharr_combined = cv2.bitwise_or(scharr_x, scharr_y)

# Prewitt Edge Detection
# Prewitt operator kernels
prewitt_kernelx = np.array([[1, 0, -1], [1, 0, -1], [1, 0, -1]], dtype=int)
prewitt_kernely = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]], dtype=int)

# Applying kernels to the grayscale image
prewitt_x = cv2.filter2D(image, -1, prewitt_kernelx)
prewitt_y = cv2.filter2D(image, -1, prewitt_kernely)
prewitt_combined = cv2.bitwise_or(prewitt_x, prewitt_y)

# Roberts Cross Edge Detection
# Roberts Cross operator kernels
roberts_kernelx = np.array([[1, 0], [0, -1]], dtype=int)
roberts_kernely = np.array([[0, 1], [-1, 0]], dtype=int)

# Applying kernels to the grayscale image
roberts_cross_x = cv2.filter2D(image, -1, roberts_kernelx)
roberts_cross_y = cv2.filter2D(image, -1, roberts_kernely)
roberts_cross_combined = cv2.bitwise_or(roberts_cross_x, roberts_cross_y)

# Save the edge-detected images if you want
cv2.imwrite('testimages/grayscale.png', image)
cv2.imwrite('testimages/sobel_edge_detection.png', sobel_combined)
cv2.imwrite('testimages/scharr_edge_detection.png', scharr_combined)
cv2.imwrite('testimages/prewitt_edge_detection.png', prewitt_combined)
cv2.imwrite('testimages/roberts_cross_edge_detection.png', roberts_cross_combined)
