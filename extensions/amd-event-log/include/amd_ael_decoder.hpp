#pragma once

#include <string>
#include <array>
#include <optional>

namespace phosphor::logging::amd
{

struct Result
{
    int afid;
    std::string origin;
};

std::optional<Result> getAFID(
    const std::string& registry,
    const std::string& messageId,
    const std::array<std::string, 4>& args);

} // namespace
