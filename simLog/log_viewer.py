import numpy as np

from pathlib import Path

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot


def parse_log_file(file_name):
	# returns chaser, target, impulses
	# chaser   : list of tuple(vec3, vec3) -> (r, v)
	# target   : list of tuple(vec3, vec3) -> (r, v)
	# impulses : list of tuple(vec3, vec3, vec3, vec3, vec3)
	# mass     : float 
	#	-> (impuls_vel, chaser_r, chaser_v, target_r, target_v)
	
	# TODO : this is too bad, rework

	chaser = []
	target = []
	impulses = []
	mass = 0.0
	with open(file_name, 'r') as log_file:
		line = log_file.readline().strip()
		while True:
			if line == '[[target]]':
				line = log_file.readline().strip()
				while line != '[[target]]' and line != '[[chaser]]' and line != '[[impulses]]' and line != '[[mass]]' and line != '':
					tokens = line.split(' ')

					r = np.zeros((3,), np.float64)
					v = np.zeros((3,), np.float64)

					r[0] = np.float64(tokens[0])
					r[1] = np.float64(tokens[1])
					r[2] = np.float64(tokens[2])
					v[0] = np.float64(tokens[3])
					v[1] = np.float64(tokens[4])
					v[2] = np.float64(tokens[5])

					target.append((r, v))

					line = log_file.readline().strip()

			if line == '[[chaser]]':
				line = log_file.readline().strip()
				while line != '[[target]]' and line != '[[chaser]]' and line != '[[impulses]]' and line != '[[mass]]' and line != '':
					tokens = line.split(' ')

					r = np.zeros((3,), np.float64)
					v = np.zeros((3,), np.float64)

					r[0] = np.float64(tokens[0])
					r[1] = np.float64(tokens[1])
					r[2] = np.float64(tokens[2])
					v[0] = np.float64(tokens[3])
					v[1] = np.float64(tokens[4])
					v[2] = np.float64(tokens[5])

					chaser.append((r, v))

					line = log_file.readline().strip()

			if line == '[[impulses]]':
				line = log_file.readline().strip()
				while line != '[[target]]' and line != '[[chaser]]' and line != '[[impulses]]' and line != '[[mass]]' and line != '':
					tokens = line.split(' ')

					i_v = np.zeros((3,), np.float64)

					c_r = np.zeros((3,), np.float64)
					c_v = np.zeros((3,), np.float64)

					t_r = np.zeros((3,), np.float64)
					t_v = np.zeros((3,), np.float64)

					i_v[0] = np.float64(tokens[0])
					i_v[1] = np.float64(tokens[1])
					i_v[2] = np.float64(tokens[2])

					c_r[0] = np.float64(tokens[3])
					c_r[1] = np.float64(tokens[4])
					c_r[2] = np.float64(tokens[5])
					c_v[0] = np.float64(tokens[6])
					c_v[1] = np.float64(tokens[7])
					c_v[2] = np.float64(tokens[8])

					t_r[0] = np.float64(tokens[9])
					t_r[1] = np.float64(tokens[10])
					t_r[2] = np.float64(tokens[11])
					t_v[0] = np.float64(tokens[12])
					t_v[1] = np.float64(tokens[13])
					t_v[2] = np.float64(tokens[14])

					impulses.append((i_v, c_r, c_v, t_r, t_v))
					line = log_file.readline().strip()				

			if line == '[[mass]]':
				mass = float(log_file.readline().strip())	
				line = log_file.readline().strip()

			if line == '':
				print('end of file reached')
				break

	return chaser, target, impulses, mass


def cross(v1, v2):
	v1x, v1y, v1z = v1[0], v1[1], v1[2]
	v2x, v2y, v2z = v2[0], v2[1], v2[2]

	res = np.zeros((3, ), np.float64)
	res[0] = v1y * v2z - v1z * v2y
	res[1] = v1z * v2x - v1x * v2z
	res[2] = v1x * v2y - v1y * v2x
	return res

def dot(v1, v2):
	v1x, v1y, v1z = v1[0], v1[1], v1[2]
	v2x, v2y, v2z = v2[0], v2[1], v2[2]
	return v1x * v2x + v1y * v2y + v1z * v2z

def length(v):

	return np.sqrt(dot(v, v))

def natural_axes_mat(r, v):
	h = cross(r, v)
	
	i = r / length(r)
	k = h / length(h)
	j = cross(k, i)

	q = np.zeros((3, 3), np.float64)
	q[ 0, : ] = i
	q[ 1, : ] = k
	q[ 2, : ] = j
	return q


def plot_log(chaser, target, impulses):
	c_x = []
	c_y = []
	c_z = []
	for i in range(len(chaser)):
		t_r = target[i][0]
		t_v = target[i][1]
		c_r = chaser[i][0]
		c_v = chaser[i][1]

		q = natural_axes_mat(t_r, t_v)

		r_rel = q @ (c_r - t_r)
		c_x.append(r_rel[0])
		c_y.append(r_rel[1])
		c_z.append(r_rel[2])
	
	i_x = []
	i_y = []
	i_z = []
	for i in range(len(impulses)):
		i_v = impulses[i][0]
		c_r = impulses[i][1]
		c_v = impulses[i][2]
		t_r = impulses[i][3]
		t_v = impulses[i][4]

		q = natural_axes_mat(t_r, t_v)
		r_rel = q @ (c_r - t_r)
		i_x.append(r_rel[0])
		i_y.append(r_rel[1])
		i_z.append(r_rel[2])

	fig = pyplot.figure()
	# ax = fig.add_subplot(1, 1, 1, projection='3d')
	ax = fig.add_subplot(1, 1, 1)
	ax.plot(c_x, c_z);
	# ax.scatter3D(i_x, i_y, i_z, s = 5, c = 'r')
	ax.scatter(i_x, i_z, s = 25, c = 'r')
	ax.set_xlabel('x')
	ax.set_ylabel('y')
	# ax.set_xlabel('x')
	# ax.set_ylabel('y')
	# ax.set_zlabel('z')
	ax.legend(['chaser trajectory'])
	pyplot.show()
	
	
def plot_mass_consumption(impulses_count, mass_cons):
	fig = pyplot.figure()
	ax = fig.add_subplot(1, 1, 1)
	ax.plot(impulses_count, mass_cons);
	ax.set_xlabel('n')
	ax.set_ylabel('m')
	ax.legend(['count-mass'])
	pyplot.show()


def traverse_dir(dir_name):
	log_data = []

	p = Path.cwd() # initialize with dir_name
	for d in p.iterdir():
		if d.is_file and d.suffix == '.log':
			print('parsing ', str(d), ' ...')
			log_data.append(parse_log_file(str(d)))
			print()

	print('plotting...')
	for chaser, target, impulses, mass in log_data:
		plot_log(chaser, target, impulses)

	n = []
	m = []
	for chaser, target, impulses, mass in log_data:
		n.append(len(impulses) - 1)
		m.append(mass)
	plot_mass_consumption(n, m)


if __name__ == '__main__':
	# TODO : add argparse
	traverse_dir(None)
