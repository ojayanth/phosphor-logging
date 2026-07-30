#pragma once

#include "plugin.hpp"
#include "plugin_descriptor.hpp"

#include <memory>

namespace phosphor::logging
{

class IEntryView;

/**
 * @brief Base plugin factory.
 */
class PluginFactory
{
  public:
    virtual ~PluginFactory() = default;

    virtual std::unique_ptr<Plugin> create(
        const IEntryView& entry, const plugin::Descriptor& descriptor) = 0;
};

} // namespace phosphor::logging
