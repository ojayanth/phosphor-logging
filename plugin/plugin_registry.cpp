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

std::unique_ptr<Plugin> PluginRegistry::create(
    const PluginContext& context, const plugin::Descriptor& descriptor) const
{
    auto createFunc = lookup(descriptor.type());

    if (createFunc == nullptr)
    {
        return nullptr;
    }

    return (*createFunc)(context, descriptor);
}

} // namespace phosphor::logging
