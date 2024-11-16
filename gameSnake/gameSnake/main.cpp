﻿#include <iostream>
#include "kmin_console.h"
#include <cstdlib>
using namespace std;

// Định nghĩa ký hiệu và các thông số ban đầu
#define DOT_RAN 254
#define MAX 100
#define LEN 1
#define XUONG 2
#define TRAI 3
#define PHAI 4
#define TUONG_TREN 2
#define TUONG_TRAI 3

// Biến toàn cục
struct ToaDo {
    int x;
    int y;
};
ToaDo ran[MAX];
int SoDot = 3;
int TUONG_DUOI, TUONG_PHAI; // Kích thước ván chơi
int highScore = 0;          // Kỷ lục cao nhất
int speed = 200;            // Tốc độ di chuyển (ms)

// Khai báo hàm
void KhoiTaoRan();
void HienThiRan(ToaDo lasttail);
ToaDo DiChuyen(int huong);
void BatSuKien(int& huong);
void VeTuong();
bool GameOver();
void XuLiThua();
ToaDo HienThiMoi();
bool KiemTraAnMoi(ToaDo moi);
void ThemDot();
bool KiemTraChamThan();
void Menu();
void ThietLapDoKhoVaKichThuoc();

int main() {
    Menu();
    while (true) {
        KhoiTaoRan();
        int huong = PHAI;
        int score = 0;
        VeTuong();
        ToaDo moi = HienThiMoi();

        // Hiển thị kỷ lục cao nhất
        gotoXY(TUONG_PHAI + 4, TUONG_TREN);
        cout << "HIGH SCORE: " << highScore;

        while (true) {
            ToaDo lasttail = DiChuyen(huong);
            BatSuKien(huong);
            HienThiRan(lasttail);
            if (KiemTraAnMoi(moi)) {
                moi = HienThiMoi();
                ThemDot();
                score++;
                gotoXY(TUONG_PHAI + 4, TUONG_TREN + 1);
                cout << "SCORE: " << score;
            }
            if (GameOver()) break;
            Sleep(speed);
        }

        // Cập nhật kỷ lục cao nhất
        if (score > highScore) {
            highScore = score;
        }

        XuLiThua();

        // Hỏi người chơi có muốn chơi lại không
        gotoXY(10, TUONG_DUOI + 2);
        cout << "Do you want to play again? (Y/N): ";
        char choice;
        cin >> choice;
        if (choice == 'N' || choice == 'n') {
            break;
        }
        clrscr();
    }

    // Hiển thị thông báo kết thúc trò chơi
    clrscr();
    cout << "Thanks for playing! Your high score: " << highScore << endl;
    return 0;
}

// Khởi tạo rắn ban đầu
void KhoiTaoRan() {
    ran[0].x = TUONG_TRAI + 3;
    ran[1].x = TUONG_TRAI + 2;
    ran[2].x = TUONG_TRAI + 1;
    ran[0].y = ran[1].y = ran[2].y = TUONG_TREN + 1;
    SoDot = 3; // Đặt lại số đốt của rắn
}

// Hiển thị rắn
void HienThiRan(ToaDo lasttail) {
    for (int i = 0; i < SoDot; i++) {
        gotoXY(ran[i].x, ran[i].y);
        cout << (char)DOT_RAN;
    }
    gotoXY(lasttail.x, lasttail.y);
    cout << ' ';
}

// Di chuyển rắn
ToaDo DiChuyen(int huong) {
    ToaDo lasttail = ran[SoDot - 1];
    for (int i = SoDot - 1; i > 0; i--) {
        ran[i] = ran[i - 1];
    }
    switch (huong) {
    case LEN: ran[0].y--; break;
    case XUONG: ran[0].y++; break;
    case TRAI: ran[0].x--; break;
    case PHAI: ran[0].x++; break;
    }
    return lasttail;
}

// Bắt sự kiện điều khiển
void BatSuKien(int& huong) {
    int key = inputKey();
    if (key == KEY_UP && huong != XUONG) huong = LEN;
    else if (key == KEY_DOWN && huong != LEN) huong = XUONG;
    else if (key == KEY_LEFT && huong != PHAI) huong = TRAI;
    else if (key == KEY_RIGHT && huong != TRAI) huong = PHAI;
}

