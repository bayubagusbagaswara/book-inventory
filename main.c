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

typedef struct {
    char kode[10];
    char nama[100];
    int jumlah;
    int totalHarga;
} History;

void tampilkanMenu() {
  printf("\n=== Menu Toko Buku ===\n");
    printf("1. Input Buku\n");
    printf("2. View History\n");
    printf("3. View Buku\n");
    printf("4. Delete History\n");
    printf("5. Delete Buku\n");
    printf("6. Input Transaksi\n");
    printf("7. Exit\n");
    printf("Pilih menu (1-6): ");
}

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

void inputBuku(Buku daftarBuku[], int *jumlahBuku) {
    if (*jumlahBuku >= MAX_BUKU) {
        printf("Buku sudah penuh. Tidak dapat menambah buku baru.\n");
        return;
    }

    Buku bukuBaru;
    
    // Minta input dari pengguna
    printf("\nMasukkan Kode Buku: ");
    scanf("%s", bukuBaru.kode);
    printf("Masukkan Nama Buku: ");
    getchar();  // membersihkan newline dari input sebelumnya
    fgets(bukuBaru.nama, sizeof(bukuBaru.nama), stdin);
    bukuBaru.nama[strcspn(bukuBaru.nama, "\n")] = '\0'; // menghapus newline yang mungkin ada

    printf("Masukkan Jenis Buku: ");
    fgets(bukuBaru.jenis, sizeof(bukuBaru.jenis), stdin);
    bukuBaru.jenis[strcspn(bukuBaru.jenis, "\n")] = '\0'; // menghapus newline

    printf("Masukkan Harga Buku: ");
    scanf("%d", &bukuBaru.harga);

    // Simpan buku baru ke array daftarBuku
    daftarBuku[*jumlahBuku] = bukuBaru;
    (*jumlahBuku)++;

    // Simpan buku ke file
    saveBuku(daftarBuku, *jumlahBuku);
}

void tampilkanBuku(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) {
        printf("Tidak ada buku yang tersedia.\n");
        return;
    }

    printf("\n=== Daftar Buku ===\n");
    for (int i = 0; i < jumlahBuku; i++) {
        printf("%d. Kode: %s | Nama: %s | Jenis: %s | Harga: Rp%d\n",
               i + 1, daftarBuku[i].kode, daftarBuku[i].nama,
               daftarBuku[i].jenis, daftarBuku[i].harga);
    }
}

void deleteBuku(Buku daftarBuku[], int *jumlah) {
    if (*jumlah == 0) {
        printf("Tidak ada data buku untuk dihapus.\n");
        return;
    }

    // Menampilkan daftar buku
    printf("Daftar Buku:\n");
    for (int i = 0; i < *jumlah; i++) {
        printf("%d. Kode: %s | Nama: %s | Jenis: %s | Harga: %d\n",
               i + 1, daftarBuku[i].kode, daftarBuku[i].nama, daftarBuku[i].jenis, daftarBuku[i].harga);
    }

    // Memilih buku yang akan dihapus
    int indeks;
    printf("Masukkan nomor buku yang ingin dihapus (1 - %d): ", *jumlah);
    scanf("%d", &indeks);

    if (indeks < 1 || indeks > *jumlah) {
        printf("Nomor tidak valid.\n");
        return;
    }

    // Menggeser elemen setelah yang dihapus
    for (int i = indeks - 1; i < *jumlah - 1; i++) {
        daftarBuku[i] = daftarBuku[i + 1];
    }

    (*jumlah)--;

    // Menyimpan kembali daftar buku ke dalam file
    FILE *file = fopen("databuku.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file databuku.txt\n");
        return;
    }

    for (int i = 0; i < *jumlah; i++) {
        fprintf(file, "%s;%s;%s;%d\n", daftarBuku[i].kode, daftarBuku[i].nama, daftarBuku[i].jenis, daftarBuku[i].harga);
    }

    fclose(file);
    printf("Buku berhasil dihapus.\n");
}

