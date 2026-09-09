#include "ael_info.hpp"

namespace phosphor::logging::extensions::ael
{

AELInfoProvider::AELInfoProvider(
    const std::string& msg, Entry::Level level,
    const std::map<std::string, std::string>& additionalData) :
    msg(msg), level(level), additionalData(additionalData)
{}

AELInfo AELInfoProvider::get() const
{
    (void)level;

    AELInfo info;

    auto afidInfo = lookupAFID(msg, additionalData);

    info.afid = afidInfo.afid;
    info.redfishList = afidInfo.redfishMappings;
    info.description = afidInfo.description;

    return info;
}

AELStaticInfo AELStaticInfoProvider::get() const
{
    AELStaticInfo info;

    const auto staticInfo = getStaticData();

    info.version = staticInfo.schema;
    info.rackUnitPosition = staticInfo.rackUnitPosition;
    info.fallthroughAFID = staticInfo.fallthroughAFID;

    return info;
}

} // namespace phosphor::logging::extensions::ael
