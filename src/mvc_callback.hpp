#ifndef MVC_CALLBACK_HPP_INCLUDED
#define MVC_CALLBACK_HPP_INCLUDED

#include <types.hpp>
#include <named_type.hpp>
#include <mvc.hpp>
#include <gsl-lite/gsl-lite.hpp>

// MQTT topics consist of three parts:
// [HOST]/[DIR]/[ENDPOINT]
//
// with HOST being a constant for the device
// DIR being one of i (input), o (output), e (error/status message)
// and ENDPOINT a path to the actual value in the status tree

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

    /// The way PubSubClient calls this callback
    void
    operator()(const char *topic, const uint8_t *payload, unsigned int payload_len);
    /// The way mikado calls this callback
    void
    operator()(gsl::span<const unsigned char> topic, gsl::span<const unsigned char> payload);
    /// The way to implement this callback
    void
    operator()(string topic, string payload);

private:
    client_t &client;
    string host;
    Tree_Model_Node &root;
};

template <class client_t>
class mqtt_viewer
{
public:
    mqtt_viewer(string _host, client_t &_client) : host{_host}, client(_client)
    {
    }

    void operator()(const Model_Node &node)
    {
        string topic, value;
        std::tie(topic, value) = node.notification();

        auto sender = generic_topic_sender<client_t>{client, host, endpoint_t{topic}};
        sender.output(value.c_str());
    }

private:
    string host;
    client_t &client;
};

#endif //MVC_CALLBACK_HPP_INCLUDED
