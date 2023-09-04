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


PERF_STAT_CMD = [
  "perf",
  "stat", 
  "-B",
  "-e",
  "cpu_core/cache-references/,cpu_core/cache-misses/,cpu_core/cycles/,cpu_core/instructions/,cpu_core/branches/,cpu_core/branch-misses/,page-faults,migrations"
]

class AppState(object):
    def __init__(self, path:str, search_exec:str):
      with open(path, 'r') as json_file_handle:
          graph = json.load(json_file_handle)
      self.graph_path = path
      self.nodes = graph['nodes']
      self.edges = graph['edges']
      self.resolution = graph['bin_size']
      self.mouse_x = 0
      self.mouse_y = 0
      self.image_bg = render(graph_data=graph)
      self.image_render = copy.deepcopy(self.image_bg)
      self.to_vertex_id_mapping = {}
      self.start_vertex_id = None
      self.goal_vertex_id = None
      self.path_vertex_ids = []
      self.search_exec = search_exec
      self.iterations = 100
      self.version = 0

      for vertex_id, node in enumerate(self.nodes):
          x = int(node['x'])
          y = int(node['y'])
          iv = InteractVertex(x=x, y=y, res=self.resolution)
          self.to_vertex_id_mapping[iv] = vertex_id

    def to_vertex_id(self, x:int, y:int) -> int:
        iv = InteractVertex(x=x, y=y, res=self.resolution)
        return self.to_vertex_id_mapping[iv] if iv in self.to_vertex_id_mapping else None

    def to_vertex(self, x:int=None, y:int=None, id:int=None) -> int:
        id = (id or self.to_vertex_id(x=x, y=y))
        return self.nodes[id] if (id != None) else None

    def to_vertex_position(self, x:int=None, y:int=None, id:int=None) -> tuple:
        node = self.to_vertex(x=x, y=y, id=id)
        return (int(node['x']), int(node['y'])) if node != None else None

    def run_search(self):
        cmd = PERF_STAT_CMD + [f"./{APP.search_exec}", APP.graph_path, TMP_PATH_JSON_FILENAME, f"{APP.start_vertex_id}", f"{APP.goal_vertex_id}", f"{APP.iterations}", f"{APP.version}"]
        if sp.call(cmd) == 0:
            print("success")
            return 0
        else:
            print("failure")
            return 1

APP = None



