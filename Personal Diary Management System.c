#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int password();
void addrecord();
void viewrecord();
void editrecord();
void editpassword();
void deleterecord();

struct record {
    char time[6];
    char name[30];
    char place[25];
    char duration[10];
    char note[500];
};

int main() {
    int ch;
    printf("\n\n\t***********************************\n");
    printf("\t*PASSWORD PROTECTED PERSONAL DIARY*\n");
    printf("\t***********************************");

    while (1) {
        printf("\n\n\t\tMAIN MENU:");
        printf("\n\n\tADD RECORD\t[1]");
        printf("\n\tVIEW RECORD\t[2]");
        printf("\n\tEDIT RECORD\t[3]");
        printf("\n\tDELETE RECORD\t[4]");
        printf("\n\tEDIT PASSWORD\t[5]");
        printf("\n\tEXIT\t\t[6]");
        printf("\n\n\tENTER YOUR CHOICE: ");
        if (scanf("%d", &ch) != 1) { fflush(stdin); continue; }

        switch (ch) {
            case 1: addrecord(); break;
            case 2: viewrecord(); break;
            case 3: editrecord(); break;
            case 4: deleterecord(); break;
            case 5: editpassword(); break;
            case 6:
                printf("\n\n\t\tTHANK YOU FOR USING THE SOFTWARE\n");
                getchar(); exit(0);
            default:
                printf("\nInvalid choice. Try again...");
                getchar(); break;
        }
        system("cls");
    }
    return 0;
}

