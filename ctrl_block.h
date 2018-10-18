#ifndef CTRL_BLOCK_H    
#define CTRL_BLOCK_H

/*
This class should have no pointer interface. To the user, it should seem like a 
self-updating std::size_t. 
*/

#include <cstdlib>
#include <stdexcept>

// Begin ctrl_block class ======================================================

// Begin ctrl_block class main body --------------------------------------------

class ctrl_block 
{
friend bool operator==(const ctrl_block&, const ctrl_block&);
friend bool operator!=(const ctrl_block&, const ctrl_block&);

public:
    ctrl_block(): raw_s_ptr(new std::size_t(1)) {}
    ctrl_block(std::size_t n): raw_s_ptr(new std::size_t(n)) {}
    ctrl_block(const ctrl_block& o): raw_s_ptr(o.raw_s_ptr) { ++*raw_s_ptr; }
    ~ctrl_block();

    ctrl_block& operator=(const ctrl_block&);

    std::size_t val() const { return *raw_s_ptr; }

    operator bool() const { return raw_s_ptr; }

private:
    std::size_t* raw_s_ptr;
};

// End ctrl_block class main body ----------------------------------------------

// End ctrl_block class ========================================================

#endif