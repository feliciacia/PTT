#pragma once

#include "User.h"
#include "Board.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;


enum BoardState
{
	MENU,
	SELECT_BOARD,
	BOARD,
	POST
};

class BoardManager
{
public:
	vector<User*> Users;
	vector<Board> boards;
	int current_user = -1;
	int current_board = -1;
	int current_post = -1;
	BoardState state = MENU;
	//Viewer viewer;
	//void State();
	BoardManager();
	void print();

	bool login(string);
	bool registered();
	void addBoard();
};