void addrecord() {
    system("cls");
    FILE *fp;
    struct record e;
    char filename[15], another = 'Y', timebuf[10];
    int duplicate;

    printf("\n\n\t\t*** WELCOME TO ADD MENU ***\n\n");
    printf("\tENTER DATE OF YOUR RECORD [yyyy-mm-dd]: ");
    fflush(stdin); fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    fp = fopen(filename, "ab+");
    if (!fp) {
        perror("SYSTEM ERROR"); getchar(); return;
    }

    while (another == 'Y' || another == 'y') {
        duplicate = 0;
        printf("\n\tENTER TIME [hh:mm]: ");
        scanf("%5s", timebuf);
        rewind(fp);
        while (fread(&e, sizeof(e), 1, fp) == 1) {
            if (!strcmp(e.time, timebuf)) {
                printf("\n\tTHE RECORD ALREADY EXISTS.\n");
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            strcpy(e.time, timebuf);
            printf("\tENTER NAME: "); fflush(stdin); fgets(e.name, sizeof(e.name), stdin);
            e.name[strcspn(e.name, "\n")] = 0;
            printf("\tENTER PLACE: "); fgets(e.place, sizeof(e.place), stdin);
            e.place[strcspn(e.place, "\n")] = 0;
            printf("\tENTER DURATION: "); fgets(e.duration, sizeof(e.duration), stdin);
            e.duration[strcspn(e.duration, "\n")] = 0;
            printf("\tNOTE: "); fgets(e.note, sizeof(e.note), stdin);
            e.note[strcspn(e.note, "\n")] = 0;

            fwrite(&e, sizeof(e), 1, fp);
            printf("\nRecord added successfully!\n");
        }

        printf("\n\tADD ANOTHER RECORD? (Y/N): ");
        fflush(stdin); another = getchar(); // get next confirm
        getchar(); // consume newline
    }

    fclose(fp);
    printf("\n\tPress any key to exit...");
    getchar();
}

void viewrecord() {
    system("cls");
    if (password()) return;

    struct record customer;
    char filename[15], timebuf[6];
    int choice;
    FILE *fp;

    do {
        printf("\n\tENTER DATE TO VIEW [yyyy-mm-dd]: ");
        fflush(stdin); fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = 0;

        fp = fopen(filename, "rb");
        if (!fp) {
            printf("\nNo such record.\nPress any key...");
            getchar(); return;
        }

        system("cls");
        printf("\n\t1. Whole day  2. Specific time\n");
        printf("\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("\n-- Records for %s --\n", filename);
            while (fread(&customer, sizeof(customer), 1, fp))
                printf("\nTIME: %s\nWITH: %s\nAT: %s\nDURATION: %s\nNOTE: %s\n",
                       customer.time, customer.name,
                       customer.place, customer.duration, customer.note);
        } else if (choice == 2) {
            printf("\nENTER TIME [hh:mm]: ");
            fflush(stdin); fgets(timebuf, sizeof(timebuf), stdin);
            timebuf[strcspn(timebuf, "\n")] = 0;
            while (fread(&customer, sizeof(customer), 1, fp)) {
                if (!strcmp(customer.time, timebuf)) {
                    printf("\nTIME: %s\nWITH: %s\nAT: %s\nDURATION: %s\nNOTE: %s\n",
                           customer.time, customer.name,
                           customer.place, customer.duration, customer.note);
                    break;
                }
            }
        } else {
            printf("\nInvalid option.\n");
        }

        printf("\nCONTINUE VIEWING (Y/N)? ");
        fflush(stdin); choice = getchar();
    } while (choice == 'Y' || choice == 'y');

    fclose(fp);
}

void editrecord() {
    system("cls");
    if (password()) return;

    struct record customer;
    char filename[15], timebuf[6], choice;
    int option, found = 0, count = 0;
    FILE *fp;

    do {
        printf("\n\tENTER DATE TO EDIT [yyyy-mm-dd]: ");
        fflush(stdin); fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = 0;
        printf("\tENTER TIME [hh:mm]: ");
        fgets(timebuf, sizeof(timebuf), stdin);
        timebuf[strcspn(timebuf, "\n")] = 0;

        fp = fopen(filename, "rb+");
        if (!fp) {
            printf("\nNo such record.\nPress any key...");
            getchar(); return;
        }

        while (fread(&customer, sizeof(customer), 1, fp)) {
            if (!strcmp(customer.time, timebuf)) {
                found = 1;
                printf("\nCurrent Record:");
                printf("\nTIME: %s\nWITH: %s\nAT: %s\nDURATION: %s\nNOTE: %s\n",
                       customer.time, customer.name,
                       customer.place, customer.duration, customer.note);

                printf("\nWhat to edit?\n1=Time 2=Name 3=Place 4=Duration 5=Note 6=All 7=Cancel\n");
                scanf("%d", &option);
                fflush(stdin);

                if (option == 1) {
                    printf("NEW TIME [hh:mm]: ");
                    fgets(customer.time, sizeof(customer.time), stdin);
                } else if (option == 2) {
                    printf("NEW NAME: ");
                    fgets(customer.name, sizeof(customer.name), stdin);
                } else if (option == 3) {
                    printf("NEW PLACE: ");
                    fgets(customer.place, sizeof(customer.place), stdin);
                } else if (option == 4) {
                    printf("NEW DURATION: ");
                    fgets(customer.duration, sizeof(customer.duration), stdin);
                } else if (option == 5) {
                    printf("NEW NOTE: ");
                    fgets(customer.note, sizeof(customer.note), stdin);
                } else if (option == 6) {
                    printf("NEW TIME: "); fgets(customer.time, sizeof(customer.time), stdin);
                    printf("NEW NAME: "); fgets(customer.name, sizeof(customer.name), stdin);
                    printf("NEW PLACE: "); fgets(customer.place, sizeof(customer.place), stdin);
                    printf("NEW DURATION: "); fgets(customer.duration, sizeof(customer.duration), stdin);
                    printf("NEW NOTE: "); fgets(customer.note, sizeof(customer.note), stdin);
                } else {
                    fclose(fp); return;
                }

                fseek(fp, -sizeof(customer), SEEK_CUR);
                fwrite(&customer, sizeof(customer), 1, fp);
                fclose(fp);
                count++;
                printf("\nEDIT SUCCESSFUL!\nPress any key...");
                getchar();
                break;
            }
        }

        if (!found) {
            printf("\nRecord not found. Try again? (Y/N): ");
            fflush(stdin); choice = getchar();
        } else {
            break;
        }
    } while (choice == 'Y' || choice == 'y');

    printf("\nTotal edited: %d\nPress enter to exit...", count);
    getchar();
}

int password() {
    char pass[15] = {0}, check[15] = {0}, ch;
    FILE *fpp;
    int i, attempts;

    printf(":: ONLY THREE TRIALS ALLOWED ::\n");
    for (attempts = 0; attempts < 3; attempts++) {
        printf("ENTER PASSWORD: ");
        i = 0;
        pass[0] = getch();
        while (pass[i] != '\r') {
            if (pass[i] == '\b') {
                if (i > 0) { i--; printf("\b \b"); }
                pass[i] = getch();
            } else {
                printf("*");
                i++;
                pass[i] = getch();
            }
        }
        pass[i] = '\0';

        fpp = fopen("SE", "r");
        if (!fpp) {
            printf("\nPassword file missing!\n"); getchar(); return 1;
        }

        i = 0;
        while ((ch = fgetc(fpp)) != EOF) {
            check[i++] = ch - 5;
        }
        check[i] = '\0';
        fclose(fpp);

        if (!strcmp(pass, check)) {
            printf("\nACCESS GRANTED\n"); return 0;
        } else {
            printf("\nWRONG PASSWORD\n");
        }
    }

    printf("\nACCESS DENIED\n");
    getchar(); return 1;
}

void editpassword() {
    system("cls");
    if (password()) return;

    char pass[15] = {0}, confirm[15] = {0}, ch;
    int i, retry;
    FILE *fp;

    do {
        printf("ENTER NEW PASSWORD: ");
        i = 0; pass[0] = getch();
        while (pass[i] != '\r') {
            if (pass[i] == '\b' && i > 0) {
                i--; printf("\b \b"); pass[i] = getch();
            } else {
                printf("*");
                i++; pass[i] = getch();
            }
        }
        pass[i] = '\0';
        printf("\nCONFIRM PASSWORD: ");
        i = 0; confirm[0] = getch();
        while (confirm[i] != '\r') {
            if (confirm[i] == '\b' && i > 0) {
                i--; printf("\b \b"); confirm[i] = getch();
            } else {
                printf("*");
                i++; confirm[i] = getch();
            }
        }
        confirm[i] = '\0';

        if (strcmp(pass, confirm)) {
            printf("\nPasswords do not match. Retry? (Y/N): ");
            retry = getchar(); fflush(stdin);
        } else {
            fp = fopen("SE", "wb");
            if (!fp) { printf("\nError saving password\n"); return; }
            for (i = 0; pass[i]; i++)
                putc(pass[i] + 5, fp);
            fclose(fp);
            printf("\nPassword changed!\n");
            retry = 'N';
        }
    } while (retry == 'Y' || retry == 'y');

    printf("Press any key to return...");
    getchar();
}

void deleterecord() {
    system("cls");
    if (password()) return;

    char filename[15], timebuf[10], another = 'Y';
    int choice;
    FILE *fp, *fptr;
    struct record rec;

    while (another == 'Y') {
        printf("\nDelete whole file [1] or specific record by time [2]? ");
        scanf("%d", &choice);
        fflush(stdin);

        if (choice == 1) {
            printf("Enter date [yyyy-mm-dd]: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            remove(filename);
            printf("Deleted successfully!\n");
        } else if (choice == 2) {
            printf("Enter date [yyyy-mm-dd]: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            fp = fopen(filename, "rb");
            if (!fp) { printf("File not found!\n"); return; }
            fptr = fopen("temp", "wb");
            printf("Enter time [hh:mm] to delete: ");
            fgets(timebuf, sizeof(timebuf), stdin);
            timebuf[strcspn(timebuf, "\n")] = 0;
            while (fread(&rec, sizeof(rec), 1, fp))
                if (strcmp(rec.time, timebuf))
                    fwrite(&rec, sizeof(rec), 1, fptr);
            fclose(fp); fclose(fptr);
            remove(filename); rename("temp", filename);
            printf("Record deleted!\n");
        } else {
            printf("Invalid option!\n");
        }

        printf("Delete another? (Y/N): ");
        another = getchar(); fflush(stdin);
    }

    printf("Press any key to return...");
    getchar();
}

// Note: getch() must be provided or replaced with getchar/no-echo equivalent on your platform.
