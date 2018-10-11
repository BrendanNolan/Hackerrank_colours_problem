#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


// Begin utility functions-----------------------------------------------------

// convert a string to a vector<int>
vector<int> str_to_intvec(const string&);

// print the contents between two iterators
template <typename T>
void iter_print(T, T);

// End utility functions-------------------------------------------------------


// Brick class-----------------------------------------------------------------

// A Brick is implemented as a map<int, int>. The key is the colour of a 
// coloured token in the Brick, the associated value is the number of points 
// attached to the coloured token. 
class Brick {
public:
	// constructors
	Brick() {}
	Brick(const vector<int>&);

	// getter methods
	int get_value() const;
	int get_mean() const { return get_value() / size(); }
	const map<int, int>& get_data() const { return data; }

	// other methods
	map<int, int>::size_type size() const { return data.size(); }
	map<int, int>::const_iterator begin() const { return data.begin(); }
	map<int, int>::const_iterator end() const { return data.end(); }
private:
	map<int, int> data;
};

// End of Brick class----------------------------------------------------------


// Functions related to Brick class--------------------------------------------

// The following functions ending in `sort_Brick_indices` are designed to be 
// called by the Board::play method (see the Board class below).
// They will allow Board::play to try to add Bricks to the Board in various 
// (greedy) orders.

// Take a const reference to a vector<Brick> and return a vector of the ranks
// of the Bricks where Bricks are ranked in decreasing order of the sum of the 
// values of their coloured tokens (so that the Brick which is worth the most 
// points has rank 0). This ranking does not handle ties; for example Bricks 
// ranked n and n + 1 may have the same points value. This non-handling of
// ties is desired in order that we do not attempt to add the same Brick twice.
vector<vector<Brick>::size_type> abs_sort_Brick_indices(const vector<Brick>&);

// Return the same vector as abs_sort_Brick_indices, except with the 1st and 
// 2nd elements swapped, the third and fourth elements swapped etc.
vector<vector<Brick>::size_type> twisted_abs_sort_Brick_indices(
	const vector<Brick>&);

// The same as abs_sort_Brick_indices except Bricks are ranked according to the
// mean value rather than the total value of their coloured tokens.
vector<vector<Brick>::size_type> mean_sort_Brick_indices(const vector<Brick>&);

// The same as abs_sort_Brick_indices except Bricks are ranked according to the 
// mean of the mean and total values of their coloured tokens.
vector<vector<Brick>::size_type> sesqui_sort_Brick_indices(
	const vector<Brick>&);

// End of functions related to Brick class-------------------------------------


// Board class-----------------------------------------------------------------

// A Board is implemented as a vector< vector<char> > whose jth element encodes
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

	// Take a vector of Bricks and try to add them to the baord in an order 
	// determined by Brick_sorter. Starting with the leftmost column and
	// moving rightward, if there is room for a Brick, place that Brick on 
	// the Board by marking as 'o' (occupied) the spaces on the board covered 
	// by the Brick's tokens and adding the Brick's value (the sum of the 
	// values of its coloured tokens) to the score of the board.
	// The returned vector< vector<int> > records in its jth element the Bricks 
	// which were added to column j of the board (for 0 <= j < M).
	vector< vector<int> > play(
		const vector<Brick>& Bricks,
		vector<vector<Brick>::size_type> Brick_sorter(const vector<Brick>&));
	
	// get score
	int get_score() const { return score; }
private:
	int score;
	int N;
	int M;

	vector< vector<char> > filled_spaces;
	// filled_spaces[j][i] == 'e'  
	// means that the ith colour on the jth column 
	// of the board is empty i.e. not covered by a Brick.
	// Note that this reverses the usual "matrix notation".
};

// End of Board class----------------------------------------------------------



// main function---------------------------------------------------------------

int main() {
	// construct a vector<string> whose elements are the lines of input
	vector<string> input_strings;
	string s;
	while (getline(cin, s))
		input_strings.push_back(s);

	// Deduce the values of N, M, and B
	vector<int> general_info = str_to_intvec(input_strings[0]);
	int N = general_info[0]; // number of colours i.e. number of rows
	int M = general_info[1]; // number of columns
	int B = general_info[2]; // number of available bricks     

	vector<string>::size_type sz = input_strings.size();

	// Construct the vector of Bricks to be added to the Board
	vector<Brick> Bricks;
	for (vector<string>::size_type i = 1; i < sz; ++i)
	{
		Brick next_Brick(str_to_intvec(input_strings[i]));
		Bricks.push_back(next_Brick);
	}

	// We have defined several functions (all ending in `sort_Brick_indices`)
	// which will allow us to add Bricks to the Board in various greedy orders. 
	// Let us prepare to use these in a for loop.
	typedef  vector<vector<Brick>::size_type> (*sorting_ptr)(
		const vector<Brick>&);
	vector<sorting_ptr> sorting_ptrs = { &abs_sort_Brick_indices,  
		&mean_sort_Brick_indices,
		&sesqui_sort_Brick_indices, 
		&twisted_abs_sort_Brick_indices };


	// Using the various greedy strategies, add Bricks to the board and record 
	// the score and the placement of the Bricks, clearing the board before 
	// testing a new strategy. Place the paired results in results_vec.
	vector< pair< int, vector< vector<int> > > > results_vec;
	Board brd(N, M);
	vector< vector<int> > record;
	int scr;
	for (vector<sorting_ptr>::size_type i = 0; i < sorting_ptrs.size(); ++i)
	{
		record = brd.play(Bricks, sorting_ptrs[i]);
		scr = brd.get_score();
		results_vec.push_back(make_pair(scr, record));
		brd.clear();
	}

	// Sort results_vec so that the pair in position 0 is the pair 
	// consisting of the score of the best strategy and the best strategy 
	// itself.
	sort(results_vec.rbegin(), results_vec.rend());
	scr = results_vec[0].first;
	record = results_vec[0].second;
	
	// Print the score of the best strategy followed by the best strategy 
	// itself
	cout << scr << endl;
	for (int i = 0; i < M; ++i)
	{
		if (!record[i].empty())
			iter_print(record[i].begin(), record[i].end());
		cout << endl;
	}

	return 0;
}

