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
		~Board();
		Board& operator = (const Board &b);
		Board(const Board &b);
		void printBoard();
		int getBoardSize();
		bool isGameOver();
		void setColor(int i, int j, char c);
		char getColor(int i, int j);
		void moveColor(int i, int j, int p, int q);
		bool isOk(int x , int m , string &s);
		int scoreHelper(string &s);
		int calculateScore();
};


#endif