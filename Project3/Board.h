#pragma once
#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include <vector>
#include "User.h"
#include "Post.h"
#include <iomanip>
#include <conio.h>
using namespace std;

class Board
{
public:
	vector<Post> posts;
	User owner;
	string title;

	Board();
	Board(string explain);
	void addPost(User&);
	void printBoard();
};



#endif