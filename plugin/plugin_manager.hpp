#pragma once

#include "plugin/plugin.hpp"
#include "plugin/plugin_descriptor.hpp"

#include <memory>

namespace phosphor::logging
{

class Entry;
struct PluginPersistInfo;

/**
 * @class PluginManager
 *
 * @brief Coordinates plugin lifecycle operations.
 *
 * PluginManager is responsible for creating and
 * restoring plugin instances using the registered
 * plugin implementations.
 *
 * PluginManager does not own plugin instances.
 * Created plugins are owned by the parent Entry.
 */
class PluginManager
{
  public:
    PluginManager() = default;
    PluginManager(const PluginManager&) = delete;
    PluginManager(PluginManager&&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    PluginManager& operator=(PluginManager&&) = delete;
    ~PluginManager() = default;

    /**
     * @brief Create a plugin instance.
     *
     * Creates a runtime plugin associated with a
     * log entry object path.
     *
     * @param[in] context Plugin creation context.
     * @param[in] descriptor Plugin creation descriptor.
     *
     * @return Plugin instance or nullptr if creation
     *         fails.
     */
    std::unique_ptr<Plugin> create(const PluginContext& context,
                                   const plugin::Descriptor& descriptor);

    /**
     * @brief Restore a plugin instance.
     *
     * @param[in] info Persisted plugin metadata.
     *
     * @return Restored plugin instance.
     */
    std::unique_ptr<Plugin> restore(const PluginPersistInfo& info);
};

} // namespace phosphor::logging
