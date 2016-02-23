import numpy as np
from skimage.feature import peak_local_max
from skimage.measure import label

def find_n_local_minima(image, n=0):
    """Takes a 3D image and creates a local minimum mask, and n minima mask

    """
    # Invert image so we are searching for maximas
    im_inv = np.invert(image)
    
    # Find peaks in each of r,g,b
    minimaR = peak_local_max(im_inv[:,:,0])
    minimaG = peak_local_max(im_inv[:,:,1])
    minimaB = peak_local_max(im_inv[:,:,2])
    
    # each of the minima will have an array of shape (n, 2)
    # where n is the number of minima

    # get 2D dimensions of the image
    y, x, z = image.shape
    zR = np.zeros((y, x))
    zG = np.zeros((y, x))
    zB = np.zeros((y, x))
    nMinima = np.zeros((y, x))

    # Make zR, zG, zB an accumulator space for the minima points
    zR[minimaR[:,0], minimaR[:,1]] = 1
    zG[minimaG[:,0], minimaG[:,1]] = 1
    zB[minimaB[:,0], minimaB[:,1]] = 1
    # Find where peaks in rgb overlap
    minima = zR * zG * zB

    if n != 0:
        i, j = np.nonzero(minima)

        # making sure the number of minima is not exceeded
        if n > np.sum(minima):
            n = np.sum(minima)

        ix = np.random.choice(len(i), n, replace=False)
        
        nMinima[i[ix], j[ix]] = 1

        # Label the masked image
        labelled = label(nMinima)

        #Make an nMinima shape of the image
        markers = np.zeros(image.shape)
        for i in range(z):
            markers[:,:,i] = labelled
        return minima, markers
    else:
        return minima
