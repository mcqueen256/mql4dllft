import os

test_env = Environment()
test_env.Append(CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/ -I./test/include/')
test_env.Append(LINKFLAGS= '--coverage') # -lgcov for linux

src_dir = 'src/'
test_src_dir = 'test/src/'
test_dir = 'test/'
build_dir = 'build/'

cleaning_map = {}

project_test_objects = []
for file in os.listdir(src_dir):
	if file.endswith('cpp'):
		t_name = '.'.join([file.split('.')[0], 'test', 'o'])
		obj = test_env.Object(target=t_name, source=file, srcdir=src_dir)
		project_test_objects.extend(obj)
		cleaning_map[file.split('.')[0] + '.gcda'] = obj
		cleaning_map[file.split('.')[0] + '.test.gcno'] = obj

test_main = test_env.Object(target='test.o', source='test.cpp', srcdir=test_dir)
cleaning_map['test.gcda'] = obj
cleaning_map['test.gcno'] = obj

test_programs = []
test_program_names = []
for file in os.listdir(test_src_dir):
	if file.endswith('cpp'):
		t_name = '.'.join([file.split('.')[0], 'test', 'out'])
		test_program_names.append(t_name)
		dependencies = [file, test_main] + project_test_objects
		program = test_env.Program(target=t_name, source=dependencies, srcdir=test_src_dir)
		test_programs.extend(program)
		cleaning_map[file.split('.')[0] + '.test.gcda'] = program
		cleaning_map[file.split('.')[0] + '.test.gcno'] = program

for file, node in cleaning_map.iteritems():
	Clean(node, file)

#for file in os.listdir(test_src_dir):
#	if file.endswith('cpp'):
#		#objects.extend(test_env.Object(file, srcdir=src_dir))
#		test_env.Program(target)

#print(objects)

#test_program = test_env.Program(target='mql', source=objects)
