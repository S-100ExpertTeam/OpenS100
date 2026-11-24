#pragma once
// Opening datasets off the thread that draws.
//
// A chart opens in most of a second (reading the file, building its geometry, running the
// portrayal rules), and a view asks for several at once. Opened by the thread that draws, the
// window stands still for all of them. A DatasetLayerSet that is told to (BackgroundOpening)
// hands them to one worker thread instead, goes on drawing what it has, and takes the layers that
// are ready when it next draws.
//
// There is one worker for the process, and one lock every opening holds: the portrayal rules run
// in one Lua state with one set of host data, so two datasets are never opened at a time, whoever
// opens them.
#include <functional>
#include <memory>
#include <mutex>
#include <string>

class Layer;

namespace GISLibrary
{
	// Held while a dataset is opened and portrayed, on any thread. The thread that holds it may
	// take it again.
	[[nodiscard]] std::recursive_mutex& OpeningLock();

	// Whether the worker may open the file: an ISO 8211 dataset (S-101, extension .000 to .999).
	// The others are opened where they are asked for: a gridded coverage sets what the display
	// reads while it draws (the outlines the depth features are hidden under), and the datasets
	// portrayed by XSLT go through files of fixed names.
	[[nodiscard]] bool OpensInBackground(const std::wstring& path);

	// Whether, and how, the layer sets of the process open their datasets in the background.
	class BackgroundOpening
	{
	public:
		// Called on the worker thread each time a dataset has been opened (or has failed to):
		// the application asks its window to draw again, from that thread, without waiting
		// (PostMessage). Several may finish before the window draws; it draws once for all of
		// them.
		using Ready = std::function<void()>;

		// Turns background opening on, with what is called when a dataset is ready. Without
		// this, datasets are opened where they are asked for.
		static void Enable(Ready ready);
		static void Disable();
		[[nodiscard]] static bool Enabled();

		// Runs a task on the worker, after the ones queued before it.
		static void Run(std::function<void()> task);
		// Tells the application something is ready (called by the tasks).
		static void NotifyReady();
		// Comes back when the worker has nothing left to do. For a caller that must have
		// everything it asked for (a test, a picture to export).
		static void WaitUntilIdle();
	};
}
