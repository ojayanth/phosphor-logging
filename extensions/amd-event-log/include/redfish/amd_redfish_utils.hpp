#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace phosphor::logging::extensions::ael::redfish
{

/**
 * @brief Parse a comma-separated FRU list.
 *
 * @param[in] value Comma-separated FRU list.
 *
 * @return Parsed FRU vector.
 */
[[nodiscard]] std::vector<std::string> splitFRUs(const std::string& value);

/**
 * @brief Return the final component of a path.
 *
 * @param[in] path Input path.
 *
 * @return Final path component.
 */
[[nodiscard]] std::string basename(const std::string& path);

} // namespace phosphor::logging::extensions::ael::redfish
