#pragma once

#include "plugin/plugin_descriptor.hpp"
#include "plugin/plugin_type.hpp"

#include <map>
#include <string>

namespace phosphor::logging::plugin::cper
{
/**
 * @enum DiagnosticDataType
 *
 * @brief CPER diagnostic data types supported by the
 *        plugin framework.
 */
enum class DiagnosticDataType
{
    /**
     * @brief Common Platform Error Record.
     */
    cper,
};

/**
 * @class Descriptor
 *
 * @brief CPER plugin creation descriptor.
 *
 * Carries producer-supplied CPER metadata from the log
 * creation API into the plugin infrastructure.
 *
 * The descriptor contains all information required to
 * create a CPER plugin and expose the
 * xyz.openbmc_project.Logging.CPER interface.
 */
class Descriptor : public phosphor::logging::plugin::Descriptor
{
  public:
    Descriptor() = delete;
    Descriptor(const Descriptor&) = delete;
    Descriptor(Descriptor&&) = default;
    Descriptor& operator=(const Descriptor&) = delete;
    Descriptor& operator=(Descriptor&&) = default;
    ~Descriptor() override = default;

    /**
     * @brief Construct a CPER descriptor.
     *
     * @param[in] diagnosticDataType CPER diagnostic data type.
     * @param[in] notificationType CPER notification type GUID.
     * @param[in] sectionType CPER section type GUID.
     * @param[in] artifactPath Path to the CPER artifact.
     * @param[in] oem OEM extension metadata.
     */
    Descriptor(DiagnosticDataType diagnosticDataType,
               std::string notificationType, std::string sectionType,
               std::string artifactPath,
               std::map<std::string, std::string> oem = {});

    /**
     * @brief Return the descriptor type.
     *
     * @return Plugin type identifier.
     */
    Type type() const override;

    /**
     * @brief Return the diagnostic data type.
     *
     * @return Diagnostic data type.
     */
    const DiagnosticDataType& diagnosticDataType() const;

    /**
     * @brief Return the notification type GUID.
     *
     * @return Notification type GUID.
     */
    const std::string& notificationType() const;

    /**
     * @brief Return the section type GUID.
     *
     * @return Section type GUID.
     */
    const std::string& sectionType() const;

    /**
     * @brief Return the CPER artifact path.
     *
     * @return Artifact path.
     */
    const std::string& artifactPath() const;

    /**
     * @brief Return OEM extension metadata.
     *
     * The key identifies an OEM namespace and the
     * value contains vendor-specific JSON metadata.
     *
     * @return OEM metadata.
     */
    const std::map<std::string, std::string>& oem() const;

  private:
    /**
     * @brief CPER diagnostic data type.
     */
    DiagnosticDataType diagnosticDataTypeValue;

    /**
     * @brief CPER notification type GUID.
     */
    std::string notificationTypeGuid;

    /**
     * @brief CPER section type GUID.
     */
    std::string sectionTypeGuid;

    /**
     * @brief Path to the persisted CPER artifact.
     */
    std::string artifactLocation;

    /**
     * @brief OEM extension metadata.
     */
    std::map<std::string, std::string> oemMetadata;
};

} // namespace phosphor::logging::plugin::cper
