import torch
import torch.nn as nn
import matplotlib.pyplot as plt
import seaborn as sns
import cv2
import numpy as np
from skimage import exposure

class Creation:
    """
    A class used to generate radar images using a pre-trained GAN model.
    """
    def __init__(self,ngpu):
        """
        Initializes the Creation class with the number of GPUs to use and device selection.

        Parameters:
        ngpu (int): Number of GPUs available. If greater than 0 and CUDA is available, use GPU; otherwise, use CPU.
        """
        self.nz = 100 #the size of the generators input
        self.ngpu = 0 #saying there is one gpu available
        self.device = torch.device("cuda:0" if (torch.cuda.is_available() and ngpu > 0) else "cpu")

    def produce_images(self, no_images,netG, visualise):
        """
        Generates images using a given neural network generator and optionally visualizes them.

        Parameters:
        no_images (int): Number of images to generate.
        netG (nn.Module): Neural network generator to produce images.
        visualise (bool): If True, display the generated images using a heatmap.

        Returns:
        list: A list of generated images as numpy arrays.
        """
        noise = torch.randn(no_images,self.nz, 1, 1, device=self.device)
        with torch.no_grad():
            generated_images = netG(noise).detach().cpu()
            generated_images = (generated_images * 0.5 + 0.5)
            images =[]
            for i, img_tensor in enumerate(generated_images):
                n =np.array(img_tensor)
                kn =n.squeeze()
                images.append(kn)
        if visualise:
            select_img = images[:8]#is this slice first 8?
            fig, axes = plt.subplots(2, 4, figsize=(16, 8))
            fig.suptitle('Generated Image Examples ', fontsize=16)
            for i, ax in enumerate(axes.flatten()):
                heatmap = sns.heatmap(select_img[i], ax=ax, cbar=True)
                ax.set_title(f'Image {i+1}')
                ax.axis('off')
            plt.tight_layout(rect=[0, 0, 1, 0.95])
            plt.show()
        else:
            pass
        return images
    
    def historgram_equaliser(self,image,visualise):
        """
        Applies histogram equalization to an image and optionally visualizes the results.

        Parameters:
        image (numpy.ndarray): The input image to be equalized.
        visualise (bool): If True, display the original and equalized images.

        Returns:
        tuple: A tuple containing the adaptive histogram equalized image and the equalized image using OpenCV.
        """
        test_img_uint8 = (image * 255).astype(np.uint8)
        ahe_image = (exposure.equalize_adapthist(test_img_uint8 / 255.0) * 255).astype(np.uint8)
        equalized_image = cv2.equalizeHist(test_img_uint8)
        if visualise:
            # Create a figure with 2 subplots
            fig, axes = plt.subplots(1, 3, figsize=(12, 6))
            axes[0].imshow(test_img_uint8, cmap='gray')
            axes[0].set_title('Original Image')
            axes[0].axis('off')
            axes[1].imshow(equalized_image, cmap='gray')
            axes[1].set_title('Histogram Equalized Image')
            axes[1].axis('off')
            axes[2].imshow(ahe_image, cmap='gray')
            axes[2].set_title('Histogram Equalized Image')
            axes[2].axis('off')

            plt.tight_layout()
            plt.show()
        else:
            pass

        return ahe_image, equalized_image
    
    def add_salt_and_pepper_noise(self,image, max_val=1,min_val=0, salt_prob=0.01, pepper_prob=0.01):
        """
        Adds salt-and-pepper noise to an image.

        Parameters:
        image (numpy.ndarray): The input image to which noise will be added.
        max_val (int or float, optional): The maximum value for salt noise. Default is 1.
        min_val (int or float, optional): The minimum value for pepper noise. Default is 0.
        salt_prob (float, optional): Probability of salt noise. Default is 0.01.
        pepper_prob (float, optional): Probability of pepper noise. Default is 0.01.

        Returns:
        numpy.ndarray: The noisy image with salt-and-pepper noise applied.
        """
        noisy = image.copy()
        salt_mask = np.random.rand(*image.shape) < salt_prob
        pepper_mask = np.random.rand(*image.shape) < pepper_prob
        noisy[salt_mask] = max_val
        noisy[pepper_mask] = min_val
        return noisy

    def add_gaussian_noise_photo(self,image, mean=0, var=0.1):
        """
        This is specifically designed for creating 8bit photo images as the pixel values
        vary from 0 to 255
        """
        sigma = var**0.5
        gauss = np.random.normal(mean, sigma, image.shape)
        #sns.heatmap(gauss)
        noisy = image + gauss * 255
        return np.clip(noisy, 0, 255).astype(np.uint8)
    
    def add_gaussian_noise(self,image, mean=0, var=0.01):
        """
        this adds gaussian noise and the pixel values can range from any size
        """
    # Determine the image's data type and maximum value
        dtype = image.dtype
        max_val = np.iinfo(dtype).max if np.issubdtype(dtype, np.integer) else 1.0
       # Calculate the standard deviation
        sigma = var**0.5

        # Generate Gaussian noise
        gauss = np.random.normal(mean, sigma, image.shape)

        # Calculate the range of pixel values
        img_range = np.max(image) - np.min(image)
        
        # Scale the noise to match the image's range of pixel values
        noisy = image + gauss * img_range

        # Clip the values to ensure they stay within the valid range
        noisy = np.clip(noisy, 0, max_val)

        # Convert back to the original data type
        return noisy.astype(dtype)

    def add_speckle_noise(self, image,scale=0.1,max_val=1,min_val=0):
        gauss = np.random.randn(*image.shape)
        noisy = image + scale*image * gauss
        return np.clip(noisy, min_val, max_val)


