#pragma once

#include "plugin/plugin.hpp"
#include "plugin/plugin_descriptor.hpp"
#include "plugin/plugin_registry.hpp"

#include <memory>

namespace phosphor::logging
{

/**
 * Manager responsible for creating runtime plugin
 * instances from descriptors.
 *
 * PluginManager uses PluginRegistry to locate the
 * creation callback associated with a descriptor type.
 *
 * Runtime plugin restoration and lifecycle management
 * support will be introduced in subsequent changes.
 */
class PluginManager
{
  public:
    explicit PluginManager(PluginRegistry& registry);

    PluginManager(const PluginManager&) = delete;
    PluginManager(PluginManager&&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    PluginManager& operator=(PluginManager&&) = delete;
    ~PluginManager() = default;

    /**
     * Create a plugin instance.
     *
     * @param[in] context Plugin creation context.
     * @param[in] descriptor Plugin creation descriptor.
     *
     * @return Plugin instance or nullptr if no plugin is
     *         registered for the descriptor type.
     */
    std::unique_ptr<Plugin> create(const PluginContext& context,
                                   const plugin::Descriptor& descriptor) const;

  private:
    /** Plugin registry. */
    PluginRegistry& registry;
};

} // namespace phosphor::logging
