#pragma once

#include <phosphor-logging/lg2/plugin_extension.hpp>

namespace lg2
{

/**
 * @brief Create a plugin extension request.
 *
 * This template forms the extension point for plugin-specific lg2 helpers.
 *
 * Example:
 *
 * @code
 * CperInfo cperInfo;
 *
 * lg2::commit(
 *     Event(),
 *     extend<Cper>(cperInfo));
 * @endcode
 *
 * Specializations are responsible for converting producer-supplied metadata
 * into plugin descriptors.
 *
 * @tparam T Extension type.
 *
 * @param[in] value Extension metadata.
 *
 * @return Plugin extension request.
 */
template <typename T>
auto extend(T&& value)
{
    static_assert(sizeof(T) == 0, "Unsupported plugin extension type");
}

/**
 * @brief Process a plugin extension argument.
 *
 * Transfers the contained descriptor into the descriptor collection associated
 * with the current log entry creation request.
 *
 * @param[in,out] descriptors Descriptor collection.
 * @param[in] extension Plugin extension request.
 */
inline void processExtension(
    phosphor::logging::plugin::DescriptorList& descriptors,
    PluginExtension&& extension)
{
    descriptors.emplace_back(std::move(extension.descriptor));
}

} // namespace lg2
