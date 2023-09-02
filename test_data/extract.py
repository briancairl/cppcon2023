#!/bin/python3
import argparse
import numpy as np
import os
import pyrosm
import matplotlib.pyplot as plt
import json

def main():
  parser = argparse.ArgumentParser(description='Creates a graph from a black/white map image')
  parser.add_argument('place', help='Place to download OSM data for', type=str)
  parser.add_argument('--dir', help='Network data directory', type=str, default="/tmp/pyrosm/", required=False)
  args = parser.parse_args()

  json_file_path = os.path.join(args.dir, f"{args.place}.json");
  if os.path.exists(json_file_path):
    print(f"{json_file_path} exists")
    return 0

  osm_file_path = os.path.join(args.dir, f"{args.place}.osm.pbf");
  if not os.path.exists(osm_file_path):
    osm_file_path = pyrosm.get_data(args.place)
  osm = pyrosm.OSM(osm_file_path)

  nodes, edges = osm.get_network(network_type='driving', nodes=True)

  g_nodes = []
  for id, lat, lon in zip(nodes.id, nodes.lat, nodes.lon):
    g_nodes.append({
      "n" : id,
      "x" : lat,
      "y" : lon,
    })

  g_edges = []
  for u, v, length in zip(edges.u, edges.v, edges.length):
    g_edges.append({
      "u" : u,
      "v" : v,
    })

  with open(json_file_path, 'w+', encoding='utf-8') as wf:
    json.dump({"edges":g_edges, "nodes":g_nodes}, wf, indent=4)
    return 0


if __name__ == '__main__':
  main()

