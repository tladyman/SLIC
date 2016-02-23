import numpy as np
import matplotlib.pyplot as plt
from minima import find_n_local_minima
from skimage.color import rgb2grey
from skimage.morphology import watershed
from skimage.segmentation import felzenszwalb, slic, quickshift, mark_boundaries
plt.rcParams['figure.figsize'] = (20, 8)


def water(im, nMarkers, display_im):
    minima, nMinima = find_n_local_minima(im, nMarkers)
    labels = watershed(im, nMinima)
    marked = mark_boundaries(display_im, labels[:,:,0].astype(int))
    return marked


def felz(im, scale, sigma, min_size, display_im):
    labels = felzenszwalb(im, scale, sigma, min_size)
    marked = mark_boundaries(display_im, labels)
    return marked



def quick(im, ratio, max_dist, display_im):
    labels = quickshift(im, ratio, max_dist)
    marked = mark_boundaries(display_im, labels)
    return marked


def SLIC(im,n_segments, compactness, display_im):
    labels = slic(im, n_segments, compactness)
    marked = mark_boundaries(display_im, labels)
    return marked