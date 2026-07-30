#pragma once

#include "plugin/plugin_descriptor.hpp"

namespace lg2
{

/**
 * @class PluginExtension
 *
 * @brief lg2 plugin extension request.
 *
 * Carries plugin creation descriptors from log
 * producers into the phosphor-logging framework.
 */
class PluginExtension
{
  public:
    PluginExtension() = delete;
    PluginExtension(const PluginExtension&) = delete;
    PluginExtension(PluginExtension&&) = default;
    PluginExtension& operator=(const PluginExtension&) = delete;
    PluginExtension& operator=(PluginExtension&&) = default;
    ~PluginExtension() = default;

    /**
     * @brief Construct a plugin extension.
     *
     * @param[in] descriptor Plugin descriptor.
     */
    explicit PluginExtension(
        phosphor::logging::plugin::DescriptorPtr descriptor) :
        descriptor(std::move(descriptor))
    {}

    /**
     * @brief Plugin descriptor.
     */
    phosphor::logging::plugin::DescriptorPtr descriptor;
};

} // namespace lg2
