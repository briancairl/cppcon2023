#!/bin/python3

import argparse
import copy
import cv2
import os
import numpy as np
import json
import math
import subprocess as sp
import hashlib
import sys
from typing import Optional

BUILD_DIR = "build"
WINDOW_NAME = "dijkstras"
TMP_PATH_JSON_FILENAME = "/tmp/path.json"


def render(graph_data: dict):
  # Load the image
  input_image = cv2.imread(graph_data["source"], cv2.IMREAD_GRAYSCALE)

  # Unpack graph data
  nodes = graph_data["nodes"]
  edges = graph_data["edges"]
  bin_size_px = graph_data["bin_size"]

  # Display the image
  canvas = np.zeros((input_image.shape[0], input_image.shape[1], 3), np.uint8)
  canvas[:,:,0] = input_image
  canvas[:,:,1] = input_image
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

  return canvas


class InteractVertex(object):
    def __init__(self, x:int, y:int, res:int):
        self.x_r = int(x // res)
        self.y_r = int(y // res)

    def __hash__(self):
        return hash(self.x_r) ^ hash(self.y_r)

    def __eq__(self, other):
        return self.x_r == other.x_r and self.y_r == other.y_r

    def __repr__(self):
        return f"({self.x_r}, {self.y_r})"


class AppState(object):
    def __init__(self, graph_json_path:str, results_json_path:Optional[str] = None):

      with open(graph_json_path, 'r') as graph_json_file_handle:
          graph = json.load(graph_json_file_handle)

      self.results = None
      self.shuffle_mapping = []
      if results_json_path:
          with open(results_json_path, 'r') as results_json_file_handle:
              data = json.load(results_json_file_handle)
              self.results = data["results"]
              shuffle = data["shuffle"]
              self.shuffle_mapping = [0]*len(shuffle)
              for i, s in enumerate(shuffle):
                  self.shuffle_mapping[s] = i

      self.image_original = render(graph_data=graph)
      self.image_render = copy.deepcopy(self.image_original)

      self.graph_graph_json_path = graph_json_path
      self.nodes = graph['nodes']
      self.edges = graph['edges']
      self.resolution = graph['bin_size']
      self.path_line_width = int(self.resolution)
      self.path_result_index = 0

      self.start_vertex_id = None
      self.goal_vertex_id = None
      self.collected_vertex_pairs = []

      self.to_vertex_id_mapping = {}
      for vertex_id, node in enumerate(self.nodes):
          x = int(node['x'])
          y = int(node['y'])
          iv = InteractVertex(x=x, y=y, res=self.resolution)
          self.to_vertex_id_mapping[iv] = vertex_id

    def to_vertex_id(self, x:int, y:int) -> int:
        iv = InteractVertex(x=x, y=y, res=self.resolution)
        return self.to_vertex_id_mapping[iv] if iv in self.to_vertex_id_mapping else None

    def to_vertex(self, x:int=None, y:int=None, id:int=None) -> int:
        id = id if (id != None) else self.to_vertex_id(x=x, y=y)
        return self.nodes[id] if (id != None) else None

    def to_vertex_position(self, x:int=None, y:int=None, id:int=None) -> tuple:
        node = self.to_vertex(x=x, y=y, id=id)
        return (int(node['x']), int(node['y'])) if node != None else None


APP = None


def mouse_event(event,x,y,flags,param):
    global APP

    vertex_id = APP.to_vertex_id(x=y, y=x)

    if (event == cv2.EVENT_RBUTTONDOWN) and (APP.start_vertex_id != None) and (APP.goal_vertex_id != None):
        APP.collected_vertex_pairs.append({"start" : APP.start_vertex_id, "goal" : APP.goal_vertex_id})
        APP.start_vertex_id = None
        APP.goal_vertex_id = None
        APP.image_render = copy.deepcopy(APP.image_original)
        cv2.imshow(WINDOW_NAME, APP.image_render)

    elif (event == cv2.EVENT_LBUTTONDBLCLK) and vertex_id:

        if APP.start_vertex_id == None:
            APP.start_vertex_id = vertex_id
            start_pt = APP.to_vertex_position(id=APP.start_vertex_id)
            cv2.circle(APP.image_render, (start_pt[1], start_pt[0]), APP.resolution//2, (50, 200, 100), -1)
            cv2.circle(APP.image_render, (start_pt[1], start_pt[0]), APP.resolution//2, (10, 255, 255), 2)
            cv2.imshow(WINDOW_NAME, APP.image_render)
        elif APP.goal_vertex_id == None:
            APP.goal_vertex_id = vertex_id
            start_pt = APP.to_vertex_position(id=APP.start_vertex_id)
            goal_pt = APP.to_vertex_position(id=APP.goal_vertex_id)
            cv2.line(APP.image_render, (start_pt[1], start_pt[0]), (goal_pt[1], goal_pt[0]), (10, 255, 255), APP.resolution//3)
            cv2.circle(APP.image_render, (goal_pt[1], goal_pt[0]), APP.resolution//2, (50, 200, 100), -1)
            cv2.circle(APP.image_render, (goal_pt[1], goal_pt[0]), APP.resolution//2, (10, 255, 255), 2)
            cv2.imshow(WINDOW_NAME, APP.image_render)


def render_path():
  global APP
  APP.image_render = copy.deepcopy(APP.image_original)
  path = APP.results[APP.path_result_index]["path"]
  path = [APP.shuffle_mapping[p] for p in path]
  for i in range(1, len(path)):
      l_pt = APP.to_vertex_position(id=path[i-1])
      r_pt = APP.to_vertex_position(id=path[i-0])
      cv2.line(APP.image_render, (l_pt[1], l_pt[0]), (r_pt[1], r_pt[0]), (10, 255, 10), APP.path_line_width)
  cv2.imshow(WINDOW_NAME, APP.image_render)


def update_path_shown(index:int):
  global APP
  APP.path_result_index = index
  render_path()


def update_path_line_width(value:int):
  global APP
  APP.path_line_width = value
  render_path()


def main():
  global APP

  parser = argparse.ArgumentParser(description='Runs search on graph interactively')
  parser.add_argument('graph_json', type=str, help='Graph JSON')
  parser.add_argument('--problems-json', '-p', type=str, help='Problem JSON', required=False, default=None)
  parser.add_argument('--results-json', '-r', type=str, help='Result JSON', required=False, default=None)
  args = parser.parse_args()

  APP = AppState(graph_json_path=args.graph_json, results_json_path=args.results_json)

  cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL)

  if APP.results:
      cv2.createTrackbar('result', WINDOW_NAME, 0, len(APP.results)-1, update_path_shown)
      cv2.createTrackbar('width', WINDOW_NAME, 1, 100, update_path_line_width)

  cv2.setMouseCallback(WINDOW_NAME, mouse_event)
  cv2.imshow(WINDOW_NAME, APP.image_original)
  cv2.waitKey(0)
  cv2.destroyAllWindows()

  if not args.problems_json:
      args.problems_json = f"{os.path.splitext(args.graph_json)[0]}.problems.json"

  if os.path.exists(args.problems_json):
      if input(f"{args.problems_json} exists. overwrite? [y/N]: ").lower() != 'y':
          print(f"Not saving {args.problems_json}")
          sys.exit(0)

  with open(args.problems_json, 'w+') as problems_json_file_handle:
      json.dump({"problems" : APP.collected_vertex_pairs}, problems_json_file_handle, indent=4)

if __name__ == '__main__':
  main()