// Vẽ tường
void VeTuong() {
    for (int i = TUONG_TRAI; i <= TUONG_PHAI; i++) {
        gotoXY(i, TUONG_TREN);
        cout << (char)220;
    }
    for (int i = TUONG_TREN + 1; i <= TUONG_DUOI; i++) {
        gotoXY(TUONG_TRAI, i);
        cout << (char)221;
    }
    for (int i = TUONG_TRAI; i <= TUONG_PHAI; i++) {
        gotoXY(i, TUONG_DUOI);
        cout << (char)223;
    }
    for (int i = TUONG_TREN + 1; i <= TUONG_DUOI - 1; i++) {
        gotoXY(TUONG_PHAI, i);
        cout << (char)222;
    }
}

// Kiểm tra thua
bool GameOver() {
    if (ran[0].y == TUONG_TREN || ran[0].y == TUONG_DUOI || ran[0].x == TUONG_TRAI || ran[0].x == TUONG_PHAI) {
        return true;
    }
    if (KiemTraChamThan()) {
        return true;
    }
    return false;
}

// Kiểm tra rắn chạm thân
bool KiemTraChamThan() {
    for (int i = 1; i < SoDot; i++) {
        if (ran[0].x == ran[i].x && ran[0].y == ran[i].y) {
            return true;
        }
    }
    return false;
}

// Xử lý khi thua
void XuLiThua() {
    clrscr();
    cout << "GAME OVER! Better luck next time!" << endl;
    Sleep(2000);
}

// Hiển thị mồi
ToaDo HienThiMoi() {
    ToaDo moi;
    do {
        srand(time(0));
        moi.x = TUONG_TRAI + 1 + rand() % (TUONG_PHAI - TUONG_TRAI - 1);
        moi.y = TUONG_TREN + 1 + rand() % (TUONG_DUOI - TUONG_TREN - 1);
    } while (moi.x <= TUONG_TRAI || moi.x >= TUONG_PHAI || moi.y <= TUONG_TREN || moi.y >= TUONG_DUOI);

    gotoXY(moi.x, moi.y);
    cout << '*';
    return moi;
}

// Kiểm tra rắn ăn mồi
bool KiemTraAnMoi(ToaDo moi) {
    return ran[0].x == moi.x && ran[0].y == moi.y;
}

// Thêm đốt vào rắn
void ThemDot() {
    ran[SoDot] = ran[SoDot - 1];
    SoDot++;
}

// Menu thiết lập
void Menu() {
    clrscr();
    cout << "===== SNAKE GAME MENU =====" << endl;
    cout << "1. Select difficulty:" << endl;
    cout << "   [1] Easy   (Slow)" << endl;
    cout << "   [2] Medium (Normal)" << endl;
    cout << "   [3] Hard   (Fast)" << endl;
    cout << "Enter your choice: ";
    int difficulty;
    cin >> difficulty;
    switch (difficulty) {
    case 1: speed = 300; break;
    case 2: speed = 200; break;
    case 3: speed = 100; break;
    default: speed = 200; break;
    }

    // Kiểm tra kích thước ván chơi
    while (true) {
        cout << "\n2. Enter board size:" << endl;
        cout << "   Width (min 20, max 100): ";
        cin >> TUONG_PHAI;
        TUONG_PHAI += TUONG_TRAI; // Tính vị trí biên phải
        cout << "   Height (min 10, max 27): ";
        cin >> TUONG_DUOI;
        TUONG_DUOI += TUONG_TREN; // Tính vị trí biên dưới

        // Kiểm tra xem kích thước có hợp lệ không
        if ((TUONG_PHAI - TUONG_TRAI >= 20 && TUONG_PHAI - TUONG_TRAI <= 100) &&
            (TUONG_DUOI - TUONG_TREN >= 10 && TUONG_DUOI - TUONG_TREN <= 27)) {
            break; // Kích thước hợp lệ
        }
        else {
            cout << "Invalid size! Width must be between 20 and 100, height must be between 10 and 27.\n";
        }
    }

    clrscr();
}
