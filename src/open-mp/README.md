# Open MP

> Made to fulfill Tugas Kecil IF3230 Parallel and Distributed Systems

## Parallel program
Program ini dibuat menggunakan bahasa pemrograman `C++` dan library `OpenMP`. Paralelisasi program memanfaatkan pragma parallel `#pragma omp parallel` dengan batasan jumlah thread yang digunakan selalu faktor dari jumlah baris masukan matrix. Untuk setiap pragma parallel,semua thread menggunakan variabel `mat` yang sama namun iterasi dikerjakan oleh beberapa thread sekaligus. sehingga setiap thread akan melakukan alokasi memori, inisialisasi matriks identitas, perhitungan pengurangan matriks, dan juga konversi ke matrix unit semuanya setiap thread lakukan untuk baris yang berbeda secara independen.


## Data partitioning
Dalam program ini, skema pembagian data yang dipilih adalah dengan membagi baris matriks antara thread. Ini dipilih karena setiap iterasi atau operasi dalam loop utama tidak bergantung pada hasil iterasi atau operasi lainnya pada baris yang sama kecuali pada saat proses `reduksi ke baris diagonal atau eliminasi gauss-jordan` pada loop pertama setiap baris bergantung pada pivot row yang selalu berubah-ubah setiap iterasi terluarnya. Oleh karena itu, setiap thread dapat bekerja secara independen pada baris matriks yang berbeda, meningkatkan efisiensi paralelisme. 
