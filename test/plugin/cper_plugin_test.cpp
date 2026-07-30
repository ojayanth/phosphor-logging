#include "plugin/cper_descriptor.hpp"
#include "plugin/cper_plugin.hpp"
#include "plugin/plugin_registry.hpp"

#include <gtest/gtest.h>

namespace phosphor::logging
{

TEST(CperPluginTest, CreatePlugin)
{
    auto bus = sdbusplus::bus::new_default();

    plugin::cper::Descriptor descriptor(plugin::cper::DiagnosticDataType::cper,
                                        "Corrected", "Processor",
                                        "/tmp/test.cper");

    plugins::cper::Plugin plugin(bus, "/xyz/openbmc_project/logging/entry/1",
                                 descriptor);

    EXPECT_EQ(plugin.type(), plugin::Type::cper);
}

TEST(CperPluginTest, PluginRegistered)
{
    auto createFunc = PluginRegistry::instance().lookup(plugin::Type::cper);

    if (createFunc == nullptr)
    {
        ADD_FAILURE() << "CPER plugin is not registered with "
                      << "PluginRegistry";
    }

    ASSERT_NE(createFunc, nullptr);
}

TEST(CperPluginTest, CreatePluginFromDescriptor)
{
    plugin::cper::Descriptor descriptor(plugin::cper::DiagnosticDataType::cper,
                                        "Corrected", "Processor",
                                        "/tmp/test.cper");

    auto createFunc = PluginRegistry::instance().lookup(plugin::Type::cper);

    ASSERT_NE(createFunc, nullptr) << "CPER plugin registration was not found";

    auto bus = sdbusplus::bus::new_default();

    const std::string objectPath = "/xyz/openbmc_project/logging/entry/1";

    PluginContext context{
        .bus = bus,
        .objectPath = objectPath,
    };

    auto plugin = PluginRegistry::instance().create(context, descriptor);

    ASSERT_NE(plugin, nullptr) << "PluginRegistry returned nullptr for "
                               << "plugin::Type::cper";

    EXPECT_EQ(plugin->type(), plugin::Type::cper);
}

} // namespace phosphor::logging
