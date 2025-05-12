#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_BUKU 100

typedef enum {
    MASUK,
    KELUAR
} TipeTransaksi;

typedef struct {
    char waktu[20];
    char aksi[10]; // "KELUAR" atau "MASUK"
    char kode[10];
    char nama[50];
    int jumlah;
    int total;
} History;

void viewHistory() {
    FILE *file = fopen("history.txt", "r");
    if (file == NULL) {
        printf("Belum ada history.\n");
        return;
    }

    History histories[100];
    int jumlahData = 0;
    char baris[256];

    // Membaca file dan simpan ke struct
    while (fgets(baris, sizeof(baris), file)) {
        sscanf(baris, "%[^,], %[^,], %[^,], %[^,], %d, %d",
               histories[jumlahData].waktu,
               histories[jumlahData].aksi,
               histories[jumlahData].kode,
               histories[jumlahData].nama,
               &histories[jumlahData].jumlah,
               &histories[jumlahData].total);
        jumlahData++;
    }
    fclose(file);

    if (jumlahData == 0) {
        printf("Belum ada history.\n");
        return;
    }

    // Tampilkan header kolom
    printf("\n==================================== List History Buku =================================================\n");
    printf("\n%-5s | %-20s | %-10s | %-10s | %-20s | %-7s | %-10s\n", 
           "No", "Waktu", "Aksi", "Kode", "Nama Buku", "Jumlah", "Total (Rp)");
    printf("------------------------------------------------------------------------------------------------------\n");

    // Tampilkan semua data history
    for (int i = 0; i < jumlahData; i++) {
        printf("%-5d | %-20s | %-10s | %-10s | %-20s | %-7d | %-10d\n",
               i + 1,
               histories[i].waktu,
               histories[i].aksi,
               histories[i].kode,
               histories[i].nama,
               histories[i].jumlah,
               histories[i].total);
    }
}

void deleteHistory() {
    FILE *file = fopen("history.txt", "r");
    if (file == NULL) {
        printf("Belum ada history.\n");
        return;
    }

    History histories[100];
    int jumlahData = 0;
    char baris[256];

    // Membaca semua data ke dalam array of struct
    while (fgets(baris, sizeof(baris), file)) {
        sscanf(baris, "%[^,], %[^,], %[^,], %[^,], %d, %d",
               histories[jumlahData].waktu,
               histories[jumlahData].aksi,
               histories[jumlahData].kode,
               histories[jumlahData].nama,
               &histories[jumlahData].jumlah,
               &histories[jumlahData].total);
        jumlahData++;
    }
    fclose(file);

    if (jumlahData == 0) {
        printf("Tidak ada history yang bisa dihapus.\n");
        return;
    }

    printf("\n==================================== Menu: Hapus History =================================================\n");
    viewHistory();

    // Meminta input index untuk dihapus
    int index;
    printf("Pilih index data yang ingin dihapus (1 - %d): ", jumlahData);
    scanf("%d", &index);

    if (index < 1 || index > jumlahData) {
        printf("Index tidak valid.\n");
        return;
    }

    // Menulis ulang data ke file kecuali data yang dihapus
    FILE *newFile = fopen("history.txt", "w");
    for (int i = 0; i < jumlahData; i++) {
        if (i != index - 1) {
            fprintf(newFile, "%s, %s, %s, %s, %d, %d\n",
                    histories[i].waktu,
                    histories[i].aksi,
                    histories[i].kode,
                    histories[i].nama,
                    histories[i].jumlah,
                    histories[i].total);
        }
    }
    fclose(newFile);

    printf("Data berhasil dihapus.\n");
}



int main() {

    viewHistory();
    return 0;
}