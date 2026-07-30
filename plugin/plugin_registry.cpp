#include "plugin_registry.hpp"

namespace phosphor::logging
{

PluginRegistry& PluginRegistry::instance()
{
    static PluginRegistry registry;
    return registry;
}

void PluginRegistry::registerPlugin(plugin::Type type,
                                    PluginCreateFunc createFunc)
{
    registry.emplace(type, std::move(createFunc));
}

const PluginCreateFunc* PluginRegistry::lookup(plugin::Type type) const
{
    auto it = registry.find(type);

    if (it == registry.end())
    {
        return nullptr;
    }

    return &it->second;
}

} // namespace phosphor::logging
