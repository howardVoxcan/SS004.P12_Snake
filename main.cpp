#include <iostream>
#include <cstdlib>
#include<ctime>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
using namespace std;


#define DOT_RAN 254
#define MAX 10000
#define LEN 1
#define XUONG 2
#define TRAI 3
#define PHAI 4
#define TUONG_TREN 2
#define TUONG_TRAI 3
#define KEY_UP		72
#define KEY_DOWN	80
#define KEY_LEFT	75
#define KEY_RIGHT	77
#define KEY_NONE	-1

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
// Lấy nút bàn phím do người dùng bấm
// Trả về: Mã của phím
int inputKey();
// Xóa màn hình
void clrscr();
// Di chuyển con trỏ console đến vị trí có tọa độ (x, y)
void gotoXY (int x, int y);
// Lấy tọa độ x hiện tại của con trỏ console
int whereX();
// Lấy tọa độ y hiện tại của con trỏ console
int whereY();
// Xóa con trỏ nháy
void noCursorType();

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
    ran[0].x = TUONG_TRAI + 5;
    ran[1].x = TUONG_TRAI + 3;
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
    case TRAI: ran[0].x-=2; break;
    case PHAI: ran[0].x+=2; break;
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
    bool valid;

    do {
        valid = true;
        srand(time(0));
        moi.x = TUONG_TRAI + 1 + rand() % (TUONG_PHAI - TUONG_TRAI - 1);
        while (moi.x % 2 != 0)  
            moi.x = TUONG_TRAI + 1 + rand() % (TUONG_PHAI - TUONG_TRAI - 1);
        moi.y = TUONG_TREN + 1 + rand() % (TUONG_DUOI - TUONG_TREN - 1);

        // Kiểm tra mồi không trùng với bất kỳ đốt nào của rắn
        for (int i = 0; i < SoDot; i++) {
            if (moi.x == ran[i].x && moi.y == ran[i].y) {
                valid = false;
                break;
            }
        }
    } while (!valid || moi.x <= TUONG_TRAI || moi.x >= TUONG_PHAI || moi.y <= TUONG_TREN || moi.y >= TUONG_DUOI);

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
    cout << "   [1] Very Easy   (Very Slow)" << endl;
    cout << "   [2] Easy   (Slow)" << endl;
    cout << "   [3] Medium   (Normal)" << endl;
    cout << "   [4] Hard (Fast)" << endl;
    cout << "   [5] Very Hard   (Very Fast)" << endl;
    cout << "Enter your choice: ";
    int difficulty;
    cin >> difficulty;
    switch (difficulty) {
    case 1: speed = 300; break;
    case 2: speed = 250; break;
    case 3: speed = 200; break;
    case 4: speed = 150; break;
    case 5: speed = 100; break;
    default: speed = 200; break;
    }

    // Kiểm tra kích thước ván chơi
    while (true) {
        cout << "\n2. Enter board size:" << endl;
        cout << "   Width (min 20, max 70): ";
        cin >> TUONG_PHAI;
        TUONG_PHAI *= 2;
        TUONG_PHAI += TUONG_TRAI + 1; // Tính vị trí biên phải
        cout << "   Height (min 10, max 27): ";
        cin >> TUONG_DUOI;
        TUONG_DUOI += TUONG_TREN; // Tính vị trí biên dưới

        // Kiểm tra xem kích thước có hợp lệ không
        if ((TUONG_PHAI - TUONG_TRAI >= 40 && TUONG_PHAI - TUONG_TRAI <= 140) &&
            (TUONG_DUOI - TUONG_TREN >= 10 && TUONG_DUOI - TUONG_TREN <= 27)) {
            break; // Kích thước hợp lệ
        }
        else {
            cout << "Invalid size! Width must be between 20 and 100, height must be between 10 and 27.\n";
        }
    }

    clrscr();
}int inputKey()
{
	if (_kbhit())
	{
		int key = _getch();

		if (key == 224)
		{
			key = _getch();
			return key + 1000;
		}

		return key;
	}
	else
	{
		return KEY_NONE;
	}

	return KEY_NONE;
}

// Xóa màn hình
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;                  
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}

// Di chuyển con trỏ console đến vị trí có tọa độ (x, y)
void gotoXY (int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

// Lấy tọa độ x hiện tại của con trỏ console
int whereX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}

// Lấy tọa độ y hiện tại của con trỏ console
int whereY()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}

// Xóa con trỏ nháy
void noCursorType()
{
	CONSOLE_CURSOR_INFO info;
	info.bVisible = FALSE;
	info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
