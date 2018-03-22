# Perbandingan Kinerja Bitonic Sort antara Parallel dengan Non-Parallel
- **Turfa Auliarachman** - 13515133

Pembagian tugas: Semua dikerjakan oleh **Turfa Auliarachman**, 13515133.

## Requirements
1. *C Compiler wrapper* implementasi MPI dapat diakses dengan perintah `mpicc`.
3. *Wrapper* program MPI dapat diakses dengan perintah `mpirun`.
2. *Compiler* C dapat diakses dengan perintah `cc`.

## Petunjuk Penggunaan
### Kompilasi
Untuk melakukan kompilasi, gunakan perintah:
```
make
```

### Menjalankan Program
Untuk menjalankan program, gunakan perintah:
```
mpirun -np [jumlah_proses] ./bitonic_sort [jumlah_data]

[jumlah_data] dan [jumlah_proses] harus berupa bilangan bulat positif.
```

## Laporan
### Solusi Parallel
Solusi parallel yang saya buat adalah dengan melakukan *data parallelism*,
yaitu memparallelkan program dengan membagi-bagi berdasarkan datanya.

### Analisis Solusi
Karena dalam satu waktu, (*virtually*) program akan menangani lebih banyak data
sekaligus, maka solusi ini akan lebih cepat daripada tanpa dilakukan secara
parallel.

Saya belum menemukan solusi parallel yang cocok digunakan menggunakan MPI yang
lebih baik dari ini.

### Jumlah Proses dan Analisis
Jumlah proses yang saya gunakan adalah 8.

Hal ini dikarenakan *overhead* dari mekanisme *message passing* yang cukup
tinggi. Pada jumlah data yang diuji, ketika jumlah proses lebih banyak dari 8,
*overhead* dari mekanisme *message passing* justru akan membuat program berjalan
lebih lambat.

### Pengukuran Waktu
| Banyak Elemen | Waktu Percobaan 1 (ms) | Waktu Percobaan 2 (ms) | Waktu Percobaan 3 (ms) | Waktu Rata-Rata |
| :-----------: | :--------------------: | :--------------------: | :--------------------: | :-------------: |
| 5.000 | 1.662 (**single process**) <br /> 13.005 (**multiprocess**) | 1.652 (**single process**) <br /> 14.850 (**multiprocess**) | 1.637 (**single process**) <br /> 6.683 (**multiprocess**) | 1.650 <br /> 11.512 |
| 50.000 | 51.123 (**single process**) <br /> 34.959 (**multiprocess**) | 26.257 (**single process**) <br /> 17.627 (**multiprocess**) | 29.146 (**single process**) <br /> 40.307 (**multiprocess**) | 35.508 <br /> 30.964 |
| 100.000 | 116.486 (**single process**) <br /> 47.266 (**multiprocess**) | 44.883 (**single process**) <br /> 53.626 (**multiprocess**) | 109.692 (**single process**) <br /> 47.733 (**multiprocess**) | 90.353 <br /> 49.541 |
| 200.000 | 146.608 (**single process**) <br /> 95.732 (**multiprocess**) | 203.378 (**single process**) <br /> 98.319 (**multiprocess**) | 161.972 (**single process**) <br /> 113.130 (**multiprocess**) | 170.652 <br /> 102.393 |
| 400.000 | 170.652 (**single process**) <br /> 186.570 (**multiprocess**) | 314.907 (**single process**) <br /> 230.354 (**multiprocess**) | 332.193 (**single process**) <br /> 156.663 (**multiprocess**) | 272.584 <br /> 191.195 |

### Analisis Perbandingan
Pada percobaan dengan banyak elemen 5.000, terjadi penurunan kecepatan
sebanyak 600%. Pqda percobaan dengan banyak elemen 50.000, 100.000, 200.000,
dan 400.000, berturut-turut peningkatannya adalah sebanyak 13%, 46%, 40%, dan
30%.

Dari sana, kita melihat bahwa semakin banyak jumlah elemen, semakin besar juga
peningkatan kecepatannya. Bahkan ketika jumlah elemen 5.000, yang terjadi malah
penurunan kecepatan. Hal itu dikarenakan ketika jumlah elemennya lebih
sedikit, waktu *overhead* akan lebih terasa dibanding ketika jumlah elemennya
lebih banyak.
