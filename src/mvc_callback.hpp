#ifndef MVC_CALLBACK_HPP_INCLUDED
#define MVC_CALLBACK_HPP_INCLUDED

#include <types.hpp>

// MQTT topics consist of three parts:
// [HOST]/[DIR]/[ENDPOINT]
//
// with HOST being a constant for the device
// DIR being one of i (input), o (output), e (error/status message)
// and ENDPOINT a path to the actual value in the status tree


//FIXME: someone needs to pass the host information to topic_sender
class PubSubClient;
class Tree_Model_Node;

auto get_callback(PubSubClient &client, Tree_Model_Node& root);

template <class client>
class generic_topic_sender
{
    public:
    generic_topic_sender(client &_client, string in_topic);
    
    void output(const char* message);
    void status(const char* message);
    string endpoint() const {return m_endpoint;}

    private:
    client &m_client;
    string m_endpoint;
    string m_host = "none";
};

typedef generic_topic_sender<PubSubClient> topic_sender;

#endif //MVC_CALLBACK_HPP_INCLUDED
