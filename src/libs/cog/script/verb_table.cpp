#include "verb_table.hpp"

void gorc::cog::verb_table::internal_add_verb(std::unique_ptr<verb> &&v)
{
    int verb_id = static_cast<int>(verbs.size());
    if(!verb_index.emplace(v->name, verb_id).second) {
        LOG_FATAL(format("verb '%s' already registered") % v->name);
    }

    verbs.push_back(std::forward<std::unique_ptr<verb>>(v));
}

void gorc::cog::verb_table::add_synonym(std::string const &name,
                                        std::string const &syn)
{
    verb_index.emplace(syn, static_cast<int>(get_verb_id(name)));
}

gorc::cog::verb_id gorc::cog::verb_table::get_verb_id(std::string const &name) const
{
    auto it = verb_index.find(name);
    if(it == verb_index.end()) {
        LOG_FATAL(format("verb '%s' does not exist") % name);
    }

    return verb_id(it->second);
}

gorc::cog::verb const& gorc::cog::verb_table::get_verb(verb_id id) const
{
    return *verbs[static_cast<int>(id)];
}