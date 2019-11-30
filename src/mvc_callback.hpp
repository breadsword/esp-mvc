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
//TODO: make topic_sender take different Clients
class Client;
class Tree_Model_Node;

auto get_callback(Client &client, Tree_Model_Node& root);
class topic_sender
{
    public:
    topic_sender(Client &_client, string in_topic);
    
    void output(const char* message);
    void status(const char* message);
    string endpoint() const;

    private:
    Client &client;
    string m_endpoint;
};


#endif //MVC_CALLBACK_HPP_INCLUDED
