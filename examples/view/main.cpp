// A view of several layers drawn with no window: the datasets named on the command line are
// opened as the layers of a view, the view is drawn once on the Skia frame, and the picture is
// written as a PNG file. The same code on Windows and Linux.
//
//   opens100_view <feature catalogue xml> <portrayal catalogue xml> <picture.png>
//                 [--background <shapefile>] [--font <file>]... [--size <width> <height>]
//                 [--catalogues <feature catalogue xml> <portrayal catalogue xml>]... <dataset>...
//
// The first two arguments are the catalogues of one product; --catalogues adds those of another.
#include <GISLibrary/GISLibrary.h>
#include <GISLibrary/SkiaViewFrame.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		std::cerr << "usage: opens100_view <feature catalogue xml> <portrayal catalogue xml> <picture.png>"
			" [--background <shapefile>] [--font <file>]... [--size <width> <height>]"
			" [--catalogues <feature catalogue xml> <portrayal catalogue xml>]... <dataset>...\n";
		return 2;
	}

	struct Catalogues
	{
		std::filesystem::path features;
		std::filesystem::path portrayal;
	};
	std::vector<Catalogues> catalogues{ { argv[1], argv[2] } };
	const std::filesystem::path picture(argv[3]);
	std::filesystem::path background;
	std::vector<std::filesystem::path> fonts;
	std::vector<std::filesystem::path> datasets;
	int width = 1600;
	int height = 1000;
	for (int i = 4; i < argc; i++)
	{
		const std::string argument(argv[i]);
		if (argument == "--background" && i + 1 < argc)
		{
			background = argv[++i];
		}
		else if (argument == "--font" && i + 1 < argc)
		{
			fonts.emplace_back(argv[++i]);
		}
		else if (argument == "--size" && i + 2 < argc)
		{
			width = std::atoi(argv[++i]);
			height = std::atoi(argv[++i]);
		}
		else if (argument == "--catalogues" && i + 2 < argc)
		{
			catalogues.push_back({ argv[i + 1], argv[i + 2] });
			i += 2;
		}
		else
		{
			datasets.emplace_back(argument);
		}
	}
	if (datasets.empty() || width <= 0 || height <= 0)
	{
		std::cerr << "no dataset, or no size\n";
		return 2;
	}

	CGISLibraryApp view;
	for (const Catalogues& pair : catalogues)
	{
		if (!view.GetCatalogManager()->addFC(pair.features.string()))
		{
			std::cerr << "cannot read " << pair.features.string() << "\n";
			return 1;
		}
		if (!view.GetCatalogManager()->addPC(pair.portrayal.wstring()))
		{
			std::cerr << "cannot read " << pair.portrayal.string() << "\n";
			return 1;
		}
	}

	ScreenRect screen{};
	screen.right = width;
	screen.bottom = height;
	view.SetScreen(screen);

	if (!background.empty() && !view.AddBackgroundLayer(background.wstring()))
	{
		std::cerr << "cannot read " << background.string() << "\n";
		return 1;
	}
	const auto opening = std::chrono::steady_clock::now();
	for (const auto& dataset : datasets)
	{
		// Opening a layer reads the dataset and runs the portrayal rules of its product.
		if (!view.AddLayer(dataset.wstring()))
		{
			std::cerr << "cannot open " << dataset.string() << "\n";
			return 1;
		}
	}
	// The map range the layers left is given the shape of the picture.
	view.SetViewMBR(screen);
	const auto drawing = std::chrono::steady_clock::now();

	GISLibrary::SkiaViewFrame frame;
	frame.SetFontFiles(fonts);
	frame.Draw(view);
	const auto drawn = std::chrono::steady_clock::now();

	if (!frame.WritePng(picture, GISLibrary::ColorF{ 1.0f, 1.0f, 1.0f, 1.0f }))
	{
		std::cerr << "cannot write " << picture.string() << "\n";
		return 1;
	}
	const auto ms = [](auto from, auto to) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
	};
	std::cout << "layers:  " << datasets.size() << ", opened in " << ms(opening, drawing) << " ms\n"
		<< "picture: " << picture.string() << " (" << width << " x " << height << "), drawn in " << ms(drawing, drawn) << " ms\n";
	return 0;
}
