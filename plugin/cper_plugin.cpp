#include "plugin/cper_plugin.hpp"

#include <phosphor-logging/lg2/cper.hpp>

#include <memory>
#include <stdexcept>

namespace phosphor::logging::plugin::cper
{

std::unique_ptr<phosphor::logging::Plugin> Factory::create(
    const phosphor::logging::PluginContext& context,
    const phosphor::logging::plugin::Descriptor& descriptor) const
{
    if (descriptor.type() != plugin::Type::cper)
    {
        throw std::invalid_argument("CPER plugin received non-CPER descriptor");
    }

    return std::make_unique<Plugin>(context,
                                    static_cast<const Descriptor&>(descriptor));
}

std::unique_ptr<plugin::Descriptor> Factory::createDescriptor(
    const plugin::Info& info) const
{
    auto notificationType = info.data.at(lg2::cper::notificationTypeKey);

    auto sectionType = info.data.at(lg2::cper::sectionTypeKey);

    auto cperFd = std::stoi(info.data.at(lg2::cper::cperFdKey));

    auto oem = info.data;

    oem.erase(lg2::cper::notificationTypeKey);

    oem.erase(lg2::cper::sectionTypeKey);

    oem.erase(lg2::cper::cperFdKey);

    return std::make_unique<Descriptor>(
        DiagnosticDataType::CPER, std::move(notificationType),
        std::move(sectionType), cperFd, std::move(oem));
}

Plugin::Plugin(const PluginContext& context, const Descriptor& descriptor) :
    CperIface(context.bus, context.objectPath.c_str()),
    cperFd(descriptor.cperFd())
{
    diagnosticDataType(descriptor.diagnosticDataType(), true);
    notificationType(descriptor.notificationType(), true);
    sectionType(descriptor.sectionType(), true);
    oem(descriptor.oem(), true);
}

plugin::Type Plugin::type() const
{
    return plugin::Type::cper;
}

sdbusplus::message::unix_fd Plugin::getCPERBinary()
{
    return sdbusplus::message::unix_fd(cperFd);
}

void registerPlugin(PluginRegistry& registry)
{
    registry.registerPlugin(plugin::Type::cper, std::make_unique<Factory>());
}

} // namespace phosphor::logging::plugin::cper
