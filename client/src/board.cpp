#include "board.h"

/*
 * Constructor
 */
Board::Board(int _board_size){
	board_size = _board_size;
	board = new vector< vector<char>* >(0);
	//colors_available = new vector<int>(board_size,board_size); 
	//num_cells_free = board_size*board_size;
	
	
	weights = new vector< vector<int>* >(0);
	for (int i=0; i<board_size; i++){
		vector<int>* temp  = new vector<int>(board_size, 0);
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
	weights->at(2)->at(2) = 10;
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
	
	num_cells_free = b.getNumCellsFree();
	*/

	weights = new vector< vector<int>* >(0);
	for (int i=0; i<board_size; i++){
		vector<int>* temp  = new vector<int>(board_size, 0);
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
		
		weights = new vector< vector<int>* >(0);
		for (int i=0; i<board_size; i++){
			vector<int>* temp  = new vector<int>(board_size, 0);
			weights->push_back(temp);
		}

		/*
		colors_available = new vector<int>(board_size);
		for (int i=0;i<board_size;i++){
			colors_available->at(i) = b.getColorAvailable(i);
		}
		
		num_cells_free = b.getNumCellsFree();
		*/

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
	for (int i=0; i<board_size; i++){
		for (int j=0; j<board_size; j++){
			if (board->at(i)->at(j) == '-')
				return false;
		}
	}
	return true;
}


/*
 * Sets color of the cell at (i,j) to c
 */
void Board::setColor(int i, int j, char c){
	/*
	if (c == '-'){
		colors_available->at(board->at(i)->at(j) - 'A')++;
		num_cells_free++;
	} else {
		colors_available->at(c - 'A')--;
		num_cells_free--;
	}
	*/
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



int Board::extraHelper()
{
	int score = 0;
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

/*
int Board::getNumCellsFree() const{
	return num_cells_free;
}
*/


int Board::getWeight(int i, int j) const{
	return weights->at(i)->at(j);
}



/*
int Board::getColorAvailable(int index) const{
	return colors_available->at(index);
}
*/

int Board::evalFunction(){
/*
	// for 2 sized palindromes
	int score2 = 0;
	// horizontal scan
	for (int i=0; i<board_size; i++)
	{
		for (int j=0; j<board_size-1; j++)
		{
			if (board->at(i)->at(j) == '-')
			{
				if (board->at(i)->at(j+1) == '-')
				{
					// scan in the colors used vector
					for (int k=0; k<board_size; k++)
					{
						if (colors_available->at(k) >= 2)
							score2++;
					}
				}
				else
				{
					// check if the same color is available
					if (colors_available->at(board->at(i)->at(j+1) - 'A') >= 1)
							score2++;
				}
			}
			else
			{
				if (board->at(i)->at(j+1) == '-')
				{
					// check if the same color is available
					if (colors_available->at(board->at(i)->at(j) - 'A') >= 1)
							score2++;
				}
			}
		}
	}

	// vertical scan
	for (int i=0; i<board_size-1; i++)
	{
		for (int j=0; j<board_size; j++)
		{
			if (board->at(j)->at(i) == '-')
			{
				if (board->at(j)->at(i+1) == '-')
				{
					// scan in the colors used vector
					for (int k=0; k<board_size; k++)
					{
						if (colors_available->at(k) >= 2)
							score2++;
					}
				}
				else
				{
					// check if the same color is available
					if (colors_available->at(board->at(j)->at(i+1) - 'A') >= 1)
							score2++;
				}
			}
			else
			{
				if (board->at(j)->at(i+1) == '-')
				{
					// check if the same color is available
					if (colors_available->at(board->at(j)->at(i) - 'A') >= 1)
							score2++;
				}
			}
		}
	}

	int score3 = 0;

	for (int i=0; i<board_size; i++){
		for (int j=1; j<board_size-1; j++){
			
			char left_color = board->at(i)->at(j-1);
			char right_color = board->at(i)->at(j+1);
			bool left_empty = (left_color == '-');
			bool right_empty = (right_color == '-');
			
			if (board->at(i)->at(j) == '-'){
				if (left_empty && right_empty){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 3)
							score3++;
					}

					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 2){
							for (int z=0; z<board_size; z++){
								if ((k != z) && (colors_available->at(z) >= 1))
									score3++;
							}
						}
					}
				} else if (left_empty){
					if (colors_available->at(right_color - 'A') >= 1){
						// scan in the colors used vector
						for (int k=0; k<board_size; k++){
							if ((k == (right_color - 'A')) && (colors_available->at(k) >= 2))
								score3++;
							else if (colors_available->at(k) >= 1)
								score3++;
						}
					}
				} else if (right_empty){
					if (colors_available->at(left_color - 'A') >= 1){
						// scan in the colors used vector
						for (int k=0; k<board_size; k++){
							if ((k == (left_color - 'A')) && (colors_available->at(k) >= 2))
								score3++;
							else if (colors_available->at(k) >= 1)
								score3++;
						}
					}
				} else if (left_color == right_color){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 1)
							score3++;
					}
				}
			} else {
				if (left_empty && right_empty){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 2)
							score3++;
					}
				} else if (left_empty){
					if (colors_available->at(right_color - 'A') >= 1)
						score3++;
				} else if (right_empty){
					if (colors_available->at(left_color - 'A') >= 1)
						score3++;
				}
			}
		}
	}

	for (int i=1; i<board_size-1; i++){
		for (int j=0; j<board_size; j++){
			
			char left_color = board->at(j)->at(i-1);
			char right_color = board->at(j)->at(i+1);
			bool left_empty = (left_color == '-');
			bool right_empty = (right_color == '-');
			
			if (board->at(j)->at(i) == '-'){
				if (left_empty && right_empty){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 3)
							score3++;
					}

					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 2){
							for (int z=0; z<board_size; z++){
								if ((k != z) && (colors_available->at(z) >= 1))
									score3++;
							}
						}
					}
				} else if (left_empty){
					if (colors_available->at(right_color - 'A') >= 1){
						// scan in the colors used vector
						for (int k=0; k<board_size; k++){
							if ((k == (right_color - 'A')) && (colors_available->at(k) >= 2))
								score3++;
							else if (colors_available->at(k) >= 1)
								score3++;
						}
					}
				} else if (right_empty){
					if (colors_available->at(left_color - 'A') >= 1){
						// scan in the colors used vector
						for (int k=0; k<board_size; k++){
							if ((k == (left_color - 'A')) && (colors_available->at(k) >= 2))
								score3++;
							else if (colors_available->at(k) >= 1)
								score3++;
						}
					}
				} else if (left_color == right_color){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 1)
							score3++;
					}
				}
			} else {
				if (left_empty && right_empty){
					// scan in the colors used vector
					for (int k=0; k<board_size; k++){
						if (colors_available->at(k) >= 2)
							score3++;
					}
				} else if (left_empty){
					if (colors_available->at(right_color - 'A') >= 1)
						score3++;
				} else if (right_empty){
					if (colors_available->at(left_color - 'A') >= 1)
						score3++;
				}
			}
		}
	}


	int score = calculateScore() + 2*score2 + 3* score3;
	return score;
*/
	return /*extraHelper() + */calculateScore();
}