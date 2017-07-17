import os

test_env = Environment()
test_env.Append(CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/ -I./test/include/')

optimised_env = Environment()
optimised_env.Append(CXXFLAGS='-std=c++11 -O3 -I./test/include/')
optimised_env.Append(LINKFLAGS= '--coverage -pthread') # -lgcov for linux

src_dir = 'src/'
test_src_dir = 'test/src/'
test_dir = 'test/'
build_dir = 'build/'

cleaning_map = {}

# Object files for robot code
project_test_objects = []
for file in os.listdir(src_dir):
	if file.endswith('cpp'):
		t_name = '.'.join([file.split('.')[0], 'test', 'o'])
		obj = test_env.Object(target=t_name, source=file, srcdir=src_dir)
		project_test_objects.extend(obj)
		cleaning_map[file.split('.')[0] + '.test.gcda'] = obj
		cleaning_map[file.split('.')[0] + '.test.gcno'] = obj

# Test Programs
test_objects = []
for file in os.listdir(test_src_dir):
	if file.endswith('cpp'):
		t_name = '.'.join([file.split('.')[0], 'test', 'out'])
		program = test_env.Object(target=t_name, source=file, srcdir=test_src_dir)
		test_objects.extend(program)
		cleaning_map[file.split('.')[0] + '.test.gcda'] = program
		cleaning_map[file.split('.')[0] + '.test.gcno'] = program

test_main = optimised_env.Program(target='mql4dllft', source=['test.cpp'] + test_objects + project_test_objects, srcdir=test_dir)

for file, node in cleaning_map.iteritems():
	Clean(node, file)




#for file in os.listdir(test_src_dir):
#	if file.endswith('cpp'):
#		#objects.extend(test_env.Object(file, srcdir=src_dir))
#		test_env.Program(target)

#print(objects)

#test_program = test_env.Program(target='mql', source=objects)
