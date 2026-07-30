#pragma once

#include "plugin/plugin_descriptor.hpp"

namespace lg2
{

/**
 * @struct PluginExtension
 *
 * @brief Plugin extension request supplied to lg2::commit().
 *
 * Plugin extensions are created by helper APIs such
 * as extend<Cper>() and carried through the lg2 commit
 * pipeline until log entry creation.
 *
 * During commit processing, the contained descriptor
 * is transferred into the plugin descriptor list
 * associated with the log entry.
 */
struct PluginExtension
{
    /**
     * @brief Plugin creation descriptor.
     *
     * Ownership is transferred to the logging
     * framework during commit processing.
     */
    phosphor::logging::plugin::DescriptorPtr descriptor;
};

} // namespace lg2
