#include "board.h"

/*
 * Constructor
 */
Board::Board(int _board_size){
	board_size = _board_size;
	board = new vector< vector<char>* >(0);
	//colors_available = new vector<int>(board_size,board_size); 
	num_cells_free = board_size*board_size;
	
	
	weights = new vector< vector<double>* >(0);
	for (int i=0; i<board_size; i++){
		vector<double>* temp  = new vector<double>(board_size, 0);
		weights->push_back(temp);
	}

	weights->at(0)->at(0) = 1;
	weights->at(0)->at(1) = 2;
	weights->at(0)->at(2) = 4;
	weights->at(0)->at(3) = 2;
	weights->at(0)->at(4) = 1;
	
	weights->at(1)->at(0) = 2;
	weights->at(1)->at(1) = 6;
	weights->at(1)->at(2) = 8;
	weights->at(1)->at(3) = 6;
	weights->at(1)->at(4) = 2;

	weights->at(2)->at(0) = 4;
	weights->at(2)->at(1) = 8;
	weights->at(2)->at(2) = 12;
	weights->at(2)->at(3) = 8;
	weights->at(2)->at(4) = 4;

	weights->at(3)->at(0) = 2;
	weights->at(3)->at(1) = 6;
	weights->at(3)->at(2) = 8;
	weights->at(3)->at(3) = 6;
	weights->at(3)->at(4) = 2;

	weights->at(4)->at(0) = 1;
	weights->at(4)->at(1) = 2;
	weights->at(4)->at(2) = 4;
	weights->at(4)->at(3) = 2;
	weights->at(4)->at(4) = 1;
	

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
	//delete colors_available;
	for(int i = 0 ; i < weights->size() ; ++i)
	{
		delete weights->at(i);
	}
	delete weights;
}


