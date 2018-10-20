#include <map>

#include "Smart_Ptr.h"

template <>
std::map<int, int>* clone(const std::map<int, int>* ptr) 
{ 
    return new std::map<int, int>(*ptr); 
} 