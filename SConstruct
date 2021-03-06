import os, platform
windows = lambda: platform.system().lower() == 'windows'
linux = lambda: 'linux' in platform.system().lower()
osx = lambda: platform.system().lower() == 'darwin'

project_env = Environment(TARGET_ARCH='x86')
if windows():
	CXXFLAGS='/EHsc /I.\\include\\'
	if 'dll' in COMMAND_LINE_TARGETS:
		CXXFLAGS += ' /DPRODUCTION'
elif linux():
	CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/'
elif osx():
	CXXFLAGS='-g -O0 -std=c++11 -Wall -pedantic -I./include/'
project_env.Append(CXXFLAGS=CXXFLAGS)

test_env = Environment(TARGET_ARCH='x86')
if windows():
	CXXFLAGS='/EHsc /I.\\include\\ /I.\\test\\include\\'
elif linux():
	CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage -std=c++11 -Wall -pedantic -I./include/ -I./test/include/'
elif osx():
	CXXFLAGS='-g -O0 -std=c++11 -Wall -pedantic -I./include/ -I./test/include/'
test_env.Append(CXXFLAGS=CXXFLAGS)

program_env = Environment(TARGET_ARCH='x86')
if windows():
	CXXFLAGS='/EHsc -I./test/include/'
	LINKFLAGS=''
elif linux():
	CXXFLAGS='-std=c++11 -O0 -I./test/include/'
	LINKFLAGS='--coverage -pthread -lpthread -Wl,--no-as-needed -lgcov'
elif osx():
	CXXFLAGS='-std=c++11 -O0 -I./test/include/'
	LINKFLAGS=''
program_env.Append(CXXFLAGS=CXXFLAGS)
program_env.Append(LINKFLAGS= LINKFLAGS)

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
project_objects = []
for file in cpp_dir(src_dir):
	t_name = '.'.join([file.split('.')[0], 'test', 'o'])
	obj = project_env.Object(target=t_name, source=file, srcdir=src_dir)
	project_objects.extend(obj)

# Test case objects
case_test_objects = []
for file in cpp_dir(test_src_dir):
	t_name = '.'.join([file.split('.')[0], 'test', 'o'])
	obj = test_env.Object(target=t_name, source=file, srcdir=test_src_dir)
	case_test_objects.extend(obj)

# The catch code including main
main_test_object = program_env.Object(target='test', source=['test.cpp'], srcdir=test_dir)
main_test_program = program_env.Program(target=build_dir + 'mql4dllft', source=main_test_object + case_test_objects + project_objects, srcdir=test_dir)
Default(main_test_program)

dll_env = Environment(TARGET_ARCH='x86')
CXXFLAGS='/EHsc /D_USRDLL /D_WINDLL -I./test/include/ /DPRODUCTION'
dll_env.Append(CXXFLAGS=CXXFLAGS)
dll_env.SharedLibrary(target=build_dir + 'bot.dll', source=project_objects)
dll_env.Alias('dll', build_dir + 'bot.dll')
