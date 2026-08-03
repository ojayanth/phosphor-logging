#pragma once

#include "plugin/plugin_info.hpp"

#include <sdbusplus/exception.hpp>
#include <xyz/openbmc_project/Logging/Entry/client.hpp>

#include <map>
#include <string>
#include <vector>

namespace lg2::details
{

using Entry = sdbusplus::client::xyz::openbmc_project::logging::Entry<>;
using PluginInfo = phosphor::logging::plugin::Info;

/**
 * @brief Information extracted from a generated event.
 */
struct EventInfo
{
    /** Event message identifier. */
    std::string message;

    /** Event level. */
    Entry::Level level;

    /** Event metadata. */
    std::map<std::string, std::string> additionalData;

    /**
     * @brief Requested plugins.
     *
     * Plugins are attached to an event through
     * Event::extend() and consumed during log creation.
     *
     * The list may be empty if the event does not
     * request any plugins.
     */
    std::vector<PluginInfo> plugins;
};

/**
 * @brief Extract information from a generated event.
 *
 * @param[in] event Event to extract information from.
 *
 * @return Extracted event information.
 */
auto extractEvent(sdbusplus::exception::generated_event_base&& event)
    -> EventInfo;

bool filterEvent(const std::string&);
bool filterError(const std::string&);

} // namespace lg2::details
