compiler = clang++
linkflags = -lSDL2 -lGLEW -lGL -lSDL2_image -lcudart
directories = -I gl_util -I world_gen -I cudaKernels
compilerflags = -O2
outfile = mcclone

cuda_comp = nvcc
cuda_compflags = -O2

objs = 	main.o \
		gl_util/shaders.o \
		rendering.o \
		gl_util/Camera.o \
		cudaKernels/bufferinterface.o \
		cudaKernels/worldgen.o \
		world_gen/ChunkSystem.o \
		Entities/Player.o \

all: $(objs) Makefile
	$(compiler) $(objs) -o build/$(outfile) $(linkflags) -v

 
run : compile
	(cd build && exec ./$(outfile))

build:
	mkdir build

./build/shaders: build
	ln -sr ./shaders ./build

./build/textures: build
	ln -sr ./textures ./build

cucomp: $(cuda_files)
	$(cuda_comp) $(cuda_files) -c $(cuda_compflags)

%.o:	%.cu
	$(cuda_comp) $(cuda_compflags) -c $< -o $@

%.o:	%.cpp
	$(compiler) $(compilerflags) -c $< -o $@ $(directories)

oclean :
	rm -rf $(objs)

clean : oclean
	rm -rf build/*


