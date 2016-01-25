#pragma once

#include "button_bindable_command.hpp"
#include <functional>

namespace gorc {

    class press_button_bindable_command : public button_bindable_command {
    private:
        std::function<void(time_delta)> on_press;
        bool prev_state = false;
        bool curr_state = false;

    public:
        press_button_bindable_command(std::function<void(time_delta)> &&on_press);

        virtual void set_pressed() override;
        virtual void update(time_delta) override;
    };

}
