#include <iostream>
#include <fstream>
#include "BoardManager.h"
#include <direct.h>
#include <io.h>

using namespace std;

void loadFile(BoardManager&);
void saveFile(BoardManager&);

int main() {

	BoardManager BM;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	loadFile(BM);

	BM.print();
	while (true) {
		saveFile(BM);
		SetConsoleTextAttribute(hOut, 15);
		BM.print();
	}
	SetConsoleTextAttribute(hOut, 15);
	saveFile(BM);
}

void loadFile(BoardManager& BM) {
	fstream fin;
	fin.open("saved\\user.txt", ios_base::in);
	int num;
	BM.Users.clear();
	fin >> num;
	for (int i = 0; i < num; i++) {
		string name;
		string password;
		int level;
		fin >> name >> password >> level;
		if (level == 1) {
			BM.Users.push_back(new Adiministrator(name, password));
		}
		else if (level == 2) {
			BM.Users.push_back(new Member(name, password));
		}
		else if (level == 3) {
			BM.Users.push_back(new Guest(name, password));
		}
	}
	fin.close();

	vector<int>poNum;
	fin.open("saved\\boards.txt", ios_base::in);
	fin >> num;
	for (int i = 0; i < num; i++) {
		Board newBoard;
		string owner;
		int postNum;
		fin >> newBoard.title >> owner >> postNum;
		for (User* u : BM.Users) {
			if (u->name_ == owner) { newBoard.owner = *u; }
		}
		BM.boards.push_back(newBoard);
		poNum.push_back(postNum);
	}
	fin.close();
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < poNum[i]; j++) {
			Post newPost;
			string postFile;
			postFile = "saved\\boards\\" + BM.boards[i].title + "\\" + BM.boards[i].title + "_" + to_string(j + 1);
			fin.open((postFile + ".txt").c_str(), ios_base::in);
			getline(fin, newPost.title);
			string writer;
			fin >> writer;
			for (User* u : BM.Users) {
				if (u->name_ == writer) { newPost.writer = *u; }
			}
			fin >> newPost.deleted;
			string str;
			getline(fin, str);
			while (!fin.eof()) {
				getline(fin, str);
				newPost.content += str;
				newPost.content += "\n";
			}
			newPost.content.pop_back();
			BM.boards[i].posts.push_back(newPost);
			fin.close();

			fin.open((postFile + "_comment.txt").c_str(), ios_base::in);
			int size_;
			fin >> size_;
			for (int k = 0; k < size_; k++) {
				Reply newReply;
				fin >> newReply.vote;
				fin >> writer;
				for (User* u : BM.Users) {
					if (u->name_ == writer) { newReply.writer = *u; }
				}
				getline(fin, str);
				getline(fin, str);
				newReply.content = str;
				BM.boards[i].posts[j].reply.push_back(newReply);
			}
			fin.close();
		}
	}
}
void saveFile(BoardManager& BM) {
	fstream fout;
	fout.open("saved\\user.txt", ios::out | ios::trunc);
	fout << BM.Users.size() << endl;
	for (User* u : BM.Users) {
		fout << u->name_ << endl;
		fout << u->getPassword() << endl;
		fout << u->getLevel() << endl;
	}
	fout.close();

	fout.open("saved\\boards.txt", ios::out | ios::trunc);
	fout << BM.boards.size() << endl;
	for (Board b : BM.boards) {
		fout << b.title << endl;
		fout << b.owner.name_ << endl;
		fout << b.posts.size() << endl;
	}
	fout.close();

	for (Board b : BM.boards) {
		string fileName = "saved\\boards\\";
		fileName = "saved\\boards\\" + b.title;
		if (_access(fileName.c_str(), 0)!=0) {
			int tt = _mkdir(fileName.c_str());
		}
		fileName = "saved\\boards\\" + b.title + "\\" + b.title + "_";
		for (int i = 0; i < (int)b.posts.size(); i++) {
			Post post = b.posts[i];
			string postFile;
			postFile = fileName + to_string(i + 1) + ".txt";
			fout.open(postFile.c_str(), ios::out | ios::trunc);
			fout << post.title << endl;
			fout << post.writer.name_ << endl;
			fout << post.deleted << endl;
			fout << post.content;
			fout.close();

			postFile= fileName + to_string(i + 1) + "_comment.txt";
			fout.open(postFile.c_str(), ios::out | ios::trunc);
			fout << post.reply.size() << endl;
			for (Reply r : post.reply) {
				fout << r.vote << endl;
				fout << r.writer.name_ << endl;
				fout << r.content << endl;
			}
			fout.close();
		}
	}
}