# Open MPI

> Made to fulfill Tugas Kecil IF3230 Parallel and Distributed Systems

## Parallel program
Program ini dibuat menggunakan bahasa pemrograman `C++` dan library `Open MPI`. Paralelisasi program memanfaatkan MPI (Message Passing Interface) dengan fungsi `MPI_Bcast`, `MPI_Scatter`, dan `MPI_Gather` untuk mengirimkan data antar proses. juga digunakan fungsi `MPI_Barrier` untuk melakukan sinkronisasi antar proses.

rank 0 menerima input matrix, kemudian diubah menjadi augmented matrix. Setelah itu, rank 0 akan melakukan scatter untuk membagi matrix ke setiap proses sama rata. kemudian setiap proses akan melakukan Gauss elimination dan normalisasi secara paralel, setiap kali iterasi untuk gauss elimination, pivot row akan dibroadcast dari proses dimana pivot row saat itu berada ke semua proses. Oleh karena itu, `MPI_Barrier` akan dipanggil setiap iterasi untuk memastikan pivot row dari semua proses terupdate saat memulai iterasi selanjutnya. 

## Data partitioning
Skema pembagian data yang digunakan di sini adalah pembagian matrix berdasarkan jumlah baris, yaitu setiap proses bertanggung jawab untuk sebagian baris matrix. Hal ini dilakukan dengan menggunakan MPI_Scatter. Alasan pemilihan skema ini adalah agar setiap proses memiliki bagian matrix yang cukup kecil untuk diproses secara efisien, dan distribusi kerja di antara proses-proses secara merata.

## How to use
```
make mpi-run
```
