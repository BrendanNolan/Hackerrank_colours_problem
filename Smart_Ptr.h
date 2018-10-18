#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <cstdlib>
#include <stdexcept>

#include "ctrl_block.h"

// Begin definitions of global utility functions -------------------------------

// clone is never called on an unbound pointer, so in the definition of clone, 
// we do not need to check that p is bound. 
template <typename T>
T* clone(const T* p)
{
    return p->clone();    
}

// End definitions of global utility functions ---------------------------------



// Begin Smart_Ptr class =======================================================

// Begin Smart_Ptr class main body ---------------------------------------------
 
template <typename T>
class Smart_Ptr 
{
public: 
    // The default constructor and the constructor taking a raw pointer 
    // need not worry about the ref_counter member - this is default initialized
    // and that default initalization does what we want - it points ref_counter
    // to a new control block with value 1.
    Smart_Ptr(): raw_ptr(0) {}
    Smart_Ptr(T* other_raw): raw_ptr(other_raw) {}
    // The compiler-defined shallow copy is what we want, due to the design of 
    // the ctrl_block class, which updates counts automatically. However, in 
    // order to obey the rule of three, we define this shallow copy constructor. 
    Smart_Ptr(const Smart_Ptr&);

    // Again we do not need to worry about managing the control block memory or
    // its count;
    // when the Smart_Ptr destructor is finished running, the destructor for 
    // ref_counter will then be run; this will update the count and 
    // conditionally free the memory where ref_counter (or rather its raw_s_ptr 
    // member) points. 
    ~Smart_Ptr() { if (ref_count() == 1) delete raw_ptr; }


    Smart_Ptr& operator=(const Smart_Ptr&);
    T& operator*() const;
    T* operator->() const;
    operator bool() const { return raw_ptr; }


    std::size_t ref_count() const { return ref_counter.val(); }
    void make_unique();

private:
    T* raw_ptr;
    ctrl_block ref_counter;    
};

// End Smart_Ptr class main body -----------------------------------------------

// Begin definitions of Smart_Ptr class methods --------------------------------

template <typename T>
Smart_Ptr<T>::Smart_Ptr(const Smart_Ptr& o): raw_ptr(o.raw_ptr), 
    ref_counter(o.ref_counter) 
{}


template <typename T>
Smart_Ptr<T>& Smart_Ptr<T>::operator=(const Smart_Ptr& rhs)
{
    if (ref_counter != rhs.ref_counter)
    {
        if (ref_count() == 1)
            delete raw_ptr;

        raw_ptr = rhs.raw_ptr;
        ref_counter = rhs.ref_counter; // this updates the counts automatically
    }
    
    return *this;
}


template <typename T>
T& Smart_Ptr<T>::operator*() const 
{
    if (raw_ptr)
        return *raw_ptr;
    throw std::runtime_error("cannot dereference unbound Smart_Ptr");
}

template <typename T>
T* Smart_Ptr<T>::operator->() const 
{
    if (raw_ptr)
        return raw_ptr;
    throw std::runtime_error("unbound Smart_Ptr");
}

template <typename T>
void Smart_Ptr<T>::make_unique()
{
    if (ref_count() != 1)
    { 
        raw_ptr = (raw_ptr ? clone(raw_ptr) : 0);

        // construct a ctrl_block pointing at 1
        ctrl_block temp(1);
        // assign temp to ref_counter
        ref_counter = temp;
        /* 
            Now ref_counter points at 2 but when make_unique returns, temp will 
            die and the ctrl_block destructor will decrement the pointee of 
            ref_counter back to 1.
        */
    }
}

// End definitions of Smart_Ptr class methods ----------------------------------

// End Smart_Ptr class =========================================================



#endif