#include "parse_topic.hpp"

#include <algorithm>


string::const_iterator next_token(string::const_iterator start, string::const_iterator end)
{
    // skip one leading slash for token
    if (*start == '/' && start != end)
    {
        ++start;
    }

    const auto r = std::find(start, end, '/');
    return r;
}

string parse_topic(const string &topic)
{
    auto begin = std::begin(topic);

    auto end = next_token(begin, std::end(topic));
    if (end == std::end(topic))
    {
        return "";
        //throw (topic_parse_error{"no Host part found"});
    }

    const string host{begin, end};


    begin = end+1;
    end = next_token(begin, std::end(topic));
    if (end == std::end(topic))
    {
        return "";
        // throw (topic_parse_error{"no Dir part found"});
    }
    const string dir{begin, end};

    begin = end+1;
    if (begin == std::end(topic))
    {
        return "";
        // throw(topic_parse_error{"no topic fonud"});
    }

    return string{begin, std::end(topic)};
}

