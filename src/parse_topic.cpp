#include "parse_topic.hpp"

#include <mvc.hpp>

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

template <class Iterator>
string concat(Iterator begin, Iterator end)
{
    if (begin == end)
    {
        // empty string on empty range
        return "";
    }
    const auto el = *begin;
    const auto next = ++begin;
    if (next == end)
    {
        // just the last element, if it is one
        return el;
    }
    else
    {
        // this element separated from the rest
        return el + Tree_Model_Node::separator + concat(next, end);
    }
}

string parse_topic(const string &topic)
{
    auto tokens = tokenize(topic);

    if (tokens.size() >= 3)
    {

        return concat(tokens.begin() + 2, tokens.end());
    }
    else
    {
        // not found
        return "";
    }
}

// strip leading slashes from string passed in.
// Keep rest of string intact
string parse_endpoint(const string &endpoint)
{
    auto i = endpoint.begin();
    while (i != endpoint.end() && *i == '/')
    {
        ++i;
    }

    return string{i, endpoint.end()};
}
