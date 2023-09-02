#!/bin/python3
import argparse
import os
import json
import pyrosm
import geopy.distance


def main():
  parser = argparse.ArgumentParser(description='Creates a graph from a black/white map image')
  parser.add_argument('place', help='Place to download OSM data for', type=str)
  parser.add_argument('--dir', help='Network data directory', type=str, default="/tmp/pyrosm/", required=False)
  parser.add_argument('--force', '-f', help='Force reprocessing', action='store_true')
  args = parser.parse_args()

  json_file_path = os.path.join(args.dir, f"{args.place}.json");
  if not args.force and os.path.exists(json_file_path):
    print(f"{json_file_path} exists")
    return 0

  osm_file_path = os.path.join(args.dir, f"{args.place}.osm.pbf");
  if not os.path.exists(osm_file_path):
    osm_file_path = pyrosm.get_data(args.place)
  osm = pyrosm.OSM(osm_file_path)

  nodes, edges = osm.get_network(network_type='driving', nodes=True)

  g_nodes = []
  g_node_id_map = {}
  for id, lat, lon in zip(nodes.id, nodes.lat, nodes.lon):
    g_node_id_map[id] = len(g_nodes)
    g_nodes.append({
      "lat" : lat,
      "lon" : lon,
    })

  g_edges = []
  for u, v, length in zip(edges.u, edges.v, edges.length):
    u_n = g_node_id_map[u]
    v_n = g_node_id_map[v]
    g_edges.append({
      "u" : u_n,
      "v" : v_n,
      "w" : geopy.distance.geodesic((g_nodes[u_n]["lat"], g_nodes[u_n]["lon"]), (g_nodes[v_n]["lat"], g_nodes[v_n]["lon"])).m
    })

  with open(json_file_path, 'w+', encoding='utf-8') as wf:
    json.dump({"edges":g_edges, "nodes":g_nodes}, wf, indent=4)
    return 0


if __name__ == '__main__':
  main()

