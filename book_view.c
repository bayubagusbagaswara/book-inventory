// tidak menggunkan Pointer karena arena fungsi lihatBuku hanya membaca data, bukan mengubahnya.
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

int main() {
    Buku daftarBuku[MAX_BUKU];
    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);  // Memanggil fungsi loadBuku dan mendapatkan jumlah buku yang berhasil dimuat

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);

    viewBuku(daftarBuku, jumlahBuku);

    return 0;
}