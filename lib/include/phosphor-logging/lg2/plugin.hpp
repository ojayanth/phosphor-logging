#pragma once

namespace lg2
{

/**
 * @brief Create a plugin extension request.
 *
 * Plugin-specific specializations convert producer
 * supplied metadata into plugin request information
 * that is processed during log entry creation.
 *
 * Example:
 *
 * @code
 * lg2::commit(
 *     PlatformError(...)
 *         .extend<lg2::CPER>(std::move(cperInfo)));
 * @endcode
 *
 * @tparam Plugin Plugin extension type.
 * @tparam Info Plugin metadata type.
 *
 * @param[in] value Plugin metadata.
 *
 * @return Plugin extension request.
 */
template <typename Plugin, typename Info>
auto extend(Info&& value);

} // namespace lg2
