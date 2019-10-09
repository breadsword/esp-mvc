#include <mvc.hpp>

#include <iostream>

static Value_Model<double> temperature{0.0, "temperature"};
static Value_Model<bool> thermos_output{false, "pin/thermos_output"}, output_enable{true, "pin/output_enable"};

static bool switch_value = false;

void thermos()
{
    const double target = 24.0;

    if (temperature.get() < target)
    {
        thermos_output.set(true);
    }
    else
    {
        thermos_output.set(false);
    }
}

void pin()
{
    switch_value = thermos_output.get() && output_enable.get();
}


int main(int, char **)
{
    temperature.register_change_callback(thermos);

    thermos_output.register_change_callback(pin);
    output_enable.register_change_callback(pin);

    std::cout << std::boolalpha << switch_value << " expected: " << "false" << std::endl;
    temperature.set(32.0);
    std::cout << std::boolalpha << switch_value << " expected: " << "false" << std::endl;
    temperature.set(22.0);
    std::cout << std::boolalpha << switch_value << " expected: " << "true" << std::endl;
    output_enable.set(false);
    std::cout << std::boolalpha << switch_value << " expected: " << "false" << std::endl;
    temperature.set(20.0);
    std::cout << std::boolalpha << switch_value << " expected: " << "false" << std::endl;
    output_enable.set(true);
    std::cout << std::boolalpha << switch_value << " expected: " << "true" << std::endl;

    return 0;
}
