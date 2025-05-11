#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void viewMenu() {
    printf("\n======= Menu Toko Buku =======\n");
    printf("1. Input Buku\n");
    printf("2. View History\n");
    printf("3. View Buku\n");
    printf("4. Delete History\n");
    printf("5. Delete Buku\n");
    printf("6. Input Transaksi\n");
    printf("7. Exit\n");
    printf("Pilih menu (1-6): ");
}

int main() {

    int pilihan;
    do {
        viewMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                // TODO: Implement inputBuku function
                break;
            case 2:
                // TODO: Implement viewHistory function
                break;
            case 3:
                // TODO: Implement viewBuku function
                break;
            case 4:
                // TODO: Implement deleteHistory function
                break;
            case 5:
                // TODO: Implement deleteBuku function
                break;
            case 6:
                // TODO: Implement inputTransaksi function
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