void inputTransaksi(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) {
        printf("Tidak ada buku yang tersedia untuk dijual.\n");
        return;
    }

    tampilkanBuku(daftarBuku, jumlahBuku);

    int index, jumlah;
    printf("\nMasukkan nomor buku yang ingin dibeli: ");
    scanf("%d", &index);
    if (index < 1 || index > jumlahBuku) {
        printf("Index tidak valid.\n");
        return;
    }

    printf("Masukkan jumlah pembelian: ");
    scanf("%d", &jumlah);
    if (jumlah <= 0) {
        printf("Jumlah tidak valid.\n");
        return;
    }

    History history;
    strcpy(history.kode, daftarBuku[index - 1].kode);
    strcpy(history.nama, daftarBuku[index - 1].nama);
    history.jumlah = jumlah;
    history.totalHarga = jumlah * daftarBuku[index - 1].harga;

    FILE *file = fopen("history.txt", "a");
    if (!file) {
        printf("Gagal membuka file history.txt\n");
        return;
    }

    fprintf(file, "%s, %s, %d, %d\n", 
            history.kode, history.nama, 
            history.jumlah, history.totalHarga);
    fclose(file);

    printf("Transaksi berhasil dicatat.\n");
}

void tampilkanHistory() {
    FILE *file = fopen("history.txt", "r");
    if (!file) {
        printf("Belum ada history transaksi.\n");
        return;
    }

    char kode[10], nama[100];
    int jumlah, total;
    int index = 1;
    
    printf("\n=== History Transaksi ===\n");

    // Membaca setiap baris dan memformat data transaksi
    while (fscanf(file, " %[^,], %[^,], %d, %d\n", kode, nama, &jumlah, &total) == 4) {
        printf("%d. Kode: %s | Buku: %s | Jumlah: %d | Total: Rp%d\n",
               index++, kode, nama, jumlah, total);
    }

    if (index == 1) {
        printf("Belum ada data transaksi.\n");
    }
    fclose(file);
}

void hapusHistory() {
    FILE *file = fopen("history.txt", "r");
    if (!file) {
        printf("Belum ada data history untuk dihapus.\n");
        return;
    }

    History history[100];  // Gunakan struct Transaksi
    int count = 0;

    // Baca data dari file
    while (fscanf(file, " %[^,], %[^,], %d, %d\n",
                  history[count].kode,
                  history[count].nama,
                  &history[count].jumlah,
                  &history[count].totalHarga) == 4) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("Belum ada data history.\n");
        return;
    }

    // Tampilkan daftar history
    printf("\n=== History Transaksi ===\n");
    for (int i = 0; i < count; i++) {
        printf("%d. Kode: %s | Buku: %s | Jumlah: %d | Total: Rp.%d\n",
               i + 1, history[i].kode, history[i].nama,
               history[i].jumlah, history[i].totalHarga);
    }

    // Input index
    int index;
    printf("Pilih nomor history yang ingin dihapus (1-%d): ", count);
    scanf("%d", &index);

    if (index < 1 || index > count) {
        printf("Index tidak valid.\n");
        return;
    }

    // Simpan ulang tanpa index yang dihapus
    file = fopen("history.txt", "w");
    for (int i = 0; i < count; i++) {
        if (i != index - 1) {
            fprintf(file, "%s, %s, %d, %d\n",
                    history[i].kode,
                    history[i].nama,
                    history[i].jumlah,
                    history[i].totalHarga);
        }
    }
    fclose(file);

    printf("Data berhasil dihapus.\n");
}

int main() {
    Buku daftarBuku[MAX_BUKU];
    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);  // Memanggil fungsi loadBuku dan mendapatkan jumlah buku yang berhasil dimuat

    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);
    
    int pilihan;
    do {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                inputBuku(daftarBuku, &jumlahBuku); // Memanggil fungsi untuk input buku baru
                break;
            case 2:
                tampilkanHistory();
                break;
            case 3:
                tampilkanBuku(daftarBuku, jumlahBuku);
                break;
            case 4:
                hapusHistory();
                break;
            case 5:
                deleteBuku(daftarBuku, &jumlahBuku);
                break;
            case 6:
                inputTransaksi(daftarBuku, jumlahBuku);
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