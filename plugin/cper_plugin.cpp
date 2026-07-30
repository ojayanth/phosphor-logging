#include "cper_plugin.hpp"

#include <fcntl.h>
#include <unistd.h>

namespace phosphor::logging::plugins::cper
{
Plugin::Plugin(sdbusplus::bus_t& bus, const char* path, const Entry& entry,
               const phosphor::logging::plugin::cper::Descriptor& descriptor) :
    phosphor::logging::Plugin(entry), CperIface(bus, path),
    artifactPath(descriptor.artifactPath())
{
    diagnosticDataType(
        convertDiagnosticDataType(descriptor.diagnosticDataType()), true);

    notificationType(descriptor.notificationType(), true);

    sectionType(descriptor.sectionType(), true);

    oem(descriptor.oem(), true);
}

plugin::Type Plugin::type() const
{
    return plugin::Type::cper;
}

void Plugin::persist() {}

void Plugin::restore() {}

void Plugin::erase() {}

sdbusplus::message::unix_fd Plugin::getCPERBinary()
{
    int fd = ::open(artifactPath.c_str(), O_RDONLY);

    return sdbusplus::message::unix_fd(fd);
}

CperIface::DiagnosticDataType Plugin::convertDiagnosticDataType(
    phosphor::logging::plugin::cper::DiagnosticDataType type)
{
    switch (type)
    {
        case phosphor::logging::plugin::cper::DiagnosticDataType::cper:
            return CperIface::DiagnosticDataType::CPER;

        default:
            throw std::invalid_argument("Unsupported diagnostic data type");
    }
}

} // namespace phosphor::logging::plugins::cper
