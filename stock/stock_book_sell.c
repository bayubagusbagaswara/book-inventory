#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>  // Untuk mencatat waktu transaksi

#define MAX_BUKU 100

typedef enum {
    MASUK,
    KELUAR
} TipeTransaksi;


typedef struct {
    char kode[10];
    char nama[100];
    char jenis[50];
    int harga;
    int stok;
} Buku;

int loadBuku(Buku daftarBuku[], int max) {
    FILE *file = fopen("databukustok.txt", "r");
    if (!file) return 0;

    int count = 0;
    while (fscanf(file, " %[^,], %[^,], %[^,], %d, %d\n",
                  daftarBuku[count].kode,
                  daftarBuku[count].nama,
                  daftarBuku[count].jenis,
                  &daftarBuku[count].harga,
                  &daftarBuku[count].stok) == 5) {
        count++;
        if (count >= max) break;
    }

    fclose(file);
    return count;
}

void saveBuku(Buku daftarBuku[], int jumlahBuku) {
    FILE *file = fopen("databukustok.txt", "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < jumlahBuku; i++) {
        fprintf(file, "%s, %s, %s, %d, %d\n", 
                daftarBuku[i].kode, 
                daftarBuku[i].nama, 
                daftarBuku[i].jenis, 
                daftarBuku[i].harga, 
                daftarBuku[i].stok);
    }

    fclose(file);
}

void viewBuku(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) {
        printf("Tidak ada buku yang tersedia.\n");
        return;
    }

    printf("\n==================================== View Buku =================================================\n");
    printf("\n%-5s | %-10s | %-30s | %-20s | %-11s | %-5s\n", 
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga", "Stok");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBuku; i++) {
        printf("%-5d | %-10s | %-30s | %-20s | Rp.%-8d | %-5d\n",
               i + 1, 
               daftarBuku[i].kode, 
               daftarBuku[i].nama,
               daftarBuku[i].jenis, 
               daftarBuku[i].harga,
               daftarBuku[i].stok);
    }
}

void catatHistory(const char *kode, const char *nama, int jumlah, int totalHarga, TipeTransaksi tipe) {
    FILE *file = fopen("history.txt", "a");  // mode append
    if (!file) {
        printf("Gagal membuka file history.txt\n");
        return;
    }

    // Ambil waktu sekarang
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Format tanggal
    char tanggal[100];
    strftime(tanggal, sizeof(tanggal), "%Y-%m-%d %H:%M:%S", t);

    // Konversi tipe transaksi ke string
    const char *tipeStr = (tipe == MASUK) ? "MASUK" : "KELUAR";

    // Tulis ke file
    fprintf(file, "[%s] [%s] Kode: %s, Nama: %s, Jumlah: %d, Total: Rp.%d\n",
            tanggal, tipeStr, kode, nama, jumlah, totalHarga);

    fclose(file);
}

void jualBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku == 0) {
        printf("Tidak ada buku untuk dijual.\n");
        return;
    }

    char kodeJual[10];
    int jumlahJual;

    viewBuku(daftarBuku, *jumlahBuku);

    printf("\n==================================== Jual Buku =================================================\n");
    printf("\nMasukkan kode buku yang ingin dijual: ");
    scanf("%s", kodeJual);

    int ditemukan = 0;
    for (int i = 0; i < *jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kode, kodeJual) == 0) {
            ditemukan = 1;
            printf("Masukkan jumlah yang ingin dijual: ");
            scanf("%d", &jumlahJual);

            if (jumlahJual > daftarBuku[i].stok) {
                printf("Stok tidak cukup. Stok tersedia: %d\n", daftarBuku[i].stok);
            } else {
                daftarBuku[i].stok -= jumlahJual;
                saveBuku(daftarBuku, *jumlahBuku);

                // Catat ke file history
                catatHistory(daftarBuku[i].kode, daftarBuku[i].nama, jumlahJual,
                jumlahJual * daftarBuku[i].harga, KELUAR);
                
                printf("Penjualan berhasil. Sisa stok: %d\n", daftarBuku[i].stok);
            }
            break;
        }
    }

    if (!ditemukan) {
        printf("Buku dengan kode '%s' tidak ditemukan.\n", kodeJual);
    }
}

int main() {
    Buku daftarBuku[MAX_BUKU];

    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);
  
    jualBuku(daftarBuku, &jumlahBuku);
                 
    return 0;
}
