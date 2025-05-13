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
    char kode[10];
    char nama[100];
    char jenis[50];
    int harga;
    int stok;
} Buku;

typedef struct {
    char waktu[20];
    char aksi[10]; // "KELUAR" atau "MASUK"
    char kode[10];
    char nama[50];
    int jumlah;
    int total;
} History;

void tampilkanMenu() {
    printf("\n==================================== Menu Utama ================================================\n");
    printf("1. Input Buku\n");
    printf("2. View Buku\n");
    printf("3. Delete Buku\n");
    printf("4. Input Transaksi\n");
    printf("5. View History\n");
    printf("6. Delete History\n");
    printf("7. Keluar\n");
    printf("Pilih menu (1-7): ");
}

int kembaliKeMenuUtama() {
    char jawaban;
    printf("\nApakah ingin kembali ke menu utama? (Y/N): ");
    scanf(" %c", &jawaban);
    return toupper(jawaban) == 'Y';
}

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
    fprintf(file, "%s, %s, %s, %s, %d, %d\n",
            tanggal, tipeStr, kode, nama, jumlah, totalHarga);

    fclose(file);
}

void inputBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku >= MAX_BUKU) {
        printf("Buku sudah penuh. Tidak dapat menambah buku baru.\n");
        return;
    }

    Buku bukuBaru;
    
    printf("\n==================================== Menu Input Buku ===========================================\n");
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

    catatHistory(bukuBaru.kode, bukuBaru.nama, bukuBaru.stok,
             bukuBaru.stok * bukuBaru.harga, MASUK);

    printf("Data buku berhasil disimpan.\n");
}

void viewBuku(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) {
        printf("Tidak ada buku yang tersedia.\n");
        return;
    }

    printf("\n==================================== List Data Buku ============================================\n");
    printf("\n%-5s | %-10s | %-30s | %-20s | %-11s | %-5s\n", 
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga (Rp)", "Stok");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahBuku; i++) {
        printf("%-5d | %-10s | %-30s | %-20s | %-11d | %-5d\n",
               i + 1, 
               daftarBuku[i].kode, 
               daftarBuku[i].nama,
               daftarBuku[i].jenis, 
               daftarBuku[i].harga,
               daftarBuku[i].stok);
    }
}

void inputTransaksi(Buku daftarBuku[], int *jumlahBuku) {
    char kode[10];
    char perintah;
    int jumlah;
    int ditemukan = 0;

    printf("\n==================================== Menu: Input Transaksi =====================================\n");
    viewBuku(daftarBuku, *jumlahBuku);

    do {
        printf("\nTransaksi Barang Masuk / Barang Keluar (M/K): ");
        scanf(" %c", &perintah);
        perintah = toupper(perintah);
        if (perintah != 'M' && perintah != 'K') {
            printf("Input tidak valid. Masukkan 'M' untuk Masuk atau 'K' untuk Keluar.\n");
        }
    } while (perintah != 'M' && perintah != 'K');

    printf("Masukkan kode buku: ");
    scanf("%s", kode);

    for (int i = 0; i < *jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kode, kode) == 0) {
            ditemukan = 1;

            printf("Masukkan jumlah barang: ");
            scanf("%d", &jumlah);

            if (perintah == 'M') { // Barang Masuk
                daftarBuku[i].stok += jumlah;
                printf("Transaksi barang masuk berhasil. Stok sekarang: %d\n", daftarBuku[i].stok);
                catatHistory(daftarBuku[i].kode, daftarBuku[i].nama,
                             jumlah, jumlah * daftarBuku[i].harga, MASUK);

            } else if (perintah == 'K') { // Barang Keluar
                if (jumlah > daftarBuku[i].stok) {
                    printf("Stok tidak cukup. Stok tersedia: %d\n", daftarBuku[i].stok);
                } else {
                    daftarBuku[i].stok -= jumlah;
                    printf("Transaksi barang keluar berhasil. Sisa stok: %d\n", daftarBuku[i].stok);
                    catatHistory(daftarBuku[i].kode, daftarBuku[i].nama,
                                 jumlah, jumlah * daftarBuku[i].harga, KELUAR);
                }
            } else {
                printf("Perintah tidak valid. Gunakan B untuk beli atau J untuk jual.\n");
            }

            saveBuku(daftarBuku, *jumlahBuku);
            break;
        }
    }

    if (!ditemukan) {
        printf("Buku dengan kode %s tidak ditemukan.\n", kode);
    }
}

void deleteBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku == 0) {
        printf("Tidak ada buku yang dapat dihapus.\n");
        return;
    }

    printf("\n==================================== Menu: Hapus Buku ==========================================\n");
    viewBuku(daftarBuku, *jumlahBuku);

    int index;
    printf("\nMasukkan index buku yang ingin dihapus (1 - %d): ", *jumlahBuku);
    scanf("%d", &index);

    if (index < 1 || index > *jumlahBuku) {
        printf("Index tidak valid. Penghapusan dibatalkan.\n");
        return;
    }

    // Geser data ke kiri untuk menghapus
    for (int i = index - 1; i < *jumlahBuku - 1; i++) {
        daftarBuku[i] = daftarBuku[i + 1];
    }

    (*jumlahBuku)--;

    saveBuku(daftarBuku, *jumlahBuku);

    printf("Data buku successfully deleted.\n");
}

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
    printf("\n==================================== List History ==================================================\n");
    printf("\n%-5s | %-20s | %-10s | %-10s | %-20s | %-7s | %-10s\n", 
           "No", "Waktu", "Aksi", "Kode", "Nama Buku", "Jumlah", "Total (Rp)");
    printf("----------------------------------------------------------------------------------------------------\n");

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

    printf("\n==================================== Menu: Hapus History ===========================================\n");
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

    printf("Data history successfully deleted.\n");
}


int main() {
    Buku daftarBuku[MAX_BUKU];
    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    printf("\n==================================== Toko Buku =================================================\n");
    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);

    int pilihan;
    do {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                do {
                    inputBuku(daftarBuku, &jumlahBuku);
                } while (!kembaliKeMenuUtama());
                break;
            case 2:
                do {
                    viewBuku(daftarBuku, jumlahBuku);
                } while (!kembaliKeMenuUtama());
                break;
            case 3:
                do {
                    deleteBuku(daftarBuku, &jumlahBuku);
                } while (!kembaliKeMenuUtama());
                break;
            case 4:
                do {
                    inputTransaksi(daftarBuku, &jumlahBuku);
                } while (!kembaliKeMenuUtama());
                break;
            case 5:
                do {
                    viewHistory();
                } while (!kembaliKeMenuUtama());
                break;
            case 6:
                do {
                    deleteHistory();
                } while (!kembaliKeMenuUtama());
                break;
            case 7:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan != 7);

    return 0;
}
