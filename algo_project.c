// Anggota Kelompok:
// 1. BAYU BAGUS BAGASWARA - 2802641312
// 2. I MADE BUDIAWAN - 2802640152
// 3. NOVA DWI SETYA - 2802642826
// 4. MRISKI PADILA - 2802639056

#include <stdio.h> // Header file untuk input/output
#include <string.h> // Header file untuk string
#include <stdlib.h> // Header file untuk fungsi umum
#include <time.h> // Header file untuk waktu
#include <ctype.h> // Header file untuk fungsi karakter

#define MAX_BUKU 100

// struct untuk menyimpan data buku
typedef struct {
    char kode[10];
    char nama[100];
    char jenis[50];
    int harga;
} Buku;

// struct untuk menyimpan data history penjualan
typedef struct {
    char waktu[20];
    char kode[10];
    char nama[50];
    int harga;
    char jenis[50];
} Penjualan;

// Fungsi untuk menampilkan menu utama
void tampilkanMenu() {
    printf("\n==================================== Menu Utama ================================================\n");
    printf("1. Input Buku\n");
    printf("2. View History\n");
    printf("3. View Buku\n");
    printf("4. Delete History\n");
    printf("5. Delete Buku\n");
    printf("6. Exit\n");
    printf("Pilih menu (1-6): ");
}

// Fungsi untuk menampilkan konfirmasi kembali ke menu utama
int kembaliKeMenuUtama() {
    char jawaban;
    printf("\nApakah ingin kembali ke menu utama? (Y/N): ");
    scanf(" %c", &jawaban);
    return toupper(jawaban) == 'Y';
    
}

// Fungsi untuk memuat data buku dari file
int loadBuku(Buku daftarBuku[], int max) {
    // Membuka file penyimpanan data buku dengan mode read
    FILE *file = fopen("databuku.txt", "r");

    // Jika file tidak ditemukan, mengembalikan 0 atau false
    if (!file) return 0;
 
    // Membaca input dari file yang dibuka
    int count = 0;
    // Menggunakan fscanf untuk membaca data buku dari file
    // Format: kode, nama, jenis, harga
    // Menggunakan %[^,] untuk membaca string hingga koma
    // Menggunakan %d untuk membaca integer
    // Menggunakan %s untuk membaca string hingga newline
    while (fscanf(file, " %[^,], %[^,], %[^,], %d\n",
                  daftarBuku[count].kode,
                  daftarBuku[count].nama,
                  daftarBuku[count].jenis,
                  &daftarBuku[count].harga) == 4) {
                    // Menggunakan 4 karena ada 4 data yang dibaca
        count++;
        // Jika jumlah buku sudah mencapai maksimum, keluar dari loop
        // Menggunakan break untuk menghentikan loop
        // Menggunakan if untuk memeriksa apakah count sudah mencapai max
        if (count >= max) break;
    }
    
    // Menutup file setelah selesai membaca
    fclose(file);
    // Mengembalikan jumlah buku yang berhasil dimuat
    return count;
}

