#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
using namespace std;

class Board{
	private:
		int board_size;
		vector< vector<char>* >* board;
		//vector<int>* colors_available;
		//int num_cells_free;
		vector< vector<int>* >* weights;

	public:
		Board(int _board_size);
		~Board();
		Board& operator = (const Board &b);
		Board(const Board &b);
		void printBoard();
		int getBoardSize() const;
		bool isGameOver();
		void setColor(int i, int j, char c);
		char getColor(int i, int j) const;
		void moveColor(int i, int j, int p, int q);
		bool isOk(int x , int m , string &s);
		int scoreHelper(string &s);
		int extraHelper();
		int calculateScore();
		//int getNumCellsFree() const;
		int getWeight(int i, int j) const;
		//int getColorAvailable(int index) const;
		int evalFunction();
};


#endif