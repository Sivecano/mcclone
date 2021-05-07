compiler = clang++
linkflags = -lSDL2 -lGLEW -lGL -lSDL2_image -lcudart
directories = -I gl_util -I world_gen -I cudaKernels
compilerflags = -O2
outfile = mcclone

cuda_comp = nvcc
cuda_compflags = -O2

		cudaKernels/bufferinterface.o \
all: $(objs) Makefile build
objs = 	main.o \
		gl_util/shaders.o \
		rendering.o \
		gl_util/Camera.o \
		cudaKernels/bufferinterface.o \
		cudaKernels/worldgen.o

all: $(objs) Makefile build/shaders build/textures
	$(compiler) $(objs) -o build/$(outfile) $(linkflags)
	make oclean

 
run : compile
	(cd build && exec ./$(outfile))

build: build/shaders build/textures
	mkdir build

build/shaders: build
	ln -sr ./shaders ./build/shaders

build/textures: build
	ln -sr ./textures ./build/textures

cucomp: $(cuda_files)
	$(cuda_comp) $(cuda_files) -c $(cuda_compflags)

%.o:	%.cu
	$(cuda_comp) $(cuda_compflags) -c $< -o $@

%.o:	%.cpp
	$(compiler) $(compilerflags) -c $< -o $@ $(directories)

oclean :
	rm -rf *.o

clean : oclean
	rm -rf build/*


