#include "cper_descriptor.hpp"

#include <utility>

namespace phosphor::logging::plugin::cper
{

Descriptor::Descriptor(DiagnosticDataType diagnosticDataType,
                       std::string notificationType, std::string sectionType,
                       std::string artifactPath,
                       std::map<std::string, std::string> oem) :
    diagnosticDataTypeValue(std::move(diagnosticDataType)),
    notificationTypeGuid(std::move(notificationType)),
    sectionTypeGuid(std::move(sectionType)),
    artifactLocation(std::move(artifactPath)), oemMetadata(std::move(oem))
{}

Type Descriptor::type() const
{
    return Type::cper;
}

const DiagnosticDataType& Descriptor::diagnosticDataType() const
{
    return diagnosticDataTypeValue;
}

const std::string& Descriptor::notificationType() const
{
    return notificationTypeGuid;
}

const std::string& Descriptor::sectionType() const
{
    return sectionTypeGuid;
}

const std::string& Descriptor::artifactPath() const
{
    return artifactLocation;
}

const std::map<std::string, std::string>& Descriptor::oem() const
{
    return oemMetadata;
}

} // namespace phosphor::logging::plugin::cper
