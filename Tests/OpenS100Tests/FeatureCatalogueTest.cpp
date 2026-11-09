#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <pugixml.hpp>
#include "TestPaths.h"
#include "FeatureCatalogue.h"

TEST(FeatureCatalogue, ReadsBundledS101Catalogue)
{
	auto path = testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml";
	ASSERT_TRUE(std::filesystem::exists(path)) << path;

	FeatureCatalogue fc;
	EXPECT_TRUE(fc.Read(path.string()));
}

TEST(FeatureCatalogue, ReadReturnsFalseForMissingFile)
{
	FeatureCatalogue fc;
	EXPECT_FALSE(fc.Read(std::string("Z:\\does\\not\\exist\\FC.xml")));
}

TEST(FeatureCatalogue, PathConstructorAndProductId)
{
	const auto path = testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml";
	FeatureCatalogue fromNarrow(path.string());
	FeatureCatalogue fromWide(path.wstring());
	EXPECT_TRUE(fromNarrow.isProductId("S-101"));
	EXPECT_TRUE(fromWide.isProductId("S-101"));
	EXPECT_FALSE(fromWide.isProductId("S-102"));
	EXPECT_EQ(fromWide.GetVersionNumber(), fromNarrow.GetVersionNumber());
	EXPECT_FALSE(fromWide.GetVersionNumber().empty());

	FeatureCatalogue missing(std::string("Z:/does/not/exist/FC.xml"));
	EXPECT_TRUE(missing.getProductId().empty());
}

TEST(FeatureCatalogue, FindsAssociationBindings)
{
	FeatureCatalogue fc((testpaths::repoRoot() / "ProgramData" / "FC" / "S-101_FC.xml").string());
	ASSERT_TRUE(fc.isProductId("S-101"));

	int featureBindings = 0;
	int informationBindings = 0;
	for (auto* featureType : fc.GetFeatureTypes()->GetVecFeatureType())
	{
		for (auto* binding : featureType->GetFeatureBindingPointer())
		{
			for (const auto& target : binding->GetFeatureTypes())
			{
				// Every listed target resolves to a binding of the same association and role.
				auto* found = fc.FindFeatureBinding(featureType, target);
				ASSERT_NE(found, nullptr) << featureType->GetCode() << " -> " << target;
				const auto listed = found->GetFeatureTypes();
				EXPECT_NE(std::find(listed.begin(), listed.end(), target), listed.end());
				EXPECT_EQ(fc.FindFeatureBinding(featureType->GetCode(), target), found);
				EXPECT_EQ(fc.FindFeatureBinding(featureType->GetCodeW(), std::wstring(target.begin(), target.end())), found);
				++featureBindings;
			}
		}
		for (auto* binding : featureType->GetInformationBindingPointer())
		{
			for (const auto& target : binding->GetInformationTypes())
			{
				EXPECT_NE(fc.FindInformationBinding(featureType, target), nullptr) << featureType->GetCode() << " -> " << target;
				++informationBindings;
			}
		}
		EXPECT_EQ(fc.FindFeatureBinding(featureType, std::string("NoSuchFeatureType")), nullptr);
		EXPECT_EQ(fc.FindInformationBinding(featureType, std::string("NoSuchInformationType")), nullptr);
	}
	EXPECT_GT(featureBindings, 0);
	EXPECT_GT(informationBindings, 0);

	EXPECT_EQ(fc.FindFeatureBinding(static_cast<FeatureType*>(nullptr), std::string("Landmark")), nullptr);
	EXPECT_EQ(fc.FindFeatureBinding(std::string("NoSuchFeatureType"), std::string("Landmark")), nullptr);
}

