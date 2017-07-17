env = Environment()
env.Append(CXXFLAGS='-g -O0 -fprofile-arcs -ftest-coverage')
env.Append(LINKFLAGS= '-lgcov --coverage')
env.Program(target = 'mql.out', source = ["mql.cpp"])