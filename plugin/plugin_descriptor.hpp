#pragma once

#include "plugin/plugin_type.hpp"

#include <memory>
#include <vector>

namespace phosphor::logging::plugin
{

/**
 * Base descriptor used to create plugins.
 */
class Descriptor
{
  public:
    Descriptor() = default;
    virtual ~Descriptor() = default;

    Descriptor(const Descriptor&) = default;
    Descriptor(Descriptor&&) = default;
    Descriptor& operator=(const Descriptor&) = delete;
    Descriptor& operator=(Descriptor&&) = default;

    /**
     * Return descriptor type.
     *
     * @return Plugin type.
     */
    virtual Type type() const = 0;
};

using DescriptorPtr = std::unique_ptr<Descriptor>;
using DescriptorList = std::vector<DescriptorPtr>;

} // namespace phosphor::logging::plugin
