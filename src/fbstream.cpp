#include "fbstream.hpp"

size_t fixedbufstream::free_bytes() const
{
    return S;
}

string fixedbufstream::str()
{
    return string(&buffer[begin], &buffer[end]);
}

void fixedbufstream::putc(char c)
{
    buffer[end] = c;
    end++;
}

fixedbufstream &operator<<(fixedbufstream &stream, const std::string &data)
{
    for (const char c : data)
    {
        stream.putc(c);
    }

    return stream;
}
