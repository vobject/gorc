#include "source_file_entity.hpp"
#include "build/boc-build/engine/entity_serializer.hpp"
#include "build/boc-build/engine/entity_deserializer.hpp"

gorc::source_file_entity::source_file_entity(entity_input_stream &is)
    : file_entity(is)
{
    return;
}

gorc::source_file_entity::source_file_entity(path const &new_filename,
                                             std::time_t previous_timestamp)
    : file_entity(new_filename, previous_timestamp)
{
    return;
}

void gorc::source_file_entity::serialize(entity_output_stream &os)
{
    file_entity::serialize(os);
}

std::type_index gorc::source_file_entity::get_type_index() const
{
    return typeid(source_file_entity);
}
