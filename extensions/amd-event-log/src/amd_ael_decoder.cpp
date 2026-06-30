#include "amd_ael_decoder.hpp"
#include "amd_ael_lut_gen.hpp"

namespace phosphor::logging::amd
{

std::optional<Result> getAFID(
    const std::string& registry,
    const std::string& messageId,
    const std::array<std::string, 4>& args)
{
    if (registry.empty() || messageId.empty())
    {
        return std::nullopt;
    }

    auto r = lookupAFID(
        registry,
        messageId,
        args[0], args[1], args[2], args[3]);

    return Result{
        r.afid,
        std::string(r.origin)
    };
}

} // namespace