#include "Address.hpp"


#include <cassert>


bool operator==(const Address& l, const Address& r)
{
    assert(l.Dimension() == r.Dimension());
    bool equal{true};
    std::vector<int>::const_iterator it1{l.address.cbegin()};
    std::vector<int>::const_iterator it2{r.address.cbegin()};
    for(; it1 != l.address.cend(); ++ it1, ++ it2)
    {
        if(*it1 != *it2)
        {
            equal = false;
            break;
        }
    }
    return equal;
}

bool operator!=(const Address& l, const Address& r)
{
    return !operator==(l, r);
}

std::ostream& operator<<(std::ostream& os, const Address& address)
{
    for(int c{0}; c < address.address.size(); )
    {
        os << address.address.at(c);
        if(++ c < address.address.size()) os << " ";
    }
    return os;
}
