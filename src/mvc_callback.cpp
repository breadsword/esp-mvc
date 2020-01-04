#include <PubSubClient.h>

#include "mvc_callback.hpp"
#include "mvc_callback_impl.hpp"
#include <mvc.hpp>
#include "parse_topic.hpp"

string make_string(const uint8_t *data, unsigned int len)
{
    return string{data, data + len};
}

auto get_callback(PubSubClient &client, Tree_Model_Node &root)
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

