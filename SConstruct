import os

test_env = Environment()
test_env.Append(CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/ -I./test/include/')

optimised_env = Environment()
optimised_env.Append(CXXFLAGS='-std=c++11 -O0 -I./test/include/')
optimised_env.Append(LINKFLAGS= '--coverage -pthread -lpthread -Wl,--no-as-needed') # -lgcov for linux

src_dir = 'src/'
test_src_dir = 'test/src/'
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
for file in cpp_dir(test_src_dir):
	t_name = '.'.join([file.split('.')[0], 'test', 'out'])
	obj = test_env.Object(target=t_name, source=file, srcdir=test_src_dir)
	case_test_objects.extend(obj)

# The catch code including main
main_test_object = optimised_env.Object(target='test', source=['test.cpp'], srcdir=test_dir)
main_test_program = optimised_env.Program(target=build_dir + 'mql4dllft', source=main_test_object + case_test_objects + project_test_objects, srcdir=test_dir)



