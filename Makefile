compiler = clang++
linkflags = -lSDL2 -lGLEW -lGL
directories = -I gl_util -I world_gen
files = main.cpp gl_util/shaders.cpp rendering.cpp
compilerflags = -O2
outfile = mcclone

all: $(files) Makefile ./build
	$(compiler) $(files) -o build/$(outfile) $(linkflags) $(compilerflags) $(directories)
 
run : compile
	(cd build && exec ./$(outfile))

build:
	mkdir build
	ln -sr ./shaders ./build/shaders 

clean : 
	rm build/$(outfile)