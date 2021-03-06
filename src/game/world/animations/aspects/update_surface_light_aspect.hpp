#pragma once

#include "game/world/animations/components/surface_light.hpp"
#include "game/world/level_model.hpp"
#include "ecs/inner_join_aspect.hpp"

namespace gorc {
namespace game {
namespace world {
namespace animations {
namespace aspects {

class update_surface_light_aspect : public inner_join_aspect<thing_id, components::surface_light> {
private:
    level_model& model;

public:
    update_surface_light_aspect(entity_component_system<thing_id>& cs, level_model& model);

    virtual void update(time_delta t, thing_id id, components::surface_light& anim) override;
};

}
}
}
}
}
