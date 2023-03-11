#include "Board.h"
#include "User.h"
#include <iomanip>
#include <iostream>
#include <Windows.h>
using namespace std;

namespace {
	void gotoxy(short x, short y) {
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position = { x,y };
		SetConsoleCursorPosition(h, position);
	}
}

Board::Board()
{
	title = "";
}

Board::Board(string explain)
{
	title = explain;
}

void Board::printBoard() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hOut, 15);
	system("CLS");

	SetConsoleTextAttribute(hOut, 31);
	cout << "�m" << this->title << "�n";
	for (int i = 0; i < (int)(107 - this->title.size() - this->owner.name_.size()); i++) {
		cout << " ";
	}
	cout << "�i�O�D:" << this->owner.name_ << "�j";

	SetConsoleTextAttribute(hOut, 15);
	cout << endl;
	cout << "                       [��][q]�^�W�h  [��][r]�\Ū  [����]���  [c]�o��峹  [e]�s��峹  [d]�R��                        ";
	SetConsoleTextAttribute(hOut, 112);
	cout << endl;
	cout << "      �s��  �@  ��          ��  ��  ��  �D                                                                              ";
	SetConsoleTextAttribute(hOut, 15);
	COORD position = { 2,3 };
	gotoxy(position.X, position.Y);
	for (int i = 0; i < (int)this->posts.size(); i++) {
		cout << setw(8) << i + 1;
		gotoxy(12, position.Y);
		cout << this->posts[i].writer.name_;
		gotoxy(28, position.Y);
		cout << this->posts[i].title;
		position.Y++;
		gotoxy(position.X, position.Y);
	}
	cout << "    BACK  ��^";
}

void Board::addPost(User& currentUser)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	vector<string>title;
	SetConsoleTextAttribute(hOut, 15);
	system("CLS");

	SetConsoleTextAttribute(hOut, 31);
	cout << "�i�o��峹�j                                                                                                            ";
	SetConsoleTextAttribute(hOut, 15);
	cout << endl;
	cout << "���D�G";
	SetConsoleTextAttribute(hOut, 240);
	cout << "                                                                                          ";
	gotoxy(6, 1);
	int length = 0;
	while (true) {
		char c = _getch();
		string tmp;
		if (c == '\r') { break; }
		else if (c == '\b' && length > 0) {
			for (int i = 0; i < (int)title.back().size(); i++) {
				cout << "\b \b";
			}
			title.pop_back();
			continue;
		}
		else if (c == -32) {
			c = _getch();
			continue;
		}
		else if (c < 0) {
			tmp.clear();
			tmp.push_back(c);
			c = _getch();
			tmp.push_back(c);
			cout << tmp;
			length += tmp.size();
		}
		else if (c < 32 || c > 126) { continue; }
		else if (length > 88) { continue; }
		else {
			tmp.clear();
			tmp.push_back(c);
			cout << tmp;
			length += tmp.size();
		}
		title.push_back(tmp);
	}
	if (title.size() == 0) { return; }

	this->posts.push_back(Post());
	if (this->posts.back().edit()) {
		for (string s : title) {
			this->posts.back().title = this->posts.back().title + s;
		}
		this->posts.back().writer = currentUser;
	}
	else {
		this->posts.pop_back();
	}
}