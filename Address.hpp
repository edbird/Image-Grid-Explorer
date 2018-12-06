#ifndef ADDRESS_HPP
#define ADDRESS_HPP


#include <vector>
#include <ostream>
#include <cassert>


class Address
{

    public:

    Address(const int dimension)
    {
        address.resize(dimension);
        for(std::vector<int>::iterator it{address.begin()}; it != address.end(); ++ it)
        {
            *it = 0;
        }
    }


    virtual
    ~Address()
    {
    }

    int Dimension() const
    {
        return address.size();
    }

    int At(const int dimension) const
    {
        return address.at(dimension);
    }

    void Incr(const int dimension)
    {
        ++ address.at(dimension);
    }

    void Decr(const int dimension)
    {
        -- address.at(dimension);
    }

    void SetAddress(const std::vector<int>& value)
    {
        assert(value.size() == address.size());
        address = value;
    }

    friend
    bool operator==(const Address& l, const Address& r);

    friend
    bool operator!=(const Address& l, const Address& r);

    friend
    std::ostream& operator<<(std::ostream& os, const Address& address);



    protected:

    std::vector<int> address;

};

#endif // ADDRESS_HPP
