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
     * @param[in] entry Parent log entry.
     * @param[in] descriptor Plugin creation descriptor.
     *
     * @return Newly created plugin instance.
     */
    std::unique_ptr<Plugin> create(const Entry& entry,
                                   const plugin::Descriptor& descriptor);

    /**
     * @brief Restore a plugin instance.
     *
     * @param[in] entry Parent log entry.
     * @param[in] info Persisted plugin metadata.
     *
     * @return Restored plugin instance.
     */
    std::unique_ptr<Plugin> restore(const Entry& entry,
                                    const PluginPersistInfo& info);
};

} // namespace phosphor::logging
