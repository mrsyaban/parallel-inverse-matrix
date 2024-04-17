# CUDA

## Parallelisasi program
Program menggunakan CUDA untuk memanfaatkan pemrosesan paralel GPU. Fungsi-fungsi reduceToDiagonal dan reduceToUnit dijalankan secara paralel di GPU. Setiap blok dan thread di GPU akan mengolah bagian matriks yang berbeda secara bersamaan. Setelah operasi selesai, hasilnya disalin kembali dari memori perangkat ke memori host menggunakan cudaMemcpy.

## Data partitioning
setiap thread di GPU akan mengolah masing-masing satu baris dari matriks.
pada proses `reduceToUnit`, setiap thread akan mengoperasikan satu baris matriks dan karena tidak ada ketergantungan antar baris di matriks maka operasi bisa berjalan secara independen.
pada proses `reduceToDiagonal` paralelisasi dilakukan dalam iterasi karena pada loop pertama setiap baris bergantung pada pivot row yang selalu berubah-ubah pada iterasi selanjutnya. Sehingga, setiap thread dapat bekerja secara independen pada baris matriks yang berbeda, meningkatkan efisiensi paralelisme. 

## How to run

1. make sure you have `nvcc` installed on your device
2. compile cuda.cu
```
make cuda
```
3. run object file
```
make cuda-run
```
4. your result will be on `res/cuda`