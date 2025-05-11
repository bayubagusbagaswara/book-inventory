#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUKU 100

typedef struct {
    char kode[10];
    char nama[100];
    char jenis[50];
    int harga;
    int stok;
} Buku;

int loadBuku(Buku daftarBuku[], int max) {
    FILE *file = fopen("databuku.txt", "r");
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
    FILE *file = fopen("databuku.txt", "w");
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

void inputBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku >= MAX_BUKU) {
        printf("Buku sudah penuh. Tidak dapat menambah buku baru.\n");
        return;
    }

    Buku bukuBaru;
    
    printf("\n=============== Input Buku ===================\n");
    printf("\nMasukkan Kode Buku: ");
    scanf("%s", bukuBaru.kode);
    printf("Masukkan Nama Buku: ");
    getchar();


    fgets(bukuBaru.nama, sizeof(bukuBaru.nama), stdin);
    bukuBaru.nama[strcspn(bukuBaru.nama, "\n")] = '\0';

    printf("Masukkan Jenis Buku: ");
    fgets(bukuBaru.jenis, sizeof(bukuBaru.jenis), stdin);
    bukuBaru.jenis[strcspn(bukuBaru.jenis, "\n")] = '\0';

    printf("Masukkan Harga Buku: ");
    scanf("%d", &bukuBaru.harga);

    printf("Masukkan Stok Buku: ");
    scanf("%d", &bukuBaru.stok);

    int kodeSudahAda = 0;

    for (int i = 0; i < *jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kode, bukuBaru.kode) == 0) {
            kodeSudahAda = 1;
            break;
        }
    }

    if (kodeSudahAda) {
        printf("Kode buku '%s' sudah digunakan. Gunakan kode lain.\n", bukuBaru.kode);
        return;
    }

    daftarBuku[*jumlahBuku] = bukuBaru;
    (*jumlahBuku)++;

    saveBuku(daftarBuku, *jumlahBuku);
    printf("Data buku berhasil disimpan.\n");
}

int main() {
    Buku daftarBuku[MAX_BUKU];

    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);

    inputBuku(daftarBuku, &jumlahBuku);
                 
    return 0;
}
