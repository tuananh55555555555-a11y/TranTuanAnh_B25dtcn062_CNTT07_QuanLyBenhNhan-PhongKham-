#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#define MAX_BENHNHAN 100
#define MAX_KHAM_MOT_BENHNHAN 20

long long idLichSuToanCuc = 1000;

typedef struct {
    char maLichSu[20];
    char CCCD[15];
    char ngayKham[20];
    char trangThai[20];
} LichSu;

typedef struct {
    char CCCD[15];
    char ten[50];
    char sdt[15];
    double no;
    int soNgayKham;
    LichSu cacLichSu[MAX_KHAM_MOT_BENHNHAN];
    int soLichSu;
} BenhNhan;

BenhNhan dsBenhNhan[MAX_BENHNHAN];
int soBenhNhan = 0;

void xoaBoDemNhap() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void xoaManHinh() {
    system(CLEAR_SCREEN);
}

void choNhanEnter() {
    printf("\nNhan ENTER de tiep tuc...");
    while (getchar() != '\n');
}

void vietThuong(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void xoaXuongDong(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int soDienThoaiHopLe(const char *sdt) {
    if (strlen(sdt) != 9) return 0;
    for (int i = 0; sdt[i] != '\0'; i++) {
        if (!isdigit((unsigned char)sdt[i])) return 0;
    }
    return 1;
}

int tenHopLe(const char *ten) {
    for (int i = 0; ten[i] != '\0'; i++) {
        if (!isalpha((unsigned char)ten[i]) && !isspace((unsigned char)ten[i])) {
            return 0;
        }
    }
    return 1;
}

int tatCaLaSo(const char *str) {
    if (strlen(str) == 0) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int ngayHopLe(const char *ngay) {
    int d, m, y;
    if (strlen(ngay) != 10) return 0;
    if (ngay[2] != '/' || ngay[5] != '/') return 0;
    if (!isdigit(ngay[0]) || !isdigit(ngay[1]) ||
        !isdigit(ngay[3]) || !isdigit(ngay[4]) ||
        !isdigit(ngay[6]) || !isdigit(ngay[7]) || !isdigit(ngay[8]) || !isdigit(ngay[9]))
        return 0;
    d = (ngay[0] - '0') * 10 + (ngay[1] - '0');
    m = (ngay[3] - '0') * 10 + (ngay[4] - '0');
    y = (ngay[6] - '0') * 1000 + (ngay[7] - '0') * 100 + (ngay[8] - '0') * 10 + (ngay[9] - '0');
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    if (m == 2) {
        int namNhuan = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (d > 29 || (d == 29 && !namNhuan)) return 0;
    }
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return 0;
    return 1;
}

int timBenhNhanBangCCCD(const char *CCCD) {
    for (int i = 0; i < soBenhNhan; i++) {
        if (strcmp(dsBenhNhan[i].CCCD, CCCD) == 0) {
            return i;
        }
    }
    return -1;
}

void hienThiThongTinBenhNhan(BenhNhan *bn) {
    printf("\n+-------------+----------------------+----------+--------+-----+\n");
    printf("| %-11s | %-20s | %-8s | %-6s | %-3s |\n",
        "CCCD", "Ten", "SDT", "No", "KB");
    printf("+-------------+----------------------+----------+--------+-----+\n");
    printf("| %-11s | %-20s | %-8s | %-6.0f | %-3d |\n",
        bn->CCCD, bn->ten, bn->sdt, bn->no, bn->soNgayKham);
    printf("+-------------+----------------------+----------+--------+-----+\n");
}

void hienThiDSBenhNhan() {
    xoaManHinh();
    if (soBenhNhan == 0) {
        printf("Danh sach rong.\n");
        choNhanEnter();
        return;
    }

    printf("\nDANH SACH CAC BENH NHAN:\n");
    for (int i = 0; i < soBenhNhan; i++) {
        printf("- So %d\n", i+1);
        printf("  CCCD: %s\n", dsBenhNhan[i].CCCD);
        printf("  Ten: %s\n", dsBenhNhan[i].ten);
        printf("  SDT: %s\n", dsBenhNhan[i].sdt);
        printf("  No: %.0f VND\n", dsBenhNhan[i].no);
        printf("  So lan kham: %d\n", dsBenhNhan[i].soNgayKham);
        printf("------------------------------\n");
    }
    choNhanEnter();
}

void themBenhNhanMoi() {
    xoaManHinh();
    char CCCD[15], ten[50], sdt[15], noStr[20];
    double no;
    int hopLe = 0;

    if (soBenhNhan >= MAX_BENHNHAN) {
        printf("\nDanh sach day roi. Khong them duoc nua.\n");
        choNhanEnter();
        return;
    }

    printf("--- THEM BENH NHAN MOI ---\n");

    do {
        hopLe = 1;
        printf("Nhap CCCD benh nhan (12 so): ");
        if (fgets(CCCD, sizeof(CCCD), stdin) == NULL) return;
        xoaXuongDong(CCCD);

        if (strlen(CCCD) == 0) {
            printf("Khong duoc de trong. Nhap lai.\n");
            hopLe = 0;
        } else if (!tatCaLaSo(CCCD) || strlen(CCCD) != 12) {
            printf("CCCD phai la 12 so. Nhap lai.\n");
            hopLe = 0;
        } else if (timBenhNhanBangCCCD(CCCD) != -1) {
            printf("CCCD bi trung. Nhap CCCD khac.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    do {
        hopLe = 1;
        printf("Nhap ten benh nhan: ");
        if (fgets(ten, sizeof(ten), stdin) == NULL) return;
        xoaXuongDong(ten);
        if (!tenHopLe(ten)) {
            printf("Ten khong hop le hoac de trong. Nhap lai.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    do {
        hopLe = 1;
        printf("Nhap SDT (9 so): ");
        if (fgets(sdt, sizeof(sdt), stdin) == NULL) return;
        xoaXuongDong(sdt);
        if (strlen(sdt) == 0) {
            printf("SDT khong duoc de trong. Nhap lai.\n");
            hopLe = 0;
        } else if (!soDienThoaiHopLe(sdt)) {
            printf("SDT phai la 9 so. Nhap lai.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    do {
        hopLe = 1;
        printf("Nhap cong no ban dau: ");
        if (fgets(noStr, sizeof(noStr), stdin) == NULL) return;
        xoaXuongDong(noStr);
        no = atof(noStr);

        if (strlen(noStr) == 0) {
            printf("Cong no khong duoc de trong. Nhap lai.\n");
            hopLe = 0;
        } else if (no < 0) {
            printf("Nhap so duong hoac 0 nha.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    BenhNhan moi;
    strcpy(moi.CCCD, CCCD);
    strcpy(moi.ten, ten);
    strcpy(moi.sdt, sdt);
    moi.no = no;
    moi.soNgayKham = 0;
    moi.soLichSu = 0;

    dsBenhNhan[soBenhNhan++] = moi;

    printf("Them thanh cong ban %s (CCCD: %s) roi nhe!\n", ten, CCCD);
    hienThiThongTinBenhNhan(&dsBenhNhan[soBenhNhan-1]);
    choNhanEnter();
}

void capNhatBenhNhan() {
    xoaManHinh();
    char CCCD[15], tenMoi[50], sdtMoi[15], noMoiStr[20];
    double noMoi;
    int index, hopLe = 0;

    if (soBenhNhan == 0) {
        printf("Chua co benh nhan nao. Khong cap nhat duoc.\n");
        choNhanEnter();
        return;
    }

    printf("--- CAP NHAT BENH NHAN ---\n");
    do {
        hopLe = 1;
        printf("Nhap CCCD benh nhan: ");
        if (fgets(CCCD, sizeof(CCCD), stdin) == NULL) return;
        xoaXuongDong(CCCD);

        if (strlen(CCCD) == 0) {
            printf("Khong duoc de trong. Nhap lai.\n");
            hopLe = 0;
        } else if (!tatCaLaSo(CCCD) || strlen(CCCD) != 12) {
            printf("CCCD phai la 12 so. Nhap lai.\n");
            hopLe = 0;
        } else {
            index = timBenhNhanBangCCCD(CCCD);
            if (index == -1) {
                printf("Khong tim thay CCCD %s.\n", CCCD);
                hopLe = 0;
            }
        }
    } while (!hopLe);

    printf("Cap nhat thong tin cho %s:\n", dsBenhNhan[index].ten);

    do {
        hopLe = 1;
        printf("Nhap ten moi (Dang la: %s): ", dsBenhNhan[index].ten);
        if (fgets(tenMoi, sizeof(tenMoi), stdin) == NULL) return;
        xoaXuongDong(tenMoi);
        if (strlen(tenMoi) == 0 || !tenHopLe(tenMoi)) {
            printf("Ten khong hop le. Nhap lai.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    do {
        hopLe = 1;
        printf("Nhap so dien thoai moi (Dang la: %s, 9 so): ", dsBenhNhan[index].sdt);
        if (fgets(sdtMoi, sizeof(sdtMoi), stdin) == NULL) return;
        xoaXuongDong(sdtMoi);
        if (strlen(sdtMoi) == 0 || !soDienThoaiHopLe(sdtMoi)) {
            printf("SDT khong hop le. Nhap lai.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    do {
        hopLe = 1;
        printf("Nhap cong no moi (Dang la: %.0f): ", dsBenhNhan[index].no);
        if (fgets(noMoiStr, sizeof(noMoiStr), stdin) == NULL) return;
        xoaXuongDong(noMoiStr);
        noMoi = atof(noMoiStr);
        if (strlen(noMoiStr) == 0 || noMoi < 0) {
            printf("Nhap so duong hoac 0 nha.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    strcpy(dsBenhNhan[index].ten, tenMoi);
    strcpy(dsBenhNhan[index].sdt, sdtMoi);
    dsBenhNhan[index].no = noMoi;

    printf("Da cap nhat xong, thong tin moi nhu sau:\n");
    hienThiThongTinBenhNhan(&dsBenhNhan[index]);
    choNhanEnter();
}

void xuatBenhNhan() {
    xoaManHinh();
    char CCCD[15], xacNhan[5];
    int hopLe = 0;

    if (soBenhNhan == 0) {
        printf("Chua co benh nhan nao de xuat vien.\n");
        choNhanEnter();
        return;
    }

    printf("--- XUAT VIEN ---\n");
    do {
        hopLe = 1;
        printf("Nhap CCCD benh nhan: ");
        if (fgets(CCCD, sizeof(CCCD), stdin) == NULL) return;
        xoaXuongDong(CCCD);
        if (strlen(CCCD) == 0 || !tatCaLaSo(CCCD) || strlen(CCCD) != 12) {
            printf("Nhap lai CCCD nha.\n");
            hopLe = 0;
        } else {
            int kiemtra = timBenhNhanBangCCCD(CCCD);
            if (kiemtra == -1) {
                printf("Khong tim thay CCCD %s.\n", CCCD);
                hopLe = 0;
            }
        }
    } while (!hopLe);

    int index = timBenhNhanBangCCCD(CCCD);

    if (dsBenhNhan[index].no > 0) {
        char traTienStr[20];
        double daTra = 0.0;
        double noHienTai = dsBenhNhan[index].no;
        printf("Ban %s con no: %.0f VND.\n", dsBenhNhan[index].ten, noHienTai);

        do {
            hopLe = 1;
            printf("Nhap so tien muon tra (0 - %.0f): ", noHienTai);
            if (fgets(traTienStr, sizeof(traTienStr), stdin) == NULL) return;
            xoaXuongDong(traTienStr);
            if (strlen(traTienStr) == 0) {
                printf("Khong duoc de trong. Nhap lai.\n");
                hopLe = 0;
            } else {
                daTra = atof(traTienStr);
                if (daTra < 0) {
                    printf("Nhap so duong thoi. Nhap lai.\n");
                    hopLe = 0;
                }
            }
        } while (!hopLe);

        dsBenhNhan[index].no -= daTra;
        if (dsBenhNhan[index].no < 0) {
            dsBenhNhan[index].no = 0;
        }

        printf("Con no: %.0f VND.\n", dsBenhNhan[index].no);
    }

    if (dsBenhNhan[index].no > 0) {
        printf("Con no %.0f VND nhe.\n", dsBenhNhan[index].no);
        do {
            hopLe = 1;
            printf("Co muon xuat vien khi con no khong? (Y/N): ");
            if (fgets(xacNhan, sizeof(xacNhan), stdin) == NULL) return;
            xoaXuongDong(xacNhan);
            vietThuong(xacNhan);
            if (strcmp(xacNhan, "y") != 0 && strcmp(xacNhan, "n") != 0) {
                printf("Nhap Y hoac N thoi.\n");
                hopLe = 0;
            } else if (strcmp(xacNhan, "n") == 0) {
                printf("Huy xuat vien.\n");
                choNhanEnter();
                return;
            }
        } while (!hopLe);
    } else {
        printf("Benh nhan da tra het no.\n");
    }

    printf("--> Benh nhan da xuat vien:\n");
    hienThiThongTinBenhNhan(&dsBenhNhan[index]);

    for (int i = index; i < soBenhNhan - 1; i++) {
        dsBenhNhan[i] = dsBenhNhan[i + 1];
    }
    soBenhNhan--;

    choNhanEnter();
}

void timKiemBenhNhanBangTen() {
    xoaManHinh();
    char tuTim[50], tuTimThuong[50];
    int timThay = 0, hopLe = 0;

    if (soBenhNhan == 0) {
        printf("Khong co benh nhan nao de tim.\n");
        choNhanEnter();
        return;
    }

    printf("--- TIM KIEM BENH NHAN ---\n");
    do {
        hopLe = 1;
        printf("Nhap ten can tim: ");
        if (fgets(tuTim, sizeof(tuTim), stdin) == NULL) return;
        xoaXuongDong(tuTim);
        if (strlen(tuTim) == 0) {
            printf("Khong duoc de trong. Nhap lai.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    strcpy(tuTimThuong, tuTim);
    vietThuong(tuTimThuong);

    xoaManHinh();
    printf("+==========================================================+\n");
    printf("| Ket qua tim kiem                                         |\n");
    printf("+-------------+----------------------+----------+--------+-----+\n");
    printf("| %-11s | %-20s | %-8s | %-6s | %-3s |\n",
           "CCCD", "Ten", "SDT", "No", "KB");
    printf("+-------------+----------------------+----------+--------+-----+\n");

    for (int i = 0; i < soBenhNhan; i++) {
        char tenThuong[50];
        strcpy(tenThuong, dsBenhNhan[i].ten);
        vietThuong(tenThuong);
        if (strstr(tenThuong, tuTimThuong) != NULL) {
            printf("| %-11s | %-20s | %-8s | %-6.0f | %-3d |\n",
                   dsBenhNhan[i].CCCD, dsBenhNhan[i].ten, dsBenhNhan[i].sdt,
                   dsBenhNhan[i].no, dsBenhNhan[i].soNgayKham);
            timThay++;
        }
    }
    printf("+-------------+----------------------+----------+--------+-----+\n");
    if (timThay == 0) {
        printf("Khong co ai ten \"%s\".\n", tuTim);
    } else {
        printf("Tim thay %d benh nhan.\n", timThay);
    }
    choNhanEnter();
}

void sapXepTheoNo() {
    xoaManHinh();
    char kieuStr[5];
    int kieu, hopLe = 0;

    if (soBenhNhan == 0) {
        printf("Khong co du lieu de sap xep.\n");
        choNhanEnter();
        return;
    }

    printf("--- SAP XEP THEO NO ---\n");
    do {
        hopLe = 1;
        printf("Chon kieu:\n1. Tang dan (no thap nhat truoc)\n2. Giam dan (no cao nhat truoc)\nNhap 1 hoac 2: ");
        if (fgets(kieuStr, sizeof(kieuStr), stdin) == NULL) return;
        xoaXuongDong(kieuStr);
        kieu = atoi(kieuStr);
        if (strlen(kieuStr) == 0 || (kieu != 1 && kieu != 2)) {
            printf("Nhap 1 hoac 2 thoi.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    for (int i = 0; i < soBenhNhan - 1; i++) {
        for (int j = 0; j < soBenhNhan - 1 - i; j++) {
            int doi = 0;
            if (kieu == 1) {
                if (dsBenhNhan[j].no > dsBenhNhan[j + 1].no)
                    doi = 1;
            } else {
                if (dsBenhNhan[j].no < dsBenhNhan[j + 1].no)
                    doi = 1;
            }
            if (doi) {
                BenhNhan tmp = dsBenhNhan[j];
                dsBenhNhan[j] = dsBenhNhan[j + 1];
                dsBenhNhan[j + 1] = tmp;
            }
        }
    }

    printf("Da sap xep danh sach theo no xong.\nChon \"hien thi danh sach\" de xem ket qua.\n");
    choNhanEnter();
}

void ghiNhanKhamBenh() {
    xoaManHinh();
    char CCCD[15], ngayKham[20];
    int hopLe = 0;
    if (soBenhNhan == 0) {
        printf("Khong co ai de ghi nhan.\n");
        choNhanEnter();
        return;
    }
    printf("--- GHI NHAN KHAM BENH ---\n");
    do {
        hopLe = 1;
        printf("Nhap CCCD benh nhan: ");
        if (fgets(CCCD, sizeof(CCCD), stdin) == NULL) return;
        xoaXuongDong(CCCD);
        if (strlen(CCCD) == 0 || timBenhNhanBangCCCD(CCCD) == -1) {
            printf("Nhap lai CCCD nhe.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    int index = timBenhNhanBangCCCD(CCCD);

    do {
        hopLe = 1;
        printf("Nhap ngay kham (dd/mm/YYYY): ");
        if (fgets(ngayKham, sizeof(ngayKham), stdin) == NULL) return;
        xoaXuongDong(ngayKham);
        if (strlen(ngayKham) == 0 || !ngayHopLe(ngayKham)) {
            printf("Nhap dung dinh dang dd/mm/YYYY nhe.\n");
            hopLe = 0;
        } else {
            for (int i = 0; i < dsBenhNhan[index].soLichSu; i++) {
                if (strcmp(dsBenhNhan[index].cacLichSu[i].ngayKham, ngayKham) == 0) {
                    printf("Da ghi lich kham ngay %s roi.\n", ngayKham);
                    choNhanEnter();
                    return;
                }
            }
        }
    } while (!hopLe);

    if (dsBenhNhan[index].soLichSu >= MAX_KHAM_MOT_BENHNHAN) {
        printf("Da ghi du %d lan roi.\n", MAX_KHAM_MOT_BENHNHAN);
        choNhanEnter();
        return;
    }

    dsBenhNhan[index].soNgayKham++;

    LichSu lsMoi;
    sprintf(lsMoi.maLichSu, "LS%lld", idLichSuToanCuc++);
    strcpy(lsMoi.CCCD, CCCD);
    strcpy(lsMoi.ngayKham, ngayKham);
    strcpy(lsMoi.trangThai, "Theo doi");

    dsBenhNhan[index].cacLichSu[dsBenhNhan[index].soLichSu++] = lsMoi;

    printf("\nLich su moi vua ghi nhan cho benh nhan:\n");
    printf("+-----------+------------+-----------+\n");
    printf("| %-9s | %-10s | %-9s |\n", "MaLS", "NgayKham", "TrangThai");
    printf("+-----------+------------+-----------+\n");
    printf("| %-9s | %-10s | %-9s |\n",
        lsMoi.maLichSu, lsMoi.ngayKham, lsMoi.trangThai);
    printf("+-----------+------------+-----------+\n");
    hienThiThongTinBenhNhan(&dsBenhNhan[index]);
    choNhanEnter();
}

void xemLichSuKhamBenh() {
    xoaManHinh();
    char CCCD[15];
    int hopLe = 0;

    if (soBenhNhan == 0) {
        printf("Khong co ai de xem lich su.\n");
        choNhanEnter();
        return;
    }
    printf("--- XEM LICH SU KHAM BENH ---\n");
    do {
        hopLe = 1;
        printf("Nhap CCCD benh nhan: ");
        if (fgets(CCCD, sizeof(CCCD), stdin) == NULL) return;
        xoaXuongDong(CCCD);
        if (strlen(CCCD) == 0 || timBenhNhanBangCCCD(CCCD) == -1) {
            printf("Nhap lai CCCD nhe.\n");
            hopLe = 0;
        }
    } while (!hopLe);

    int index = timBenhNhanBangCCCD(CCCD);

    xoaManHinh();
    printf("\n+------------------------------------------------+\n");
    printf("| Lich su kham cua %-20s (CCCD: %s) |\n", dsBenhNhan[index].ten, CCCD);

    if (dsBenhNhan[index].soLichSu == 0) {
        printf("| %-46s |\n", "Chua co lich su kham nao.");
        printf("+------------------------------------------------+\n");
    } else {
        printf("+-----------+------------+-----------+\n");
        printf("| %-9s | %-10s | %-9s |\n", "MaLS", "NgayKham", "TrangThai");
        printf("+-----------+------------+-----------+\n");
        for (int i = 0; i < dsBenhNhan[index].soLichSu; i++) {
            printf("| %-9s | %-10s | %-9s |\n",
                   dsBenhNhan[index].cacLichSu[i].maLichSu, dsBenhNhan[index].cacLichSu[i].ngayKham,
                   dsBenhNhan[index].cacLichSu[i].trangThai);
        }
        printf("+-----------+------------+-----------+\n");
        printf("| Tong so lan kham: %-24d         |\n", dsBenhNhan[index].soLichSu);
        printf("+------------------------------------------------+\n");
    }
    hienThiThongTinBenhNhan(&dsBenhNhan[index]);
    choNhanEnter();
}

void khoiTaoDuLieu() {
    soBenhNhan = 7;
    strcpy(dsBenhNhan[0].CCCD, "001234567890");
    strcpy(dsBenhNhan[0].ten, "Nguyen Van An");
    strcpy(dsBenhNhan[0].sdt, "123456789");
    dsBenhNhan[0].no = 8500000;
    dsBenhNhan[0].soNgayKham = 8;
    dsBenhNhan[0].soLichSu = 2;
    sprintf(dsBenhNhan[0].cacLichSu[0].maLichSu, "LS%lld", idLichSuToanCuc++);
    strcpy(dsBenhNhan[0].cacLichSu[0].CCCD, "001234567890");
    strcpy(dsBenhNhan[0].cacLichSu[0].ngayKham, "01/11/2025");
    strcpy(dsBenhNhan[0].cacLichSu[0].trangThai, "Tai kham");
    sprintf(dsBenhNhan[0].cacLichSu[1].maLichSu, "LS%lld", idLichSuToanCuc++);
    strcpy(dsBenhNhan[0].cacLichSu[1].CCCD, "001234567890");
    strcpy(dsBenhNhan[0].cacLichSu[1].ngayKham, "05/11/2025");
    strcpy(dsBenhNhan[0].cacLichSu[1].trangThai, "Theo doi");

    strcpy(dsBenhNhan[1].CCCD, "001234567891");
    strcpy(dsBenhNhan[1].ten, "Tran Thi Binh");
    strcpy(dsBenhNhan[1].sdt, "987654321");
    dsBenhNhan[1].no = 300000;
    dsBenhNhan[1].soNgayKham = 2;
    dsBenhNhan[1].soLichSu = 1;
    sprintf(dsBenhNhan[1].cacLichSu[0].maLichSu, "LS%lld", idLichSuToanCuc++);
    strcpy(dsBenhNhan[1].cacLichSu[0].CCCD, "001234567891");
    strcpy(dsBenhNhan[1].cacLichSu[0].ngayKham, "10/11/2025");
    strcpy(dsBenhNhan[1].cacLichSu[0].trangThai, "Tai kham");

    strcpy(dsBenhNhan[2].CCCD, "001234567892");
    strcpy(dsBenhNhan[2].ten, "Le Van Cuong");
    strcpy(dsBenhNhan[2].sdt, "147852369");
    dsBenhNhan[2].no = 2500000;
    dsBenhNhan[2].soNgayKham = 12;
    dsBenhNhan[2].soLichSu = 0;

    strcpy(dsBenhNhan[3].CCCD, "001234567893");
    strcpy(dsBenhNhan[3].ten, "Nguyen Thi Dung");
    strcpy(dsBenhNhan[3].sdt, "333444555");
    dsBenhNhan[3].no = 100000;
    dsBenhNhan[3].soNgayKham = 1;
    dsBenhNhan[3].soLichSu = 0;

    strcpy(dsBenhNhan[4].CCCD, "001234567894");
    strcpy(dsBenhNhan[4].ten, "Pham Van Hung");
    strcpy(dsBenhNhan[4].sdt, "912345678");
    dsBenhNhan[4].no = 1200000;
    dsBenhNhan[4].soNgayKham = 5;
    dsBenhNhan[4].soLichSu = 0;

    strcpy(dsBenhNhan[5].CCCD, "001234567895");
    strcpy(dsBenhNhan[5].ten, "Hoang Van Khoa");
    strcpy(dsBenhNhan[5].sdt, "888777666");
    dsBenhNhan[5].no = 750000;
    dsBenhNhan[5].soNgayKham = 3;
    dsBenhNhan[5].soLichSu = 0;

    strcpy(dsBenhNhan[6].CCCD, "001234567896");
    strcpy(dsBenhNhan[6].ten, "Vuong Thi Mai");
    strcpy(dsBenhNhan[6].sdt, "999888777");
    dsBenhNhan[6].no = 4000000;
    dsBenhNhan[6].soNgayKham = 10;
    dsBenhNhan[6].soLichSu = 0;
}

void hienMenu() {
    xoaManHinh();
    printf("\n+===============================================+\n");
    printf("|   CHAO BAN DEN HE THONG QUAN LY BENH NHAN     |\n");
    printf("+===============================================+\n");
    printf("| So luong benh nhan: %d/%d                     |\n", soBenhNhan, MAX_BENHNHAN);
    printf("+-----+-----------------------------------------+\n");
    printf("| STT | CHUC NANG                               |\n");
    printf("+-----+-----------------------------------------+\n");
    printf("| 1   | Them benh nhan moi                      |\n");
    printf("| 2   | Cap nhat benh nhan                      |\n");
    printf("| 3   | Xuat vien                               |\n");
    printf("| 4   | Hien thi danh sach                      |\n");
    printf("| 5   | Tim kiem benh nhan theo ten             |\n");
    printf("| 6   | Sap xep benh nhan theo no               |\n");
    printf("| 7   | Ghi nhan kham benh                      |\n");
    printf("| 8   | Xem lich su kham benh                   |\n");
    printf("+-----+-----------------------------------------+\n");
    printf("| 0   | Thoat chuong trinh                      |\n");
    printf("+===============================================+\n");
    printf("Nhap so chuc nang ban muon chon: ");
}

int main() {
    khoiTaoDuLieu();
    char luaChon[5];
    int chon;
    do {
        hienMenu();
        if (fgets(luaChon, sizeof(luaChon), stdin) == NULL) {
            chon = -1;
        } else {
            xoaXuongDong(luaChon);
            if (sscanf(luaChon, "%d", &chon) != 1) {
                chon = -1;
            }
        }
        switch (chon) {
            case 1: themBenhNhanMoi(); break;
            case 2: capNhatBenhNhan(); break;
            case 3: xuatBenhNhan(); break;
            case 4: hienThiDSBenhNhan(); break;
            case 5: timKiemBenhNhanBangTen(); break;
            case 6: sapXepTheoNo(); break;
            case 7: ghiNhanKhamBenh(); break;
            case 8: xemLichSuKhamBenh(); break;
            case 0:
                xoaManHinh();
                printf("Cam on ban da su dung chuong trinh. Hen gap lai!\n");
                break;
            default:
                if (chon != 0) {
                    printf("Nhap sai, thu lai nha!\n");
                    choNhanEnter();
                }
                break;
        }
    } while (chon != 0);
    return 0;
}
