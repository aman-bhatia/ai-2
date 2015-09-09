#include "board.h"

/*
 * Constructor
 */
Board::Board(int _board_size){
	board_size = _board_size;
	board = new vector< vector<char>* >(0);
	
	vector<char>* temp;
	for (int i=0; i<board_size; i++){
		temp  = new vector<char>(board_size,'-');
		board->push_back(temp);
	}
}


/*
 * Prints the configuration of the board
 */
void Board::printBoard(){
	for (int i=0; i<board_size; i++){
		for (int j=0; j<board_size; j++){
			cerr << board->at(i)->at(j) << " ";
		}
		cerr << endl;
	}
}


/*
 * returns size of the board
 */
int Board::getBoardSize(){
	return board_size;
}

/*
 * Sets color of the cell at (i,j) to c
 */
bool Board::setColor(int i, int j, char c){
	if (board->at(i)->at(j) != '-')
		return false;
	
	board->at(i)->at(j) = c;
	return true;
}


/*
 * returns color of the cell at (i,j)
 */
char Board::getColor(int i, int j){
	return board->at(i)->at(j);
}


/*
 * Sets the color of cell at (p,q) to be equal to that of cell at (i,j) and free the cell at (i,j)
 */
bool Board::moveColor(int i, int j, int p, int q){
	if (i==p && j==q)
		return true;
	if (board->at(p)->at(q) != '-')
		return false;
	
	board->at(p)->at(q) = board->at(i)->at(j);
	board->at(i)->at(j) = '-';
	return true;
}
