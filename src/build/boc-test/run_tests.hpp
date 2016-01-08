#pragma once

#include <set>
#include "io/path.hpp"

namespace gorc {

    int run_tests(std::set<path> const &tests, path const &fail_log);

}