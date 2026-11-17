// Smallest useful program on top of the installed opens100 package:
// read an S-100 feature catalogue and report what is in it.
#include <FeatureCatalog/FeatureCatalogue.h>
#include <FeatureCatalog/FeatureTypes.h>
#include <GeoMetryLibrary/MBR.h>
#include <PortrayalCatalogue/PortrayalCatalogue.h>
#ifdef OPENS100_CONSUMER_HAS_DATASET
#include <GISLibrary/S101Cell.h>
#endif

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "usage: opens100_consumer <feature catalogue xml> [portrayal catalogue xml] [S-101 cell]\n";
		return 2;
	}

	FeatureCatalogue fc;
	if (!fc.Read(std::string(argv[1])))
	{
		std::cerr << "cannot read " << argv[1] << "\n";
		return 1;
	}

	auto* featureTypes = fc.GetFeatureTypes();
	std::cout << "product:       " << fc.getProductId() << "\n"
		<< "version:       " << fc.GetVersionNumber() << "\n"
		<< "feature types: " << (featureTypes ? featureTypes->GetVecFeatureType().size() : 0) << "\n";

	// A second module, to prove more than one target links.
	MBR korea(124.0, 33.0, 132.0, 39.0);
	std::cout << "Busan inside the Korean box: " << std::boolalpha << korea.PtInMBR(129.04, 35.10) << "\n";

	// Optional: a portrayal catalogue, read as data (no resource store, so nothing is drawn).
	if (argc >= 3)
	{
		PortrayalCatalogue pc;
		if (!pc.Open(std::string(argv[2])))
		{
			std::cerr << "cannot read " << argv[2] << "\n";
			return 1;
		}

		auto* mainRule = pc.GetMainRuleFile();
		std::cout << "portrayal:     " << pc.GetProduct() << " " << pc.GetVersion() << "\n"
			<< "rule files:    " << pc.GetRules()->GetRuleFiles().size() << "\n"
			<< "symbols:       " << pc.GetSymbols()->GetSymbolsVector()->size() << "\n"
			<< "viewing groups: " << pc.GetViewingGroups()->GetViewingGroup()->size() << "\n"
			<< "main rule:     " << (mainRule ? mainRule->GetFileName() : "(none)") << "\n";
	}

#ifdef OPENS100_CONSUMER_HAS_DATASET
	// Optional: an S-101 dataset (ISO 8211), read against the feature catalogue above.
	if (argc >= 4)
	{
		const std::filesystem::path cellPath(argv[3]);
		S101Cell cell(&fc);
		if (!cell.Open(cellPath.wstring()))
		{
			std::cerr << "cannot read " << argv[3] << "\n";
			return 1;
		}

		std::cout << "dataset:       " << cellPath.filename().string() << "\n"
			<< "features:      " << cell.GetCount_FeatureTypeRecord() << "\n"
			<< "information:   " << cell.GetCount_InformationRecord() << "\n"
			<< "points:        " << cell.GetCount_PointRecord() << "\n"
			<< "curves:        " << cell.GetCount_CurveRecord() << "\n"
			<< "surfaces:      " << cell.GetCount_SurfaceRecord() << "\n";
	}
#endif
	return 0;
}
