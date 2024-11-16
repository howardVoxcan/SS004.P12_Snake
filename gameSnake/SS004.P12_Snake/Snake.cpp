#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>

using namespace std;

void gotoxy(int column, int line);

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD topLeft = { 0, 0 };
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

vector<pair<int, int>> snake(4);  // Khởi tạo rắn với độ dài 4
vector<pair<int, int>> object;    // Vị trí thức ăn

void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawBorder(int width, int height) {
    for (int i = 0; i < width; i += 2) {  // Tạo đường viền ngang
        gotoxy(i, 0); cout << '#';
        gotoxy(i, height - 1); cout << '#';
    }
    for (int i = 0; i < height; i++) {   // Tạo đường viền dọc
        gotoxy(0, i); cout << '#';
        gotoxy(width - 2, i); cout << '#';
    }
}

bool collisionWithObject(const vector<pair<int, int>>& object, int x, int y) {
    for (auto& o : object) {
        if (o.first == x && o.second == y) {
            return true;
        }
    }
    return false;
}

void displayScore(int score) {
    gotoxy(0, 0);  // Hiển thị điểm số ở góc trên bên trái
    cout << "Score: " << score;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./snake_game <width> <height> <speed_level>" << endl;
        cout << "Example: ./snake_game 40 20 1" << endl;
        return 1;
    }

    int width = atoi(argv[1]) * 2, height = atoi(argv[2]), speed_level = atoi(argv[3]);
    char direction = 'd';
    bool gameOver = false;
    int score = 0;

    // Vị trí bắt đầu của rắn
    snake[0] = { 10, 5 };
    snake[1] = { 9, 5 };
    snake[2] = { 8, 5 };
    snake[3] = { 7, 5 };

    // Vẽ viền
    drawBorder(width, height);

    // Đặt vị trí thức ăn ban đầu
    srand(time(0));
    int foodX, foodY;
    do {
        foodX = (rand() % ((width - 2) / 2)) * 2 + 1;
        foodY = rand() % (height - 2) + 1;
    } while (collisionWithObject(snake, foodX, foodY));

    object.push_back({ foodX, foodY });
    gotoxy(foodX, foodY);
    cout << '*';

    displayScore(score);  // Hiển thị điểm số ban đầu

    while (!gameOver) {
        // Xóa vị trí cũ của đuôi rắn
        gotoxy(snake.back().first, snake.back().second);
        cout << ' ';

        // Di chuyển thân rắn
        for (int i = snake.size() - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Thay đổi hướng dựa vào phím bấm
        if (_kbhit()) {
            char newDirection = _getch();
            if ((newDirection == 'w' && direction != 's') ||
                (newDirection == 's' && direction != 'w') ||
                (newDirection == 'a' && direction != 'd') ||
                (newDirection == 'd' && direction != 'a')) {
                direction = newDirection;
            }
        }

        // Cập nhật vị trí đầu rắn
        switch (direction) {
        case 'w': snake[0].second--; break;
        case 's': snake[0].second++; break;
        case 'a': snake[0].first -= 2; break;  // Di chuyển 2 đơn vị
        case 'd': snake[0].first += 2; break;
        }

        // Kiểm tra va chạm với viền
        if (snake[0].first <= 0 || snake[0].first >= width - 1 ||
            snake[0].second <= 0 || snake[0].second >= height - 1) {
            gameOver = true;
            break;
        }

        // Kiểm tra va chạm với chính nó
        for (size_t i = 1; i < snake.size(); i++) {
            if (snake[0] == snake[i]) {
                gameOver = true;
                break;
            }
        }

        // Kiểm tra va chạm với thức ăn
        if (collisionWithObject(object, snake[0].first, snake[0].second)) {
            // Tăng độ dài rắn
            snake.push_back(snake.back());
            score += 10;  // Cộng điểm
            displayScore(score);  // Cập nhật điểm số

            // Tạo thức ăn mới
            object.pop_back();
            do {
                foodX = (rand() % ((width - 2) / 2)) * 2 + 1;
                foodY = rand() % (height - 2) + 1;
            } while (collisionWithObject(snake, foodX, foodY));

            object.push_back({ foodX, foodY });
            gotoxy(foodX, foodY);
            cout << '*';
        }

        // Vẽ lại rắn
        gotoxy(snake[0].first, snake[0].second);
        cout << 'O';
        for (size_t i = 1; i < snake.size(); i++) {
            gotoxy(snake[i].first, snake[i].second);
            cout << 'o';
        }

        // Tăng dần tốc độ khi điểm số cao hơn
        int delay = max(50, 100 / speed_level - score / 10);
        Sleep(delay);
    }

    // Màn hình Game Over
    gotoxy(width / 2 - 5, height / 2);
    cout << "Game Over!";
    gotoxy(width / 2 - 6, height / 2 + 1);
    cout << "Your Score: " << score;
    gotoxy(width / 2 - 8, height / 2 + 2);
    cout << "Press any key to exit.";
    _getch();

    return 0;
}
