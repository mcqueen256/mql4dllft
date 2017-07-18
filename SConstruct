import os

test_env = Environment()
test_env.Append(CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/ -I./test/include/')

optimised_env = Environment()
optimised_env.Append(CXXFLAGS='-g -std=c++11 -O0 -I./include/ -I./test/include/')
optimised_env.Append(LINKFLAGS= '--coverage -pthread') # -lgcov for linux

src_dir = 'src/'
catch_test_src_dir = 'test/catch_src/'
mem_test_src_dir = 'test/mem_src/'
test_dir = 'test/'
build_dir = 'build/'

def cpp_dir(dir):
	return [file for file in os.listdir(dir) if file.endswith('cpp')]

# make a build directory if one does not already exist
if not os.path.isdir(build_dir):
	os.mkdir(build_dir)

# Object files for robot code with coverage
project_test_objects = []
for file in cpp_dir(src_dir):
	t_name = '.'.join([file.split('.')[0], 'test', 'o'])
	obj = test_env.Object(target=t_name, source=file, srcdir=src_dir)
	project_test_objects.extend(obj)

# Test case objects
case_test_objects = []
for file in cpp_dir(catch_test_src_dir):
	t_name = '.'.join([file.split('.')[0], 'test', 'out'])
	obj = test_env.Object(target=t_name, source=file, srcdir=catch_test_src_dir)
	case_test_objects.extend(obj)

# The catch code including main
main_test_object = optimised_env.Object(target='test', source=['test.cpp'], srcdir=test_dir)
main_test_program = optimised_env.Program(target=build_dir + 'mql4dllft', source=main_test_object + case_test_objects + project_test_objects, srcdir=test_dir)

# clean up debug generated files
cleaning_map = {}
for file in cpp_dir(src_dir):
	cleaning_map[file.split('.')[0] + '.test.gcda'] = obj
	cleaning_map[file.split('.')[0] + '.test.gcno'] = obj
for file in cpp_dir(catch_test_src_dir):
	cleaning_map[file.split('.')[0] + '.test.gcda'] = obj
	cleaning_map[file.split('.')[0] + '.test.gcno'] = obj
for file, node in cleaning_map.iteritems():
	Clean(node, file)


# Object files for robot code without coverage
project_objects = []
for file in cpp_dir(src_dir):
	obj = optimised_env.Object(target=file+'.o', source=file, srcdir=src_dir)
	project_objects.extend(obj)

# Test case programs
for file in cpp_dir(catch_test_src_dir):
	t_name = '.'.join([file.split('.')[0], 'test'])
	optimised_env.Program(target=build_dir + t_name, source=[file] + main_test_object + project_objects, srcdir=catch_test_src_dir)

# Memory leaks test programs
for file in cpp_dir(mem_test_src_dir):
	t_name = '.'.join([file.split('.')[0], 'memtest'])
	optimised_env.Program(target=build_dir + t_name, source=[file] + project_objects, srcdir=mem_test_src_dir)

