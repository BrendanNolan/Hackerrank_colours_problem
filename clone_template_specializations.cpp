#include <vector>
#include "Smart_Ptr.h"

template <>
std::vector<char>* clone(const std::vector<char>* svc_ptr)
{
    return new std::vector<char>(*svc_ptr);
}