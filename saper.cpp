#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int game_field[100][100];
// "-1" - мина
// "0-8" - любая другая клетка, где число означает количество мин вокруг
int player_field[100][100];
// "-2" - неизвестная игроку клетка
// "-1" - флаг, который игрок поставил
// "0-8" - открытая любая другая клетка, где число означает количество мин вокруг
const int ROWS = 9, COLUMNS = 9, MINES = 10;


void fill_space_around_mine(int i, int j) { //создание функции отвечающей за пространство вокруг мины

	for (int k = i - 1; k <= i + 1; k++) {
		for (int m = j - 1; m <= j + 1; m++) {
			if (k >= 0 && k < ROWS && m >= 0 && m <= COLUMNS && game_field[k][m] != -1) //заполнение игрового пространства вокруг мин
				game_field[k][m]++;
		}
	}
}

bool out_of_bounds(int x, int y, bool condition) { //создание функции
	return x < 0 || x >= ROWS || y < 0 || y >= COLUMNS || condition; //возвращение к игровому полю при выходе за игровое полу
}

void print_field() { //создание функции
	system("cls"); // очистка экрана
	cout << "  ";
	for (int i = 1; i <= COLUMNS; i++) { //кординаты для навигации
		cout << " " << i << " "; //выстраиваем колонны
	}
	cout << endl;
	for (int i = 0; i < ROWS; i++) {
		cout << i + 1 << "  "; //выстраиваем ряды
		for (int j = 0; j < COLUMNS; j++) {
			switch (player_field[i][j]) { //заменяем игровые клетки
			case -1: cout << "f  "; break;//флаг
			case -2: cout << "_  "; break;//нераскрытая игровая клетка
			case -3: cout << "*  "; break;//мина
			default: cout << player_field[i][j] << "  "; break;
			}
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

void depth_first_search(int x, int y) { //созданине функции отвечающей за первый клик (чтобы он был не в мину и очищал большое пространство
	if (out_of_bounds(x, y, player_field[x][y] > -2)) return;
	player_field[x][y] = game_field[x][y];
	if (game_field[x][y] > 0) return;
	for (int i = x - 1; i <= x + 1; ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {
			if (i != x || j != y)
				depth_first_search(i, j);
		}
	}
}

bool open_cell(int x, int y) { //создание функции в которой открытие клетки:
	if (game_field[x][y] == -1) return false; //не просихоидт
	if (game_field[x][y] > 0) { //происходит
		player_field[x][y] = game_field[x][y];
		return true;
	}
	depth_first_search(x, y);
}

bool is_win() { //выйграшный случай
	int opened = 0;
	for (int i = 0; i < ROWS; i++) { //проверка рядов
		for (int j = 0; j < COLUMNS; j++) { //проверка колонн
			if (player_field[i][j] >= 0) opened++; //открытие игровых клеток
		}
	}
	return (ROWS * COLUMNS - MINES == opened); //возвращение статуса "открыто" к первоначальному состаянию
}

void end_game(bool is_win) { //создание функии
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (player_field[i][j] == -1) player_field[i][j] = -2; //неизвестная игроку мина
			if (game_field[i][j] == -1) player_field[i][j] = -3; //??
		}
	}
	print_field(); //очищаем
	cout << "You " << (is_win ? "WIN" : "LOSE") << "!\nEnter any key to restart:\n";
}

int main()
{
	string command = "\\n";
	while (command != "\\q") {
		srand(time(0));
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLUMNS; j++) {
				game_field[i][j] = 0;
				player_field[i][j] = -2;
			}
		}
		int i = 0;
		while (i < MINES) { //спавним мины
			int x = rand() % ROWS; //рандомим позиции
			int y = rand() % COLUMNS; //рандомим позиции
			if (game_field[x][y] != -1) { //раставляем их
				game_field[x][y] = -1;
				i++;
			}
		}

		for (int i = 0; i < ROWS; i++) { //обеспечиваем нужные клетки вокруг мины (по рядам)
			for (int j = 0; j < COLUMNS; j++) { //по колоннам
				if (game_field[i][j] == -1) {
					fill_space_around_mine(i, j);
				}
			}
		}
		bool flag = true;
		while (flag) { //флаг определяет продолжение игры
			print_field();
			cout << "Enter the command:";
			// открыть клетку "\o x y"
			// поставить флаг "\f x y"
			// новая игра "\n"
			// выйти из игры "\q"

			cin >> command; //пользователь вводит команду

			if (command == "\\o") { //определяем команду
				int x, y;
				cin >> x >> y;
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //выход за пределы
				if (!open_cell(x, y)) { //открытие клетки
					end_game(false); //игра продолжается
					cin >> command; //вновь вводим команду
					flag = false;
				}
				if (is_win()) { //победа
					end_game(true);
					cin >> command;
					flag = false;
				}
			}
			else if (command == "\\f") { //спавним флаг
				int x, y;
				cin >> x >> y; //на этих кординатах
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //выход за пределы
				if (player_field[x][y] == -1) player_field[x][y] = -2;
				else player_field[x][y] = -1;
			}
			else if (command == "\\n") { //новая игра
				flag = false;
			}
			else if (command == "\\q") { //выйти из игры
				flag = false;
			}
		}
	}
	return 0;
}