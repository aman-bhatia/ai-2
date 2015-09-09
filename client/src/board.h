#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
using namespace std;

class Board{
	private:
		int board_size;
		vector< vector<char>* >* board;
	public:
		Board(int _board_size);
		void printBoard();
		int getBoardSize();
		bool setColor(int i, int j, char c);
		char getColor(int i, int j);
		bool moveColor(int i, int j, int p, int q);
};


#endif