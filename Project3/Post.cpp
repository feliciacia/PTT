#include "Post.h"
#include <iomanip>

namespace {
	void gotoxy(short x, short y) {
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position = { x,y };
		SetConsoleCursorPosition(h, position);
	}
}

bool Post::edit() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 15);
	system("CLS");

	
	CONSOLE_CURSOR_INFO cursorInfo = { 1, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);

	COORD pos = { 0,0 };
	gotoxy(pos.X, pos.Y + 1);
	int sPos = 0;
	vector<int>length;
	vector<string>text;
	text.push_back("內");
	text.push_back("文");
	text.push_back("");
	length.push_back(4);
	while (true) {
		cursorInfo = { 1, FALSE };
		SetConsoleCursorInfo(hOut, &cursorInfo);
		SetConsoleTextAttribute(hOut, 15);
		system("CLS");
		SetConsoleTextAttribute(hOut, 31);
		cout << "【編輯文章】                                                                                                            ";
		SetConsoleTextAttribute(hOut, 15);
		cout << endl;
		for (string s : text) {
			cout << s;
		}
		gotoxy(0, 29);
		SetConsoleTextAttribute(hOut, 116);
		cout << "[ctrl+Q]"; 
		SetConsoleTextAttribute(hOut, 112);
		cout << "離開                                                                                                           ";

		gotoxy(pos.X, pos.Y + 1);
		cursorInfo = { 1, TRUE };
		SetConsoleCursorInfo(hOut, &cursorInfo);
		char c = _getch();
		if (c == '\r') {
			if (length.size() == 28) { continue; }
			if (length[pos.Y] == 0) {
				text.insert(text.begin() + sPos, "\n");
				length.insert(length.begin() + pos.Y, 0);
			}
			else {
				text.insert(text.begin() + sPos, "\n");
				length.insert(length.begin() + pos.Y, pos.X);
				length[pos.Y + 1] -= length[pos.Y];
			}
			pos.X = 0;
			pos.Y++;
			sPos++;
		}
		else if (c == '\b' && sPos != 0) {
			sPos--;
			if (pos.X == 0) {
				pos.X = length[pos.Y - 1];
				length[pos.Y - 1] += length[pos.Y];
				length.erase(length.begin() + pos.Y);
				pos.Y--;
			}
			else {
				length[pos.Y] -= text[sPos].size();
				pos.X -= (SHORT)text[sPos].size();
			}
			text.erase(text.begin() + sPos);
			continue;
		}
		else if (c == -32) {
			c = _getch();
			//Down
			if (c == 80) {
				if (pos.Y == length.size() - 1) { continue; }
				else {
					if (text[sPos] == "\n") { sPos++; }
					else {
						while (text[sPos] != "\n") { sPos++; }
						sPos++;
					}
					pos.Y++;
					pos.X = 0;
				}
			}
			//Up
			else if (c == 72) {
				if (pos.Y == 0) { continue; }
				else {
					if (text[sPos-1] == "\n") {
						sPos -= 2;
						if (sPos < 0) { sPos = 0; }
						while (text[sPos] != "\n" && sPos != 0) { sPos--; }
						if (sPos != 0) { sPos++; }
					}

					else {
						bool run = false;
						sPos--;
						while (text[sPos] != "\n") { sPos--; }
						sPos--;
						if (sPos < 0) { sPos = 0; }
						while (text[sPos] != "\n" && sPos != 0) { sPos--; }
						if (sPos != 0) { sPos++; }
					}
					pos.Y--;
					pos.X = 0;
				}
			}
			//Left
			else if (c == 75) {
				if (sPos == 0) { continue; }
				else if (pos.X == 0) {
					sPos--;
					/*if (text[sPos] == "\n") {

					}*/
					pos.Y--;
					pos.X = length[pos.Y];
				}
				else {
					sPos--;
					pos.X -= (SHORT)text[sPos].size();
				}
			}
			//Right
			else if (c == 77) {
				if (sPos == text.size() - 1) { continue; }
				else if (pos.X == length[pos.Y]) {
					pos.Y++;
					pos.X = 0;
				}
				else {
					pos.X += (SHORT)text[sPos].size();
				}
				sPos++;
			}
			continue;
		}
		else if (c < 0) {
			string tmp;
			tmp.push_back(c);
			c = _getch();
			tmp.push_back(c);

			text.insert(text.begin() + sPos, tmp);
			length[pos.Y] += tmp.size();
			pos.X += (SHORT)tmp.size();
			sPos++;
		}
		//^q Exit
		else if (c == 17 || c == 27) {
			cursorInfo = { 1, FALSE };
			SetConsoleCursorInfo(hOut, &cursorInfo);
			gotoxy(0, 29);
			SetConsoleTextAttribute(hOut, 27);
			cout << "結束並儲存？ [y/n] [C]取消                                                                                              ";
			char t = _getch();
			if (t == 'y') {
				this->content.clear();
				for (string s : text) {
					this->content = this->content + s;
				}
				gotoxy(0, 29);
				SetConsoleTextAttribute(hOut, 27);
				cout << "                                                     請按任意鍵繼續                                                     ";
				while (1) {
					if (_kbhit()) {
						break;
					}
				}
				return true;
			}
			else if(t == 'n') {
				gotoxy(0, 29);
				SetConsoleTextAttribute(hOut, 27);
				cout << "                                                     請按任意鍵繼續                                                     ";
				while (1) {
					if (_kbhit()) {
						break;
					}
				}
				return false;
			}
			else {
				continue;
			}
		}
		else if (c < 32 || c > 126) { continue; }
		else if (length[pos.Y] > 100) { continue; }
		else {
			string tmp;
			tmp.push_back(c);

			text.insert(text.begin() + sPos, tmp);
			length[pos.Y] += tmp.size();
			pos.X += (SHORT)tmp.size();
			sPos++;
		}

	}
}

