OUTPUT_FOLDER = bin
MATRIX_SIZE = 128

all: serial parallel

parallel:
	mpic++ src/open-mpi/mpi.cpp -o $(OUTPUT_FOLDER)/parallel

serial:
	g++ src/serial/serial.cpp -o $(OUTPUT_FOLDER)/serial

run:
	mpirun -n 4 ./bin/parallel < test_cases/$(MATRIX_SIZE).txt > res/mpi/$(MATRIX_SIZE).txt