#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUKU 100

typedef struct {
    char kode[10];
    char nama[100];
    char jenis[50];
    int harga;
} Buku;

int loadBuku(Buku daftarBuku[], int max) {
    FILE *file = fopen("databuku.txt", "r");
    if (!file) return 0;  // Jika file gagal dibuka, return 0

    int count = 0;  // Variabel untuk menghitung jumlah buku yang berhasil dimuat
    while (fscanf(file, " %[^,], %[^,], %[^,], %d\n",
                  daftarBuku[count].kode,     // Membaca kode buku
                  daftarBuku[count].nama,     // Membaca nama buku
                  daftarBuku[count].jenis,    // Membaca jenis buku
                  &daftarBuku[count].harga) == 4) {  // Membaca harga buku
        count++;  // Menambah jumlah buku setelah berhasil memuat satu buku
        if (count >= max) break;  // Jika jumlah buku mencapai batas maksimum, keluar dari loop
    }

    fclose(file);  // Menutup file setelah selesai membaca
    return count;  // Mengembalikan jumlah buku yang berhasil dimuat
}

// Fungsi saveBuku untuk menyimpan data buku ke file
void saveBuku(Buku daftarBuku[], int jumlahBuku) {
    FILE *file = fopen("databuku.txt", "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < jumlahBuku; i++) {
        fprintf(file, "%s, %s, %s, %d\n", 
                daftarBuku[i].kode, 
                daftarBuku[i].nama, 
                daftarBuku[i].jenis, 
                daftarBuku[i].harga);
    }

    fclose(file);
    printf("Data buku berhasil disimpan.\n");
}

// Fungsi inputBuku untuk menambah buku baru
// daftarBuku[]: array berisi data buku (Buku daftarBuku[MAX_BUKU] dari main).
// int *jumlahBuku: pointer ke jumlah buku saat ini, agar bisa diperbarui dari fungsi ini
void inputBuku(Buku daftarBuku[], int *jumlahBuku) {
    // Cek apakah jumlah buku sudah mencapai batas maksimum
    if (*jumlahBuku >= MAX_BUKU) {
        printf("Buku sudah penuh. Tidak dapat menambah buku baru.\n");
        return;
    }

    // untuk menyimpan input buku dari pengguna sebelum dimasukkan ke array
    Buku bukuBaru;
    
    // Minta input dari pengguna
    printf("\nMasukkan Kode Buku: ");
    scanf("%s", bukuBaru.kode);
    printf("Masukkan Nama Buku: ");
    getchar();  // membersihkan newline dari input sebelumnya

    // fgets membaca satu baris penuh (termasuk spasi)
    // fgets juga menyimpan newline, jadi kita perlu menghapusnya
    // dengan mengganti karakter newline dengan null terminator
    // strcspn menghitung panjang string hingga karakter tertentu
    fgets(bukuBaru.nama, sizeof(bukuBaru.nama), stdin);
    bukuBaru.nama[strcspn(bukuBaru.nama, "\n")] = '\0'; // menghapus newline yang mungkin ada

    printf("Masukkan Jenis Buku: ");
    fgets(bukuBaru.jenis, sizeof(bukuBaru.jenis), stdin);
    bukuBaru.jenis[strcspn(bukuBaru.jenis, "\n")] = '\0'; // menghapus newline

    printf("Masukkan Harga Buku: ");
    scanf("%d", &bukuBaru.harga);

    // Simpan buku baru ke array daftarBuku
    // Menyimpan bukuBaru ke array daftarBuku di posisi terakhir.
    // *jumlahBuku menunjukkan posisi kosong berikutnya.
    // Setelah ditambahkan, jumlah buku dinaikkan +1
    daftarBuku[*jumlahBuku] = bukuBaru;
    (*jumlahBuku)++;

    // Simpan buku ke file
    saveBuku(daftarBuku, *jumlahBuku);
}

int main() {
    // Inisialisasi array untuk menyimpan data buku
    // dan variabel untuk menyimpan jumlah buku
    // yang berhasil dimuat dari file
    Buku daftarBuku[MAX_BUKU];

    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);
  
    // Kenapa kita menggunakan pointer disini?
    // Karena kita ingin mengubah nilai dari jumlahBuku
    // di dalam fungsi inputBuku, dan kita perlu
    // mengirimkan alamatnya agar bisa diubah
    // Kamu ingin menambah jumlah buku.
    // Nilai itu harus tersimpan di luar fungsi.
    // Pointer memungkinkan fungsi mengubah variabel asli di main
    inputBuku(daftarBuku, &jumlahBuku);
                 
    return 0;
}
