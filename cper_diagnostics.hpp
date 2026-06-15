#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace phosphor::logging::diagnostics
{

/**
 * @brief CPER diagnostic data container
 *
 * Holds normalized CPER metadata extracted from log entry
 * AdditionalData. This structure is used as a transport object
 * between metadata parsing and DBus interface creation.
 */
struct CperData
{
    /** @brief True if this log represents a CPER event */
    bool present = false;

    /** @brief CPER type (e.g., "CPER", "CPERSection") */
    std::string type;

    /** @brief Structured diagnostic summary (DBus-sized, bounded) */
    std::string diagnosticInfo;

    /**
     * @brief Object path/URI to extended CPER data
     *
     * Typically references a dump object or persistent storage
     * containing the full CPER payload.
     */
    std::string additionalDataObject;
};

/**
 * @brief Extract CPER metadata from AdditionalData
 *
 * Identifies CPER events using MESSAGE_ID and extracts CPER-related
 * fields from AdditionalData. Extracted fields are removed from
 * AdditionalData to avoid duplication in Logging.Entry.
 *
 * Extraction rules:
 *  - MESSAGE_ID == xyz.openbmc_project.Logging.CperDetected
 *  - TYPE → CperData::type
 *  - DATA → CperData::diagnosticInfo
 *  - ADDITIONALDATAURI → CperData::additionalDataObject
 *
 * @param[in] errMsg        Log message
 * @param[in] entryId       Entry ID
 * @param[in,out] additionalData
 *      Metadata map; CPER keys will be removed if found
 *
 * @return Extracted CPER diagnostic data
 */
CperData collectCperData(const std::string& errMsg, uint32_t entryId,
                         std::map<std::string, std::string>& additionalData);

/**
 * @brief Attach CPER DBus interface to log entry
 *
 * Adds xyz.openbmc_project.Logging.Diagnostic.CPER interface
 * to the provided Entry instance if CPER data is present.
 *
 * This must be called before the Entry is exposed on DBus to
 * ensure a single InterfacesAdded signal.
 *
 * @tparam EntryT Entry class type
 *
 * @param[in,out] entry Entry instance
 * @param[in] data CPER diagnostic data
 */
template <typename EntryT>
inline void attachCperInterface(EntryT& entry, const CperData& data)
{
    if (!data.present)
    {
        return;
    }

    entry.createCperInterface(data.type, data.diagnosticInfo,
                              data.additionalDataObject);
}

} // namespace phosphor::logging::diagnostics
