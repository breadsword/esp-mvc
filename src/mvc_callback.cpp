#include <PubSubClient.h>

#include "mvc_callback.hpp"

#include <mvc.hpp>

string make_string(const uint8_t *data, unsigned int len)
{
    return string{data, data+len};
}


auto get_callback(Client &client, Tree_Model_Node& root)
{
    return [&client, &root](char *topic, uint8_t *payload, unsigned int payload_len) {
        auto sender = topic_sender(client, string{topic});

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

string parse_topic(const string& topic)
{
    return string{topic};
}

topic_sender::topic_sender(Client &_client, string in_topic) : client(_client), m_endpoint(parse_topic(in_topic))
{}
