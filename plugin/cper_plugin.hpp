#pragma once

#include "plugin/cper_descriptor.hpp"
#include "plugin/plugin.hpp"
#include "plugin/plugin_factory.hpp"
#include "plugin/plugin_registry.hpp"

#include <sdbusplus/message/native_types.hpp>
#include <xyz/openbmc_project/Logging/Diagnostic/CPER/server.hpp>

namespace phosphor::logging::plugin::cper
{

using CperIface =
    sdbusplus::server::xyz::openbmc_project::logging::diagnostic::CPER;

inline constexpr auto pluginType = "cper";
inline constexpr auto notificationTypeKey = "notificationType";
inline constexpr auto sectionTypeKey = "sectionType";
inline constexpr auto cperFdKey = "cperFd";
inline constexpr auto oemKey = "oem";

/**
 * @class Plugin
 *
 * @brief CPER plugin implementation.
 *
 * Augments a log entry with the
 * xyz.openbmc_project.Logging.Diagnostic.CPER interface.
 *
 * The plugin consumes a CPER descriptor and exposes
 * CPER-specific metadata through the generated D-Bus
 * interface.
 */
class Plugin : public phosphor::logging::Plugin, public CperIface
{
  public:
    Plugin() = delete;
    Plugin(const Plugin&) = delete;
    Plugin(Plugin&&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin& operator=(Plugin&&) = delete;
    ~Plugin() override = default;

    /**
     * @brief Construct a CPER plugin.
     *
     * Initializes the CPER D-Bus interface properties
     * from the supplied descriptor.
     *
     * @param[in] context Plugin creation context.
     * @param[in] descriptor CPER plugin descriptor.
     */
    Plugin(const PluginContext& context, const Descriptor& descriptor);

    /**
     * @brief Return plugin type.
     *
     * @return Plugin type identifier.
     */
    plugin::Type type() const override;

    /**
     * @brief Return the CPER binary associated with the log.
     *
     * @return Read-only CPER file descriptor.
     */
    sdbusplus::message::unix_fd getCPERBinary() override;

  private:
    /**
     * @brief CPER binary file descriptor.
     *
     * Ownership remains with the plugin framework.
     */
    int cperFd;
};

/**
 * @class Factory
 *
 * @brief Factory for CPER runtime plugins.
 *
 * Creates runtime CPER plugin instances from
 * CPER descriptors.
 */
class Factory : public phosphor::logging::PluginFactory
{
  public:
    Factory() = default;
    ~Factory() override = default;

    /**
     * @brief Create a CPER runtime plugin.
     *
     * @param[in] context Plugin creation context.
     * @param[in] descriptor Plugin descriptor.
     *
     * @return Runtime CPER plugin instance.
     */
    std::unique_ptr<phosphor::logging::Plugin> create(
        const PluginContext& context,
        const plugin::Descriptor& descriptor) const override;

    /**
     * @brief Create a CPER descriptor.
     *
     * @param[in] info Plugin request information.
     *
     * @return CPER descriptor.
     */
    std::unique_ptr<plugin::Descriptor> createDescriptor(
        const plugin::Info& info) const override;
};

/**
 * @brief Register the CPER plugin implementation.
 *
 * Associates the CPER plugin type with the factory
 * callback used to construct runtime plugin instances.
 *
 * @param[in,out] registry Plugin registry.
 */
void registerPlugin(PluginRegistry& registry);

} // namespace phosphor::logging::plugin::cper
