import math
import random
import tkinter as Tk
import os
import subprocess
from subprocess import PIPE

# # TODO: adapt the paths to the binary of your solution (the one that also outputs the list of edges on the shortest path)
# # and to the input file (either lie.in, bern.in or nyc.in)
binary_path = "/path/to/solution" # TODO
filepath = "/path/to/lie.in" # TODO

# # TODO Uncomment the following three lines if you want to use the Google Maps images in the background
# from PIL import Image, ImageTk
# import requests
# from io import BytesIO

# # TODO and set this flag to True:
use_gmaps = False # TODO
# # TODO and request an API key from here (charges may apply!):
# # https://developers.google.com/maps/documentation/static-maps/get-api-key
api_key = "TODO" # TODO
# END OF THE TODOS

# Global values of the visualization
source = 0
target = 0
highlight_edges = []

def read_graph():
	infile = open(filepath)
	global N
	global M
	global Q
	global positions
	global arcs
	global queries
	global source
	global target
	N, M, Q = map(int, infile.readline().split())

	positions = {}
	arcs = []
	queries = []

	for i in range(N):
		lat, lon = map(int, infile.readline().split())
		positions[i] = (lat/1000000, lon/1000000)

	for i in range(M):
		u, v, l = map(int, infile.readline().split())
		arcs.append((u,v,l))

	for i in range(Q):
		s, t = map(int, infile.readline().split())
		queries.append((s,t))

	# set the first query as a start
	source, target = queries[0]

# Create an input file for the query on the fly
def interactive_input():
	res = ""
	res = res + "{} {} 1\n".format(N,M)
	for i in range(N):
		lat, lon = positions[i]
		res = res + "{} {}\n".format(int(lat*1000000), int(lon*1000000))
	for i in range(M):
		u, v, l = arcs[i]
		res = res + "{} {} {}\n".format(u,v,l)
	res = res + "{} {}\n".format(source, target)
	return res

