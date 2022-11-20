#pragma once

#include <functional>

namespace bxx::app::timers
{
    void register_timer(double initial_delay, std::function<double()> callback);
}