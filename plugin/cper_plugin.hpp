#pragma once

#include "plugin/cper_descriptor.hpp"
#include "plugin/plugin.hpp"

#include <sdbusplus/message/native_types.hpp>
#include <xyz/openbmc_project/Logging/Diagnostic/CPER/server.hpp>

namespace phosphor::logging::plugins::cper
{

using CperIface =
    sdbusplus::server::xyz::openbmc_project::logging::diagnostic::CPER;

/**
 * @class Plugin
 *
 * @brief CPER plugin implementation.
 *
 * Implements the
 * xyz.openbmc_project.Logging.Diagnostic.CPER
 * D-Bus interface and augments a parent log entry
 * with CPER-specific diagnostic information.
 *
 * The plugin is responsible for:
 * - Publishing CPER metadata on D-Bus
 * - Providing access to CPER diagnostic artifacts
 * - Managing OEM diagnostic information
 * - Participating in plugin persistence and restoration
 *
 * The plugin is owned by a parent log Entry and
 * participates in the complete Entry lifecycle.
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
     * using information provided by the supplied
     * descriptor.
     *
     * @param[in] bus D-Bus connection.
     * @param[in] path Object path where the CPER
     *                 interface is registered.
     * @param[in] descriptor CPER creation descriptor.
     */
    Plugin(sdbusplus::bus_t& bus, const char* path,
           const phosphor::logging::plugin::cper::Descriptor& descriptor);

    /**
     * @brief Return plugin type.
     *
     * @return Plugin type identifier.
     */
    plugin::Type type() const override;

    /**
     * @brief Persist plugin state.
     *
     * Persist any plugin-specific information required
     * to reconstruct the plugin following a service
     * restart.
     */
    void persist() override;

    /**
     * @brief Restore plugin state.
     *
     * Reconstruct runtime plugin state from previously
     * persisted information.
     */
    void restore() override;

    /**
     * @brief Remove plugin resources.
     *
     * Delete plugin-specific persisted state and any
     * associated resources.
     */
    void erase() override;

    /**
     * @brief Return CPER binary artifact.
     *
     * Provides read-only access to the CPER artifact
     * associated with the parent log entry.
     *
     * @return Read-only file descriptor.
     */
    sdbusplus::message::unix_fd getCPERBinary() override;

  private:
    /**
     * @brief Convert a CPER descriptor diagnostic data type
     *        into the corresponding CPER D-Bus enum value.
     *
     * The plugin framework uses descriptor-defined types to
     * represent CPER metadata during log creation. This helper
     * translates the descriptor representation into the
     * generated D-Bus schema representation used by the
     * CPER interface.
     *
     * @param[in] type Descriptor diagnostic data type.
     *
     * @return Corresponding CPER D-Bus diagnostic data type.
     *
     * @throws std::invalid_argument if the descriptor type
     *         cannot be mapped to a supported D-Bus value.
     */
    static CperIface::DiagnosticDataType convertDiagnosticDataType(
        phosphor::logging::plugin::cper::DiagnosticDataType type);

    /**
     * @brief Persisted CPER artifact path.
     */
    std::string artifactPath;
};

} // namespace phosphor::logging::plugins::cper