void Post::print(int row) {
	int i = 0;
	int row_ = row;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 15);

	if (i == row) {
		gotoxy(0, row - row_);
		SetConsoleTextAttribute(hOut, 31);
		cout << "                                                                                                                        ";
		SetConsoleTextAttribute(hOut, 241);
		gotoxy(0, row - row_);
		cout << " 作者 ";
		SetConsoleTextAttribute(hOut, 31);
		cout << " " << this->writer.name_;
		row++;
	}
	i++;
	if (i == row) {
		gotoxy(0, row - row_);
		SetConsoleTextAttribute(hOut, 31);
		cout << "                                                                                                                        ";
		SetConsoleTextAttribute(hOut, 241);
		gotoxy(0, row - row_);
		cout << " 標題 ";
		SetConsoleTextAttribute(hOut, 31);
		cout << " " << this->title;
		row++;
	}
	i++;
	if (i == row) {
		gotoxy(0, row - row_);
		SetConsoleTextAttribute(hOut, 3);
		cout << "------------------------------------------------------------------------------------------------------------------------";
		row++;
	}
	i++;
	SetConsoleTextAttribute(hOut, 15);
	if (i == row) {
		row++;
	}
	i++;
	gotoxy(0, row - row_);
	for (char ch : this->content) {
		if (i == row) {
			cout << ch;
			if (ch == '\n') { row++; }
		}
		if (ch == '\n') { 
			i++; 
			if (row - row_ == 29) { return; }
		}
	}
	if (i == row) { row++; }
	i++;
	if (row - row_ == 29) { return; }
	if (i == row) { row++; }
	i++;
	if (row - row_ == 29) { return; }
	gotoxy(0, row - row_);
	if (i == row) { 
		cout << "--";
		row++; 
	}
	i++;
	if (row - row_ == 29) { return; }
	gotoxy(0, row - row_);
	for (int j = 0; j < (int)this->reply.size(); j++) {
		if (i == row) {
			this->reply[j].print();
			gotoxy(113, row - row_);
			cout << setw(6) << j + 1;
			row++;
		}
		i++;
		if (row - row_ == 29) { return; }
		gotoxy(0, row - row_);
	}
}
int Post::rowNum() {
	int page = 1;
	for (char ch : this->content) {
		if (ch == '\n') { page++; }
	}
	return page + 6 + this->reply.size();
}


Reply::Reply(int vote, User writer, string content) {
	this->vote = vote;
	this->writer = writer;
	this->content = content;
}
void Reply::print() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->vote == 1) { 
		SetConsoleTextAttribute(hOut, 11); 
		cout << "推";
	}
	else if (this->vote == 2) { 
		SetConsoleTextAttribute(hOut, 12); 
		cout << "噓";
	}
	else if (this->vote == 3) { 
		SetConsoleTextAttribute(hOut, 15); 
		cout << "→";
	}
	cout << " ";
	SetConsoleTextAttribute(hOut, 14);
	cout << this->writer.name_;
	for (int i = this->writer.name_.size() - 1; i < 16; i++) { cout << " "; }
	cout << ": ";
	cout << this->content;
}
