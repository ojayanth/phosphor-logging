#include "ael_info.hpp"

#include <gtest/gtest.h>

namespace phosphor::logging::extensions::ael
{

TEST(AELInfoTest, UnknownMessageReturnsNullopt)
{
    auto result = resolveAFID("INVALID.MESSAGE", {});

    EXPECT_FALSE(result.has_value());
}

TEST(AELInfoTest, LookupReturnsAFID)
{
    std::map<std::string, std::string> additionalData{
        {"SENSOR_NAME", "GPU0_TEMP"},
    };
    auto result = resolveAFID("xyz.openbmc_project.Sensor.Threshold."
                              "ReadingAboveLowerCriticalThreshold",
                              additionalData);
    ASSERT_TRUE(result.has_value());

    EXPECT_NE(result->afid, 0ULL);
}

TEST(AELInfoTest, LookupReturnsValidMetadataObject)
{
    std::map<std::string, std::string> additionalData{
        {"SENSOR_NAME", "GPU0_TEMP"},
    };
    auto result = resolveAFID("xyz.openbmc_project.Sensor.Threshold."
                              "ReadingAboveLowerCriticalThreshold",
                              additionalData);
    ASSERT_TRUE(result.has_value());

    EXPECT_NE(result->afid, 0ULL);
    EXPECT_GE(result->origins.size(), 0U);
    EXPECT_GE(result->redfishMappings.size(), 0U);
}

TEST(AELInfoTest, LookupReturnsConsistentResult)
{
    std::map<std::string, std::string> additionalData{
        {"SENSOR_NAME", "GPU0_TEMP"},
    };
    auto result1 = resolveAFID("xyz.openbmc_project.Sensor.Threshold."
                               "ReadingAboveLowerCriticalThreshold",
                               additionalData);
    auto result2 = resolveAFID("xyz.openbmc_project.Sensor.Threshold."
                               "ReadingAboveLowerCriticalThreshold",
                               additionalData);
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_EQ(result1->afid, result2->afid);
    EXPECT_EQ(result1->description, result2->description);
    EXPECT_EQ(result1->origins.size(), result2->origins.size());
    EXPECT_EQ(result1->redfishMappings.size(), result2->redfishMappings.size());
}

TEST(AELInfoTest, EmptyAdditionalDataHandled)
{
    auto result = resolveAFID("xyz.openbmc_project.Sensor.Threshold."
                              "ReadingAboveLowerCriticalThreshold",
                              {});

    EXPECT_TRUE(!result.has_value() || result->afid != 0);
}

} // namespace phosphor::logging::extensions::ael
