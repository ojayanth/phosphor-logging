#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace lg2
{

/**
 * @brief CPER event extension type.
 *
 * Used with Event::extend() to attach Common Platform
 * Error Record (CPER) metadata to an event.
 *
 * Example:
 *
 * @code
 * lg2::commit(
 *     PlatformError(...)
 *         .extend<lg2::CPER>(std::move(cperInfo)));
 * @endcode
 */
struct CPER
{};

/**
 * @brief Metadata used to request a CPER extension.
 *
 * The metadata is attached to an event through
 * Event::extend<CPER>() and consumed by the logging
 * framework during log creation.
 */
struct CperInfo
{
    /**
     * @brief CPER notification type GUID.
     */
    std::string notificationType;

    /**
     * @brief CPER section type GUID.
     */
    std::string sectionType;

    /**
     * @brief CPER binary file descriptor.
     *
     * The file descriptor contains the CPER payload
     * associated with the event. Ownership is
     * transferred to the logging framework when the
     * event is committed.
     */
    int cperFd;

    /**
     * @brief Vendor-specific CPER metadata.
     *
     * May contain implementation-defined JSON content,
     * including nested objects and arrays.
     */
    nlohmann::json oem;
};

} // namespace lg2
