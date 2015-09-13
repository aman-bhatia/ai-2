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

Board::~Board()
{
	for(int i = 0 ; i < board->size() ; ++i)
	{
		delete board->at(i);
	}
	delete board;
}


bool Board::isGameOver(){
	for (int i=0; i<board_size; i++){
		for (int j=0; j<board_size; j++){
			if (board->at(i)->at(j) == '-')
				return false;
		}
	}
	return true;
}

Board::Board(const Board &b)
{
	board_size = b.board_size;
	board = new vector< vector<char>* >(0);
	
	vector<char>* temp;
	for (int i=0; i<board_size; i++){
		temp  = new vector<char>(board_size);
		for(int j = 0 ; j < board_size;++j)
			temp->at(j) = (b.board->at(i))->at(j);
		board->push_back(temp);
	}
}

Board& Board::operator=(const Board &b)
{
	if (this != &b)
	{
		board->clear();

		board_size = b.board_size;
		board = new vector< vector<char>* >(0);

		vector<char>* temp;
		for (int i=0; i<board_size; i++){
			temp = new vector<char>(board_size);
			for(int j = 0 ; j < board_size;++j)
				temp->at(j) = (b.board->at(i))->at(j);
			board->push_back(temp);
		}
	}
	return *this;
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
void Board::setColor(int i, int j, char c){
	board->at(i)->at(j) = c;
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
void Board::moveColor(int i, int j, int p, int q){
	if (i==p && j==q)
		return;
	
	board->at(p)->at(q) = board->at(i)->at(j);
	board->at(i)->at(j) = '-';
}


bool Board::isOk(int x , int m , string &s)
{
	if(0 <= x and x < m and s[x] != '-')
	  return true;
	return false;
}

/*
 * Helper function for calculating score
 */
int Board::scoreHelper(string &s)
{
	int max = s.size();
	int score = 0;
	for(int i = 1 ; i < max ; ++i)
	{
		int length = 0;
		int scoreX = 0;
		int right = i;
		int left = i - 1;
		while(isOk(right , max , s) && isOk(left , max , s) && (s[left] == s[right]))
		{
			scoreX += length + 2;
			length += 2;
			right += 1;
			left -= 1;
		}
		score += scoreX;
		length = 1;
		scoreX = 0;
		right = i + 1;
		left = i - 1;
		while(isOk(right , max , s) && isOk(left , max , s) && (s[left] == s[right]) && isOk((right+left)/2, max, s))
		{
			scoreX += length + 2;
			length += 2;
			right += 1;
			left -= 1;
		}
		score += scoreX;
	}
  return score;
}


/*
 * Returns score of the board
 */
int Board::calculateScore()
{
	int score = 0;
	for(int i = 0 ; i < board_size ; ++i)
	{
		string str( (board->at(i))->begin() , (board->at(i))->end() );
		score += scoreHelper(str);
	}
	for(int i = 0 ; i < board_size ; ++i)
	{
		string temp = "";
		for(int j = 0 ; j < board_size ; ++j)
		{
		temp += (board->at(j)->at(i));
		}
		score += scoreHelper(temp);
	}
	return score;
}
