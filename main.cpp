#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


#include "game.h"

int main() 
{
	// construct a std::vector<string> whose elements are the lines of input
	std::vector<std::string> input_strings;
	std::string s;
	while (getline(std::cin, s))
		input_strings.push_back(s);

	// Deduce the values of N, M, and B
	std::vector<int> general_info = str_to_intvec(input_strings[0]);
	int N = general_info[0]; // number of colours i.e. number of rows
	int M = general_info[1]; // number of columns
	int B = general_info[2]; // number of available bricks     

	std::vector<std::string>::size_type sz = input_strings.size();

	// Construct the std::vector of Bricks to be added to the Board
	std::vector<Brick> Bricks;
	for (std::vector<std::string>::size_type i = 1; i < sz; ++i)
	{
		Brick next_Brick(str_to_intvec(input_strings[i]));
		Bricks.push_back(next_Brick);
	}

	// We have defined several functions (all ending in `sort_Brick_indices`)
	// which will allow us to add Bricks to the Board in various greedy orders. 
	// Let us prepare to use these in a for loop.
	typedef  std::vector<std::vector<Brick>::size_type> (*sorting_ptr)(
		const std::vector<Brick>&);
	std::vector<sorting_ptr> sorting_ptrs = { &abs_sort_Brick_indices,  
		&mean_sort_Brick_indices,
		&sesqui_sort_Brick_indices, 
		&twisted_abs_sort_Brick_indices };


	// Using the various greedy strategies, add Bricks to the board and record 
	// the score and the placement of the Bricks, clearing the board before 
	// testing a new strategy. Place the paired results in results_vec.
	std::vector< std::pair< int, std::vector< std::vector<int> > > > results_vec;
	Board brd(N, M);
	std::vector< std::vector<int> > record;
	int scr;
	for (std::vector<sorting_ptr>::size_type i = 0; i < sorting_ptrs.size(); ++i)
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
	std::cout << scr << std::endl;
	for (int i = 0; i < M; ++i)
	{
		if (!record[i].empty())
			iter_print(record[i].begin(), record[i].end());
		std::cout << std::endl;
	}

	return 0;
}