#include <algorithm>
#include <map>
#include <vector>

// Begin utility functions-----------------------------------------------------

// convert a string to a std::vector<int>
std::vector<int> str_to_intvec(const std::string&);

// print the contents between two iterators
template <typename T>
void iter_print(T, T);

// End utility functions-------------------------------------------------------

// Brick class-----------------------------------------------------------------

// A Brick is implemented as a std::map<int, int>. The key is the colour of a 
// coloured token in the Brick, the associated value is the number of points 
// attached to the coloured token. 
class Brick {
public:
	// constructors
	Brick() {}
	Brick(const std::vector<int>&);

	// getter methods
	int get_value() const;
	int get_mean() const { return get_value() / size(); }
	const std::map<int, int>& get_data() const { return data; }

	// other methods
	std::map<int, int>::size_type size() const { return data.size(); }
	std::map<int, int>::const_iterator begin() const { return data.begin(); }
	std::map<int, int>::const_iterator end() const { return data.end(); }
private:
	std::map<int, int> data;
};

// End of Brick class----------------------------------------------------------

// Functions related to Brick class--------------------------------------------

// The following functions ending in `sort_Brick_indices` are designed to be 
// called by the Board::play method (see the Board class below).
// They will allow Board::play to try to add Bricks to the Board in various 
// (greedy) orders.

// Take a const reference to a std::vector<Brick> and return a std::vector of the ranks
// of the Bricks where Bricks are ranked in decreasing order of the sum of the 
// values of their coloured tokens (so that the Brick which is worth the most 
// points has rank 0). This ranking does not handle ties; for example Bricks 
// ranked n and n + 1 may have the same points value. This non-handling of
// ties is desired in order that we do not attempt to add the same Brick twice.
std::vector<std::vector<Brick>::size_type> abs_sort_Brick_indices(const std::vector<Brick>&);

// Return the same std::vector as abs_sort_Brick_indices, except with the 1st and 
// 2nd elements swapped, the third and fourth elements swapped etc.
std::vector<std::vector<Brick>::size_type> twisted_abs_sort_Brick_indices(
	const std::vector<Brick>&);

// The same as abs_sort_Brick_indices except Bricks are ranked according to the
// mean value rather than the total value of their coloured tokens.
std::vector<std::vector<Brick>::size_type> mean_sort_Brick_indices(const std::vector<Brick>&);

// The same as abs_sort_Brick_indices except Bricks are ranked according to the 
// mean of the mean and total values of their coloured tokens.
std::vector<std::vector<Brick>::size_type> sesqui_sort_Brick_indices(
	const std::vector<Brick>&);

// End of functions related to Brick class-------------------------------------


// Board class-----------------------------------------------------------------

// A Board is implemented as a std::vector< std::vector<char> > whose jth element encodes
// exactly which spaces on the jth column of the board are occupied ('o'
// signifies "occupied" and 'e' signifies "empty").

class Board {
public:
	// constructors
	Board() {}
	explicit Board(int, int); 

	// methods

	// Clear the board, setting its score back to zero and 
	// making all of its spaces available
	void clear();

	// Take a std::vector of Bricks and try to add them to the baord in an order 
	// determined by Brick_sorter. Starting with the leftmost column and
	// moving rightward, if there is room for a Brick, place that Brick on 
	// the Board by marking as 'o' (occupied) the spaces on the board covered 
	// by the Brick's tokens and adding the Brick's value (the sum of the 
	// values of its coloured tokens) to the score of the board.
	// The returned std::vector< std::vector<int> > records in its jth element the Bricks 
	// which were added to column j of the board (for 0 <= j < M).
	std::vector< std::vector<int> > play(
		const std::vector<Brick>& Bricks,
		std::vector<std::vector<Brick>::size_type> Brick_sorter(const std::vector<Brick>&));
	
	// get score
	int get_score() const { return score; }
private:
	int score;
	int N;
	int M;

	std::vector< std::vector<char> > filled_spaces;
	// filled_spaces[j][i] == 'e'  
	// means that the ith colour on the jth column 
	// of the board is empty i.e. not covered by a Brick.
	// Note that this reverses the usual "matrix notation".
};

// End of Board class----------------------------------------------------------