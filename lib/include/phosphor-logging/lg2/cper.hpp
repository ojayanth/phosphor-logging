#pragma once

#include <map>
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
 * The supplied metadata is serialized into plugin
 * request information and later translated into a
 * CPER runtime descriptor by the logging framework.
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
     */
    std::map<std::string, std::string> oem;
};

} // namespace lg2
