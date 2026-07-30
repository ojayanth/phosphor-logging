#include "plugin_manager.hpp"

namespace phosphor::logging
{

std::unique_ptr<Plugin> PluginManager::create(
    [[maybe_unused]] const Entry& entry,
    [[maybe_unused]] const plugin::Descriptor& descriptor)
{
    return {};
}

std::unique_ptr<Plugin> PluginManager::restore(
    [[maybe_unused]] const Entry& entry,
    [[maybe_unused]] const PluginPersistInfo& info)
{
    return {};
}

} // namespace phosphor::logging
