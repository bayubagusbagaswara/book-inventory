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
    if (!file) return 0;

    int count = 0;
    while (fscanf(file, " %[^,], %[^,], %[^,], %d\n",
                  daftarBuku[count].kode,
                  daftarBuku[count].nama,
                  daftarBuku[count].jenis,
                  &daftarBuku[count].harga) == 4) {
        count++;
        if (count >= max) break;
    }

    fclose(file);
    return count;
}

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
}

void viewBuku(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) {
        printf("Tidak ada buku yang tersedia.\n");
        return;
    }

    printf("\n%-5s | %-10s | %-30s | %-20s | %-10s\n", 
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga");
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBuku; i++) {
        printf("%-5d | %-10s | %-30s | %-20s | Rp.%-8d\n",
               i + 1, 
               daftarBuku[i].kode, 
               daftarBuku[i].nama,
               daftarBuku[i].jenis, 
               daftarBuku[i].harga);
    }
}

void deleteBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku == 0) {
        printf("Tidak ada buku untuk dihapus.\n");
        return;
    }

    // Tampilkan daftar buku terlebih dahulu
    printf("\n=============================== Daftar Buku =========================================\n");
    viewBuku(daftarBuku, *jumlahBuku);

    char kodeHapus[10];
    printf("Masukkan kode buku yang ingin dihapus: ");
    scanf("%s", kodeHapus);

    int ditemukan = 0;
    for (int i = 0; i < *jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kode, kodeHapus) == 0) {
            // Geser semua buku setelahnya ke kiri
            for (int j = i; j < *jumlahBuku - 1; j++) {
                daftarBuku[j] = daftarBuku[j + 1];
            }
            (*jumlahBuku)--;  // Kurangi jumlah buku
            ditemukan = 1;
            printf("Buku dengan kode '%s' berhasil dihapus.\n", kodeHapus);
            break;
        }
    }

    if (!ditemukan) {
        printf("Buku dengan kode '%s' tidak ditemukan.\n", kodeHapus);
    } else {
        // Simpan ulang ke file
        saveBuku(daftarBuku, *jumlahBuku);
        printf("Data buku berhasil diupdate.\n");
    }
}

int main() {
    Buku daftarBuku[MAX_BUKU];

    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);

    deleteBuku(daftarBuku, &jumlahBuku);

    return 0;
}