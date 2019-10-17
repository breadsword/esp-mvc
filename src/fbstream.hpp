#ifndef FBSTREAM_HPP_INCLUDED
#define FBSTREAM_HPP_INCLUDED

#include <cstddef>
#include <array>

#include "types.hpp"

class fixedbufstream
{
public:
    static constexpr auto S = 1024;

    size_t free_bytes() const;

    string str();

    void putc(char c);

private:
    std::array<char, S> buffer;
    size_t begin = 0;
    size_t end = 0;

};

template <typename T>
fixedbufstream& operator<<(fixedbufstream& stream, const T& data);
fixedbufstream& operator<<(fixedbufstream& stream, const std::string& data);

#endif //FBSTREAM_HPP_INCLUDED
