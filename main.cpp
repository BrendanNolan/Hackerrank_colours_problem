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
	std::ifstream infile("input.txt");

	// Initialise integers N, M, B to store the number of rows on the board, 
	// the number of columns on the board, and 
	// the number of available Bricks
	// respectively.
	int N, M, B;
	infile >> N >> M >> B; 

	// make sure that the first call to Brick::read reads the second line of
	// infile, rather than whatever follows the first three entries on the first
	// line of infile
	char c;
	while (infile.get(c))
	{
		if (c == '\n')
			break;
	}

	std::vector<Brick> bricks;
	Brick brick;
	while(brick.read(infile))
		bricks.push_back(brick);

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
	std::vector< std::pair< int, std::vector< std::vector<int> > > > 
		results_vec;
	Board brd(N, M);
	std::vector< std::vector<int> > record;
	int scr;
	for (std::vector<sorting_ptr>::size_type i = 0; i < sorting_ptrs.size(); 
		 ++i) 
	{
		record = brd.play(bricks, sorting_ptrs[i]);
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
	std::cout << scr << '\n';
	for (int i = 0; i < M; ++i)
	{
		if (!record[i].empty())
		{
			iter_print(record[i].begin(), record[i].end());
			std::cout << '\n';
		}
	}
	
	return 0;
}