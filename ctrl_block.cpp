#include "ctrl_block.h"

// Begin definitions of ctrl_block friends -------------------------------------

bool operator==(const ctrl_block& a, const ctrl_block& b)
{
    return a.raw_s_ptr == b.raw_s_ptr;
}

bool operator!=(const ctrl_block& a, const ctrl_block& b)
{
    return !(a == b);
}

// End definitions of ctrl_block friends ---------------------------------------

// Begin definitions of ctrl_block methods -------------------------------------

ctrl_block::~ctrl_block() 
{
    if (--*raw_s_ptr == 0)
        delete raw_s_ptr;
}

ctrl_block& ctrl_block::operator=(const ctrl_block& rhs)
{
    if (*this != rhs)
    {
        if (--*raw_s_ptr == 0)
            delete raw_s_ptr;
        raw_s_ptr = rhs.raw_s_ptr;
        ++*raw_s_ptr;
    }
    return *this;
}

// End definitions of ctrl_block methods ---------------------------------------