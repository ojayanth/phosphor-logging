#pragma once

#include "ael_metadata.hpp"
#include "amd_afid_utils.hpp"
#include "elog_entry.hpp"

#include <phosphor-logging/log.hpp>

#include <map>
#include <string>
#include <string_view>
#include <cstdint>
#include <vector>

namespace phosphor::logging::extensions::ael
{

/**
 * @brief AMD Event Log static metadata.
 *
 * Represents the complete set of static metadata exported
 * by the AEL runtime metadata provider.
 */
struct AELStaticInfo
{
    /**
     * Fallthorugh AMD Field Identifier.
     */
    uint64_t fallthroughAFID{};

    /**
     * AEL schema version.
     */
    std::string version{std::string(constants::SchemaVersion)};

    /**
     * Inventory path identifying the rack unit position.
     */
    std::string rackUnitPosition;
};

/**
 * @brief AMD Event Log metadata.
 *
 * Represents the complete set of metadata exported by
 * the AEL runtime metadata provider.
 */
struct AELInfo
{
    /**
     * AMD Field Identifier.
     */
    uint64_t afid{};

    /**
     * Associated redfish URI paths.
     */
    std::vector<std::string> redfishList;

    /**
     * Human readable description of the fault.
     */
    std::string description;
};

/**
 * @class AELInfoProvider
 *
 * @brief Produces AMD Event Log metadata for a
 *        phosphor-logging event.
 *
 * Responsibilities:
 *
 *   - Populate AEL.DESCRIPTION
 *   - Resolve AEL.AFID
 *   - Populate AEL.FRU_LIST
 *
 * Event producers are not required to emit AEL-specific
 * metadata. AEL properties are derived from the event
 * message and event additional data.
 */
class AELInfoProvider
{
  public:
    /**
     * @brief Construct an AEL information provider.
     *
     * @param[in] msg
     *     Event message.
     *
     * @param[in] level
     *     Event severity.
     *
     * @param[in] additionalData
     *     Event additional data.
     */
    AELInfoProvider(const std::string& msg, Entry::Level level,
                    const std::map<std::string, std::string>& additionalData);

    ~AELInfoProvider() = default;

    AELInfoProvider(const AELInfoProvider&) = default;
    AELInfoProvider& operator=(const AELInfoProvider&) = delete;

    AELInfoProvider(AELInfoProvider&&) = default;
    AELInfoProvider& operator=(AELInfoProvider&&) = delete;

    /**
     * @brief Generate AEL metadata for the event.
     *
     * @return Populated AEL information.
     */
    AELInfo get() const;

  private:
    const std::string& msg;

    Entry::Level level;

    const std::map<std::string, std::string>& additionalData;
};

/**
 * @class AELStaticInfoProvider
 *
 * @brief Produces AMD Event Log static metadata for a
 *        phosphor-logging event.
 *
 * Responsibilities:
 *
 *   - Populate AEL.VERSION
 *   - Populate AEL.RACK_UNIT_POSTION
 *
 * Event producers are not required to emit AEL-specific
 * metadata. AEL properties are derived from the event
 * message and event additional data.
 */
class AELStaticInfoProvider
{
  public:
    /**
     * @brief Generate static AEL metadata.
     *
     * @return Populated static AEL information.
     */
    AELStaticInfo get() const;

    ~AELStaticInfoProvider() = default;

    AELStaticInfoProvider() = default;

    AELStaticInfoProvider(const AELStaticInfoProvider&) = default;
    AELStaticInfoProvider&
        operator=(const AELStaticInfoProvider&) = delete;

    AELStaticInfoProvider(AELStaticInfoProvider&&) = default;
    AELStaticInfoProvider&
        operator=(AELStaticInfoProvider&&) = delete;
};

} // namespace phosphor::logging::extensions::ael
