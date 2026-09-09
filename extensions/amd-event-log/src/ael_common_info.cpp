#include "ael_info.hpp"
#include "ael_common_info.hpp"
#include <phosphor-logging/lg2.hpp>

namespace phosphor::logging::extensions::ael
{

CommonInfo getCommonInfo()
{
    CommonInfo info;

    const auto staticInfo = AELStaticInfoProvider{}.get();

    // TODO replace with actual data
    info.rackId = "AMD:RACK_ID";
    info.version = staticInfo.version;

    return info;
}

} // namespace phosphor::logging::extensions::ael
