#include "fbstream.hpp"


basic_fixedbufstream &operator<<(basic_fixedbufstream &stream, const std::string &data)
{
    for (const char c : data)
    {
        stream.putc(c);
    }

    return stream;
}

basic_fixedbufstream &operator<<(basic_fixedbufstream &stream, const char *data)
{
    size_t i = 0;
    while (data[i] != '\0')
    {
        stream.putc(data[i]);
        ++i;
    }
    return stream;
}
