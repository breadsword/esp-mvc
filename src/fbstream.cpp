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

template<typename T, size_t S>
size_t generate_digits(T data, std::array<char, S>& res)
{
    constexpr auto base = 10;
    size_t i = 0;
    while(data>0 && i<S)
    {
        const uint8_t digit = (data % base);
        const char c = '0' + static_cast<char>(digit);
        res[i] = c;
        ++i;
        data /= base;
    }

    return i;
}

template <size_t S>
void put_reverse(size_t& rbegin, std::array<char, S>& res, basic_fixedbufstream &stream, size_t rend = 0)
{
    while(rbegin>rend)
    {
        --rbegin;
        stream.putc(res[rbegin]);
    }
}

basic_fixedbufstream &operator<<(basic_fixedbufstream &stream, int data)
{
    std::array<char, 16> res;

    if (data < 0)
    {
        stream.putc('-');
        data = -data;
    }

    size_t i = generate_digits(data, res);

    put_reverse(i, res, stream);
    return stream;
}

double mypow(double base, uint8_t exponent)
{
    auto res = 1.0;
    for (auto i=0; i<exponent; ++i)
    {
        res *= base;
    }

    return res;
}

basic_fixedbufstream &operator<<(basic_fixedbufstream &stream, double data)
{
    std::array<char, 16> res;

    if (data < 0)
    {
        stream.putc('-');
        data = -data;
    }

    constexpr auto n_digits_after_comma = 2;

    constexpr auto base = 10;
    data = mypow(base, n_digits_after_comma) * data + 0.5;

    size_t i = generate_digits(static_cast<int>(data), res);

    put_reverse(i, res, stream, n_digits_after_comma);
    stream.putc('.');
    put_reverse(i, res, stream);

    return stream;
}
