#pragma once

#include <phosphor-logging/lg2/plugin_extension.hpp>

namespace lg2
{

/**
 * @struct CperInfo
 *
 * @brief CPER metadata supplied by a log producer.
 *
 * The information contained within this structure is
 * used to construct a CPER plugin descriptor which is
 * subsequently used to instantiate a runtime CPER
 * plugin.
 */
struct CperInfo
{
    phosphor::logging::plugin::cper::DiagnosticDataType diagnosticDataType;

    std::string notificationType;
    std::string sectionType;
    std::string artifactPath;

    std::map<std::string, std::string> oem;
};

/**
 * @brief Create a CPER extension request.
 *
 * Converts CPER metadata into a CPER plugin descriptor.
 *
 * @param[in] info CPER metadata.
 *
 * @return Plugin extension request.
 */
PluginExtension extend(const CperInfo& info);

} // namespace lg2
