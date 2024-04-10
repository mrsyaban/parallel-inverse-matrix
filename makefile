OUTPUT_FOLDER = bin
MATRIX_SIZE = 128

all: serial parallel

serial:
	g++ src/serial/serial.cpp -o $(OUTPUT_FOLDER)/serial

mpi:
	mpic++ src/open-mpi/mpi.cpp -o $(OUTPUT_FOLDER)/parallel

mpi-run:
	mpirun -n 4 ./bin/parallel < test_cases/$(MATRIX_SIZE).txt > res/mpi/$(MATRIX_SIZE).txt

cuda:
	nvcc -o bin/cuda src/cuda/cuda.cu

cuda-run:
	./bin/cuda < test_cases/32.txt > res/cuda/32.txt
