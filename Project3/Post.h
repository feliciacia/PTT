#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include "User.h"
using namespace std;

class Reply;

class Post
{
public:
	vector<Reply>reply;
	string content;
	User writer;
	string title;
	bool deleted = false;
	bool edit();
	void print(int);
	int rowNum();
};

class Reply {
public:
	string content;
	User writer;
	int vote;//1:±À  2:¼N  3:½bÀY

	Reply() {
		this->content = "";
		this->vote = 3;
	}
	Reply(int, User, string);
	void print();
};