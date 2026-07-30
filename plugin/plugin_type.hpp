#pragma once

namespace phosphor::logging::plugin
{

/**
 * @enum Type
 *
 * @brief Supported plugin types.
 *
 * Plugin types are used by the plugin framework to
 * identify plugin descriptors and runtime plugin
 * implementations.
 */
enum class Type
{
    /**
     * @brief Common Platform Error Record plugin.
     */
    cper,

    /**
     * @brief OEM diagnostic plugin.
     */
    oemDiagnostic,
};

} // namespace phosphor::logging::plugin
