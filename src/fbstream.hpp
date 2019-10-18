#ifndef FBSTREAM_HPP_INCLUDED
#define FBSTREAM_HPP_INCLUDED

#include <cstddef>
#include <array>

#include "types.hpp"

class basic_fixedbufstream
{
public:
    virtual ~basic_fixedbufstream() = default;
    virtual void putc(char c) = 0;
};

template <size_t S>
class fixedbufstream : public basic_fixedbufstream
{
public:
    size_t free_bytes() const ;
    string str();
    void putc(char c) override;

    explicit operator bool() const
    {
        return !isfull;
    }


private:
    std::array<char, S> buffer;
    size_t begin = 0;
    size_t end = 0;
    bool isfull = false;
};

template<size_t S>
size_t fixedbufstream<S>::free_bytes() const
{
    return S - (end-begin);
}

template<size_t S>
string fixedbufstream<S>::str()
{
    return string(&buffer[begin], &buffer[end]);
}

template<size_t S>
void fixedbufstream<S>::putc(char c)
{
    if (end < buffer.size())
    {
        buffer[end] = c;
        end++;
    }
    else
    {
        isfull = true;
    }
}

//template <typename T>
//basic_fixedbufstream& operator<<(basic_fixedbufstream& stream, const T& data);

basic_fixedbufstream& operator<<(basic_fixedbufstream& stream, const char *data);
basic_fixedbufstream& operator<<(basic_fixedbufstream& stream, const std::string& data);
basic_fixedbufstream& operator<<(basic_fixedbufstream& stream, int data);
basic_fixedbufstream& operator<<(basic_fixedbufstream& stream, double data);


#endif //FBSTREAM_HPP_INCLUDED