// End of main function--------------------------------------------------------



// Definitions of functions prototyped above main function---------------------


// Utility functions-----------------------------------------------------------

vector<int> str_to_intvec(const string& str)
{
	vector<int> intvec;
	string::size_type i = 0;
	string::size_type arg_size = str.size();
	while (i != arg_size)
	{
		while (i != arg_size && isspace(str[i]))
			++i;
		string::size_type j = i;
		while (j != arg_size && !isspace(str[j]))
			++j;
		if (i != j)
		{
			intvec.push_back(stoi(str.substr(i, j - i)));
			i = j;
		}
	}
	return intvec;
}

template <typename T>
void iter_print(T beg, T fin)
{
	while (beg != fin)
	{
		cout << *beg << " ";
		++beg;
	}
}

// End of utility functions----------------------------------------------------


// Brick methods---------------------------------------------------------------

Brick::Brick(const vector<int>& ints) // constructor
{
	vector<int>::size_type sz = ints.size();
	vector<int>::size_type i = 0;
	while (i != sz)
	{
		data[ints[i]] = ints[i + 1];
		i += 2;
	}
}

int Brick::get_value() const
{
	int val = 0;
	map<int, int>::const_iterator iter = data.begin();
	const map<int, int>::const_iterator fin = data.end();

	while (iter != fin)
	{
		val += iter->second;
		iter++;
	}
	return val;
}

// End of Brick methods--------------------------------------------------------


// Functions related to Brick class--------------------------------------------

vector<vector<Brick>::size_type> abs_sort_Brick_indices(
	const vector<Brick>& Bricks)
{
	typedef vector<Brick>::size_type st;

	st size = Bricks.size();
	vector< pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = make_pair(Bricks[i].get_value(), i);
	}
	sort(sorting_vec.rbegin(), sorting_vec.rend());

	vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

vector<vector<Brick>::size_type> twisted_abs_sort_Brick_indices(
	const vector<Brick>& Bricks)
{
	typedef vector<Brick>::size_type st;
	st size = Bricks.size();

	vector<st> ret = abs_sort_Brick_indices(Bricks);

	if (size > 1)
	{
		for (st i = 0; i < size - 1; i += 2)
			swap(ret[i], ret[i + 1]);
	}
	return ret;
}

vector<vector<Brick>::size_type> mean_sort_Brick_indices(
	const vector<Brick>& Bricks)
{
	typedef vector<Brick>::size_type st;

	st size = Bricks.size();
	vector< pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = make_pair(Bricks[i].get_mean(), i);
	}
	sort(sorting_vec.rbegin(), sorting_vec.rend());

	vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

vector<vector<Brick>::size_type> sesqui_sort_Brick_indices(
	const vector<Brick>& Bricks)
{
	typedef vector<Brick>::size_type st;

	st size = Bricks.size();
	vector< pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = make_pair(
			(Bricks[i].get_mean() + Bricks[i].get_value()) / 2, i);
	}
	sort(sorting_vec.rbegin(), sorting_vec.rend());

	vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

// End of functions related to Brick class-------------------------------------


// Board methods---------------------------------------------------------------

Board::Board(int n, int m) : score(0), N(n), M(m)
{
	vector<char> es(N, 'e');
	// the 'e' signifies empty; 
	// 'o' will signify occupied

	for (int j = 0; j < M; ++j)
		filled_spaces.push_back(es);
}

void Board::clear()
{
	score = 0;

	vector<char> es(N, 'e');
	for (int j = 0; j < M; ++j)
		filled_spaces[j] = es;
}

vector< vector<int> > Board::play(
	const vector<Brick>& Bricks,
	vector<vector<Brick>::size_type> Brick_sorter(const vector<Brick>&))
{
	vector< vector<int> > record(M);

	vector<Brick>::size_type B = Bricks.size();
	vector<vector<Brick>::size_type> sorted_indices = Brick_sorter(Bricks);
	for (vector<Brick>::size_type b = 0; b < B; ++b)
	{
		const Brick& current = Bricks[sorted_indices[b]];
		const map<int, int>::const_iterator beg = current.begin();
		const map<int, int>::const_iterator fin = current.end();

		for (int j = 0; j < M; ++j)
		{
			map<int, int>::const_iterator it = beg;
			while (it != fin && filled_spaces[j][it->first] == 'e')
				++it;
			if (it == fin) // true if column j has room for `current`
			{
				it = beg;
				while (it != fin)
				{
					filled_spaces[j][it->first] = 'o';
					score += it->second;
					++it;
				}
				record[j].push_back(sorted_indices[b]);
				break;
			}
		}
	}

	return record;
}

// End of Board methods--------------------------------------------------------


// End of definitions prototyped above main function---------------------------