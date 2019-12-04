#ifndef PARSE_TOPIC_HPP
#define PARSE_TOPIC_HPP

#include <stdexcept>

#include "types.hpp"

string parse_topic(const string& topic);

class topic_parse_error : public std::logic_error
{
    using std::logic_error::logic_error;
};


#endif // PARSE_TOPIC_HPP
