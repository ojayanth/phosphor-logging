#pragma once

#include "plugin/plugin_descriptor.hpp"

#include <sdbusplus/exception.hpp>
#include <xyz/openbmc_project/Logging/Entry/client.hpp>

#include <map>
#include <string>

namespace lg2::details
{

using Entry = sdbusplus::client::xyz::openbmc_project::logging::Entry<>;

/**
 * @struct EventInfo
 *
 * @brief Information extracted from a generated event.
 */
struct EventInfo
{
    /**
     * @brief Event message identifier.
     */
    std::string message;

    /**
     * @brief Event severity.
     */
    Entry::Level severity;

    /**
     * @brief Event additional data.
     */
    std::map<std::string, std::string> additionalData;
};

/**
 * @struct CreateRequest
 *
 * @brief Request used to create a log entry.
 *
 * This structure aggregates all information required
 * to create a phosphor-logging entry.
 */
struct CreateRequest
{
    /**
     * @brief Event information.
     */
    EventInfo event;

    /**
     * @brief Plugin descriptors associated with the log.
     */
    phosphor::logging::plugin::DescriptorList descriptors;
};

/**
 * @brief Extract event information from a generated event.
 *
 * @param[in] event Generated event.
 *
 * @return Extracted event information.
 */
auto extractEvent(sdbusplus::exception::generated_event_base&& event)
    -> EventInfo;

bool filterEvent(const std::string& event);
bool filterError(const std::string& event);

} // namespace lg2::details
