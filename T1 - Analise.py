#!/usr/bin/python
import matplotlib.pyplot as plt
import math
import re
import sys

# Person class
class Person:
	name = ''
	duration = 0
	paths = []
	speed = 0.0
	
	def __init__(self, name, duration, paths):
		self.name = name
		self.duration = duration
		self.paths = paths

	def calculate_speed(self):
		global metrics
		
		time_i = int(self.paths[0].time)
		time_f = int(self.paths[len(self.paths)-1].time)
		x_i = int(self.paths[0].x)
		x_f = int(self.paths[len(self.paths)-1].x)
		y_i = int(self.paths[0].y)
		y_f = int(self.paths[len(self.paths)-1].y)

		distance = float(math.sqrt(math.pow((x_i - x_f),2) + math.pow((y_i - y_f), 2)))
		speed = float(float((distance/int(metrics))) / (float((time_f - time_i) / 30))) # Return distance in m/s

		return speed
			

# Path class
class Path:
	x = 0
	y = 0
	time = 0

	def __init__(self, x, y, time):
		self.x = x
		self.y = y
		self.time = time

# Group class
class Group:
	group_id = 0
	elements = []

	def __init__(self, group_id, elements):
		self.group_id = group_id
		self.elements = elements
			

# GLOBAL VARS
people = []
metrics = 0 # pixels to meter scale
g_id = 0 # group id
groups = []

# Parses trajectory file
def parse_trajectory(file):
	lines = file.split('\n')
	linePointer = 1 # used to point position inside a line 
	global metrics

	personNumber = 0
	for line in lines:
		if len(line) > 0:
			# Get metrics parameter
			if line[0] == '[':
				line = line.strip('[')[:-2]
				metrics = line
			# Get each person from file
			else:
				parameters = line.split('\t')

				paths = split_path(parameters[1])

				people.append(Person("Person {}".format(personNumber), parameters[0], paths))
			personNumber = personNumber + 1

# Returns the paths parsed correctly
def split_path(allpaths):
	unparsedPaths = []
	parsedPaths = []

	allpaths = allpaths.strip('(')[:-2]

	unparsedPaths = allpaths.split(')(')

	for path in unparsedPaths:
		coordinatesTime = path.split(',')
		parsedPaths.append(Path(coordinatesTime[0],coordinatesTime[1],coordinatesTime[2]))

	return parsedPaths

# Gets the time of the last frame of the video
def get_last_timestamp():
	last = 1
	
	for person in people:
		for path in person.paths:
			if int(path.time) > last:
				last = int(path.time)
	return int(last)

# Gets the greater x value
def get_max_x():
	max_x = 1
	
	for person in people:
		for path in person.paths:
			if int(path.x) > max_x:
				max_x = int(path.x)
	return int(max_x)

# Gets the greater x value
def get_max_y():
	max_y = 1
	
	for person in people:
		for path in person.paths:
			if int(path.y) > max_y:
				max_y = int(path.y)
	return int(max_y)

# Generate paths png files
def generate_png_files():
	last = int(get_last_timestamp())
	for i in range(1, last+1):
		x=[]
		y=[]
		for person in people:
			for path in person.paths:
				if int(path.time) == i:
					x.append(int(path.x))
					y.append(int(path.y))

		plt.plot(x, y, 'bs')
		plt.axis([0, get_max_x(), get_max_y(), 0])
		file_name = '{}.png'.format(i)		
		plt.savefig(file_name)

# Detect group formation
def detect_group():
	threshold = int(metrics) # maximum distance to consider group formation
	
	for person in people:
		for otherPerson in people:
			tempGroupTime = 0
			if person.name != otherPerson.name: # can't check the same person

				for path in person.paths:
					for otherPath in otherPerson.paths:
						if path.time == otherPath.time:
							# calculate distance between them on the same timestamp
							if int(path.x) > int (otherPath.x):
								x1 = int(path.x)
								x2 = int(otherPath.x)
							else:
								x2 = int(path.x)
								x1 = int(otherPath.x)
							if int(path.y) > int (otherPath.y):
								y1 = int(path.y)
								y2 = int(otherPath.y)
							else:
								y2 = int(path.y)
								y1 = int(otherPath.y)
							distance = math.sqrt(math.pow((x1 - x2),2) + math.pow((y1 - y2), 2))
							
							if int(distance) < threshold: # group formation detected!
								tempGroupTime = tempGroupTime + 1
						if tempGroupTime > 48: # solid group
							add_group(person.name, otherPerson.name)

