#include "cper_plugin.hpp"

#include "plugin_registry.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <memory>

namespace phosphor::logging::plugins::cper
{

Plugin::Plugin(sdbusplus::bus_t& bus, const char* path,
               const phosphor::logging::plugin::cper::Descriptor& descriptor) :
    CperIface(bus, path), artifactPath(descriptor.artifactPath())
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

namespace
{

/**
 * @brief Create a CPER plugin instance.
 *
 * Constructs a runtime CPER plugin associated with
 * the supplied log entry object path.
 *
 * @param[in] context Plugin creation context.
 * @param[in] descriptor Plugin creation descriptor.
 *
 * @return Runtime CPER plugin instance.
 */
std::unique_ptr<phosphor::logging::Plugin> createCperPlugin(
    const phosphor::logging::PluginContext& context,
    const phosphor::logging::plugin::Descriptor& descriptor)
{
    return std::make_unique<phosphor::logging::plugins::cper::Plugin>(
        context.bus, context.objectPath.c_str(),
        static_cast<const phosphor::logging::plugin::cper::Descriptor&>(
            descriptor));
}

/**
 * @brief Register the CPER plugin type.
 */
const bool registered = [] {
    phosphor::logging::PluginRegistry::instance().registerPlugin(
        phosphor::logging::plugin::Type::cper, createCperPlugin);

    return true;
}();

} // namespace
