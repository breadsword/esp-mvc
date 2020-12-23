#include <mvc.hpp>
#include <mvc_impl.hpp>

#include <iostream>

static Tree_Model_Node root_node{"brotkiste", nullptr}, pin_node{"pin", &root_node};
static Value_Model<float> temperature{0.0, "temperature", &root_node};
static Value_Model<bool> thermos_output{false, "thermos_output", &pin_node}, output_enable{true, "output_enable", &pin_node};

static Value_Model<bool> switch_value{false, "switch", &pin_node};


void thermos(const Model_Node&)
{
    const auto target = 24.0f;

    if (temperature.get() < target)
    {
        thermos_output.set(true);
    }
    else
    {
        thermos_output.set(false);
    }
}

void pin(const Model_Node&)
{
    switch_value.set(thermos_output.get() && output_enable.get());
}

void reporting_viewer(const Model_Node& m)
{
    string t, v;
    std::tie(t, v) = m.notification();
    std::cout << "Model change: " << t << ":" << v << std::endl;
}


int main(int, char **)
{
    root_node.on_tree_execute([](Tree_Model_Node& n){n.register_change_callback(reporting_viewer);});

    temperature.register_change_callback(thermos);
    thermos_output.register_change_callback(pin);
    output_enable.register_change_callback(pin);

    std::cout << std::boolalpha << switch_value.get() << " expected: " << "false" << std::endl;
    temperature.set(32.0);
    std::cout << std::boolalpha << switch_value.get() << " expected: " << "false" << std::endl;
    temperature.set(22.4F);
    std::cout << std::boolalpha << switch_value.get() << " expected: " << "true" << std::endl;
    output_enable.set(false);
    std::cout << std::boolalpha << switch_value.get() << " expected: " << "false" << std::endl;
    temperature.set(20.0);
    std::cout << std::boolalpha << switch_value.get() << " expected: " << "false" << std::endl;
    output_enable.set(true);
    std::cout << std::boolalpha << switch_value.get() << " expected: " << "true" << std::endl;

    return 0;
}