# The Tkinter window with the graph view
class GUI:
	# Width and height of the drawing canvas
	w = 1200
	h = 800
	r = 2	
	latmid = 0
	lonmid = 0
	vertex_order = []
	edge_order = []
	recenter_mode = False

	scale = 245/2 # a hardcoded constant that fits with the size of GMaps
	zoom = 6

	def is_on_screen(self,lat, lon):
		# for cropping off-screen edges
		x, y = self.convert_to_display_coord(lat,lon)
		if x < 0 or y < 0:
			return False
		if x > self.w or y > self.h:
			return False
		return True

	def center_at(self, lat, lon):
		self.latmid = lat
		self.lonmid = lon

	def set_scale_to(self, s):
		self.scale = s

	def init_frame(self):
		# take a random edge and vertex order (only relevant if graph is too large)
		self.vertex_order = [i for i in range(N)]
		random.shuffle(self.vertex_order)
		self.edge_order = [i for i in range(M)]
		random.shuffle(self.edge_order)

		# determine the bounding box around the input graph
		latmin, latmax = 1<<30, -(1<<30)
		lonmin, lonmax = 1<<30, -(1<<30)
		for i in range(N):
			lat, lon = positions[i]
			latmin = min(latmin, lat)
			latmax = max(latmax, lat)
			lonmin = min(lonmin, lon)
			lonmax = max(lonmax, lon)
		self.latmid = (latmin+latmax)/2
		self.lonmid = (lonmin+lonmax)/2	
		# determine the scale that perfectly fits the input	
		intended_scale = min(self.w/((latmax-latmin)*math.cos(self.lonmid/180*math.pi)), self.h/(lonmax-lonmin))
		intended_scale = intended_scale * 0.9
		# determine the matching scale of Google maps
		if use_gmaps:
			while self.scale*2 < intended_scale:
				self.scale = self.scale*2
				self.zoom = self.zoom + 1
		else:
			self.scale = intended_scale
		self.center_at(self.latmid, self.lonmid)

	def closest_vertex_id(self, lat, lon):
		res = 0
		dist = 1<<30
		for i in range(N):
			plat, plon = positions[i]
			d = math.sqrt(pow(plat-lat,2)+pow(plon-lon,2))
			if d < dist:
				dist = d
				res = i
		return res

	def convert_to_real_coord(self, x, y):
		lat = self.latmid + (x-self.w/2)/self.scale/math.cos(self.lonmid/180*math.pi)
		lon = self.lonmid - (y-self.h/2)/self.scale
		return (lat,lon)

	def convert_to_display_coord(self, lat, lon):
		x = (lat-self.latmid)*self.scale*math.cos(lon/180*math.pi) + self.w/2
		y = -(lon-self.lonmid)*self.scale + self.h/2
		return (x,y)

	def draw_circle(self,lat,lon,r,**kwargs):
		x, y = self.convert_to_display_coord(lat,lon)
		self.c.create_oval(x-r,y-r,x+r,y+r, **kwargs)

	def draw_line(self,alat, alon, blat, blon,**kwargs):
		ax, ay = self.convert_to_display_coord(alat,alon)
		bx, by = self.convert_to_display_coord(blat,blon)
		self.c.create_line(ax, ay, bx, by, **kwargs)

	def draw_arc(self,alat, alon, blat, blon,**kwargs):
		ax, ay = self.convert_to_display_coord(alat,alon)
		bx, by = self.convert_to_display_coord(blat,blon)
		self.c.create_line(ax, ay, bx, by, **kwargs)
		# add arrow tips:
		cx, cy = (0.2*ax + 0.8*bx, 0.2*ay + 0.8*by)
		nx, ny = (bx-ax, by-ay)
		l = 1+math.sqrt(nx*nx+ny*ny)/(kwargs.get('width',2)*8)
		nx, ny = nx/l, ny/l
		dx, dy = cx-nx, cy-ny
		ex, ey = cx-ny, cy+nx
		fx, fy = cx+ny, cy-nx
		gx, gy = (ex+dx)/2, (ey+dy)/2
		hx, hy = (fx+dx)/2, (fy+dy)/2
		self.c.create_line(gx, gy, cx, cy, **kwargs)
		self.c.create_line(hx, hy, cx, cy, **kwargs)

	def draw_gmap(self):
		global image_ref
		url = "https://maps.googleapis.com/maps/api/staticmap?center={},{}&zoom={}&size=600x400&scale=2&key={}".format(self.lonmid, self.latmid, self.zoom, api_key)
		response = requests.get(url)
		image = Image.open(BytesIO(response.content))
		# image = Image.open("/Users/dgraf/Downloads/staticmap.png")
		large_image = image.resize((self.w, self.h))
		image_ref = ImageTk.PhotoImage(large_image)
		self.c.create_image(self.w/2,self.h/2,image=image_ref)

	def draw_graph(self):
		cn = 0
		cm = 0
		# limit the number of nodes and edges drawn to not slow down 
		# the visualization for huge graphs
		drawing_limit = 5000
		for i in self.edge_order:
			u, v, d = arcs[i]
			ax, ay = positions[u]
			bx, by = positions[v]
			if(self.is_on_screen(ax,ay) or self.is_on_screen(bx,by)):
				cm = cm + 1
				if cm > drawing_limit:
					break
				self.draw_arc(ax,ay,bx,by, fill="black", width=2)

		for i in self.vertex_order:
			lat, lon = positions[i]
			if(self.is_on_screen(lat, lon)):
				cn = cn + 1
				if cn > drawing_limit:
					break
				self.draw_circle(lat, lon, self.r, outline="black", fill="blue", width=2)

	def draw_highlight(self):
		# highlight the shortest path if it exists
		for i in highlight_edges:
			u, v, d = arcs[i]
			ax, ay = positions[u]
			bx, by = positions[v]
			# print(ax, ay, bx, by)
			self.draw_arc(ax,ay,bx,by, fill="orange", width=4)
			cx, cy = (ax+bx)/2, (ay+by)/2
			self.draw_circle(cx, cy, 3*self.r, fill="orange", width=4)
		x, y = positions[source]
		self.draw_circle(x,y, 3*self.r, outline="black", fill="green", width=4)
		x, y = positions[target]
		self.draw_circle(x,y, 3*self.r, outline="black", fill="red", width=4)

	def draw(self):
		self.c.delete("all")
		if use_gmaps:
			self.draw_gmap()
		self.draw_graph()
		self.draw_highlight()

	def fetch_path(self):
		global highlight_edges
		print("request path for (s,t)",(source,target))
		p = subprocess.Popen([binary_path], stdout=PIPE,stdin=PIPE)
		shortest_path = p.communicate(bytes(interactive_input(), "ascii"), timeout=10)[0]
		line_string = shortest_path.decode()
		if line_string[0:11] == "unreachable":
			highlight_edges = []
			return
		line = [i for i in map(int, line_string.split())]
		length = line[0]
		edge_length = line[1]
		print("found path of length", length, "consisting of", edge_length, "edge(s)")
		highlight_edges = line[2:]

	def __init__(self):
		self.root = Tk.Tk()
		header = Tk.Frame(self.root)
		header.pack(side=Tk.TOP)
		Tk.Label(header, text="Street Graph Viewer (controls: left click = set start, right click = set target, 'x'+left click = recenter, 'p' = zoom in, 'm' = zoom out)").pack(side=Tk.LEFT)

		def mouse_down(event):
			global source
			x, y = (event.x, event.y)
			lat, lon = self.convert_to_real_coord(x,y)
			if self.recenter_mode:
				self.center_at(lat, lon)
				self.recenter_mode = False
			else:
				source = self.closest_vertex_id(lat, lon)
				self.fetch_path()
			self.draw()

		def right_mouse_down(event):
			global target
			x, y = (event.x, event.y)
			lat, lon = self.convert_to_real_coord(x,y)
			target = self.closest_vertex_id(lat, lon)
			self.fetch_path()
			self.draw()

		def zoomin(event):
			self.set_scale_to(self.scale * 2)
			self.zoom = self.zoom+1
			self.draw()

		def zoomout(event):
			self.set_scale_to(self.scale * 0.5)
			self.zoom = self.zoom-1
			self.draw()

		def recenter_start(event):
			self.recenter_mode = True

		self.c = Tk.Canvas(self.root, width=self.w, height=self.h)
		self.c.bind("<Button-1>", mouse_down)
		self.c.bind("<Button-3>", right_mouse_down)
		self.root.bind("<p>", zoomin)
		self.root.bind("<m>", zoomout)
		self.root.bind("<x>", recenter_start)
		self.c.pack(side=Tk.TOP)


def main():
	global gui
	read_graph()
	gui = GUI()
	gui.init_frame()
	gui.fetch_path()
	gui.draw()
	gui.c.mainloop()

main()
