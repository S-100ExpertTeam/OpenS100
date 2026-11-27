// The data the portrayal rules ask the host for (host_data): what a dataset answers about its
// features, without a rule being run.
#include <gtest/gtest.h>

#include <string>

#include "GF_FeatureType.h"
#include "S100SpatialObject.h"
#include "host_data.h"

namespace
{
	// A dataset of one feature.
	class OneFeature : public S100SpatialObject
	{
	public:
		GF::FeatureType feature;

		bool Open(const std::wstring&) override { return true; }
		bool OpenMetadata(const std::wstring&) override { return true; }

		int GetFeatureCount() override { return 1; }
		GF::FeatureType* GetFeatureTypeByIndex(int index) override { return index == 0 ? &feature : nullptr; }
	};
}

// The rules make a spatial association of each one the host gives, and one with no spatial type
// is an error of the rule file ("nil" not of type "SpatialType"): the feature was left with no
// portrayal at all. With an empty list the rules take it for a feature of primitive type None.
TEST(HostData, AFeatureWithNoGeometryHasNoSpatialAssociation)
{
	OneFeature dataset;
	dataset.feature.id = "7";

	hd_init(static_cast<S100SpatialObject*>(&dataset));
	const auto associations = hd_get_feature_spatial_associations("7");
	hd_delete();

	EXPECT_TRUE(associations.empty());
}
