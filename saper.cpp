#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int game_field[100][100];
// "-1" - ����
// "0-8" - ����� ������ ������, ��� ����� �������� ���������� ��� ������
int player_field[100][100];
// "-2" - ����������� ������ ������
// "-1" - ����, ������� ����� ��������
// "0-8" - �������� ����� ������ ������, ��� ����� �������� ���������� ��� ������
const int ROWS = 9, COLUMNS = 9, MINES = 10;


void fill_space_around_mine(int i, int j) { //�������� ������� ���������� �� ������������ ������ ����

	for (int k = i - 1; k <= i + 1; k++) {
		for (int m = j - 1; m <= j + 1; m++) {
			if (k >= 0 && k < ROWS && m >= 0 && m <= COLUMNS && game_field[k][m] != -1) //���������� �������� ������������ ������ ���
				game_field[k][m]++;
		}
	}
}

bool out_of_bounds(int x, int y, bool condition) { //�������� �������
	return x < 0 || x >= ROWS || y < 0 || y >= COLUMNS || condition; //����������� � �������� ���� ��� ������ �� ������� ����
}

void print_field() { //�������� �������
	system("cls"); // ������� ������
	cout << "  ";
	for (int i = 1; i <= COLUMNS; i++) { //��������� ��� ���������
		cout << " " << i << " "; //����������� �������
	}
	cout << endl;
	for (int i = 0; i < ROWS; i++) {
		cout << i + 1 << "  "; //����������� ����
		for (int j = 0; j < COLUMNS; j++) {
			switch (player_field[i][j]) { //�������� ������� ������
			case -1: cout << "f  "; break;//����
			case -2: cout << "_  "; break;//����������� ������� ������
			case -3: cout << "*  "; break;//����
			default: cout << player_field[i][j] << "  "; break;
			}
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

void depth_first_search(int x, int y) { //��������� ������� ���������� �� ������ ���� (����� �� ��� �� � ���� � ������ ������� ������������
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

bool open_cell(int x, int y) { //�������� ������� � ������� �������� ������:
	if (game_field[x][y] == -1) return false; //�� ����������
	if (game_field[x][y] > 0) { //����������
		player_field[x][y] = game_field[x][y];
		return true;
	}
	depth_first_search(x, y);
}

bool is_win() { //���������� ������
	int opened = 0;
	for (int i = 0; i < ROWS; i++) { //�������� �����
		for (int j = 0; j < COLUMNS; j++) { //�������� ������
			if (player_field[i][j] >= 0) opened++; //�������� ������� ������
		}
	}
	return (ROWS * COLUMNS - MINES == opened); //����������� ������� "�������" � ��������������� ���������
}

void end_game(bool is_win) { //�������� ������
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (player_field[i][j] == -1) player_field[i][j] = -2; //����������� ������ ����
			if (game_field[i][j] == -1) player_field[i][j] = -3; //??
		}
	}
	print_field(); //�������
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
		while (i < MINES) { //������� ����
			int x = rand() % ROWS; //�������� �������
			int y = rand() % COLUMNS; //�������� �������
			if (game_field[x][y] != -1) { //���������� ��
				game_field[x][y] = -1;
				i++;
			}
		}

		for (int i = 0; i < ROWS; i++) { //������������ ������ ������ ������ ���� (�� �����)
			for (int j = 0; j < COLUMNS; j++) { //�� ��������
				if (game_field[i][j] == -1) {
					fill_space_around_mine(i, j);
				}
			}
		}
		bool flag = true;
		while (flag) { //���� ���������� ����������� ����
			print_field();
			cout << "Enter the command:";
			// ������� ������ "\o x y"
			// ��������� ���� "\f x y"
			// ����� ���� "\n"
			// ����� �� ���� "\q"

			cin >> command; //������������ ������ �������

			if (command == "\\o") { //���������� �������
				int x, y;
				cin >> x >> y;
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //����� �� �������
				if (!open_cell(x, y)) { //�������� ������
					end_game(false); //���� ������������
					cin >> command; //����� ������ �������
					flag = false;
				}
				if (is_win()) { //������
					end_game(true);
					cin >> command;
					flag = false;
				}
			}
			else if (command == "\\f") { //������� ����
				int x, y;
				cin >> x >> y; //�� ���� ����������
				--x; --y;
				if (out_of_bounds(x, y, player_field[x][y] >= 0)) continue; //����� �� �������
				if (player_field[x][y] == -1) player_field[x][y] = -2;
				else player_field[x][y] = -1;
			}
			else if (command == "\\n") { //����� ����
				flag = false;
			}
			else if (command == "\\q") { //����� �� ����
				flag = false;
			}
		}
	}
	return 0;
}