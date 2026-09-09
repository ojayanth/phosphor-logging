#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace phosphor::logging::extensions::ael
{

/**
 * @brief AMD Field ID metadata.
 */
struct AFIDInfo
{
    /** AMD Field Identifier. */
    uint64_t afid{};

    /** Associated FRU inventory paths. */
    std::vector<std::string> frus;

    /** Associated Redfish resource paths. */
    std::vector<std::string> redfishMappings;

    /** Human readable description of the fault. */
    std::string description;

    /** Inventory path identifying the rack unit position. */
    std::string rackUnitPosition;

    /** Redfish schema identifier. */
    std::string schema;
};

/**
 * @brief Static LUT metadata not associated with any specific event.
 */
struct AFIDStaticInfo
{
    /** Fallback AFID returned when a message is known but args
     *  do not resolve to a leaf. */
    uint64_t fallthroughAFID{};

    /** Inventory path identifying the rack unit position. */
    std::string rackUnitPosition;

    /** Redfish schema identifier. */
    std::string schema;
};

/**
 * @brief Retrieve static data from the LUT that is not
 *        associated with any specific event entry.
 *
 * Returns rackUnitPosition, fallthroughAFID and schema
 * as configured in the top-level of the reverse_lut.json.
 *
 * @return Static LUT metadata.
 */
AFIDStaticInfo getStaticData();

/**
 * @brief Lookup AFID metadata associated with an event.
 *
 * @param[in] message Event message identifier.
 * @param[in] addData Event additional data.
 *
 * @return AFID metadata if a matching entry is found, fallback AFID otherwise.
 */
AFIDInfo lookupAFID(const std::string& message,
                    const std::map<std::string, std::string>& addData);

} // namespace phosphor::logging::extensions::ael
