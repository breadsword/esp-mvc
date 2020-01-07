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
auto get_callback(client_t &client, string host, Tree_Model_Node &root)
{
    return [&client, &host, &root](char *topic, uint8_t *payload, unsigned int payload_len) {
        auto sender = topic_sender(client, host, string{topic});

        // look up topic
        const auto node = root.search(sender.endpoint());

        if (!node)
        {
            // topic not found
            sender.status("Requested topic not found");
        }

        // check, if value is set
        if (payload_len > 0)
        {
            node->set_from(make_string(payload, payload_len));
        }
        else
        {
            node->notify_subscribers();
        }
    };
}

#endif //MVC_CALLBACK_IMPL_INCLUDED
