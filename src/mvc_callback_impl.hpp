#ifndef MVC_CALLBACK_IMPL_INCLUDED
#define MVC_CALLBACK_IMPL_INCLUDED

#include <mvc_callback.hpp>
#include <mvc.hpp>
#include "parse_topic.hpp"

template <class client>
generic_topic_sender<client>::generic_topic_sender(client &_client, string host, string in_topic) : m_client(_client), m_endpoint(parse_topic(in_topic)), m_host(host)
{
}

template <class client>
void generic_topic_sender<client>::status(const char *message)
{
    const string outtopic = string{"/"} + m_host + "/status/" + m_endpoint;
    m_client.publish(outtopic.c_str(), message);
}

string make_string(const uint8_t *data, unsigned int len);

template <class client_t>
void mqtt_callback<client_t>::operator()(const char *topic, const uint8_t *payload, unsigned int payload_len)
{
    auto sender = create_sender(string{topic});
    auto node = lookup_node(sender.endpoint());

    if (!node)
    {
        sender.status("Topic not found");
        return;
    }

    if (payload_len > 0)
    {
        set_value(*node, make_string(payload, payload_len));
    }
    else
    {
        notify(*node);
    }
}

template <class client_t>
generic_topic_sender<client_t> mqtt_callback<client_t>::create_sender(string topic)
{
    return generic_topic_sender<client_t>(client, host, topic);
}

template <class client_t>
Tree_Model_Node *mqtt_callback<client_t>::lookup_node(string endpoint)
{
    // look up topic
    return root.search(endpoint);
}

template <class client_t>
void mqtt_callback<client_t>::set_value(Tree_Model_Node &node, string new_val)
{
    node.set_from(new_val);
}

template <class client_t>
void mqtt_callback<client_t>::notify(Tree_Model_Node &node)
{
    node.notify_subscribers();
}

#endif //MVC_CALLBACK_IMPL_INCLUDED
