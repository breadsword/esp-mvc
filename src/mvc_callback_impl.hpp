#ifndef MVC_CALLBACK_IMPL_INCLUDED
#define MVC_CALLBACK_IMPL_INCLUDED

#include <mvc_callback.hpp>
#include "parse_topic.hpp"

template <class client>
generic_topic_sender<client>::generic_topic_sender(client &_client, string host, string in_topic) : 
    m_client(_client), m_endpoint(parse_topic(in_topic)), m_host(host)
{
}

template <class client>
void generic_topic_sender<client>::output(const char *message)
{
    const string outtopic = string{"/"} + m_host + "/o/" + m_endpoint;
    m_client.publish(outtopic.c_str(), message);
}

#endif //MVC_CALLBACK_IMPL_INCLUDED
