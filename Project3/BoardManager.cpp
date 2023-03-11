#include "BoardManager.h"
#include "User.h"
#include <iomanip>
#include <vector>

using namespace std;
class AccountWrong :public exception {
public:
	string text;
	AccountWrong() {
		this->text = "�K�X����εL���b���A���ˬd�j�p�g�Φ��L��J���~�C";
	}
};

void gotoxy(short x, short y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x,y };
	SetConsoleCursorPosition(h, position);
}

void boardClear() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, 15);
	for (int i = 0; i < 29; i++) {
		gotoxy(0, i);
		cout << "\x1B[2K";
	}
}

BoardManager::BoardManager() {
	this->current_user = -1;
	this->state = BoardState::MENU;
}

void BoardManager::print() {
	system("CLS");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	/*******************************************/
	/*               MENU                      */
	/*******************************************/
	if (this->state == BoardState::MENU) {
		string input;
		SetConsoleTextAttribute(hOut, 15);
		cout << "�п�J�b���A�ΥHguest���[�A�ΥHnew���U�G";
		SetConsoleTextAttribute(hOut, 240);
		cout << "               ";

		gotoxy(40, 0);
		while (true) {
			char c = _getch();
			if (c == '\r') { break; }
			else if (c == '\b' && input.size() > 0) {
				cout << "\b \b";
				input.pop_back();
				continue;
			}
			else if (c == -32) {
				c = _getch();
				continue;
			}
			else if (c < 33 || c>126) { continue; }
			else if (input.size() > 13) { continue; }
			cout << c;
			input.push_back(c);
		}
		SetConsoleTextAttribute(hOut, 15);

		if (input == "new") {
			this->registered();
		}
		else if (input == "guest") {
			this->current_user = 0;
			this->state = BoardState::SELECT_BOARD;
		}
		else {
			try {
				this->login(input);
			}
			catch(AccountWrong c){
				SetConsoleTextAttribute(hOut, 15);
				gotoxy(0, 1);
				cout << c.text;
				char a = _getch();
			}
		}
	}
	/***********************************************/
	/*               SELECT_BOARD                  */
	/***********************************************/
	else if (this->state == BoardState::SELECT_BOARD) {
		SetConsoleTextAttribute(hOut, 15);
		system("CLS");

		SetConsoleTextAttribute(hOut, 31);
		cout << "                                                      �i�ݪO�C��j                                                      ";
		SetConsoleTextAttribute(hOut, 15);
		cout << endl;
		cout << "                           [��][q]�^�W�h  [��][r]�\Ū  [����]���  [c]�إ߷s�ݪO  [d]�R���ݪO                           ";
		SetConsoleTextAttribute(hOut, 112);
		cout << endl;
		cout << "      �s��        ��  �O                                                                  �O  �D                        ";
		SetConsoleTextAttribute(hOut, 15);
		COORD position = { 4,3 };
		gotoxy(position.X, position.Y);
		for (int i = 0; i < (int)boards.size(); i++) {
			cout << setw(6) << i + 1;
			gotoxy(18, position.Y);
			cout << this->boards[i].title;
			gotoxy(90, position.Y);
			cout << this->boards[i].owner.name_;
			position.Y++;
			gotoxy(position.X, position.Y);
		}
		cout << "  Exit        �n�X�A888888888888~~";

		position = { 0,3 };
		gotoxy(position.X, position.Y);
		CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
		SetConsoleCursorInfo(hOut, &cursorInfo);
		cout << ">";
		gotoxy(position.X, position.Y);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (GetConsoleScreenBufferInfo(hOut, &csbi))
		{
			position = csbi.dwCursorPosition;
		}
		while (true) {
			char c = _getch();
			if (c == -32) {
				c = _getch();
				//Down
				if (c == 80) {
					if (position.Y == this->boards.size() + 3) { continue; }
					cout << " ";
					position.Y++;
					SetConsoleCursorPosition(hOut, position);
					cout << ">";
					SetConsoleCursorPosition(hOut, position);
				}
				//Up
				else if (c == 72) {
					if (position.Y == + 3) { continue; }
					cout << " ";
					position.Y--;
					SetConsoleCursorPosition(hOut, position);
					cout << ">";
					SetConsoleCursorPosition(hOut, position);
				}
				//Left
				else if (c == 75) {
					cout << " ";
					position.Y = (SHORT)(this->boards.size() + 3);
					SetConsoleCursorPosition(hOut, position);
					cout << ">";
					SetConsoleCursorPosition(hOut, position);
				}
				//Right
				else if (c == 77) {
					if (position.Y == this->boards.size() + 3) {
						system("CLS");
						cout << "�T�w�n���}�ܡH(Y/N) ";
						SetConsoleTextAttribute(hOut, 240);
						cout << "   ";
						gotoxy(20, 0);
						string input;
						while (true) {
							char c = _getch();
							if (c == '\r') { break; }
							else if (c == '\b' && input.size() > 0) {
								cout << "\b \b";
								input.pop_back();
								continue;
							}
							else if (c == -32) {
								c = _getch();
								continue;
							}
							else if (c < 33 || c>126) { continue; }
							else if (input.size() > 1) { continue; }
							cout << c;
							input.push_back(c);
						}
						SetConsoleTextAttribute(hOut, 15);
						if (input == "Y" || input == "y") {
							this->current_user = -1;
							this->state = BoardState::MENU;
							return;
						}
						else {
							return;
						}
					}
					else {
						this->state = BoardState::BOARD;
						this->current_board = position.Y - 3;
						return;
					}
				}
			}
			else if (c == 'q') {
				cout << " ";
				position.Y = (SHORT)(this->boards.size() + 3);
				SetConsoleCursorPosition(hOut, position);
				cout << ">";
				SetConsoleCursorPosition(hOut, position);
			}
			else if (c == 'd') {
				int current = position.Y - 3;
				if (current==this->boards.size()) {
					continue;
				}
				else if (this->Users[this->current_user]->Permission_level_ < 2) {
					system("CLS");
					cout << "�T�w�R�����ݪO�ܡH(Y/N) ";
					SetConsoleTextAttribute(hOut, 240);
					cout << "   ";
					gotoxy(24, 0);
					string input;
					while (true) {
						char c = _getch();
						if (c == '\r') { break; }
						else if (c == '\b' && input.size() > 0) {
							cout << "\b \b";
							input.pop_back();
							continue;
						}
						else if (c == -32) {
							c = _getch();
							continue;
						}
						else if (c < 33 || c>126) { continue; }
						else if (input.size() > 1) { continue; }
						cout << c;
						input.push_back(c);
					}
					SetConsoleTextAttribute(hOut, 15);
					if (input == "Y" || input == "y") {
						this->boards.erase(this->boards.begin() + current);
						return;
					}
					else {
						return;
					}
				}
				else {
					continue;
				}
			}
			else if (c == 'r') {
				if (position.Y == this->boards.size() + 3) {
					system("CLS");
					cout << "�T�w�n���}�ܡH(Y/N) ";
					SetConsoleTextAttribute(hOut, 240);
					cout << "   ";
					gotoxy(20, 0);
					string input;
					while (true) {
						char c = _getch();
						if (c == '\r') { break; }
						else if (c == '\b' && input.size() > 0) {
							cout << "\b \b";
							input.pop_back();
							continue;
						}
						else if (c == -32) {
							c = _getch();
							continue;
						}
						else if (c < 33 || c>126) { continue; }
						else if (input.size() > 1) { continue; }
						cout << c;
						input.push_back(c);
					}
					SetConsoleTextAttribute(hOut, 15);
					if (input == "Y" || input == "y") {
						this->current_user = -1;
						this->state = BoardState::MENU;
						return;
					}
					else {
						return;
					}
				}
				else {
					this->state = BoardState::BOARD;
					this->current_board = position.Y - 3;
					return;
				}
			}
			else if (c == 'c') {
			    if (this->Users[this->current_user]->Permission_level_ > 1) {
					system("CLS");
					cout << "�v������...";
					char a = _getch();
					if (a == -32) {
						a = _getch();
					}
					return;
				}
				else {
					this->addBoard();
					return;
				}
            }
		}
	}

	/***********************************************/
	/*                   BOARD                     */
	/***********************************************/
	if (this->state == BoardState::BOARD) {
		Board& currentBoard = this->boards[this->current_board];
		currentBoard.printBoard();

		COORD position = { 0,3 };
		gotoxy(position.X, position.Y);
		CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
		SetConsoleCursorInfo(hOut, &cursorInfo);
		cout << ">";
		gotoxy(position.X, position.Y);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (GetConsoleScreenBufferInfo(hOut, &csbi))
		{
			position = csbi.dwCursorPosition;
		}
		while (true) {
			char c = _getch();
			if (c == -32) {
				c = _getch();
				//Down
				if (c == 80) {
					if (position.Y == this->boards[this->current_board].posts.size() + 3) { continue; }
					cout << " ";
					position.Y++;
					SetConsoleCursorPosition(hOut, position);
					cout << ">";
					SetConsoleCursorPosition(hOut, position);
				}
				//Up
				else if (c == 72) {
					if (position.Y == +3) { continue; }
					cout << " ";
					position.Y--;
					SetConsoleCursorPosition(hOut, position);
					cout << ">";
					SetConsoleCursorPosition(hOut, position);
				}
				//Left
				else if (c == 75) {
					this->current_board = -1;
					this->state = BoardState::SELECT_BOARD;
					return;
				}
				//Right
				else if (c == 77) {
					if (position.Y == this->boards[this->current_board].posts.size() + 3) {
						this->current_board = -1;
						this->state = BoardState::SELECT_BOARD;
						return;
					}
					else if (this->boards[this->current_board].posts[position.Y - 3].deleted) {
						continue;
					}
					else {
						this->state = BoardState::POST;
						this->current_post = position.Y - 3;
						return;
					}
				}
			}
			//Q
			else if (c == 'q') {
				this->current_board = -1;
				this->state = BoardState::SELECT_BOARD;
				return;
			}
			//D
			else if (c == 'd') {
				int current = position.Y - 3;
				if (current == this->boards[this->current_board].posts.size()) {
					continue;
				}
				else if (currentBoard.posts[current].deleted) {
					continue;
				}
				//�ۤw�R��
				else if (currentBoard.posts[current].writer.name_ == this->Users[this->current_user]->name_ || this->Users[this->current_user]->getLevel() == 1) {
					system("CLS");
					cout << "�T�w�R�����K��ܡH(Y/N) ";
					SetConsoleTextAttribute(hOut, 240);
					cout << "   ";
					gotoxy(24, 0);
					string input;
					while (true) {
						char c = _getch();
						if (c == '\r') { break; }
						else if (c == '\b' && input.size() > 0) {
							cout << "\b \b";
							input.pop_back();
							continue;
						}
						else if (c == -32) {
							c = _getch();
							continue;
						}
						else if (c < 33 || c>126) { continue; }
						else if (input.size() > 1) { continue; }
						cout << c;
						input.push_back(c);
					}
					SetConsoleTextAttribute(hOut, 15);
					if (input == "Y" || input == "y") {
						currentBoard.posts[current].deleted = true;
						currentBoard.posts[current].title = "(����w�Q�R��)";
						return;
					}
					else {
						return;
					}
				}
				//�޲z���R��
				else if (this->Users[this->current_user]->Permission_level_ == 3) {
					SetConsoleTextAttribute(hOut, 15);
					system("CLS");
					vector<string> reason;
					cout << "�R���]�G";
					SetConsoleTextAttribute(hOut, 240);
					cout << "                                              ";
					gotoxy(10, 0);
					int length = 0;
					while (true) {
						char c = _getch();
						string tmp;
						if (c == '\r') { break; }
						else if (c == '\b' && length > 0) {
							for (int i = 0; i < (int)reason.back().size(); i++) {
								cout << "\b \b";
							}
							reason.pop_back();
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
						else if (c < 32 || c>126) { continue; }
						else if (length > 45) { continue; }
						else {
							tmp.clear();
							tmp.push_back(c);
							cout << tmp;
							length += tmp.size();
						}
						reason.push_back(tmp);
					}

					SetConsoleTextAttribute(hOut, 15);
					cout << endl;
					cout << "�T�w�R�����K��ܡH(Y/N) ";
					SetConsoleTextAttribute(hOut, 240);
					cout << "   ";
					gotoxy(24, 0);
					string input;
					while (true) {
						char c = _getch();
						if (c == '\r') { break; }
						else if (c == '\b' && input.size() > 0) {
							cout << "\b \b";
							input.pop_back();
							continue;
						}
						else if (c == -32) {
							c = _getch();
							continue;
						}
						else if (c < 33 || c>126) { continue; }
						else if (input.size() > 1) { continue; }
						cout << c;
						input.push_back(c);
					}
					SetConsoleTextAttribute(hOut, 15);
					if (input == "Y" || input == "y") {
						currentBoard.posts[current].deleted = true;
						string title;
						title = "(����w�Q�R��)[" + this->Users[this->current_user]->name_ + "]:";
						for (string s : reason) {
							title = title + s;
						}
						return;
					}
					else {
						return;
					}
				}
				else {
					continue;
				}
			}
			//R
			else if (c == 'r') {
				if (position.Y == this->boards.size() + 3) {
					this->current_board = -1;
					this->state = BoardState::SELECT_BOARD;
					return;
				}
				else if (this->boards[this->current_board].posts[position.Y - 3].deleted) {
					continue;
				}
				else {
					this->state = BoardState::POST;
					this->current_post = position.Y - 3;
					return;
				}
			}
			//C
			else if (c == 'c') {
				if (this->Users[this->current_user]->Permission_level_ == 3) {
					continue;
				}
				else {
					currentBoard.addPost(*this->Users[this->current_user]);
					return;
				}
			}
			//E
			if (c == 'e') {
				int current = position.Y - 3;
				if (current == this->boards[this->current_board].posts.size()) {
					continue;
				}
				else if (currentBoard.posts[current].deleted) {
					continue;
				}
				else if (currentBoard.posts[current].writer.name_ == this->Users[this->current_user]->name_) {
					currentBoard.posts[current].edit();
					return;
				}
				else {
					continue;
				}
			}
		}
	}

	/***********************************************/
	/*                   POST                      */
	/***********************************************/
	if (this->state == BoardState::POST) {
		SetConsoleTextAttribute(hOut, 15);
		Post& currentPost = this->boards[this->current_board].posts[this->current_post];
		int row = 0;
		char c;

		currentPost.print(row);
		gotoxy(0, 29);
		SetConsoleTextAttribute(hOut, 116);
		cout << "                                                                                                       (X)";
		SetConsoleTextAttribute(hOut, 112);
		cout << "���� ";
		SetConsoleTextAttribute(hOut, 116);
		cout << "(��)";
		SetConsoleTextAttribute(hOut, 112);
		cout << "���} ";
		while (true) {
			c = _getch();

			if (c == -32) {
				c = _getch();
				//Down
				if (c == 80) {
					if (row > (currentPost.rowNum() - 29)) { continue; }
					else {
						row++;
						boardClear();
						currentPost.print(row);
						continue;
					}
				}
				//Up
				else if (c == 72) {
					if (row == 0) { continue; }
					else {
						row--;
						boardClear();
						currentPost.print(row);
						continue;
					}
				}
				//Left
				else if (c == 75) {
					this->current_post = -1;
					this->state = BoardState::BOARD;
					SetConsoleTextAttribute(hOut, 112);
					return;
				}
				//Right
				else if (c == 77) {
					continue;
				}
			}
			else if ((c == 'x' || c == 'X') && this->Users[this->current_user]->Permission_level_ < 3) {
				SetConsoleTextAttribute(hOut, 15);
				gotoxy(0, 29);
				cout << "\x1B[2K";
				gotoxy(0, 29);
				cout << "�zı�o�o�g�峹 ";
				SetConsoleTextAttribute(hOut, 14);
				cout << "1.�ȱo���� ";
				SetConsoleTextAttribute(hOut, 12);
				cout << "2.�����N ";
				SetConsoleTextAttribute(hOut, 15);
				cout << "3.�u�[������ ";
				SetConsoleTextAttribute(hOut, 7);
				cout << "[1]? ";
				Reply tmp;
				tmp.writer = *this->Users[this->current_user];
				c = _getch();
				gotoxy(0, 29);
				if (c == '2') { 
					tmp.vote = 2; 
					SetConsoleTextAttribute(hOut, 12);
					cout << "�N ";
				}
				else if (c == '3') { 
					tmp.vote = 3; 
					SetConsoleTextAttribute(hOut, 15);
					cout << "�� ";
				}
				else { 
					tmp.vote = 1; 
					SetConsoleTextAttribute(hOut, 14);
					cout << "�� ";
				}
				SetConsoleTextAttribute(hOut, 7);
				cout << tmp.writer.name_;
				for (int cou = 0; cou < (int)(16 - tmp.writer.name_.size()); cou++) { cout << " "; }
				cout << ": ";
				SetConsoleTextAttribute(hOut, 112);
				cout << "                                                                                    ";
				gotoxy(21, 29);

				int length = 0;
				vector<string>comment;
				while (true) {
					char c = _getch();
					string tmp;
					if (c == '\r') { break; }
					else if (c == '\b' && length > 0) {
						for (int i = 0; i < (int)comment.back().size(); i++) {
							cout << "\b \b";
						}
						comment.pop_back();
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
					comment.push_back(tmp);
				}
				if (comment.size() == 0) { 
					gotoxy(0, 29);
					SetConsoleTextAttribute(hOut, 116);
					cout << "                                                                                                       (X)";
					SetConsoleTextAttribute(hOut, 112);
					cout << "���� ";
					SetConsoleTextAttribute(hOut, 116);
					cout << "(��)";
					SetConsoleTextAttribute(hOut, 112);
					cout << "���} ";
					continue;
				}
				SetConsoleTextAttribute(hOut, 7);
				gotoxy(106, 29);
				cout << " �T�w[Y/N]:";
				SetConsoleTextAttribute(hOut, 112);
				cout << "  ";
				gotoxy(117, 29);
				string input;
				input.clear();
				while (true) {
					char c = _getch();
					if (c == '\r') { break; }
					else if (c == '\b' && input.size() > 0) {
						cout << "\b \b";
						input.pop_back();
						continue;
					}
					else if (c == -32) {
						c = _getch();
						continue;
					}
					else if (c < 33 || c>126) { continue; }
					else if (input.size() > 1) { continue; }
					cout << c;
					input.push_back(c);
				}
				SetConsoleTextAttribute(hOut, 15);
				if (input == "y" || input == "Y") {
					tmp.content.clear();
					for (string s : comment) {
						tmp.content += s;
					}
					currentPost.reply.push_back(tmp);
					return;
				}
				else{
					gotoxy(0, 29);
					SetConsoleTextAttribute(hOut, 116);
					cout << "                                                                                                       (X)";
					SetConsoleTextAttribute(hOut, 112);
					cout << "���� ";
					SetConsoleTextAttribute(hOut, 116);
					cout << "(��)";
					SetConsoleTextAttribute(hOut, 112);
					cout << "���} ";
					continue;
				}
			}
			else if (c == 'q' || c == 'Q') {
				this->current_post = -1;
				this->state = BoardState::BOARD;
				SetConsoleTextAttribute(hOut, 112);
				return;
			}
			else {
				continue;
			}
		}
	}
}

bool  BoardManager::login(string name) {
	gotoxy(0, 1);
	for (int i = 0; i < (int)this->Users.size(); i++) {
		if (Users[i]->name_ == name) {
			string password;
			cout << "�п�J�K�X�G";
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			//cout << "               ";
			//gotoxy(12, 1);

			while (true) {
				char c = _getch();
				if (c == '\r') { break; }
				else if (c == '\b' && password.size() > 0) {
					cout << "\b \b";
					password.pop_back();
					continue;
				}
				else if (c == -32) {
					c = _getch();
					continue;
				}
				else if (c < 33 || c > 126) { continue; }
				else if (password.size() > 13) { continue; }
				cout << "*";
				password.push_back(c);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

			if (Users[i]->password_ == password) {
				this->current_user = i;
				this->state = BoardState::SELECT_BOARD;
				return true;
			}
			else { 
				throw AccountWrong();
				return false; 
			}
		}
	}
	throw AccountWrong();
	return false;
}

bool BoardManager::registered() {
	system("CLS");

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	string username, password, temp;
	
	std::cout << "|���U|" << endl;
	cout << "�п�J�b���G";
	SetConsoleTextAttribute(hOut, 240);
	cout << "               ";
	gotoxy(12, 1);

	bool userUsed = true;
	while (userUsed) {
		userUsed = false;
		username.clear();
		while (true) {
			char c = _getch();
			if (c == '\r') { break; }
			else if (c == '\b' && username.size() > 0) {
				cout << "\b \b";
				username.pop_back();
				continue;
			}
			else if (c == 27) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				return false;
			}
			else if (c == -32) {
				c = _getch();
				continue;
			}
			else if (c < 33 || c>126) { continue; }
			else if (username.size() > 13) { continue; }
			cout << c;
			username.push_back(c);
		}
		if (username == "")
		{
			gotoxy(7, 0);
			SetConsoleTextAttribute(hOut, 15);
			cout << "�S���g��W�١A�ЦA�դ@�M";
			gotoxy(0, 1);
			cout << "�п�J�b���G";
			SetConsoleTextAttribute(hOut, 240);
			cout << "               ";
			gotoxy(12, 1);
			userUsed = true;
			continue;
		}
		for (auto user : this->Users) {
			if (user->name_ == username) {
				gotoxy(7, 0);
				SetConsoleTextAttribute(hOut, 15);
				cout << "���W�٤w�s�b�A�ШϥΨ�L�W��";
				gotoxy(0, 1);
				cout << "�п�J�b���G";
				SetConsoleTextAttribute(hOut, 240);
				cout << "               ";
				gotoxy(12, 1);
				userUsed = true;
			}
		}
	}

	SetConsoleTextAttribute(hOut, 15);
	system("CLS");
	std::cout << "|���U|" << endl;
	cout << "�п�J�K�X�G";
	while (true) {
		char c = _getch();
		if (c == '\r') { break; }
		else if (c == '\b' && password.size() > 0) {
			cout << "\b \b";
			password.pop_back();
			continue;
		}
		else if (c == 27) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			return false;
		}
		else if (c == -32) {
			c = _getch();
			continue;
		}
		else if (c < 33 || c > 126) { continue; }
		else if (password.size() > 13) { continue; }
		cout << "*";
		password.push_back(c);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	this->Users.push_back(new Member(username, password));

	system("CLS");
	cout << "�w���U�����A�����N��^��n�J�e��";
	char c = _getch();
	if (c == -32) {
		c = _getch();
	}
	return true;
}

void BoardManager::addBoard() {
	system("CLS");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 0,1 };

	string title;
	bool used = true;

	SetConsoleTextAttribute(hOut, 31);
	cout << "                                                      �i�Ыطs�O�j                                                      ";
	SetConsoleTextAttribute(hOut, 15);
	cout << endl;

	while(used){
		used = false;
		cout << "�ݪO���D�G";
		SetConsoleTextAttribute(hOut, 240);
		cout << "                ";
		gotoxy(10, position.Y);
		while (true) {
			char c = _getch();
			if (c == '\r') { break; }
			else if (c == '\b' && title.size() > 0) {
				cout << "\b \b";
				title.pop_back();
				continue;
			}
			else if (c == -32) {
				c = _getch();
				continue;
			}
			else if (c < 33 || c>126) { continue; }
			else if (title.size() > 14) { continue; }
			cout << c;
			title.push_back(c);
		}
		SetConsoleTextAttribute(hOut, 15);

		if (title.size() == 0) { return; }
		for (Board b : this->boards) {
			if (b.title == title) {
				gotoxy(28, position.Y);
				cout << "�ݪO�W�٤w�ϥ�";
				gotoxy(0, position.Y);
				used = true;
				break;
			}
		}
		if (!used) {
			position.Y += 2;

			gotoxy(0, position.Y);
			cout << "�T�w�n�ЫضܡH(Y/N) ";
			SetConsoleTextAttribute(hOut, 240);
			cout << "   ";
			gotoxy(20, position.Y);
			string input;
			while (true) {
				char c = _getch();
				if (c == '\r') { break; }
				else if (c == '\b' && input.size() > 0) {
					cout << "\b \b";
					input.pop_back();
					continue;
				}
				else if (c == -32) {
					c = _getch();
					continue;
				}
				else if (c < 33 || c>126) { continue; }
				else if (input.size() > 1) { continue; }
				cout << c;
				input.push_back(c);
			}
			SetConsoleTextAttribute(hOut, 15);
			if (input == "Y" || input == "y") {
				Board newBoard;
				newBoard.title = title;
				newBoard.owner = *this->Users[this->current_user];
				this->boards.push_back(newBoard);
				return;
			}
			else {
				return;
			}
		}
	}
}