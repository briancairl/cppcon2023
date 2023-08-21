#!/bin/python3

import argparse
import cv2
import numpy as np



def main():
  parser = argparse.ArgumentParser(description='Creates a graph from a black/white map image')
  parser.add_argument('source_map_image', help='Source image')
  args = parser.parse_args()

  # Load the image
  image = cv2.imread(args.source_map_image, cv2.IMREAD_GRAYSCALE)

  cv2.namedWindow("image", cv2.WINDOW_NORMAL)

  dist, labels = cv2.distanceTransformWithLabels(image, cv2.DIST_L2, 3)

  labels = labels.astype(np.uint8)

  canvas = cv2.Canny(labels,1,255)

  # Display the image
  cv2.imshow("image", cv2.merge([image, image, canvas]))
   
  # Wait for the user to press a key
  cv2.waitKey(0)
   
  # Close all windows
  cv2.destroyAllWindows()

if __name__ == '__main__':
  main()