// Fungsi untuk menyimpan data buku ke file
void saveBuku(Buku daftarBuku[], int jumlahBuku) {
    // Membuka file penyimpanan data buku dengan mode write
    // Jika file tidak ditemukan, mengembalikan 0 atau false
    FILE *file = fopen("databuku.txt", "w");
    if (!file) {
        // Jika gagal membuka file, menampilkan pesan error
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    // Menyimpan data buku ke file
    // Menggunakan loop untuk menulis setiap buku ke file
    for (int i = 0; i < jumlahBuku; i++) {
        // Menggunakan fprintf untuk menulis data buku ke file
        fprintf(file, "%s, %s, %s, %d\n", 
                daftarBuku[i].kode, 
                daftarBuku[i].nama, 
                daftarBuku[i].jenis, 
                daftarBuku[i].harga); 
    }
    // Menutup file setelah selesai menulis
    fclose(file);
}

// Fungsi untuk menambahkan buku baru
void inputBuku(Buku daftarBuku[], int *jumlahBuku) {

    // Validasi jika jumlah buku sudah mencapai maksimum
    if (*jumlahBuku >= MAX_BUKU) {
        printf("Buku sudah penuh. Tidak dapat menambah buku baru.\n");
        return;
    }

    // Membuat variabel bukuBaru untuk menyimpan data buku baru
    Buku bukuBaru;
    
    printf("\n==================================== Menu Input Buku ===========================================\n");
    printf("\nMasukkan Kode Buku: ");
    scanf("%s", bukuBaru.kode);
    printf("Masukkan Nama Buku: ");
    getchar();

    // Menggunakan fgets untuk membaca string dengan spasi
    fgets(bukuBaru.nama, sizeof(bukuBaru.nama), stdin);
    bukuBaru.nama[strcspn(bukuBaru.nama, "\n")] = '\0';

    printf("Masukkan Jenis Buku: ");
    fgets(bukuBaru.jenis, sizeof(bukuBaru.jenis), stdin);
    bukuBaru.jenis[strcspn(bukuBaru.jenis, "\n")] = '\0';

    printf("Masukkan Harga Buku: ");
    scanf("%d", &bukuBaru.harga);

    // kode untuk validasi jika kode buku sudah ada
    int kodeSudahAda = 0;

    // Menggunakan loop untuk memeriksa apakah kode buku sudah ada
    for (int i = 0; i < *jumlahBuku; i++) {
        if (strcmp(daftarBuku[i].kode, bukuBaru.kode) == 0) {
            kodeSudahAda = 1;
            break;
        }
    }

    // Jika kode buku sudah ada, menampilkan pesan error
    if (kodeSudahAda) {
        printf("Kode buku '%s' sudah digunakan. Gunakan kode lain.\n", bukuBaru.kode);
        return;
    }

    // Menambahkan buku baru ke daftar buku
    // Menggunakan indeks jumlahBuku untuk menambahkan buku baru
    // Menggunakan *jumlahBuku untuk mendapatkan nilai dari pointer
    // Menggunakan * untuk dereference pointer
    daftarBuku[*jumlahBuku] = bukuBaru;
    (*jumlahBuku)++;

    saveBuku(daftarBuku, *jumlahBuku);

    printf("Data buku berhasil disimpan.\n");
}

// Fungsi untuk menampilkan daftar buku
void viewBuku(Buku daftarBuku[], int jumlahBuku) {
    if (jumlahBuku == 0) { // kondisi jika penyimpanan daftar buku kosong
        printf("Tidak ada buku yang tersedia.\n");
        return;
    }

    printf("\n==================================== List Data Buku ============================================\n");
    printf("\n%-5s | %-10s | %-30s | %-20s | %-11s\n", 
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga (Rp)");
    printf("------------------------------------------------------------------------------------------------\n");
    
    // Perintah untuk menampilkan daftar buku yang tersimpan didalam file
    for (int i = 0; i < jumlahBuku; i++) {
        printf("%-5d | %-10s | %-30s | %-20s | %-11d \n",
               i + 1, 
               daftarBuku[i].kode, 
               daftarBuku[i].nama,
               daftarBuku[i].jenis, 
               daftarBuku[i].harga);
    }
}

// Fungsi untuk menghapus buku dari daftar
void deleteBuku(Buku daftarBuku[], int *jumlahBuku) {
    // Validasi jika jumlah buku sudah mencapai maksimum
    if (*jumlahBuku == 0) {
        printf("Tidak ada buku yang dapat dihapus.\n");
        return;
    }

    printf("\n==================================== Menu: Hapus Buku ==========================================\n");
    viewBuku(daftarBuku, *jumlahBuku);

    int index;
    printf("\nMasukkan index buku yang ingin dihapus (1 - %d): ", *jumlahBuku);
    scanf("%d", &index);

    // Validasi jika index tidak valid
    if (index < 1 || index > *jumlahBuku) {
        printf("Index tidak valid. Penghapusan dibatalkan.\n");
        return;
    }

    // Geser data ke kiri untuk menghapus
    // Menggunakan loop untuk menggeser data buku
    // Menggunakan index - 1 untuk mendapatkan indeks yang benar
    for (int i = index - 1; i < *jumlahBuku - 1; i++) {
        daftarBuku[i] = daftarBuku[i + 1];
    }

    // Mengurangi jumlah buku setelah penghapusan
    // Menggunakan *jumlahBuku untuk mendapatkan nilai dari pointer
    (*jumlahBuku)--;

    // Menyimpan data buku ke file setelah penghapusan
    saveBuku(daftarBuku, *jumlahBuku);
    printf("Data buku successfully deleted.\n");
}

// Fungsi untuk mencatat history penjualan
void viewHistoryPenjualan() {
    // Membuka file penyimpanan history penjualan dengan mode read
    FILE *file = fopen("historyPenjualan.txt", "r");

    // Jika file tidak ditemukan, menampilkan pesan error
    if (file == NULL) {
        printf("Belum ada history.\n");
        return;
    }

    // Membuat array untuk menyimpan data history penjualan
    Penjualan histories[100];
    // Menggunakan variabel untuk menyimpan jumlah data
    int jumlahData = 0;
    // Membuat variabel untuk menyimpan baris data
    char baris[256];

    // Membaca file dan simpan ke struct
    while (fgets(baris, sizeof(baris), file)) {
        sscanf(baris, "%[^,], %[^,], %[^,], %d, %[^\n]",
               histories[jumlahData].kode,
               histories[jumlahData].nama,
               histories[jumlahData].jenis,
               &histories[jumlahData].harga,
               histories[jumlahData].waktu);
        jumlahData++;
    }

    // Menutup file setelah selesai membaca
    fclose(file);
    
    // Validasi jika tidak ada data history
    if (jumlahData == 0) {
        printf("Belum ada history penjualan.\n");
        return;
    }

    // Tampilkan header kolom
    printf("\n==================================== List History Penjualan ==================================================\n");
    printf("\n%-5s | %-10s | %-30s | %-20s | %-20s | %-10s \n",
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga (Rp)", "Waktu");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    // Tampilkan semua data history
    for (int i = 0; i < jumlahData; i++) {
        printf("%-5d | %-10s | %-30s | %-20s | %-20d | %-10s\n",
               i + 1,
               histories[i].kode,
               histories[i].nama,
               histories[i].jenis,
               histories[i].harga,
               histories[i].waktu);
    }
}

// Fungsi untuk memuat data history penjualan dari file
int loadHistoryPenjualan() {

    // Membuka file penyimpanan history penjualan dengan mode read
    FILE *file = fopen("historyPenjualan.txt", "r");

    // Jika file tidak ditemukan, mengembalikan 0 atau false
    if (!file) return 0;

    // Penyimpanan data history penjualan
    Penjualan histories[100];
    
    // Menggunakan variabel untuk menyimpan jumlah data
    int jumlahPenjualan = 0;

    // Membaca file dan simpan ke struct
    // Menggunakan fscanf untuk membaca data history penjualan dari file   
    while (fscanf(file, "%[^,],%[^,],%[^,],%d,%[^\n]",
                  histories[jumlahPenjualan].kode,
                  histories[jumlahPenjualan].nama,
                  histories[jumlahPenjualan].jenis,
                  &histories[jumlahPenjualan].harga,
                  histories[jumlahPenjualan].waktu) == 5) {
                    // Menggunakan 5 karena ada 5 data yang dibaca

        jumlahPenjualan++; // Menghitung jumlah penjualan
        // Jika jumlah penjualan sudah mencapai maksimum, keluar dari loop
    }

    fclose(file);
    return jumlahPenjualan;
}

// Fungsi untuk menghapus history penjualan
void deletehistoryPenjualan(int *jumlahData) {

    *jumlahData = loadHistoryPenjualan();
    
    // Membuka file penyimpanan history penjualan dengan mode read
    FILE *file = fopen("historyPenjualan.txt", "r");
    
    // Membuka file penyimpanan history penjualan dengan mode write
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL) {
        // Jika file tidak ditemukan, menampilkan pesan error
        printf("Belum ada history.\n");
        return;
    } else if (*jumlahData == 0) {
        // Jika tidak ada data history, menampilkan pesan error
        printf("Tidak ada history yang dapat dihapus.\n");
        return;
    } else {
        // Menampilkan data history penjualan
        viewHistoryPenjualan();
    }

    char baris[256];
    int index;
    
    printf("\nMasukkan index history yang ingin dihapus (1 - %d): ", *jumlahData);
    scanf("%d", &index);
    
    if (index < 1 || index > *jumlahData) {
        printf("Index tidak valid. Penghapusan dibatalkan.\n");
        return;
    }

    // Menggeser data ke kiri untuk menghapus
    int barispertama = 1;
    
    // Menggunakan loop untuk membaca file dan menulis ke file sementara
    while (fgets(baris, sizeof(baris), file)!= NULL) {
        // Menggunakan if untuk memeriksa apakah baris pertama adalah index yang dihapus
        if (barispertama != index) {
            // Menggunakan fputs untuk menulis data ke file sementara
            fputs(baris,temp);
        }
        // Menggunakan increment untuk menghitung baris pertama
        barispertama++;
    }

    // Menutup file setelah selesai membaca
    fclose(file);
    // Menutup file sementara setelah selesai menulis
    fclose(temp);

    // Menghapus file history penjualan yang lama
    remove("historyPenjualan.txt");

    // Mengganti nama file sementara menjadi file history penjualan
    rename("temp.txt", "historyPenjualan.txt");
    
    printf("Data history successfully deleted.\n");
}

int main() {
    // Inisialisasi array untuk menyimpan data buku
    Buku daftarBuku[MAX_BUKU];

    // Inisialisasi jumlah buku
    int jumlahBuku = loadBuku(daftarBuku, MAX_BUKU);

    // Inisialisasi jumlah penjualan
    int jumlahPenjualan = loadHistoryPenjualan();

    printf("\n==================================== Toko Buku =================================================\n");
    printf("Total buku berhasil dimuat: %d\n", jumlahBuku);

    int pilihan;
    do {
        // Menampilkan menu utama
        // Menggunakan fungsi untuk menampilkan menu utama
        tampilkanMenu();
        scanf("%d", &pilihan);

        // Menggunakan switch case untuk memilih menu
        switch (pilihan) {
            case 1:
                // Memanggil fungsi untuk input buku
                do {
                    inputBuku(daftarBuku, &jumlahBuku);
                } while (!kembaliKeMenuUtama()); // diulang jika ingin kembali ke menu utama
                break;
            case 2:
                do {
                    // Memanggil fungsi untuk menampilkan history penjualan
                    viewHistoryPenjualan();
                } while (!kembaliKeMenuUtama()); // diulang jika ingin kembali ke menu utama
                break;
            case 3:
                do {
                    // Memanggil fungsi untuk menampilkan daftar buku
                    viewBuku(daftarBuku, jumlahBuku);
                } while (!kembaliKeMenuUtama()); // diulang jika ingin kembali ke menu utama
                break;
            case 4:
                do {
                    // Memanggil fungsi untuk menghapus history penjualan
                    deletehistoryPenjualan(&jumlahPenjualan);
                } while (!kembaliKeMenuUtama()); // diulang jika ingin kembali ke menu utama
                break;
            case 5:
                do {
                    // Memanggil fungsi untuk menghapus buku
                    deleteBuku(daftarBuku, &jumlahBuku);
                } while (!kembaliKeMenuUtama());  // diulang jika ingin kembali ke menu utama
                break;
            case 6:
            // Keluar dari program
                printf("Berhasil keluar dari Program.\n"); 
                break;
            default:
            // Pilihan tidak valid
                printf("Pilihan tidak valid. Silakan pilih menu ulang !\n");
        }
    } while (pilihan != 6); // diulang jika pilihan tidak valid

    return 0;
}