# Apply weights
class Generator(nn.Module):
    def __init__(self, nz):
        super(Generator, self).__init__()
        self.main = nn.Sequential(
            nn.ConvTranspose2d(nz, 512, 4, 1, 0, bias=False),  # Output: (512, 4, 4)
            nn.BatchNorm2d(512),
            nn.ReLU(True),
            nn.ConvTranspose2d(512, 256, 4, 2, 1, bias=False),  # Output: (256, 8, 8)
            nn.BatchNorm2d(256),
            nn.ReLU(True),
            nn.ConvTranspose2d(256, 128, 4, 2, 1, bias=False),  # Output: (128, 16, 16)
            nn.BatchNorm2d(128),
            nn.ReLU(True),
            nn.ConvTranspose2d(128, 64, 4, 2, 1, bias=False),  # Output: (64, 32, 32)
            nn.BatchNorm2d(64),
            nn.ReLU(True),
            nn.ConvTranspose2d(64, 1, 4, 2, 1, bias=False),  # Output: (1, 64, 64)
            nn.BatchNorm2d(1),
            nn.ReLU(True),
            nn.ConvTranspose2d(1, 1, 4, 2, 1, bias=False),  # Output: (1, 128, 128)
            nn.Tanh()
        )

    def forward(self, input):
        x = input
        for layer in self.main:
            x = layer(x)

        return x
class Discriminator(nn.Module):
    def __init__(self):
        super(Discriminator, self).__init__()
        self.main = nn.Sequential(
            nn.Conv2d(1, 64, 4, 2, 1, bias=False),  # From 3 to 1 input channel
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(64, 128, 4, 2, 1, bias=False),
            nn.BatchNorm2d(128),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(128, 256, 4, 2, 1, bias=False),
            nn.BatchNorm2d(256),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(256, 512, 4, 2, 1, bias=False),
            nn.BatchNorm2d(512),
            nn.LeakyReLU(0.2, inplace=True),
            nn.Conv2d(512, 1, 4, 1, 0, bias=False)
        )
        self.fc = nn.Sequential(
            nn.Linear(5*5*1, 1),
            nn.Sigmoid()
        )

    def forward(self, input):
        x = self.main(input)
        x = x.view(-1, 5*5*1)  # Flatten the tensor
        return self.fc(x)