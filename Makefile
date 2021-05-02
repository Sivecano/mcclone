compiler = clang++
linkflags = -lSDL2 -lGLEW -lGL -lSDL2_image -lcudart
directories = -I gl_util -I world_gen -I cudaKernels
files = main.cpp gl_util/shaders.cpp rendering.cpp gl_util/Camera.cpp
compilerflags = -O2
outfile = mcclone

cuda_files = cudaKernels/kernels.cu
cuda_comp = nvcc
cuda_compflags = -O2

all: $(files) Makefile build cucomp
	$(compiler) $(files) build/cuda.o -o build/$(outfile) $(linkflags) $(compilerflags) $(directories)
 
run : compile
	(cd build && exec ./$(outfile))

build:
	mkdir build
	ln -sr ./shaders ./build/shaders
	ln -sr ./textures ./build/textures

cucomp: $(cuda_files)
	$(cuda_comp) $(cuda_files) -c -o build/cuda.o $(cuda_compflags)

clean : 
	rm build/$(outfile)
	rm build/cuda.o