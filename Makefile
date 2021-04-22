compiler = clang++
linkflags = -lSDL2 -lGLEW -lGL
files = main.cpp
compilerflags = -O2
outfile = gl_template


build: $(files) Makefile
	$(compiler) $(files) -o $(outfile) $(linkflags) $(compilerflags)

run : build
	./$(outfile)

clean : 
	rm $(outfile)