#include "plugin/plugin_registry.hpp"

#include <utility>

namespace phosphor::logging
{

void PluginRegistry::registerPlugin(plugin::Type type,
                                    std::unique_ptr<PluginFactory> factory)
{
    registry.emplace(type, std::move(factory));
}

const PluginFactory* PluginRegistry::lookup(plugin::Type type) const
{
    auto it = registry.find(type);

    if (it == registry.end())
    {
        return nullptr;
    }

    return it->second.get();
}

std::unique_ptr<plugin::Descriptor> PluginRegistry::createDescriptor(
    const plugin::Info& info) const
{
    if (info.type == "cper")
    {
        return createDescriptor(plugin::Type::cper, info);
    }

    return nullptr;
}

std::unique_ptr<plugin::Descriptor> PluginRegistry::createDescriptor(
    plugin::Type type, const plugin::Info& info) const
{
    auto factory = lookup(type);

    if (factory == nullptr)
    {
        return nullptr;
    }

    return factory->createDescriptor(info);
}

} // namespace phosphor::logging
