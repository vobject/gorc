#pragma once

#include "framework/content/text_loader.h"
#include "cog/compiler.h"

namespace gorc {
namespace content {
namespace loaders {

class inventory_loader : public text_loader {
private:
	cog::compiler& compiler;

public:
	static const std::vector<boost::filesystem::path> asset_root_path;

	inventory_loader(cog::compiler& compiler);

	virtual std::unique_ptr<asset> parse(text::tokenizer& t, manager& manager, diagnostics::report& report) override;
};

}
}
}