namespace
{
	// Checks one binding: what it admits and what Find...Binding() resolves to it must agree.
	// Returns the number of admitted types that are not listed, i.e. come in through a super type.
	template <typename Find>
	size_t CheckAdmitted(const std::string& what, const std::vector<std::string>& listed,
		const std::vector<std::string>& admitted, const std::vector<std::string>& allCodes, Find&& find)
	{
		std::vector<std::string> sorted = admitted;
		std::sort(sorted.begin(), sorted.end());
		EXPECT_EQ(std::adjacent_find(sorted.begin(), sorted.end()), sorted.end()) << what << ": a code is admitted twice";

		for (const auto& code : listed)
		{
			EXPECT_TRUE(std::binary_search(sorted.begin(), sorted.end(), code)) << what << ": listed " << code << " is not admitted";
		}
		for (const auto& code : admitted)
		{
			EXPECT_TRUE(find(code)) << what << ": admits " << code << " but no binding is found for it";
		}
		for (const auto& code : allCodes)
		{
			if (!std::binary_search(sorted.begin(), sorted.end(), code))
			{
				EXPECT_FALSE(find(code)) << what << ": " << code << " resolves to a binding that does not admit it";
			}
		}

		std::vector<std::string> uniqueListed = listed;
		std::sort(uniqueListed.begin(), uniqueListed.end());
		uniqueListed.erase(std::unique(uniqueListed.begin(), uniqueListed.end()), uniqueListed.end());
		return admitted.size() - uniqueListed.size();
	}
}

TEST(FeatureCatalogue, AdmittedTypesIncludeDerivedTypes)
{
	// Catalogues whose types use super types (S-101 has none).
	size_t derivedFeatureTypes = 0;
	size_t derivedInformationTypes = 0;
	for (const char* file : { "S-122_FC.xml", "S-123_FC.xml", "S-127_FC.xml", "S-131_FC.xml", "S-201_FC.xml" })
	{
		FeatureCatalogue fc((testpaths::repoRoot() / "ProgramData" / "FC" / file).string());
		ASSERT_FALSE(fc.getProductId().empty()) << file;

		std::vector<std::string> featureCodes;
		for (auto* type : fc.GetFeatureTypes()->GetVecFeatureType())
		{
			featureCodes.push_back(type->GetCode());
		}
		std::vector<std::string> informationCodes;
		for (auto* type : fc.GetInformationTypes()->GetVecInformationType())
		{
			informationCodes.push_back(type->GetCode());
		}

		// Any binding of the source may answer for a code; "admitted by the source" is the union.
		for (auto* source : fc.GetFeatureTypes()->GetVecFeatureType())
		{
			std::vector<std::string> listed, admitted;
			for (auto* binding : source->GetFeatureBindingPointer())
			{
				for (const auto& code : binding->GetFeatureTypes()) listed.push_back(code);
				for (const auto& code : fc.AdmittedFeatureTypes(binding))
				{
					if (std::find(admitted.begin(), admitted.end(), code) == admitted.end()) admitted.push_back(code);
				}
			}
			derivedFeatureTypes += CheckAdmitted(std::string(file) + " " + source->GetCode() + " (feature)", listed, admitted, featureCodes,
				[&](const std::string& code) { return fc.FindFeatureBinding(source, code) != nullptr; });

			listed.clear();
			admitted.clear();
			for (auto* binding : source->GetInformationBindingPointer())
			{
				for (const auto& code : binding->GetInformationTypes()) listed.push_back(code);
				for (const auto& code : fc.AdmittedInformationTypes(binding))
				{
					if (std::find(admitted.begin(), admitted.end(), code) == admitted.end()) admitted.push_back(code);
				}
			}
			derivedInformationTypes += CheckAdmitted(std::string(file) + " " + source->GetCode() + " (information)", listed, admitted, informationCodes,
				[&](const std::string& code) { return fc.FindInformationBinding(source, code) != nullptr; });
		}
	}
	EXPECT_GT(derivedFeatureTypes + derivedInformationTypes, 0u) << "no binding reached a type through its super type; the test checks nothing";

	FeatureCatalogue empty;
	EXPECT_TRUE(empty.AdmittedFeatureTypes(nullptr).empty());
	EXPECT_TRUE(empty.AdmittedInformationTypes(nullptr).empty());
}
