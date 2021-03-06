#include "game.h"


// Brick methods ===============================================================

std::istream& Brick::read(std::istream& is) 
{
	/*
		The call below to the make_unique method points msp at a fresh copy of
		the map but then immediately points msp somewhere else. 
		So there was really no point in making the fresh copy of the map. 
		Perhaps instead of make_unique, it would be better to call a method 
		called make_null, which would point msp to 0 (we would have to 
		write this method of course). 
	*/
	msp.make_unique();
	
	std::string s; 
	if (std::getline(is, s))
	{
		// the line below calls the std::map<int, int> copy constructor
		// which takes an rvalue reference
		msp = new std::map<int, int>(str_to_map(s));
	}
	return is;
}

int Brick::get_value() const
{
	int val = 0;
	std::map<int, int>::const_iterator iter = msp->begin();
	const std::map<int, int>::const_iterator fin = msp->end();

	while (iter != fin)
	{
		val += iter->second;
		iter++;
	}
	return val;
}

// End of Brick methods ========================================================


// Board methods ===============================================================

Board::Board(int n, int m) : score(0), N(n), M(m)
{
	std::vector<char> es(N, 'e');
	// the 'e' signifies empty; 
	// 'o' will signify occupied

	for (int j = 0; j < M; ++j)
		filled_spaces.push_back(es);
}

void Board::clear()
{
	score = 0;

	std::vector<char> es(N, 'e');
	for (int j = 0; j < M; ++j)
		filled_spaces[j] = es;
}

std::vector< std::vector<int> > Board::play(
	const std::vector<Brick>& Bricks,
	std::vector<std::vector<Brick>::size_type> Brick_sorter(
		const std::vector<Brick>&))
{
	std::vector< std::vector<int> > record(M);

	std::vector<Brick>::size_type B = Bricks.size();
	std::vector<std::vector<Brick>::size_type> sorted_indices = Brick_sorter(
		Bricks);
	for (std::vector<Brick>::size_type b = 0; b < B; ++b)
	{
		const Brick& current = Bricks[sorted_indices[b]];
		const std::map<int, int>::const_iterator beg = current.begin();
		const std::map<int, int>::const_iterator fin = current.end();

		for (int j = 0; j < M; ++j)
		{
			std::map<int, int>::const_iterator it = beg;
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

// End of Board methods ========================================================


// Functions related to Brick class---------------------------------------------

std::vector<std::vector<Brick>::size_type> abs_sort_Brick_indices(
	const std::vector<Brick>& Bricks)
{
	typedef std::vector<Brick>::size_type st;

	st size = Bricks.size();
	std::vector< std::pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = std::make_pair(Bricks[i].get_value(), i);
	}
	std::sort(sorting_vec.rbegin(), sorting_vec.rend());

	std::vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

std::vector<std::vector<Brick>::size_type> twisted_abs_sort_Brick_indices(
	const std::vector<Brick>& Bricks)
{
	typedef std::vector<Brick>::size_type st;
	st size = Bricks.size();

	std::vector<st> ret = abs_sort_Brick_indices(Bricks);

	if (size > 1)
	{
		for (st i = 0; i < size - 1; i += 2)
			std::swap(ret[i], ret[i + 1]);
	}
	return ret;
}

std::vector<std::vector<Brick>::size_type> mean_sort_Brick_indices(
	const std::vector<Brick>& Bricks)
{
	typedef std::vector<Brick>::size_type st;

	st size = Bricks.size();
	std::vector< std::pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = std::make_pair(Bricks[i].get_mean(), i);
	}
	std::sort(sorting_vec.rbegin(), sorting_vec.rend());

	std::vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

std::vector<std::vector<Brick>::size_type> sesqui_sort_Brick_indices(
	const std::vector<Brick>& Bricks)
{
	typedef std::vector<Brick>::size_type st;

	st size = Bricks.size();
	std::vector< std::pair<int, st> > sorting_vec(size);

	for (st i = 0; i < size; ++i)
	{
		sorting_vec[i] = std::make_pair(
			(Bricks[i].get_mean() + Bricks[i].get_value()) / 2, i);
	}
	std::sort(sorting_vec.rbegin(), sorting_vec.rend());

	std::vector<st> ret(size);
	for (st i = 0; i < size; ++i)
		ret[i] = sorting_vec[i].second;

	return ret;
}

// End of functions related to Brick class--------------------------------------


// Utility functions------------------------------------------------------------

std::map<int, int> str_to_map(const std::string& str)
{
	std::vector<int> intvec;
	std::string::size_type i = 0;
	std::string::size_type arg_size = str.size();
	while (i != arg_size)
	{
		while (i != arg_size && isspace(str[i]))
			++i;
		std::string::size_type j = i;
		while (j != arg_size && !isspace(str[j]))
			++j;
		if (i != j)
		{
			intvec.push_back(stoi(str.substr(i, j - i)));
			i = j;
		}
	}
	
	std::map<int, int> ret;
	std::vector<int>::size_type sz = intvec.size();
	std::vector<int>::size_type k = 0; 
	while (k != sz)
	{
		ret[intvec[k]] = intvec[k + 1];
		k += 2;
	}

	return ret;
}

// construct a std::map<int, int> from an istream and return a pointer to it. 

// End of utility functions-----------------------------------------------------