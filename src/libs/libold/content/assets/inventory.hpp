#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include "log/log.hpp"
#include "math/vector.hpp"
#include "content/asset.hpp"
#include "libold/content/loaders/inventory_loader.hpp"
#include "inventory_bin.hpp"

namespace gorc {
namespace content {
namespace assets {

class inventory : public asset {
public:
    static fourcc const type;

    std::unordered_map<std::string, int> bin_map;
    std::map<int, inventory_bin> bins;

    inline const inventory_bin& get_bin(const std::string& name) const {
        auto it = bin_map.find(name);
        if(it != bin_map.end()) {
            auto jt = bins.find(it->second);
            if(jt != bins.end()) {
                return jt->second;
            }
        }

        LOG_FATAL(format("inventory bin '%s' not found") % name);
    }

    inline const inventory_bin& get_bin(int id) const {
        auto it = bins.find(id);
        if(it != bins.end()) {
            return it->second;
        }

        LOG_FATAL(format("inventory bin id %d not found") % id);
    }

    inline auto begin() const -> decltype(bins.begin()) {
        return bins.begin();
    }

    inline auto end() const -> decltype(bins.end()) {
        return bins.end();
    }
};

}
}
}