Board::Board(const Board &b)
{
	board_size = b.board_size;
	board = new vector< vector<char>* >(0);
	
	/*
	colors_available = new vector<int>(board_size);
	for (int i=0;i<board_size;i++){
		colors_available->at(i) = b.getColorAvailable(i);
	}
	*/
	
	num_cells_free = b.getNumCellsFree();

	weights = new vector< vector<double>* >(0);
	for (int i=0; i<board_size; i++){
		vector<double>* temp  = new vector<double>(board_size, 0);
		weights->push_back(temp);
	}


	for (int i=0; i<board_size; i++){
		for(int j = 0 ; j < board_size;++j){
			weights->at(i)->at(j) = b.getWeight(i,j);
		}
	}

	vector<char>* temp;
	for (int i=0; i<board_size; i++){
		temp  = new vector<char>(board_size);
		for(int j = 0 ; j < board_size;++j)
			temp->at(j) = b.getColor(i,j);
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
		
		weights = new vector< vector<double>* >(0);
		for (int i=0; i<board_size; i++){
			vector<double>* temp  = new vector<double>(board_size, 0);
			weights->push_back(temp);
		}

		/*
		colors_available = new vector<int>(board_size);
		for (int i=0;i<board_size;i++){
			colors_available->at(i) = b.getColorAvailable(i);
		}
		
		*/
		num_cells_free = b.getNumCellsFree();

		for (int i=0; i<board_size; i++){
			for(int j = 0 ; j < board_size;++j){
				weights->at(i)->at(j) = b.getWeight(i,j);
			}
		}

		vector<char>* temp;
		for (int i=0; i<board_size; i++){
			temp = new vector<char>(board_size);
			for(int j = 0 ; j < board_size;++j)
				temp->at(j) = b.getColor(i,j);
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
int Board::getBoardSize() const{
	return board_size;
}



bool Board::isGameOver(){
	/*
	for (int i=0; i<board_size; i++){
		for (int j=0; j<board_size; j++){
			if (board->at(i)->at(j) == '-')
				return false;
		}
	}
	return true;
	*/
	return (num_cells_free == 0);
}


/*
 * Sets color of the cell at (i,j) to c
 */
void Board::setColor(int i, int j, char c){
	
	if (c == '-'){
		//colors_available->at(board->at(i)->at(j) - 'A')++;
		num_cells_free++;

		if(i + 1 < board_size)
			setWeight(i + 1 , j , (2 * getWeight(i + 1 , j) / 3));
		if(i - 1 >= 0)
			setWeight(i - 1 , j , (2 * getWeight(i - 1 , j) / 3));
		if(j + 1 < board_size)
			setWeight(i , j + 1 , (2 * getWeight(i , j + 1) / 3));
		if(j - 1 >= 0)
			setWeight(i , j - 1 , (2 * getWeight(i , j - 1) / 3));
	} else {
		//colors_available->at(c - 'A')--;
		num_cells_free--;

		if(i + 1 < board_size)
			setWeight(i + 1 , j , (3 * getWeight(i + 1 , j)) / 2);
		if(i - 1 >= 0)
			setWeight(i - 1 , j , (3 * getWeight(i - 1 , j)) / 2);
		if(j + 1 < board_size)
			setWeight(i , j + 1 , (3 * getWeight(i , j + 1)) / 2);
		if(j - 1 >= 0)
			setWeight(i , j - 1 , (3 * getWeight(i , j - 1)) / 2);
	}

	board->at(i)->at(j) = c;
}


/*
 * returns color of the cell at (i,j)
 */
char Board::getColor(int i, int j) const{
	return board->at(i)->at(j);
}


/*
 * Sets the color of cell at (p,q) to be equal to that of cell at (i,j) and free the cell at (i,j)
 */
void Board::moveColor(int i, int j, int p, int q){
	if (i==p && j==q)
		return;
	
	setColor(p,q, board->at(i)->at(j));
	setColor(i,j,'-');
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

double Board::scoreHelper1(string &s , int row , bool isrow)
{
	int max = s.size();
	double score = 0;
	for(int i = 1 ; i < max ; ++i)
	{
		double tempscore = 0;
		double scoreX = 0;
		int right = i;
		int left = i - 1;
		while(isOk(right , max , s) && isOk(left , max , s) && (s[left] == s[right]))
		{
			if (isrow)
				tempscore += weights->at(row)->at(left) + weights->at(row)->at(right);
			else
				tempscore += weights->at(left)->at(row) + weights->at(right)->at(row);
			scoreX +=  tempscore;
			right += 1;
			left -= 1;
		}
		score += scoreX;
		if (isrow)
			tempscore = weights->at(row)->at(i);
		else
			tempscore = weights->at(i)->at(row);
		scoreX = 0;
		right = i + 1;
		left = i - 1;
		while(isOk(right , max , s) && isOk(left , max , s) && (s[left] == s[right]) && isOk((right+left)/2, max, s))
		{
			if (isrow)
				tempscore += weights->at(row)->at(left) + weights->at(row)->at(right);
			else
				tempscore += weights->at(left)->at(row) + weights->at(right)->at(row);
			scoreX +=  tempscore;
			right += 1;
			left -= 1;
		}
		score += scoreX;
	}
  return score;
}


int Board::getWeight(int i, int j) const{
	return weights->at(i)->at(j);
}


void Board::setWeight(int i , int j , int w)
{
	weights->at(i)->at(j) = w;
}

double Board::extraHelper()
{
	double score = 0;
	for(int i = 0 ; i < 5 ; ++i)
		for(int j = 0 ; j < 5 ; ++j)
			if(board->at(i)->at(j) != '-')
				score += weights->at(i)->at(j);
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



double Board::calculateScore1()
{
	double score = 0;
	for(int i = 0 ; i < board_size ; ++i)
	{
		string str( (board->at(i))->begin() , (board->at(i))->end() );
		score += scoreHelper1(str , i , true);
	}
	for(int i = 0 ; i < board_size ; ++i)
	{
		string temp = "";
		for(int j = 0 ; j < board_size ; ++j)
		{
		temp += (board->at(j)->at(i));
		}
		score += scoreHelper1(temp , i , false);
	}
	return score;
}


int Board::calculateScorea()
{
	int score = 0;
	for(int i = 0 ; i < board_size ; ++i)
	{
		string str( (board->at(i))->begin() , (board->at(i))->end() );
		score += alternato(str);
	}
	for(int i = 0 ; i < board_size ; ++i)
	{
		string temp = "";
		for(int j = 0 ; j < board_size ; ++j)
		{
		temp += (board->at(j)->at(i));
		}
		score += alternato(temp);
	}
	return score;
}



int Board::getNumCellsFree() const{
	return num_cells_free;
}

int Board::alternato(string& s)
{
	int count = 0;
	for(int i = 0 ; i < s.size() ; ++i)	
	{
		// AA
		if(i + 1 < s.size() and s[i] == s[i + 1] and s[i] != '-')
			count += 2;
		// A-A
		if(i + 2 < s.size() and s[i] == s[i + 2] and s[i] != '-')
			count += 6;
		//A--A
		if(i + 3 < s.size() and s[i] == s[i + 3] and s[i+1]==s[i+3] and s[i+1]=='-' and s[i] != '-')
			count += 5;
		//AA-A
		if(i + 3 < s.size() and s[i] == s[i + 3] and s[i+1]==s[i] and s[i+2]=='-' and s[i] != '-')
			count += 6;
		// ABBA
		if(i + 3 < s.size() and s[i] == s[i + 3] and s[i + 1] == s[i + 2] and s[i + 1] != '-' and s[i] != '-')
			count += 7;
		// AAAA
		if(i + 3 < s.size() and s[i] == s[i + 1] and s[i + 1] == s[i + 2] and s[i + 2] == s[i + 3] and s[i] != '-')
			count += 10;
		// AB-BA
		if(i + 4 < s.size() and s[i] == s[i + 4] and s[i + 1] == s[i + 3] and s[i] != '-' and s[i + 1] != '-')
			count += 8;
		// A-C-A
		if(i + 4 < s.size() and s[i] == s[i + 4] and s[i + 1] == s[i + 3] and s[i] != '-' and s[i + 2] != '-')
			count += 8;
	
		// AAA
		if(i + 4 < s.size() and s[i] == s[i + 1] and s[i + 1] == s[i + 2] and s[i] != '-')
			count += 10;
		// A-A-A
		if(i + 4 < s.size() and s[i] == s[i + 4] and s[i] == s[i + 2] and s[i] != '-')
			count += 10;
	}
	return count;
}

/*
int Board::getColorAvailable(int index) const{
	return colors_available->at(index);
}
*/

double Board::evalFunction(){
	return 0.2 * extraHelper() + 10*calculateScorea()+ 3*calculateScore()/* -0.2245 * calculateScore1() + 0 * num_cells_free*/;
}
double Board::evalFunction1(){
	return 10*calculateScorea() + 2 * calculateScore() /*- 0.2245 * calculateScore1() + 0 * num_cells_free*/;
}