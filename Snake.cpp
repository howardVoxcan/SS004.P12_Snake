#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>

using namespace std;

void gotoxy (int column, int line);

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD topLeft = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

vector <pair <int,int>> snake(4);
vector <pair <int,int>> object;

void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawBorder(int width, int height) {
    for (int i = 0; i < width; i += 2) { // Step 2 for horizontal border
        gotoxy(i, 0); cout << '#';
        gotoxy(i, height - 1); cout << '#';
    }
    for (int i = 0; i < height; i++) {
        gotoxy(0, i); cout << '#';
        gotoxy(width - 2, i); cout << '#'; // Adjusted width to align with horizontal step
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./snake_game <width> <height> <speed_level>" << endl;
        cout << "Example: ./snake_game 40 20 1" << endl;
        return 1;
    }

    cout << "\n\n\n\n\n\n\n\n\n";

    int width = atoi(argv[1]) * 2, height = atoi(argv[2]), speed_level = atoi(argv[3]);

    char direction = 'd';
    bool gameOver = false;

    // Initial snake position
    snake[0] = {10, 5};
    snake[1] = {9, 5};
    snake[2] = {8, 5};
    snake[3] = {7, 5};

    // Draw border
    drawBorder(width, height);

    // Place the first food
    srand(time(0));
    int foodX, foodY;
    do {
        foodX = (rand() % ((width - 2) / 2)) * 2 + 1;  
        foodY = rand() % (height - 2) + 1; 
    } while (collisionWithObject(snake, foodX, foodY));

    while (!gameOver) {
        // Clear previous snake position
        gotoxy(snake.back().first, snake.back().second);
        cout << ' ';
        
        // Move the snake's body
        for (int i = snake.size() - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        // Change direction based on user input
        if (_kbhit()) {
            char newDirection = _getch();
            if ((newDirection == 'w' && direction != 's') ||
                (newDirection == 's' && direction != 'w') ||
                (newDirection == 'a' && direction != 'd') ||
                (newDirection == 'd' && direction != 'a')) {
                direction = newDirection;
            }
        }

        // Update snake's head position based on direction
        switch (direction) {
            case 'w': snake[0].second--; break;
            case 's': snake[0].second++; break;
            case 'a': snake[0].first -= 2; break; // Move 2 steps horizontally
            case 'd': snake[0].first += 2; break; // Move 2 steps horizontally
        }

        // Check for wall collision
        if (snake[0].first <= 0 || snake[0].first >= width - 1 || 
            snake[0].second <= 0 || snake[0].second >= height - 1) {
            gameOver = true;
            break;
        }

        // Check for collision with itself
        for (size_t i = 1; i < snake.size(); i++) {
            if (snake[0] == snake[i]) {
                gameOver = true;
                break;
            }
        }

        // Check for food collision
        if (collisionWithObject(object, snake[0].first, snake[0].second)) {
            // Add new segment to snake
            snake.push_back(snake.back());

            // Remove the eaten food
            object.pop_back();

            // Place new food
            do {
                foodX = (rand() % ((width - 2) / 2)) * 2 + 1;
                foodY = rand() % (height - 2) + 1;
            } while (collisionWithObject(snake, foodX, foodY)); // Avoid placing on snake

            object.push_back({foodX, foodY});
            gotoxy(foodX, foodY);
            cout << '*';
        }

        // Draw the snake
        gotoxy(snake[0].first, snake[0].second);
        cout << 'O';
        for (size_t i = 1; i < snake.size(); i++) {
            gotoxy(snake[i].first, snake[i].second);
            cout << 'o';
        }

        Sleep(100/speed_level); // Delay for game speed
    }

    // Game over message
    gotoxy(width / 2 - 5, height / 2);
    cout << "Game Over!";
    gotoxy(width / 2 - 6, height / 2 + 1);
    cout << "Press any key to exit.";
    _getch();

    return 0;
}
