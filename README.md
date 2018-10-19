# Hackerrank_colours_problem
Playing a board game with tokens and bricks of different colours. Full 
explanation of the rules coming soon!

The bricks are represented by the `Brick` class, which is implemented as a 
(`Smart_Ptr` to) a `std::map<int, int>`, where the key is the colour of a token
on the brick and the value is the points available for that token. 

Just for fun, rather than using `std::shared_ptr`, I have used my own 
`Smart_Ptr` class. The use of pointers from my `Smart_ptr` class cuts down a 
lot of copying that happens when `std::vector`s of Bricks are sorted with 
`std::sort` (which happens a lot in my program). Because Smart_Ptr shares memory 
and bricks are implemented as pointers to `std::map`s, the main function's calls 
to `std::sort` involve copying only `Smart_Ptr`s rather than entire `std::map`s. 