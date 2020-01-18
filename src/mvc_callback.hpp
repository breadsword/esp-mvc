#ifndef MVC_CALLBACK_HPP_INCLUDED
#define MVC_CALLBACK_HPP_INCLUDED

#include <types.hpp>
#include <named_type.hpp>
#include <mvc.hpp>

// MQTT topics consist of three parts:
// [HOST]/[DIR]/[ENDPOINT]
//
// with HOST being a constant for the device
// DIR being one of i (input), o (output), e (error/status message)
// and ENDPOINT a path to the actual value in the status tree

class PubSubClient;
class Tree_Model_Node;

using full_topic_t = NamedType<string, struct FullTopic>;
using endpoint_t = NamedType<string, struct EndpointTopic>;

template <class client>
class generic_topic_sender
{
public:
    generic_topic_sender(client &_client, string host, full_topic_t in_topic);
    generic_topic_sender(client &_client, string host, endpoint_t in_topic);

    void status(const char *message);
    void output(const char *message);
    string endpoint() const { return m_endpoint; }

private:
    client &m_client;
    string m_endpoint;
    string m_host = "none";
};

template <class client_t>
class mqtt_callback
{
public:
    mqtt_callback(client_t &_client, string _host, Tree_Model_Node &_root) : client(_client), host(_host), root(_root)
    {
    }

    // separate functions to allow testing
    generic_topic_sender<client_t> create_sender(string topic);
    Tree_Model_Node *lookup_node(string endpoint);
    void set_value(Tree_Model_Node &node, string new_val);
    void notify(Tree_Model_Node &node);

    void
    operator()(const char *topic, const uint8_t *payload, unsigned int payload_len);

private:
    client_t &client;
    string host;
    Tree_Model_Node &root;
};

template <class client_t>
auto mqtt_viewer(string host, client_t &client)
{
    return [&host, &client](const Model_Node &node) {
        string topic, value;
        std::tie(topic, value) = node.notification();

        auto sender = generic_topic_sender<client_t>(client, host, endpoint_t(topic));
        client.publish((string{"/"} + host + "/o" + topic).c_str(), value.c_str());
    };
}

#endif //MVC_CALLBACK_HPP_INCLUDED
