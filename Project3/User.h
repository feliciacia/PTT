#pragma once
#ifndef __USER_H__
#define __USER_H__
#include <vector>
#include <string>
#include <iostream>



using namespace std;

class User
{
	friend class BoardManager;
protected:
	vector<int> postsID_;	// 紀錄建立過的post
	int Permission_level_ = 0;	// 權限等級
	string password_;
public:
	string name_;
	int getLevel() {
		return this->Permission_level_;
	}
	string getPassword() {
		return this->password_;
	}
	// Other infos, e.x. comment_nums, post_num, ….
};

class Adiministrator : public User {
public:
	Adiministrator();
	Adiministrator(string name, string password) {
		Permission_level_ = 1;
		this->name_ = name;
		this->password_ = password;
	}
};

class Member : public User {
public:
	Member();
	Member(string name, string password) {
		Permission_level_ = 2;
		this->name_ = name;
		this->password_ = password;
	}
};

class Guest : public User {
public:
	Guest();
	Guest(string name, string password) {
		Permission_level_ = 3;
		this->name_ = name;
		this->password_ = password;
	}
};
#endif