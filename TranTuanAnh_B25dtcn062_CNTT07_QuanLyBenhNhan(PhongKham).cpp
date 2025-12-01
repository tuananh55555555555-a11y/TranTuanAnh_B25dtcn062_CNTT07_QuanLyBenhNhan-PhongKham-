#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#define MAX_PATIENTS 100
#define MAX_RECORDS 500
#define MAX_RECORDS_PER_PATIENT 20
#define PAGE_SIZE 5

long long globalRecordId = 1000;

typedef struct {
    char recId[20]; 
    char cardId[10]; 
    char date[20];   
    char status[20]; 
} Record;

typedef struct {
    char cardId[10];
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
    Record records[MAX_RECORDS_PER_PATIENT];
    int recordCount;
} Patient;

Patient patientList[MAX_PATIENTS];
int patientCount = 0; 

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void clearScreen() {
    system(CLEAR_SCREEN);
}

void waitForEnter() {
    printf("\nNhan ENTER de tiep tuc...");
    while (getchar() != '\n');
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void removeNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int isValidPhone(const char *phone) {
    if (strlen(phone) != 9) return 0; 
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit((unsigned char)phone[i])) return 0;
    }
    return 1;
}

int isValidName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha((unsigned char)name[i]) && !isspace((unsigned char)name[i])) {
            return 0; 
        }
    }
    return 1; 
}

int isAllDigits(const char *str) {
    if (strlen(str) == 0) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0; 
        }
    }
    return 1;
}

int isValidDate(const char *date) {
    int day, month, year;

    if (strlen(date) != 10) return 0;

    if (date[2] != '/' || date[5] != '/') return 0;

    if (!isdigit(date[0]) || !isdigit(date[1]) ||
        !isdigit(date[3]) || !isdigit(date[4]) ||
        !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9])) 
        return 0;

    day = (date[0] - '0') * 10 + (date[1] - '0');
    month = (date[3] - '0') * 10 + (date[4] - '0');
    year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    if (month == 2) {
        int isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !isLeap)) return 0;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;

    return 1;
}

int findPatientIndex(const char *cardId) {
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(patientList[i].cardId, cardId) == 0) {
            return i;
        }
    }
    return -1;
}

