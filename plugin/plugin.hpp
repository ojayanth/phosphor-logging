#pragma once

#include "plugin/plugin_type.hpp"

#include <sdbusplus/bus.hpp>

#include <string>

namespace phosphor::logging
{

/**
 * Context used to construct runtime plugin instances.
 */
struct PluginContext
{
    /** D-Bus connection. */
    sdbusplus::bus_t& bus;

    /** Log entry object path. */
    const std::string& objectPath;
};

/**
 * Base class for all log entry plugins.
 *
 * Plugins augment a log entry with plugin-specific
 * metadata, functionality, and D-Bus interfaces.
 */
class Plugin
{
  public:
    Plugin() = default;
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin(Plugin&&) = default;
    Plugin& operator=(Plugin&&) = delete;
    virtual ~Plugin() = default;

    /**
     * Return plugin type.
     *
     * @return Plugin type.
     */
    virtual plugin::Type type() const = 0;
};

} // namespace phosphor::logging
