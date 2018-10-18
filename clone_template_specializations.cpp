#include "Smart_Ptr.h"
#include "Vec.h"

template <>
Vec<char>* clone(const Vec<char>* svc_ptr)
{
    return new Vec<char>(*svc_ptr);
}