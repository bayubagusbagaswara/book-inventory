# Soal Project Team

Sebuah toko buku menjual berbagai macam jenis buku. Pemilik toko buku tersebut menginginkan sebuah aplikasi yang mampu mencatat semua transaksi yang ada sehingga memudahkan dalam me-monitoring usahanya.

Berikut aturan dalam membuat aplikasinya:
Pertama kali program dijalankan, program akan membaca file “databuku.txt”. Pada data tersebut terdapat data buku antara lain: kode buku, nama buku, jenis buku, dan harga.

Program memiliki 6 pilihan, yaitu:
a. Input (Add Book)
b. View History
c. View Buku
d. Delete History
e. Delete Buku (Delete Book)
f. Exit

Keterangan Pilihan tampilan

1. Jika user memilih menu Input data (tekan tombol ‘1’), maka program akan:
   • Meminta inputan buku (nama buku, jenis buku, dan harga)
2. Jika user memilih menu View History (tekan tombol ‘2’), maka program akan menampilkan data history penjualan yang pernah dilakukan.
3. Jika user memilih menu View Data (tekan tombol ‘3’), maka program akan menampilkan seluruh data buku.
4. Jika user memilih menu Delete History (tekan tombol ‘4’), maka program akan menampilkan list data history penjualan.
   • Meminta inputan index. Validasikan input minimal 1 dan maksimal sebanyak jumlah data
   • Hapus data sesuai dengan index yang diinput. Contoh: Jika user memilih index 1, maka hapus data yang pertama. Lalu Tampilkan pesan “Data Successfully delete..”.
5. Jika user memilih menu Delete Buku (tekan tombol ‘5’), maka program akan menampilkan list data buku.
   • Meminta inputan index. Validasikan input minimal 1 dan maksimal sebanyak jumlah data
   • Hapus data sesuai dengan index yang diinput. Contoh: Jika user memilih index 1, maka hapus data yang pertama. Lalu Tampilkan pesan “Data Successfully delete..”.
6. Jika user memilih menu Exit (tekan tombol ‘6’), maka program akan menulis data tersebut ke dalam file ”databuku.txt” dan program selesai dijalankan.

# Catatan

int jumlahBuku = 0;
Nilai awalnya harus 0 karena:
Sebelum membaca file, belum ada data buku yang dimuat.
Setiap kali satu baris data buku berhasil dibaca, program akan menjalankan jumlahBuku++.
Jika tidak diinisialisasi ke 0, maka nilainya acak (garbage value) dan program bisa salah menyimpan ke index yang tidak valid.
