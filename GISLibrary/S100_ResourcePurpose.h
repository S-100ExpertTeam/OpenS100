#pragma once

using namespace std;

namespace S100 {
	enum class S100_ResourcePurpose
	{
		supportFile = 1,
		ISOMetadata,
		languagePack,
		GMLSchema,
		other = 100
	};


    inline string S100_ResourcePurposeToString(S100_ResourcePurpose category) {
        switch (category) {
        case S100_ResourcePurpose::supportFile: return "supportFile";
        case S100_ResourcePurpose::ISOMetadata: return "ISOMetadata";
        case S100_ResourcePurpose::languagePack: return "languagePack";
        case S100_ResourcePurpose::GMLSchema: return "GMLSchema";
        case S100_ResourcePurpose::other: return "other";
        default: throw invalid_argument("Unknown category");
        }
    }

    inline S100_ResourcePurpose S100_ResourcePurposeFromString(const string& categoryName) {
        if (categoryName == "supportFile") return S100_ResourcePurpose::supportFile;
        if (categoryName == "ISOMetadata") return S100_ResourcePurpose::ISOMetadata;
        if (categoryName == "languagePack") return S100_ResourcePurpose::languagePack;
        if (categoryName == "GMLSchema") return S100_ResourcePurpose::GMLSchema;
        if (categoryName == "other") return S100_ResourcePurpose::other;
        throw invalid_argument("Unknown category name");
    }

}
