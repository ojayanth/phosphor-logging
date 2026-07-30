#include "plugin_manager.hpp"

#include "plugin_registry.hpp"

namespace phosphor::logging
{

std::unique_ptr<Plugin> PluginManager::create(
    const PluginContext& context, const plugin::Descriptor& descriptor)
{
    return PluginRegistry::instance().create(context, descriptor);
}

std::unique_ptr<Plugin> PluginManager::restore(
    [[maybe_unused]] const PluginPersistInfo& info)
{
    return {};
}

} // namespace phosphor::logging
