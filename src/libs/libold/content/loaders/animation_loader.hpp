#pragma once

#include "libold/base/content/text_loader.hpp"

namespace gorc {
namespace content {
namespace loaders {

class animation_loader : public text_loader {
public:
    static const std::vector<boost::filesystem::path> asset_root_path;

    virtual std::unique_ptr<asset> parse(text::tokenizer& t, content_manager& manager, service_registry const &) const override;
};

}
}
}
