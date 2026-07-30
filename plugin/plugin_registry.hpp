#pragma once

#include "plugin/plugin.hpp"
#include "plugin/plugin_descriptor.hpp"
#include "plugin/plugin_type.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

namespace phosphor::logging
{

class Entry;

/**
 * @brief Plugin creation callback.
 *
 * Registered plugin implementations provide a callback
 * used to construct runtime plugin instances.
 */
using PluginCreateFunc = std::function<std::unique_ptr<Plugin>(
    const Entry&, const plugin::Descriptor&)>;

/**
 * @class PluginRegistry
 *
 * @brief Registry of supported plugin types.
 *
 * Maintains a mapping between plugin types and their
 * corresponding creation callbacks.
 */
class PluginRegistry
{
  public:
    PluginRegistry() = default;
    PluginRegistry(const PluginRegistry&) = delete;
    PluginRegistry(PluginRegistry&&) = delete;
    PluginRegistry& operator=(const PluginRegistry&) = delete;
    PluginRegistry& operator=(PluginRegistry&&) = delete;
    ~PluginRegistry() = default;

    /**
     * @brief Return registry singleton instance.
     *
     * @return Plugin registry instance.
     */
    static PluginRegistry& instance();

    /**
     * @brief Register a plugin creation callback.
     *
     * @param[in] type Plugin type.
     * @param[in] createFunc Plugin creation callback.
     */
    void registerPlugin(plugin::Type type, PluginCreateFunc createFunc);

    /**
     * @brief Lookup a plugin creation callback.
     *
     * @param[in] type Plugin type.
     *
     * @return Plugin creation callback or nullptr
     *         if no plugin is registered.
     */
    const PluginCreateFunc* lookup(plugin::Type type) const;

  private:
    /**
     * @brief Registered plugin callbacks.
     */
    std::unordered_map<plugin::Type, PluginCreateFunc> registry;
};

} // namespace phosphor::logging