# create a group
def add_group(personName, otherPersonName):
	global g_id
	groupExists = False
	shouldCreate = True
	# Checks if group already exists
	for group in groups:
		if personName in group.elements and otherPersonName in group.elements:
			groupExists = True
	if not groupExists:
		# Check if one of them belongs to a group (meaning the other person also belongs to that group)
		for group in groups:
			if personName in group.elements:
				group.elements.append(otherPersonName)
				shouldCreate = False
			elif otherPersonName in group.elements:
				group.elements.append(personName)
				shouldCreate = False
		if shouldCreate:
			elementsTemp = [personName, otherPersonName]
			groups.append(Group(g_id, elementsTemp))
			g_id = g_id + 1

# ANALISE 1
# Calculates the average person speed as walking in group or walking alone
def med_speed():
	groupMedSpeed = 0
	gCount = 0
	aloneMedSpeed = 0
	aCount = 0
	groupSpeed = []
	aloneSpeed = []

	for person in people:
		belongsToGroup = False
		pSpeed = person.calculate_speed()
		for group in groups:
			for element in group.elements:
				if person.name == element:
					belongsToGroup = True
		if belongsToGroup:
			groupSpeed.append(pSpeed)
		else:
			aloneSpeed.append(pSpeed)
	for speed in groupSpeed:
		groupMedSpeed = groupMedSpeed + speed
		gCount = gCount + 1
	for speed in aloneSpeed:
		aloneMedSpeed = aloneMedSpeed + speed
		aCount = aCount + 1		

	print "Velocidade media de pessoas andando em grupos: {} m/s".format(groupMedSpeed/gCount)
	print "Velocidade media de pessoas andando sozinhas: {} m/s".format(aloneMedSpeed/aCount)

# ANALISE 2
# Calculates the % of people walking alone and formed groups
def group_alone_ratio():
	aCount = 0
	gCount = 0
	for person in people:
		belongsToGroup = False
		pSpeed = person.calculate_speed()
		for group in groups:
			for element in group.elements:
				if person.name == element:
					belongsToGroup = True
		if not belongsToGroup:
			aCount = aCount + 1
	for group in groups:
		gCount = gCount + 1
	print "Porcentagem de pessoas andando sozinhas: {}%".format((aCount*100)/(aCount+gCount))
	print "Porcentagem de grupos: {}%".format((gCount*100)/(aCount+gCount))

# ANALISE 3
# Calculates the average distance of unknown people
def distance_unknown_people():
	totalMedDistance = 0
	pCount = 0
	global metrics
	for person in people:
		for otherPerson in people:
			shouldCalculate = True
			medDistance = 0
			medDistanceCount = 0
			# Validates if shouldCalculate or not
			for group in groups:
				for element in group.elements:
					if person.name == element:
						for otherElement in group.elements:
							if otherPerson.name == otherElement or otherElement == element:
								shouldCalculate = False
			if shouldCalculate:
				
				for path in person.paths:
					for otherPath in otherPerson.paths:
						if path.time == otherPath.time:
							# calculate distance between them on the same timestamp
							medDistanceCount = medDistanceCount + 1
							if int(path.x) > int (otherPath.x):
								x1 = int(path.x)
								x2 = int(otherPath.x)
							else:
								x2 = int(path.x)
								x1 = int(otherPath.x)
							if int(path.y) > int (otherPath.y):
								y1 = int(path.y)
								y2 = int(otherPath.y)
							else:
								y2 = int(path.y)
								y1 = int(otherPath.y)
							medDistance = medDistance + (math.sqrt(math.pow((x1 - x2),2) + math.pow((y1 - y2), 2)))
				if medDistanceCount != 0:
					pCount = pCount + 1				
					totalMedDistance = totalMedDistance + (medDistance / medDistanceCount )
	print "Distancia media de pessoas desconhecidas: {} metros".format((totalMedDistance / pCount) / int(metrics))

def main():
	if len(sys.argv) != 2:
		print "Passagem de parametros deve ser no formato: python T1 - Analise.py <arquivo_Path_D.txt>"
		return
	
	global metrics
	trajectory = open(sys.argv[1]).read()
	parse_trajectory(trajectory)
	detect_group()

	generate_png_files()
	med_speed()
	group_alone_ratio()
	distance_unknown_people()
	

if __name__ == '__main__':
	main()