void ThemBenhNhanMoi() {
    clearScreen();
    char cardId[10], name[50], phone[15], debtStr[20];
    double debt;
    int is_valid = 0;

    if (patientCount >= MAX_PATIENTS) {
        printf("\n[LOI] Danh sach da day. Khong the them benh nhan moi.\n");
        waitForEnter();
        return;
    }

    printf("\n--- TIEP NHAN BENH NHAN MOI ---\n");

    do {
        is_valid = 1;
        printf("1. Nhap CCCD (cardId - chi nhap so): ");
        if (fgets(cardId, sizeof(cardId), stdin) == NULL) return;
        removeNewline(cardId);
        
        if (strlen(cardId) == 0) {
            printf("[LOI] CCCD khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isAllDigits(cardId)) {
            printf("[LOI] CCCD chi duoc chua chu so. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (findPatientIndex(cardId) != -1) {
            printf("[LOI] cardId bi trung. Vui long nhap ma khac.\n");
            is_valid = 0;
        }
    } while (!is_valid);

    do {
        is_valid = 1;
        printf("2. Nhap Ten benh nhan (name): ");
        if (fgets(name, sizeof(name), stdin) == NULL) return;
        removeNewline(name);
        
        if (!isValidName(name)) { 
            printf("[LOI] Ten benh nhan khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } 
    } while (!is_valid);

    do {
        is_valid = 1;
        printf("3. Nhap So dien thoai (phone - 9 chu so): ");
        if (fgets(phone, sizeof(phone), stdin) == NULL) return;
        removeNewline(phone);
        
        if (strlen(phone) == 0) {
            printf("[LOI] So dien thoai khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isValidPhone(phone)) {
            printf("[LOI] So dien thoai khong hop le (Phai la 9 chu so va khong chua chu cai/dau).\n");
            is_valid = 0;
        }
    } while (!is_valid);

    do {
        is_valid = 1;
        printf("4. Nhap Cong no ban dau (debt): ");
        if (fgets(debtStr, sizeof(debtStr), stdin) == NULL) return;
        removeNewline(debtStr);
        debt = atof(debtStr);
        
        if (strlen(debtStr) == 0) {
            printf("[LOI] Cong no khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (debt < 0) {
            printf("[LOI] Cong no phai la so duong hoac bang 0 (debt >= 0).\n");
            is_valid = 0;
        }
    } while (!is_valid);

    Patient newPatient;
    strcpy(newPatient.cardId, cardId);
    strcpy(newPatient.name, name);
    strcpy(newPatient.phone, phone);
    newPatient.debt = debt;
    newPatient.visitDays = 0;
    newPatient.recordCount = 0;

    printf("\n[Xac nhan] Hoan tat nhap lieu.\n");
    
    patientList[patientCount++] = newPatient;
    printf("[THANH CONG] Tiep nhan benh nhan %s (Ma: %s) thanh cong.\n", name, cardId);
    
}

void CapNhatThongTinBenhNhan() {
    clearScreen();
    char cardId[10], newName[50], newPhone[15], newDebtStr[20];
    double newDebt;
    int index, is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the cap nhat.\n");
        waitForEnter();
        return;
    }

    printf("\n--- CAP NHAT THONG TIN BENH NHAN ---\n");
    
    do {
        is_valid = 1;
        printf("Nhap CCCD (cardId - chi nhap so): ");
        if (fgets(cardId, sizeof(cardId), stdin) == NULL) return;
        removeNewline(cardId);
        
        if (strlen(cardId) == 0) {
            printf("[LOI] CCCD khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isAllDigits(cardId)) {
            printf("[LOI] CCCD chi duoc chua chu so. Vui long nhap lai.\n");
            is_valid = 0;
        } else {
            index = findPatientIndex(cardId);
            if (index == -1) {
                printf("[LOI] Khong tim thay benh nhan co Ma ho so %s.\n", cardId);
                is_valid = 0;
            }
        }
    } while (!is_valid);

    index = findPatientIndex(cardId);

    printf("\n--- CAP NHAT THONG TIN CHO BENH NHAN %s ---\n", patientList[index].name);

    do {
        is_valid = 1;
        printf("1. Nhap Ten moi (Hien tai: %s): ", patientList[index].name);
        if (fgets(newName, sizeof(newName), stdin) == NULL) return;
        removeNewline(newName);
        
        if (strlen(newName) == 0) {
            printf("[LOI] Ten benh nhan khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isValidName(newName)) {
            printf("[LOI] Ten chi duoc chua ky tu chu cai va khoang trang (khong dau, khong so).\n");
            is_valid = 0;
        }
    } while (!is_valid);

    do {
        is_valid = 1;
        printf("2. Nhap So dien thoai moi (Hien tai: %s | 9 chu so): ", patientList[index].phone);
        if (fgets(newPhone, sizeof(newPhone), stdin) == NULL) return;
        removeNewline(newPhone);
        
        if (strlen(newPhone) == 0) {
            printf("[LOI] So dien thoai khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isValidPhone(newPhone)) {
            printf("[LOI] So dien thoai khong hop le (Phai la 9 chu so va khong chua chu cai/dau).\n");
            is_valid = 0;
        }
    } while (!is_valid);
    
    do {
        is_valid = 1;
        printf("3. Nhap Cong no moi (Hien tai: %.0f): ", patientList[index].debt);
        if (fgets(newDebtStr, sizeof(newDebtStr), stdin) == NULL) return;
        removeNewline(newDebtStr);
        newDebt = atof(newDebtStr);
        
        if (strlen(newDebtStr) == 0) {
            printf("[LOI] Cong no khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (newDebt < 0) {
            printf("[LOI] Cong no khong duoc la so am.\n");
            is_valid = 0;
        }
    } while (!is_valid);

    strcpy(patientList[index].name, newName);
    strcpy(patientList[index].phone, newPhone);
    patientList[index].debt = newDebt;

    printf("\n[THANH CONG] Cap nhat thong tin cho benh nhan %s (Ma: %s) thanh cong.\n", newName, cardId);
    
}

void XuatVien() {
    clearScreen();
    char cardId[10], confirm[5];
    int is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the xuat vien.\n");
        waitForEnter();
        return;
    }

    printf("\n--- XUAT VIEN ---\n");
    
    do {
        is_valid = 1;
        printf("Nhap CCCD (cardId - chi nhap so): ");
        if (fgets(cardId, sizeof(cardId), stdin) == NULL) return;
        removeNewline(cardId);

        if (strlen(cardId) == 0) {
            printf("[LOI] CCCD khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isAllDigits(cardId)) {
            printf("[LOI] CCCD chi duoc chua chu so. Vui long nhap lai.\n");
            is_valid = 0;
        } else {
            int index_check = findPatientIndex(cardId);
            if (index_check == -1) {
                printf("[LOI] Khong tim thay benh nhan co Ma ho so %s.\n", cardId);
                is_valid = 0;
            }
        }
    } while (!is_valid);

    int index = findPatientIndex(cardId);

    if (patientList[index].debt > 0) {
        char paymentStr[20];
        double paidAmount = 0.0;
        double currentDebt = patientList[index].debt;

        printf("\n[CONG NO] Benh nhan %s (Ma: %s) con cong no: %.0f VND.\n", 
               patientList[index].name, cardId, currentDebt);
        
        do {
            is_valid = 1;
            printf("Nhap so tien thanh toan (0 - %.0f): ", currentDebt);
            if (fgets(paymentStr, sizeof(paymentStr), stdin) == NULL) return;
            removeNewline(paymentStr);
            
            if (strlen(paymentStr) == 0) {
                printf("[LOI] So tien thanh toan khong duoc de trong. Vui long nhap lai.\n");
                is_valid = 0;
            } else {
                paidAmount = atof(paymentStr);
                if (paidAmount < 0) {
                    printf("[LOI] So tien thanh toan khong duoc la so am. Vui long nhap lai.\n");
                    is_valid = 0;
                }
            }
        } while (!is_valid);
        
        patientList[index].debt -= paidAmount;
        if (patientList[index].debt < 0) {
            patientList[index].debt = 0; 
        }

        printf("[CAP NHAT] Cong no sau thanh toan: %.0f VND.\n", patientList[index].debt);
    }
    
    if (patientList[index].debt > 0) {
        printf("\n[CANH BAO] Benh nhan van con cong no: %.0f VND.\n", patientList[index].debt);
        do {
            is_valid = 1;
            printf("Ban co chac chan muon xuat vien du con no? (Y/N): ");
            if (fgets(confirm, sizeof(confirm), stdin) == NULL) return;
            removeNewline(confirm);
            toLowerCase(confirm);
            if (strcmp(confirm, "y") != 0 && strcmp(confirm, "n") != 0) {
                printf("[LOI] Lua chon khong hop le. Vui long nhap 'Y' hoac 'N'.\n");
                is_valid = 0;
            } else if (strcmp(confirm, "n") == 0) {
                printf("[HUY BO] Da huy yeu cau xuat vien.\n");
                waitForEnter();
                return;
            }
        } while (!is_valid);
    } else {
        printf("\n[XUAT VIEN] Benh nhan da hoan thanh cong no.\n");
    }

    for (int i = index; i < patientCount - 1; i++) {
        patientList[i] = patientList[i + 1];
    }
    patientCount--;

    printf("[THANH CONG] Benh nhan %s (Ma: %s) da xuat vien thanh cong.\n", cardId, cardId);
    waitForEnter();
    
}

void HienThiDanhSachBenhNhan() {
    if (patientCount == 0) {
        clearScreen();
        printf("\n[THONG BAO] Danh sach benh nhan hien dang trong.\n");
        waitForEnter();
        return;
    }

    int totalPages = (patientCount + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentPage = 1;
    char input[10];

    do {
        clearScreen();
        
        if (currentPage < 1) currentPage = 1;
        if (currentPage > totalPages) currentPage = totalPages;

        int startIndex = (currentPage - 1) * PAGE_SIZE;
        int endIndex = startIndex + PAGE_SIZE;
        if (endIndex > patientCount) {
            endIndex = patientCount;
        }

        printf("\n+=======================================================================+\n");
        printf("|                   DANH SACH BENH NHAN (Trang %d/%d)                     |\n", currentPage, totalPages);
        printf("+----------+------------------------------+------------+----------+-----+\n");
        printf("| %-8s | %-28s | %-10s | %-8s | %-3s |\n",
               "Ma HS", "Ten Benh Nhan", "SDT", "Cong No", "KB");
        printf("+----------+------------------------------+------------+----------+-----+\n");

        for (int i = startIndex; i < endIndex; i++) {
            printf("| %-8s | %-28s | %-10s | %-8.0f | %-3d |\n",
                   patientList[i].cardId, patientList[i].name, patientList[i].phone, 
                   patientList[i].debt, patientList[i].visitDays);
        }
        printf("+----------+------------------------------+------------+----------+-----+\n");

        printf("Chon thao tac:\n");
        printf("  [T] Truoc - Nhap so trang (1-%d)\n", totalPages);
        printf("  [S] Sau\n");
        printf("  [X] Thoat\n");
        printf("Nhap lua chon: ");


        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        removeNewline(input);

        if (strcasecmp(input, "X") == 0) {
            break;
        } else if (strcasecmp(input, "S") == 0) {
            if (currentPage < totalPages) {
                currentPage++;
            } else {
                printf("[THONG BAO] Ban dang o trang cuoi.\n");
                waitForEnter();
            }
        } else if (strcasecmp(input, "T") == 0) {
            if (currentPage > 1) {
                currentPage--;
            } else {
                printf("[THONG BAO] Ban dang o trang dau.\n");
                waitForEnter();
            }
        } else {
            int newPage = atoi(input);
            if (newPage > 0 && newPage <= totalPages) {
                
                if (newPage == currentPage && newPage == 1) {
                    printf("[THONG BAO] Ban da o trang dau tien (trang 1).\n");
                    waitForEnter();
                } else if (newPage == currentPage && newPage == totalPages) {
                    printf("[THONG BAO] Ban da o trang cuoi cung (trang %d).\n", totalPages);
                    waitForEnter();
                } else {
                    currentPage = newPage;
                }
            } else {
                printf("[LOI] So trang khong hop le. Nhan ENTER de tiep tuc.");
                clearInputBuffer();
            }
        }
        
        if (strcasecmp(input, "S") != 0 && strcasecmp(input, "T") != 0 && atoi(input) == 0) {
            clearInputBuffer();
        }

    } while (1);
    clearScreen();
}

void TimKiemBenhNhan() {
    clearScreen();
    char searchStr[50], lowerSearchStr[50];
    int foundCount = 0;
    int is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the tim kiem.\n");
        waitForEnter();
        return;
    }

    printf("\n--- TIM KIEM BENH NHAN ---\n");
    
    do {
        is_valid = 1;
        printf("Nhap chuoi tim kiem (mot phan ten benh nhan): ");
        if (fgets(searchStr, sizeof(searchStr), stdin) == NULL) return;
        removeNewline(searchStr);

        if (strlen(searchStr) == 0) {
            printf("[LOI] Chuoi tim kiem khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        }
    } while (!is_valid);


    strcpy(lowerSearchStr, searchStr);
    toLowerCase(lowerSearchStr);

    clearScreen();
    printf("\n+======================================================================+\n");
    printf("|                         KET QUA TIM KIEM                             |\n");
    printf("+----------+------------------------------+------------+----------+-----+\n");
    printf("| %-8s | %-28s | %-10s | %-8s | %-3s |\n",
           "Ma HS", "Ten Benh Nhan", "SDT", "Cong No", "KB");
    printf("+----------+------------------------------+------------+----------+-----+\n");

    for (int i = 0; i < patientCount; i++) {
        char lowerName[50];
        strcpy(lowerName, patientList[i].name);
        toLowerCase(lowerName);

        if (strstr(lowerName, lowerSearchStr) != NULL) {
            printf("| %-8s | %-28s | %-10s | %-8.0f | %-3d |\n",
                   patientList[i].cardId, patientList[i].name, patientList[i].phone, 
                   patientList[i].debt, patientList[i].visitDays);
            foundCount++;
        }
    }
    printf("+----------+------------------------------+------------+----------+-----+\n");

    if (foundCount == 0) {
        printf("Khong tim thay benh nhan nao chua chuoi \"%s\".\n", searchStr);
    } else {
        printf("Tim thay %d benh nhan.\n", foundCount);
    }
    waitForEnter();
}

void SapXepTheoCongNo() {
    clearScreen();
    char choiceStr[5];
    int choice;
    int is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the sap xep.\n");
        waitForEnter();
        return;
    }

    printf("\n--- SAP XEP DANH SACH BENH NHAN THEO CONG NO ---\n");
    
    do {
        is_valid = 1;
        printf("Lua chon kieu sap xep:\n");
        printf("1. Sap tang dan (Cong no thap nhat len truoc)\n");
        printf("2. Sap giam dan (Cong no cao nhat len truoc)\n");
        printf("Nhap lua chon (1 hoac 2): ");

        if (fgets(choiceStr, sizeof(choiceStr), stdin) == NULL) return;
        removeNewline(choiceStr);
        choice = atoi(choiceStr);

        if (strlen(choiceStr) == 0 || (choice != 1 && choice != 2)) {
            printf("[LOI] Lua chon khong hop le. Vui long nhap 1 hoac 2.\n");
            is_valid = 0;
        }
    } while (!is_valid);


    for (int i = 0; i < patientCount - 1; i++) {
        for (int j = 0; j < patientCount - 1 - i; j++) {
            int swap = 0;
            if (choice == 1) {
                if (patientList[j].debt > patientList[j + 1].debt) {
                    swap = 1;
                }
            } else {
                if (patientList[j].debt < patientList[j + 1].debt) {
                    swap = 1;
                }
            }

            if (swap) {
                Patient temp = patientList[j];
                patientList[j] = patientList[j + 1];
                patientList[j + 1] = temp;
            }
        }
    }

    printf("\n[THANH CONG] Danh sach da duoc sap xep %s theo Cong no.\n", 
           (choice == 1) ? "TANG DAN" : "GIAM DAN");
    printf("Vui long chon Hien thi danh sach de xem ket qua.\n");
    waitForEnter();
    
}

void GhiNhanKhamBenh() {
    clearScreen();
    char cardId[10], date[20]; 
    int is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the ghi nhan kham benh.\n");
        waitForEnter();
        return;
    }

    printf("\n--- GHI NHAN KHAM BENH ---\n");
    
    do {
        is_valid = 1;
        printf("Nhap Ma benh nhan can ghi nhan kham benh: ");
        if (fgets(cardId, sizeof(cardId), stdin) == NULL) return;
        removeNewline(cardId);
        
        if (strlen(cardId) == 0) {
            printf("[LOI] Ma benh nhan khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (findPatientIndex(cardId) == -1) {
            printf("[LOI] Khong tim thay benh nhan co ma %s.\n", cardId);
            is_valid = 0;
        }
    } while (!is_valid);

    int index = findPatientIndex(cardId);
    
    do {
        is_valid = 1;
        printf("Nhap Ngay kham benh (dd/mm/YYYY, VD: 01/12/2025): ");
        if (fgets(date, sizeof(date), stdin) == NULL) return;
        removeNewline(date);
        
        if (strlen(date) == 0) {
            printf("[LOI] Ngay kham benh khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (!isValidDate(date)) { 
            printf("[LOI] Ngay kham benh khong dung dinh dang hoac khong hop le (dd/mm/YYYY). Vui long nhap lai.\n");
            is_valid = 0;
        } else {
            for (int i = 0; i < patientList[index].recordCount; i++) {
                if (strcmp(patientList[index].records[i].date, date) == 0) {
                    printf("[THONG BAO] Benh nhan %s da duoc ghi nhan kham vao ngay %s roi.\n", cardId, date);
                    waitForEnter();
                    return;
                }
            }
        }
    } while (!is_valid);

    if (patientList[index].recordCount >= MAX_RECORDS_PER_PATIENT) {
        printf("[LOI] Ho so kham benh cua benh nhan nay da day (%d/%d).\n", 
               MAX_RECORDS_PER_PATIENT, MAX_RECORDS_PER_PATIENT);
        waitForEnter();
        return;
    }

    patientList[index].visitDays++;

    Record newRec;
    
    sprintf(newRec.recId, "REC%lld", globalRecordId++);
    strcpy(newRec.cardId, cardId); 
    strcpy(newRec.date, date);
    strcpy(newRec.status, "Theo doi");

    patientList[index].records[patientList[index].recordCount++] = newRec;

    printf("\n[THANH CONG] Ghi nhan lich kham ngay %s (Ma giao dich: %s) cho benh nhan %s thanh cong.\n", date, newRec.recId, cardId);
    printf("So ngay kham benh hien tai: %d\n", patientList[index].visitDays);
    waitForEnter();
    
}

void XemLichSuKhamBenh() {
    clearScreen();
    char cardId[10];
    int is_valid = 0;

    if (patientCount == 0) {
        printf("\n[LOI] Danh sach benh nhan hien dang trong. Khong the xem lich su.\n");
        waitForEnter();
        return;
    }

    printf("\n--- XEM LICH SU KHAM BENH ---\n");
    
    do {
        is_valid = 1;
        printf("Nhap Ma benh nhan can xem lich kham: ");
        if (fgets(cardId, sizeof(cardId), stdin) == NULL) return;
        removeNewline(cardId);
        
        if (strlen(cardId) == 0) {
            printf("[LOI] Ma benh nhan khong duoc de trong. Vui long nhap lai.\n");
            is_valid = 0;
        } else if (findPatientIndex(cardId) == -1) {
            printf("[LOI] Khong tim thay benh nhan co ma %s.\n", cardId);
            is_valid = 0;
        }
    } while (!is_valid);

    int index = findPatientIndex(cardId);

    clearScreen();
    printf("\n+==================================================================+\n");
    printf("| LICH SU KHAM BENH CUA BENH NHAN: %s (Ma: %s)         |\n", patientList[index].name, cardId);
    printf("+============+---------------+--------------------+----------------+\n");
    
    if (patientList[index].recordCount == 0) {
        printf("[THONG BAO] Benh nhan nay chua co lich su kham benh nao duoc ghi nhan.\n");
    } else {
        printf("| %-10s | %-13s | %-18s | %-14s |\n", "Ma GD", "Ngay kham", "Trang thai", "Ghi chu");
        printf("+============+---------------+--------------------+----------------+\n");
        for (int i = 0; i < patientList[index].recordCount; i++) {
            printf("| %-10s | %-13s | %-18s | %-14s |\n", 
                   patientList[index].records[i].recId,
                   patientList[index].records[i].date, 
                   patientList[index].records[i].status,
                   ""); 
        }
        printf("+============+---------------+--------------------+----------------+\n");
        printf("Tong so lan kham benh: %d\n", patientList[index].recordCount);
    }
    waitForEnter();
    
}

void initializePatients() {
    patientCount = 7;
    
    strcpy(patientList[0].cardId, "001");
    strcpy(patientList[0].name, "Nguyen Van An"); 
    strcpy(patientList[0].phone, "123456789"); 
    patientList[0].debt = 8500000;
    patientList[0].visitDays = 8;
    patientList[0].recordCount = 2;
    
    sprintf(patientList[0].records[0].recId, "REC%lld", globalRecordId++);
    strcpy(patientList[0].records[0].cardId, "001");
    strcpy(patientList[0].records[0].date, "01/11/2025");
    strcpy(patientList[0].records[0].status, "Tai kham");
    
    sprintf(patientList[0].records[1].recId, "REC%lld", globalRecordId++);
    strcpy(patientList[0].records[1].cardId, "001");
    strcpy(patientList[0].records[1].date, "05/11/2025");
    strcpy(patientList[0].records[1].status, "Theo doi");

    strcpy(patientList[1].cardId, "002");
    strcpy(patientList[1].name, "Tran Thi Binh"); 
    strcpy(patientList[1].phone, "987654321");
    patientList[1].debt = 300000;
    patientList[1].visitDays = 2;
    patientList[1].recordCount = 1;
    
    sprintf(patientList[1].records[0].recId, "REC%lld", globalRecordId++);
    strcpy(patientList[1].records[0].cardId, "002");
    strcpy(patientList[1].records[0].date, "10/11/2025");
    strcpy(patientList[1].records[0].status, "Tai kham"); 

    strcpy(patientList[2].cardId, "003");
    strcpy(patientList[2].name, "Le Van Cuong"); 
    strcpy(patientList[2].phone, "147852369");
    patientList[2].debt = 2500000;
    patientList[2].visitDays = 12;
    patientList[2].recordCount = 0;

    strcpy(patientList[3].cardId, "004");
    strcpy(patientList[3].name, "Nguyen Thi Dung"); 
    strcpy(patientList[3].phone, "333444555");
    patientList[3].debt = 100000;
    patientList[3].visitDays = 1;
    patientList[3].recordCount = 0;

    strcpy(patientList[4].cardId, "005");
    strcpy(patientList[4].name, "Pham Van Hung"); 
    strcpy(patientList[4].phone, "912345678");
    patientList[4].debt = 1200000;
    patientList[4].visitDays = 5;
    patientList[4].recordCount = 0;
    
    strcpy(patientList[5].cardId, "006");
    strcpy(patientList[5].name, "Hoang Van Khoa"); 
    strcpy(patientList[5].phone, "888777666");
    patientList[5].debt = 750000;
    patientList[5].visitDays = 3;
    patientList[5].recordCount = 0;

    strcpy(patientList[6].cardId, "007");
    strcpy(patientList[6].name, "Vuong Thi Mai"); 
    strcpy(patientList[6].phone, "999888777");
    patientList[6].debt = 4000000;
    patientList[6].visitDays = 10;
    patientList[6].recordCount = 0;
}

void showMenu() {
    clearScreen();
    
    printf("\n\n+====================================================+\n");
    printf("|          HE THONG QUAN LY BENH NHAN                |\n");
    printf("+====================================================+\n");
    
    printf("| So luong benh nhan hien tai: %3d/%3d               |\n", patientCount, MAX_PATIENTS);
    
    printf("+-------+--------------------------------------------+\n");
    
    printf("| %-5s | %-42s |\n", "STT", "CHUC NANG");
    printf("+-------+--------------------------------------------+\n");
    
    printf("| %-5d | %-42s |\n", 1, "Tiep nhan benh nhan moi");
    printf("| %-5d | %-42s |\n", 2, "Cap nhat thong tin benh nhan");
    printf("| %-5d | %-42s |\n", 3, "Xuat vien ");
    printf("| %-5d | %-42s |\n", 4, "Hien thi danh sach");
    printf("| %-5d | %-42s |\n", 5, "Tim kiem benh nhan theo Ten");
    printf("| %-5d | %-42s |\n", 6, "Sap xep danh sach benh nhan theo Cong no");
    printf("| %-5d | %-42s |\n", 7, "Ghi nhan kham benh");
    printf("| %-5d | %-42s |\n", 8, "Xem lich su kham benh");
    
    printf("+-------+--------------------------------------------+\n");
    
    printf("| %-5d | %-42s |\n", 0, "Thoat chuong trinh");
    printf("+====================================================+\n");
    printf("Vui long nhap lua chon cua ban: ");
}

int main() {
    initializePatients(); 
    
    char choiceStr[5];
    int choice;
    int input_success;

    do {
        showMenu(); 
        
        if (fgets(choiceStr, sizeof(choiceStr), stdin) == NULL) {
            choice = -1; 
        } else {
            removeNewline(choiceStr);
            if (sscanf(choiceStr, "%d", &choice) != 1) {
                choice = -1; 
            }
        }

        switch (choice) {
            case 1:
                ThemBenhNhanMoi();
                break;
            case 2:
                CapNhatThongTinBenhNhan();
                break;
            case 3:
                XuatVien();
                break;
            case 4:
                HienThiDanhSachBenhNhan();
                break;
            case 5:
                TimKiemBenhNhan();
                break;
            case 6:
                SapXepTheoCongNo();
                break;
            case 7:
                GhiNhanKhamBenh();
                break;
            case 8:
                XemLichSuKhamBenh();
                break;
            case 0:
                clearScreen();
                printf("\nDa thoat chuong trinh. Tam biet!\n");
                break;
            default:
                if (choice != 0) { 
                    printf("\n[LOI] Lua chon khong hop le. Vui long thu lai.\n");
                    waitForEnter();
                }
                break;
        }

    } while (choice != 0);

    return 0;
}
