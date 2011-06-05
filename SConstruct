env = Environment(CC = 'g++', CCFLAGS = '-ggdb', CPPPATH = ['../inc'])

SConscript('src/SConscript', build_dir='build', exports='env')
SConscript('tests/SConscript', exports='env')
SConscript('bin/SConscript', exports='env')