def mouse_event(event,x,y,flags,param):
    global APP
    APP.mouse_x = y
    APP.mouse_y = x
    vertex_id = APP.to_vertex_id(x=APP.mouse_x, y=APP.mouse_y)
    if event == cv2.EVENT_RBUTTONDBLCLK:
        APP.start_vertex_id = None
        APP.goal_vertex_id = None
        APP.image_render = copy.deepcopy(APP.image_bg)
        cv2.imshow(WINDOW_NAME, APP.image_render)
    elif event == cv2.EVENT_LBUTTONDBLCLK and vertex_id:
        # Set clicked start ID
        if APP.start_vertex_id == None:
            APP.start_vertex_id = vertex_id
            pt = APP.to_vertex_position(id=vertex_id)
            cv2.circle(APP.image_render, (pt[1], pt[0]), APP.resolution//2, (50, 200, 100), -1)
            cv2.circle(APP.image_render, (pt[1], pt[0]), APP.resolution//2, (10, 255, 255), 2)
            cv2.imshow(WINDOW_NAME, APP.image_render)

        else:
            APP.goal_vertex_id = vertex_id
            if APP.run_search() != 0:
                return
            with open(TMP_PATH_JSON_FILENAME, 'r') as path_json_file_handle:
                path_json = json.load(path_json_file_handle)

            path_vertex_ids = path_json["path"]
            for i in range(1, len(path_vertex_ids)):
                s_pt = APP.to_vertex_position(id=path_vertex_ids[i-1])
                e_pt = APP.to_vertex_position(id=path_vertex_ids[i])
                cv2.line(APP.image_render, (s_pt[1], s_pt[0]), (e_pt[1], e_pt[0]), (20, 255, 50), 15)
            cv2.imshow(WINDOW_NAME, APP.image_render)


def compute_file_hash(path:str):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read());


def build(main:str = 'main.cpp', srcs=[], copts=['-std=c++23', '-O3', '-g'], force:bool=False):
    build_cache_path = os.path.join(BUILD_DIR, 'cache.json')
    if os.path.exists(build_cache_path):
        with open(build_cache_path, 'r') as json_file_handle:
            cache = json.load(json_file_handle)
    else:
        cache = {}

    main_hash = compute_file_hash(path=main)

    all_libraries = []
    for source in srcs:
        no_ext = os.path.splitext(source)[0]
        header = f"{no_ext}.h"
        source_hash = compute_file_hash(path=source)
        header_hash = compute_file_hash(path=header)
        source_hash.update(header_hash.digest())
        out_lib = f"build/{source.split('.')[0]}.o"
        if (not force) and os.path.exists(out_lib) and (source in cache) and (cache[source] == source_hash.hexdigest()):
            print(f"Not building [{header}, {source}] -> {out_lib}. Unchanged.")
        elif sp.call(['g++', '-c', source] + copts + ['-o', out_lib, '-I', '.']) == 0:
            cache[source] = source_hash.hexdigest()
            print(f"Built [{source}] -> {out_lib}")
        else:
            raise RuntimeError(f"Failed to build [{source}] -> {out_lib}")
        all_libraries.append(out_lib)
        main_hash.update(bytes(cache[source], encoding='utf8'))

    out_main_exec = f"build/{main.split('.')[0]}.out"
    if force or not (os.path.exists(out_main_exec) and (main in cache) and (cache[main] == main_hash.hexdigest())):
        if sp.call(['g++', main] + all_libraries + copts + ['-o', out_main_exec, '-I', '.']) == 0:
            print(f"Built [{main}] -> {out_main_exec}")
        else:
            raise RuntimeError(f"Failed to build [{main}] -> {out_main_exec}")
    else:
        print(f"Not building [{main}] -> {out_main_exec}. Unchanged.")
    cache[main] = main_hash.hexdigest()

    with open(build_cache_path, 'w+') as json_file_handle:
        json.dump(cache, json_file_handle)

    return out_main_exec


def rerun(a, b):
  global APP
  APP.run_search()

def update_iterations(value):
  global APP
  APP.iterations = value

def select_v0(a, b):
  global APP
  APP.version = 0

def select_v1(a, b):
  global APP
  APP.version = 1


def main():
  global APP

  parser = argparse.ArgumentParser(description='Runs search on graph interactively')
  parser.add_argument('graph_json', type=str, help='Graph JSON')
  parser.add_argument('--force-rebuild', '-f', action="store_true", help='Force rebuild search code')
  args = parser.parse_args()

  search_exec = build(srcs=[
    "dijkstras_v0.cpp",
    "dijkstras_v1.cpp"
  ], force=args.force_rebuild)

  APP = AppState(path=args.graph_json, search_exec=search_exec)

  cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL)

  cv2.createTrackbar('iterations', WINDOW_NAME, 1, 1000, update_iterations)
  cv2.createButton("rerun",rerun, None, cv2.QT_PUSH_BUTTON,1)
  cv2.createButton("dijkstras v0",select_v0, None, cv2.QT_PUSH_BUTTON,1)
  cv2.createButton("dijkstras v1",select_v1, None, cv2.QT_PUSH_BUTTON,1)
  cv2.setMouseCallback(WINDOW_NAME, mouse_event)
  cv2.imshow(WINDOW_NAME, APP.image_bg)
  cv2.waitKey(0)
  cv2.destroyAllWindows()

if __name__ == '__main__':
  main()
