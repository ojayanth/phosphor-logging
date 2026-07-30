#pragma once

#include "plugin/plugin_type.hpp"

#include <sdbusplus/bus.hpp>

#include <string>

namespace phosphor::logging
{

/**
 * @struct PluginContext
 *
 * @brief Context used to construct runtime plugin
 *        instances.
 *
 * Provides the D-Bus information required to attach
 * plugin interfaces to a log entry object.
 *
 * Plugins are associated with a log entry object path
 * but do not require knowledge of the Entry
 * implementation.
 */
struct PluginContext
{
    /**
     * @brief D-Bus connection.
     */
    sdbusplus::bus_t& bus;

    /**
     * @brief Log entry object path.
     */
    const std::string& objectPath;
};

/**
 * @class Plugin
 *
 * @brief Base class for all log entry plugins.
 *
 * Plugins augment a parent log entry with additional metadata,
 * functionality, and D-Bus interfaces.
 *
 * A plugin is owned by an Entry and participates in the same
 * lifecycle as its parent:
 *
 * - Create
 * - Persist
 * - Restore
 * - Delete
 *
 * Plugin implementations should maintain only plugin-specific
 * state and may access parent Entry metadata through the
 * associated Entry object.
 */
class Plugin
{
  public:
    Plugin() = default;
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin(Plugin&&) = default;
    Plugin& operator=(Plugin&&) = delete;

    /**
     * @brief Destroy the plugin.
     */
    virtual ~Plugin() = default;

    /**
     * @brief Return plugin type.
     *
     * @return Plugin type identifier.
     */
    virtual plugin::Type type() const = 0;

    /**
     * @brief Persist plugin state.
     *
     * Persist any plugin-specific metadata and resources
     * required for future restoration.
     */
    virtual void persist() = 0;

    /**
     * @brief Restore plugin state.
     *
     * Reconstruct runtime plugin state from previously
     * persisted information.
     */
    virtual void restore() = 0;

    /**
     * @brief Delete plugin resources.
     *
     * Remove any plugin-specific persisted state and
     * associated resources.
     */
    virtual void erase() = 0;
};

} // namespace phosphor::logging
