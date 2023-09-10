#!/bin/python3

import argparse
import cv2
import os
import numpy as np
import json
import math


def to_network(voronoi: np.ndarray, bin_size_px:int) -> dict:
    bins = np.zeros((int(voronoi.shape[0] / bin_size_px)+1, int(voronoi.shape[1] / bin_size_px)+1, 3), np.float32)
    for x in range(0, voronoi.shape[0]):
        xb = int(x / bin_size_px)
        for y in range(0, voronoi.shape[1]):
            yb = int(y / bin_size_px)
            if voronoi[x, y] > 0:
                bins[xb, yb, 0] += x
                bins[xb, yb, 1] += y
                bins[xb, yb, 2] += 1

    nodes = []
    labels = np.ones((bins.shape[0], bins.shape[1]), np.uint32) * -1
    label = 0
    for xb in range(0, bins.shape[0]):
        for yb in range(0, bins.shape[1]):
            if bins[xb, yb, 2] > 0:
                x_node = float(bins[xb, yb, 0] / bins[xb, yb, 2])
                y_node = float(bins[xb, yb, 1] / bins[xb, yb, 2])
                nodes.append({"x":x_node, "y":y_node})
                labels[xb, yb] = label
                label += 1

    edges = []
    for xb in range(1, bins.shape[0]-1):
        for yb in range(1, bins.shape[1]-1):
            src = labels[xb, yb]
            if src < 0:
                continue
            for rx in [-1, 0, 1]:
                for ry in [-1, 0, 1]:
                    if rx == 0 and ry == 0:
                        continue
                    dst = labels[xb+rx, yb+ry]
                    if dst < 0:
                        continue
                    dxx = (nodes[src]['x'] - nodes[dst]['x'])
                    dyy = (nodes[src]['y'] - nodes[dst]['y'])
                    w = max([1, math.sqrt(dxx * dxx + dyy * dyy)])
                    edges.append({"u":float(src), "v":float(dst), "w":w})

    return (nodes, edges)



def main():
  parser = argparse.ArgumentParser(description='Creates a graph from a black/white map image')
  parser.add_argument('source_occupancy_map_image', type=str, help='Source occupancy image')
  parser.add_argument('output_graph_json', type=str, help='Output path to graph JSON')
  parser.add_argument('--bin-size', '-b', type=int, default=None, help='Node binning size (used in places of --downsampling-rate)')
  parser.add_argument('--downsampling-rate', '-d', type=float, default=0.009, help='Downsamping rate used for graph node sparsification')
  parser.add_argument('--show', '-s', action='store_true', help='Render graph once processing is complete')
  args = parser.parse_args()

  # Load the image
  input_image = cv2.imread(args.source_occupancy_map_image, cv2.IMREAD_GRAYSCALE)

  # Compute bin size
  bin_size_px = args.bin_size or max([1, int(input_image.shape[0] * args.downsampling_rate), int(input_image.shape[1] * args.downsampling_rate)])

  # Fill small gaps
  kernel_radius = bin_size_px
  kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (kernel_radius, kernel_radius))
  input_filtered = cv2.bitwise_not(input_image)
  input_filtered = cv2.dilate(input_filtered, kernel, iterations=1)
  input_filtered = cv2.erode(input_filtered, kernel, iterations=1)
  input_filtered = cv2.bitwise_not(input_filtered)

  # Run flood fill for voronoi cell location
  dist, labels = cv2.distanceTransformWithLabels(input_filtered, cv2.DIST_L2, 3)

  # Detect voronoi edges
  voronoi = cv2.Laplacian(labels.astype(np.int16), ddepth=cv2.CV_16S, ksize=3).astype(np.uint8)

  # Compute graph
  nodes, edges = to_network(voronoi=voronoi, bin_size_px=bin_size_px)

  # Save graph as JSON
  with open(args.output_graph_json, 'w+', encoding="utf-8") as out:
      json.dump({
        "source" : args.source_occupancy_map_image,
        "bin_size" : bin_size_px,
        "nodes" : nodes,
        "edges" : edges,
      }, out)

  # Display the image
  if args.show:
      canvas = np.zeros((input_image.shape[0], input_image.shape[1], 3), np.uint8)
      canvas[:,:,0] = input_filtered
      canvas[:,:,1] = (0.75 * input_image.astype(np.float32) + 0.25 * cv2.bitwise_and(input_image, cv2.bitwise_not(cv2.dilate(voronoi, kernel, iterations=1))).astype(np.float32)).astype(np.uint8)
      canvas[:,:,2] = input_image

      circle_radius = max([1, int(bin_size_px / 4)])
      line_width = max([1, int(bin_size_px / 8)])
      for e in edges:
          u = int(e['u'])
          v = int(e['v'])
          u_pt = (int(nodes[u]['y']), int(nodes[u]['x']))
          v_pt = (int(nodes[v]['y']), int(nodes[v]['x']))
          cv2.line(canvas, u_pt, v_pt, (255, 0, 0), line_width)

      for v in nodes:
          cv2.circle(canvas, (int(v['y']), int(v['x'])), circle_radius, (0, 0, 255), -1)

      cv2.namedWindow("graph preview", cv2.WINDOW_NORMAL)
      cv2.imshow("graph preview", canvas)
      cv2.waitKey(0)
      cv2.destroyAllWindows()

if __name__ == '__main__':
  main()
