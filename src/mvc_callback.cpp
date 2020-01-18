#include <PubSubClient.h>

#include "mvc_callback.hpp"
#include "mvc_callback_impl.hpp"
#include "parse_topic.hpp"

string make_string(const uint8_t *data, unsigned int len)
{
    return string{data, data + len};
}

template class generic_topic_sender<PubSubClient>;
template class mqtt_callback<PubSubClient>;
// template class mqtt_viewer<PubSubClient>;
