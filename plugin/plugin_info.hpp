#pragma once

#include <map>
#include <string>

namespace phosphor::logging::plugin
{

/**
 * @brief Plugin request information.
 *
 * Describes a plugin requested for a log entry along
 * with plugin-specific metadata.
 */
struct Info
{
    /** Registered plugin type identifier. */
    std::string type;

    /** Plugin-specific metadata. */
    std::map<std::string, std::string> data;
};

} // namespace phosphor::logging::plugin
