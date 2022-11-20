#include <bxx/app_timers.hpp>
#include <common/exec.hpp>
#include <bxx/script.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_tuple.hpp>

namespace bxx::app::timers
{
    void register_timer(double initial_time, std::function<double()> callback)
    {
        size_t event_idx = lib_register_event([=](python_tuple tuple) {
            return callback();
        });
        exec({
            fmt::format("def timer_{}_{}():",get_script_index(),event_idx),
            fmt::format("    return fire_event({},{}, ())",get_script_index(),event_idx),
            fmt::format("register_timer({}, timer_{}_{},{})", get_script_index(), get_script_index(), event_idx, initial_time)
        });
    }
}