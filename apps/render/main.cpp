// opens100-render: draws S-100 datasets as a PNG file, with no window. Everything it is asked
// comes from one JSON file (a "job"): the catalogues, the datasets, where to look and how the
// chart is displayed. The reference of the job file is apps/render/README.md; --example prints
// one that can be edited.
//
//   opens100-render <job.json>
//   opens100-render --example
//   opens100-render --help
//
// It ends with 0 when the picture was written, 1 when something of the job could not be read or
// drawn, 2 when the job itself is wrong. With "output.report" it also writes what was drawn (the
// scale and the range of the view, each dataset) as JSON.
#include <GISLibrary/GISLibrary.h>
#include <GISLibrary/SkiaViewFrame.h>
#include <GeoMetryLibrary/ENCCommon.h>
#include <GeoMetryLibrary/GeoCommonFuc.h>
#include <PortrayalCatalogue/Context.h>
#include <PortrayalCatalogue/ContextParameter.h>
#include <PortrayalCatalogue/PortrayalCatalogue.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{
	using Json = nlohmann::json;

	const char* const kExample = R"json({
  "catalogues": [
    {
      "featureCatalogue": "ProgramData/FC/S-101_FC.xml",
      "portrayalCatalogue": "ProgramData/PC/S101_Portrayal/portrayal_catalogue.xml",
      "context": { "SafetyDepth": "20", "SimplifiedSymbols": "false" }
    }
  ],
  "datasets": [ "data/101KR00659C71.000" ],
  "background": "ProgramData/World/World.shp",
  "fonts": [ "fonts/NotoSansKR-Regular.ttf" ],
  "view": {
    "size": [ 1600, 1000 ],
    "projection": "mercator",
    "center": { "latitude": 35.10, "longitude": 129.04 },
    "scale": 25000
  },
  "display": {
    "palette": "Day",
    "mode": "all",
    "text": true,
    "fullSectors": false,
    "applyScaleMin": true,
    "safetyContour": 30,
    "shallowContour": 10,
    "deepContour": 30,
    "symbolScale": 5,
    "fontSize": 15
  },
  "output": {
    "picture": "chart.png",
    "background": "#FFFFFF",
    "report": "chart.json"
  }
}
)json";

	const char* const kHelp =
		"opens100-render: draws S-100 datasets as a PNG file, with no window.\n"
		"\n"
		"  opens100-render <job.json>   draw what the job file says\n"
		"  opens100-render --example    print a job file to start from\n"
		"  opens100-render --help       this text\n"
		"\n"
		"The job file is JSON. A path in it is taken from the directory of the job file unless\n"
		"it is absolute. A name that is not listed here is an error.\n"
		"\n"
		"  catalogues   (required) list, one a product:\n"
		"      featureCatalogue     path of the feature catalogue XML\n"
		"      portrayalCatalogue   path of portrayal_catalogue.xml\n"
		"      context              context parameters of that portrayal catalogue, name: value\n"
		"  datasets     (required) list of dataset paths (.000, .gml, .h5 ...), drawn in this order\n"
		"  background   path of a shapefile drawn under the datasets\n"
		"  fonts        list of font files, the display font first (needed off Windows)\n"
		"  view\n"
		"      size         [width, height] in pixels                       default [1600, 1000]\n"
		"      projection   mercator | polarNorth | polarSouth              default mercator\n"
		"      center       { latitude, longitude } in degrees (WGS 84)\n"
		"      scale        denominator of the scale, 25000 is 1:25 000 (a pixel is 1/38 cm)\n"
		"      bbox         { west, south, east, north } in degrees; not with center or scale\n"
		"    With none of center, scale and bbox the view shows all the datasets. With only\n"
		"    center it keeps that scale; with only scale it keeps that centre.\n"
		"  display\n"
		"      palette          Day | Dusk | Night                           default Day\n"
		"      mode             base | standard | all                        default all\n"
		"      text             draw texts                                   default true\n"
		"      fullSectors      light sectors at their full length           default false\n"
		"      applyScaleMin    hide features under their scale minimum      default true\n"
		"      safetyContour    metres                                       default 30\n"
		"      shallowContour   metres                                       default 10\n"
		"      deepContour      metres                                       default 30\n"
		"      symbolScale      size of symbols                              default 5\n"
		"      fontSize         size of texts                                default 15\n"
		"      fontName         font asked of the system when no font file is given (Windows)\n"
		"    The three contours are also given to each catalogue that has the context parameter\n"
		"    SafetyContour, ShallowContour or DeepContour, unless its \"context\" names it.\n"
		"  output       (required)\n"
		"      picture      (required) path of the PNG file\n"
		"      background   colour under the chart, \"#RRGGBB\"    default white, black at Dusk and Night\n"
		"      report       path of a JSON file telling what was drawn\n"
		"\n"
		"Ends with 0: the picture was written. 1: a file of the job could not be read or drawn.\n"
		"2: the job is wrong.\n";

	// What is wrong with the job, told to the one who wrote it.
	struct JobError : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	struct CataloguePair
	{
		std::filesystem::path features;
		std::filesystem::path portrayal;
		std::map<std::string, std::string> context;
	};

	struct Position
	{
		double latitude = 0;
		double longitude = 0;
	};

	struct Box
	{
		double west = 0;
		double south = 0;
		double east = 0;
		double north = 0;
	};

	struct Job
	{
		std::vector<CataloguePair> catalogues;
		std::vector<std::filesystem::path> datasets;
		std::filesystem::path background;
		std::vector<std::filesystem::path> fonts;

		int width = 1600;
		int height = 1000;
		MapProjection projection = MapProjection::Mercator;
		std::optional<Position> center;
		std::optional<int> scale;
		std::optional<Box> bbox;

		GeoMetryLibrary::ColorTable palette = GeoMetryLibrary::ColorTable::Day;
		GeoMetryLibrary::DisplayModeTable mode = GeoMetryLibrary::DisplayModeTable::all;
		std::optional<bool> text;
		std::optional<bool> fullSectors;
		std::optional<bool> applyScaleMin;
		std::optional<double> safetyContour;
		std::optional<double> shallowContour;
		std::optional<double> deepContour;
		std::optional<double> symbolScale;
		std::optional<int> fontSize;
		std::optional<std::string> fontName;

		std::filesystem::path picture;
		std::optional<GISLibrary::ColorF> backgroundColour;
		std::filesystem::path report;
	};

	std::string Utf8(const std::filesystem::path& path)
	{
		const auto text = path.generic_u8string();
		return std::string(text.begin(), text.end());
	}

	// An object of the job with only the names it may have.
	const Json& Object(const Json& value, const std::string& where, std::initializer_list<std::string_view> names)
	{
		if (!value.is_object())
		{
			throw JobError(where + " must be an object");
		}
		for (const auto& [name, unused] : value.items())
		{
			bool known = false;
			for (const auto allowed : names)
			{
				known = known || allowed == name;
			}
			if (!known)
			{
				std::string list;
				for (const auto allowed : names)
				{
					list += (list.empty() ? "" : ", ") + std::string(allowed);
				}
				throw JobError(where + " has no \"" + name + "\" (it has: " + list + ")");
			}
		}
		return value;
	}

	double Number(const Json& value, const std::string& where)
	{
		if (!value.is_number())
		{
			throw JobError(where + " must be a number");
		}
		return value.get<double>();
	}

	int Integer(const Json& value, const std::string& where)
	{
		if (!value.is_number_integer())
		{
			throw JobError(where + " must be a whole number");
		}
		return value.get<int>();
	}

	bool Boolean(const Json& value, const std::string& where)
	{
		if (!value.is_boolean())
		{
			throw JobError(where + " must be true or false");
		}
		return value.get<bool>();
	}

	std::string Text(const Json& value, const std::string& where)
	{
		if (!value.is_string())
		{
			throw JobError(where + " must be a text");
		}
		return value.get<std::string>();
	}

	// A context parameter is a text for the rules; a number or a boolean of the job is written
	// as the rules read it.
	std::string ParameterText(const Json& value, const std::string& where)
	{
		if (value.is_string())
		{
			return value.get<std::string>();
		}
		if (value.is_boolean())
		{
			return value.get<bool>() ? "true" : "false";
		}
		if (value.is_number())
		{
			return value.dump();
		}
		throw JobError(where + " must be a text, a number or true or false");
	}

	template <typename Value>
	Value OneOf(const Json& value, const std::string& where, std::initializer_list<std::pair<std::string_view, Value>> choices)
	{
		const std::string text = Text(value, where);
		std::string list;
		for (const auto& [name, choice] : choices)
		{
			if (name == text)
			{
				return choice;
			}
			list += (list.empty() ? "" : ", ") + std::string(name);
		}
		throw JobError(where + " is \"" + text + "\"; it must be one of: " + list);
	}

	GISLibrary::ColorF Colour(const Json& value, const std::string& where)
	{
		const std::string text = Text(value, where);
		if (text.size() != 7 || text[0] != '#' || text.find_first_not_of("0123456789abcdefABCDEF", 1) != std::string::npos)
		{
			throw JobError(where + " is \"" + text + "\"; it must be a colour as \"#RRGGBB\"");
		}
		const auto part = [&text](std::size_t at) {
			return static_cast<float>(std::stoi(text.substr(at, 2), nullptr, 16)) / 255.0f;
		};
		return GISLibrary::ColorF{ part(1), part(3), part(5), 1.0f };
	}

	Job ReadJob(const std::filesystem::path& file)
	{
		std::ifstream in(file, std::ios::binary);
		if (!in)
		{
			throw JobError("cannot read the job file " + Utf8(file));
		}
		Json root;
		try
		{
			root = Json::parse(in, nullptr, true, true);   // comments are let through
		}
		catch (const Json::parse_error& error)
		{
			throw JobError(std::string("the job file is not JSON: ") + error.what());
		}

		const auto directory = std::filesystem::absolute(file).parent_path();
		const auto path = [&directory](const Json& value, const std::string& where) {
			const std::string text = Text(value, where);
			if (text.empty())
			{
				throw JobError(where + " is empty");
			}
			const std::filesystem::path given(std::u8string(text.begin(), text.end()));
			return (given.is_absolute() ? given : directory / given).lexically_normal();
		};

		Job job;
		Object(root, "the job", { "catalogues", "datasets", "background", "fonts", "view", "display", "output" });

		if (!root.contains("catalogues") || !root["catalogues"].is_array() || root["catalogues"].empty())
		{
			throw JobError("\"catalogues\" must be a list with at least one pair of catalogues");
		}
		for (std::size_t i = 0; i < root["catalogues"].size(); i++)
		{
			const std::string where = "catalogues[" + std::to_string(i) + "]";
			const Json& item = Object(root["catalogues"][i], where, { "featureCatalogue", "portrayalCatalogue", "context" });
			if (!item.contains("featureCatalogue") || !item.contains("portrayalCatalogue"))
			{
				throw JobError(where + " needs \"featureCatalogue\" and \"portrayalCatalogue\"");
			}
			CataloguePair pair;
			pair.features = path(item["featureCatalogue"], where + ".featureCatalogue");
			pair.portrayal = path(item["portrayalCatalogue"], where + ".portrayalCatalogue");
			if (item.contains("context"))
			{
				if (!item["context"].is_object())
				{
					throw JobError(where + ".context must be an object of name: value");
				}
				for (const auto& [name, value] : item["context"].items())
				{
					pair.context[name] = ParameterText(value, where + ".context." + name);
				}
			}
			job.catalogues.push_back(std::move(pair));
		}

		if (!root.contains("datasets") || !root["datasets"].is_array() || root["datasets"].empty())
		{
			throw JobError("\"datasets\" must be a list with at least one dataset");
		}
		for (std::size_t i = 0; i < root["datasets"].size(); i++)
		{
			job.datasets.push_back(path(root["datasets"][i], "datasets[" + std::to_string(i) + "]"));
		}

		if (root.contains("background"))
		{
			job.background = path(root["background"], "background");
		}
		if (root.contains("fonts"))
		{
			if (!root["fonts"].is_array())
			{
				throw JobError("\"fonts\" must be a list of font files");
			}
			for (std::size_t i = 0; i < root["fonts"].size(); i++)
			{
				job.fonts.push_back(path(root["fonts"][i], "fonts[" + std::to_string(i) + "]"));
			}
		}

		if (root.contains("view"))
		{
			const Json& view = Object(root["view"], "view", { "size", "projection", "center", "scale", "bbox" });
			if (view.contains("size"))
			{
				if (!view["size"].is_array() || view["size"].size() != 2)
				{
					throw JobError("view.size must be [width, height]");
				}
				job.width = Integer(view["size"][0], "view.size[0]");
				job.height = Integer(view["size"][1], "view.size[1]");
				if (job.width < 16 || job.height < 16 || job.width > 16384 || job.height > 16384)
				{
					throw JobError("view.size must be from 16 to 16384 pixels each way");
				}
			}
			if (view.contains("projection"))
			{
				job.projection = OneOf<MapProjection>(view["projection"], "view.projection", {
					{ "mercator", MapProjection::Mercator },
					{ "polarNorth", MapProjection::PolarStereographicNorth },
					{ "polarSouth", MapProjection::PolarStereographicSouth } });
			}
			if (view.contains("center"))
			{
				const Json& center = Object(view["center"], "view.center", { "latitude", "longitude" });
				if (!center.contains("latitude") || !center.contains("longitude"))
				{
					throw JobError("view.center needs \"latitude\" and \"longitude\"");
				}
				Position position;
				position.latitude = Number(center["latitude"], "view.center.latitude");
				position.longitude = Number(center["longitude"], "view.center.longitude");
				if (std::abs(position.latitude) > 90 || std::abs(position.longitude) > 180)
				{
					throw JobError("view.center must be within 90 degrees of latitude and 180 of longitude");
				}
				job.center = position;
			}
			if (view.contains("scale"))
			{
				job.scale = Integer(view["scale"], "view.scale");
				if (*job.scale < 1)
				{
					throw JobError("view.scale must be 1 or more (25000 is 1:25 000)");
				}
			}
			if (view.contains("bbox"))
			{
				const Json& bbox = Object(view["bbox"], "view.bbox", { "west", "south", "east", "north" });
				Box box;
				const auto side = [&bbox](const char* name) {
					if (!bbox.contains(name))
					{
						throw JobError("view.bbox needs \"west\", \"south\", \"east\" and \"north\"");
					}
					return Number(bbox[name], std::string("view.bbox.") + name);
				};
				box.west = side("west");
				box.south = side("south");
				box.east = side("east");
				box.north = side("north");
				if (box.west >= box.east || box.south >= box.north || std::abs(box.south) > 90 || std::abs(box.north) > 90
					|| std::abs(box.west) > 180 || std::abs(box.east) > 180)
				{
					throw JobError("view.bbox must have west < east and south < north, in degrees");
				}
				if (job.center || job.scale)
				{
					throw JobError("view.bbox cannot be given with view.center or view.scale");
				}
				job.bbox = box;
			}
		}

		if (root.contains("display"))
		{
			const Json& display = Object(root["display"], "display", { "palette", "mode", "text", "fullSectors", "applyScaleMin",
				"safetyContour", "shallowContour", "deepContour", "symbolScale", "fontSize", "fontName" });
			if (display.contains("palette"))
			{
				job.palette = OneOf<GeoMetryLibrary::ColorTable>(display["palette"], "display.palette", {
					{ "Day", GeoMetryLibrary::ColorTable::Day },
					{ "Dusk", GeoMetryLibrary::ColorTable::Dusk },
					{ "Night", GeoMetryLibrary::ColorTable::Night } });
			}
			if (display.contains("mode"))
			{
				job.mode = OneOf<GeoMetryLibrary::DisplayModeTable>(display["mode"], "display.mode", {
					{ "base", GeoMetryLibrary::DisplayModeTable::base },
					{ "standard", GeoMetryLibrary::DisplayModeTable::standard },
					{ "all", GeoMetryLibrary::DisplayModeTable::all } });
			}
			if (display.contains("text")) job.text = Boolean(display["text"], "display.text");
			if (display.contains("fullSectors")) job.fullSectors = Boolean(display["fullSectors"], "display.fullSectors");
			if (display.contains("applyScaleMin")) job.applyScaleMin = Boolean(display["applyScaleMin"], "display.applyScaleMin");
			if (display.contains("safetyContour")) job.safetyContour = Number(display["safetyContour"], "display.safetyContour");
			if (display.contains("shallowContour")) job.shallowContour = Number(display["shallowContour"], "display.shallowContour");
			if (display.contains("deepContour")) job.deepContour = Number(display["deepContour"], "display.deepContour");
			if (display.contains("symbolScale")) job.symbolScale = Number(display["symbolScale"], "display.symbolScale");
			if (display.contains("fontSize")) job.fontSize = Integer(display["fontSize"], "display.fontSize");
			if (display.contains("fontName")) job.fontName = Text(display["fontName"], "display.fontName");
		}

		if (!root.contains("output"))
		{
			throw JobError("\"output\" is missing: it names the picture to write");
		}
		const Json& output = Object(root["output"], "output", { "picture", "background", "report" });
		if (!output.contains("picture"))
		{
			throw JobError("output.picture is missing: the path of the PNG file to write");
		}
		job.picture = path(output["picture"], "output.picture");
		if (output.contains("background"))
		{
			job.backgroundColour = Colour(output["background"], "output.background");
		}
		if (output.contains("report"))
		{
			job.report = path(output["report"], "output.report");
		}
		return job;
	}

	// The display settings are those of the process (ENCCommon), as in the viewer.
	void ApplyDisplay(const Job& job)
	{
		ENCCommon::m_eColorTable = job.palette;
		ENCCommon::DISPLAY_MODE = job.mode;
		if (job.text) ENCCommon::TEXTOUT = *job.text;
		if (job.fullSectors) ENCCommon::FULL_SECTORS = *job.fullSectors;
		if (job.applyScaleMin) ENCCommon::APPLY_SCALE_MIN = *job.applyScaleMin;
		if (job.safetyContour) ENCCommon::SAFETY_CONTOUR = *job.safetyContour;
		if (job.shallowContour) ENCCommon::SHALLOW_CONTOUR = *job.shallowContour;
		if (job.deepContour) ENCCommon::DEEP_CONTOUR = *job.deepContour;
		if (job.symbolScale) ENCCommon::DISPLAY_SYMBOL_SCALE = static_cast<float>(*job.symbolScale);
		if (job.fontSize) ENCCommon::DISPLAY_FONT_SIZE = *job.fontSize;
		if (job.fontName) ENCCommon::DISPLAY_FONT_NAME = *job.fontName;
	}

	std::string NumberText(double value)
	{
		return Json(value).dump();
	}

	// The context parameters of a catalogue: those the job names, and the contours of the display
	// where the catalogue has them and the job did not name them. Returns what was set.
	std::map<std::string, std::string> ApplyContext(const Job& job, const CataloguePair& pair, PortrayalCatalogue& catalogue)
	{
		std::map<std::string, std::string> values = pair.context;
		const auto contour = [&values](const char* name, const std::optional<double>& value) {
			if (value && !values.contains(name))
			{
				values[name] = NumberText(*value);
			}
		};
		const bool named[3] = { pair.context.contains("SafetyContour"), pair.context.contains("ShallowContour"), pair.context.contains("DeepContour") };
		contour("SafetyContour", job.safetyContour);
		contour("ShallowContour", job.shallowContour);
		contour("DeepContour", job.deepContour);

		auto* context = catalogue.GetContext();
		std::map<std::string, std::string> set;
		for (const auto& [name, value] : values)
		{
			auto* parameter = context ? context->GetContextParameter(name) : nullptr;
			if (!parameter)
			{
				const bool fromDisplay = (name == "SafetyContour" && !named[0]) || (name == "ShallowContour" && !named[1])
					|| (name == "DeepContour" && !named[2]);
				if (fromDisplay)
				{
					continue;   // a contour of the display, for a catalogue that has no such parameter
				}
				std::string list;
				for (int i = 0; context && i < context->GetCountOfParameter(); i++)
				{
					if (auto* known = context->GetContextParameter(i))
					{
						list += (list.empty() ? "" : ", ") + known->GetId();
					}
				}
				throw JobError("the portrayal catalogue " + Utf8(pair.portrayal) + " has no context parameter \"" + name
					+ "\" (it has: " + (list.empty() ? "none" : list) + ")");
			}
			parameter->setValue(value);
			set[name] = value;
		}
		return set;
	}

	const char* PaletteName(GeoMetryLibrary::ColorTable palette)
	{
		switch (palette)
		{
		case GeoMetryLibrary::ColorTable::Dusk: return "Dusk";
		case GeoMetryLibrary::ColorTable::Night: return "Night";
		default: return "Day";
		}
	}

	int Run(const Job& job)
	{
		Json report;
		report["picture"] = Utf8(job.picture);
		report["size"] = { job.width, job.height };
		report["palette"] = PaletteName(job.palette);
		report["datasets"] = Json::array();
		report["catalogues"] = Json::array();
		std::string failure;

		const auto writeReport = [&job, &report, &failure](bool drawn) {
			if (job.report.empty())
			{
				return true;
			}
			report["drawn"] = drawn;
			if (!failure.empty())
			{
				report["error"] = failure;
			}
			std::error_code ignored;
			std::filesystem::create_directories(job.report.parent_path(), ignored);
			std::ofstream out(job.report, std::ios::binary | std::ios::trunc);
			out << report.dump(2) << "\n";
			return static_cast<bool>(out);
		};
		const auto fail = [&failure, &writeReport](const std::string& message) {
			failure = message;
			std::cerr << "opens100-render: " << message << "\n";
			writeReport(false);
			return 1;
		};

		ApplyDisplay(job);
		SetMapProjection(job.projection);

		CGISLibraryApp view;
		for (const CataloguePair& pair : job.catalogues)
		{
			if (!view.GetCatalogManager()->addFC(pair.features.wstring()))
			{
				return fail("cannot read the feature catalogue " + Utf8(pair.features));
			}
			auto* catalogue = view.GetCatalogManager()->addPC(pair.portrayal.wstring());
			if (!catalogue)
			{
				return fail("cannot read the portrayal catalogue " + Utf8(pair.portrayal));
			}
			Json entry;
			entry["featureCatalogue"] = Utf8(pair.features);
			entry["portrayalCatalogue"] = Utf8(pair.portrayal);
			entry["context"] = ApplyContext(job, pair, *catalogue);
			report["catalogues"].push_back(std::move(entry));
		}
		view.GetLayerManager()->ChangeS100ColorPalette(job.palette);

		ScreenRect screen{};
		screen.right = job.width;
		screen.bottom = job.height;
		view.SetScreen(screen);

		if (!job.background.empty() && !view.AddBackgroundLayer(job.background.wstring()))
		{
			return fail("cannot read the background " + Utf8(job.background));
		}

		const auto opening = std::chrono::steady_clock::now();
		for (const auto& dataset : job.datasets)
		{
			// Opening a layer reads the dataset and runs the portrayal rules of its product.
			const bool opened = view.AddLayer(dataset.wstring());
			report["datasets"].push_back({ { "path", Utf8(dataset) }, { "opened", opened } });
			if (!opened)
			{
				return fail("cannot open the dataset " + Utf8(dataset));
			}
		}
		const auto drawing = std::chrono::steady_clock::now();

		// Where the view looks. The layers left the map range that holds them all.
		if (job.bbox)
		{
			double west = job.bbox->west, south = job.bbox->south, east = job.bbox->east, north = job.bbox->north;
			projection(west, south);
			projection(east, north);
			view.SetMap(std::min(west, east), std::min(south, north), std::max(west, east), std::max(south, north));
		}
		// The map range is given the shape of the picture.
		view.SetViewMBR(screen);
		if (job.scale)
		{
			view.SetScale(*job.scale);
		}
		if (job.center)
		{
			double x = job.center->longitude, y = job.center->latitude;
			projection(x, y);
			view.MoveMap(job.width / 2, job.height / 2, x, y);
		}
		if (job.scale)
		{
			// The scale is true where the view is: set again at the centre it was moved to.
			view.SetScale(*job.scale);
		}

		GISLibrary::SkiaViewFrame frame;
		frame.SetFontFiles(job.fonts);
		frame.Draw(view);
		const auto drawn = std::chrono::steady_clock::now();

		const GISLibrary::ColorF background = job.backgroundColour ? *job.backgroundColour
			: job.palette == GeoMetryLibrary::ColorTable::Day ? GISLibrary::ColorF{ 1.0f, 1.0f, 1.0f, 1.0f }
			: GISLibrary::ColorF{ 0.0f, 0.0f, 0.0f, 1.0f };
		std::error_code ignored;
		std::filesystem::create_directories(job.picture.parent_path(), ignored);
		if (!frame.WritePng(job.picture, background))
		{
			return fail("cannot write the picture " + Utf8(job.picture));
		}

		const auto ms = [](auto from, auto to) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
		};
		MBR range;
		view.GetMap(&range);
		double west = range.xmin, south = range.ymin, east = range.xmax, north = range.ymax;
		double centerX = (range.xmin + range.xmax) / 2, centerY = (range.ymin + range.ymax) / 2;
		inverseProjection(west, south);
		inverseProjection(east, north);
		inverseProjection(centerX, centerY);

		report["scale"] = view.GetCurrentScale();
		report["center"] = { { "latitude", centerY }, { "longitude", centerX } };
		// The corners of the picture: lower left and upper right.
		report["bbox"] = { { "west", west }, { "south", south }, { "east", east }, { "north", north } };
		report["milliseconds"] = { { "open", ms(opening, drawing) }, { "draw", ms(drawing, drawn) } };
		if (!writeReport(true))
		{
			std::cerr << "opens100-render: cannot write the report " << Utf8(job.report) << "\n";
			return 1;
		}

		std::cout << "picture: " << Utf8(job.picture) << " (" << job.width << " x " << job.height << ")\n"
			<< "scale:   1:" << view.GetCurrentScale() << "\n"
			<< "center:  " << centerY << ", " << centerX << "\n"
			<< "layers:  " << job.datasets.size() << ", opened in " << ms(opening, drawing) << " ms, drawn in " << ms(drawing, drawn) << " ms\n";
		return 0;
	}
}

int main(int argc, char** argv)
{
	const std::string_view argument = argc == 2 ? argv[1] : "";
	if (argument == "--help" || argument == "-h")
	{
		std::cout << kHelp;
		return 0;
	}
	if (argument == "--example")
	{
		std::cout << kExample;
		return 0;
	}
	if (argc != 2 || argument.starts_with("--"))
	{
		std::cerr << kHelp;
		return 2;
	}

	try
	{
		return Run(ReadJob(std::filesystem::path(argv[1])));
	}
	catch (const JobError& error)
	{
		std::cerr << "opens100-render: " << error.what() << "\n";
		return 2;
	}
	catch (const std::exception& error)
	{
		std::cerr << "opens100-render: " << error.what() << "\n";
		return 1;
	}
}
