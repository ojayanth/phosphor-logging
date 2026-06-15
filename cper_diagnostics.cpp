#include "cper_diagnostics.hpp"

#include <xyz/openbmc_project/Logging/Diagnostic/CPER/server.hpp>

namespace phosphor::logging::diagnostics
{

static constexpr auto CPER_MESSAGE_ID =
    "xyz.openbmc_project.Logging.CperDetected";

CperData collectCperData(const std::string&, uint32_t,
                         std::map<std::string, std::string>& additionalData)
{
    CperData result{};

    // Detect CPER event via MESSAGE_ID
    auto msgIt = additionalData.find("MESSAGE_ID");
    if (msgIt == additionalData.end() || msgIt->second != CPER_MESSAGE_ID)
    {
        return result;
    }

    result.present = true;

    // Extract TYPE
    if (auto it = additionalData.find("TYPE"); it != additionalData.end())
    {
        result.type = it->second;
        additionalData.erase(it);
    }

    // Extract DATA
    if (auto it = additionalData.find("DATA"); it != additionalData.end())
    {
        result.diagnosticInfo = it->second;
        additionalData.erase(it);
    }

    // Extract AdditionalDataURI
    if (auto it = additionalData.find("ADDITIONALDATAURI");
        it != additionalData.end())
    {
        result.additionalDataObject = it->second;
        additionalData.erase(it);
    }

    // Defaults
    if (result.type.empty())
    {
        result.type = "CPER";
    }

    if (result.diagnosticInfo.empty())
    {
        result.diagnosticInfo = "CPER event detected";
    }

    return result;
}

} // namespace phosphor::logging::diagnostics
