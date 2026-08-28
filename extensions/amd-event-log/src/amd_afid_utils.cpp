#include "amd_afid_utils.hpp"

#include "amd_afid_db_gen.hpp"

#include <phosphor-logging/lg2.hpp>

namespace phosphor::logging::extensions::ael
{

AELStaticInfo getStaticData()
{
    auto result = ::getStaticData();

    return AELStaticInfo{
        result.fallthroughAFID,
        std::string(result.rackUnitPosition),
        std::string(result.schema)};
}

AFIDInfo lookupAFID(
    const std::string& message,
    const std::map<std::string, std::string>& additionalData)
{
    auto result = ::lookupAFID(message, additionalData);

    return AFIDInfo {
        result.afid,
        std::vector<std::string>(result.origins.begin(),
                                 result.origins.end()),
        std::vector<std::string>(result.redfishMappings.begin(),
                                 result.redfishMappings.end()),
        std::string(result.description),
        std::string(result.rackUnitPosition),
        std::string(result.schema)};
}

} // namespace phosphor::logging::extensions::ael