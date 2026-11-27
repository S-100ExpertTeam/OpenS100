#pragma once
// The Windows end of a frame: lays the pixels of a SkiaViewFrame on a GDI device context, over
// what is already there. The library names no type of Windows; this is of the viewer, and the
// tests draw through it as the viewer does.
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/SkiaViewFrame.h"

#include <cstring>

#pragma comment(lib, "msimg32.lib")   // AlphaBlend

class DeviceContextTarget final : public GISLibrary::FrameTarget
{
public:
	DeviceContextTarget() = default;
	explicit DeviceContextTarget(HDC deviceContext) : target(deviceContext)
	{
	}
	~DeviceContextTarget() override
	{
		Release();
	}
	DeviceContextTarget(const DeviceContextTarget&) = delete;
	DeviceContextTarget& operator=(const DeviceContextTarget&) = delete;

	// The device context the frames to come are laid on.
	void On(HDC deviceContext)
	{
		target = deviceContext;
	}

	void Show(const GISLibrary::FramePixels& pixels) override
	{
		if (!target || !pixels.data || !Size(pixels.width, pixels.height))
		{
			return;
		}
		// AlphaBlend takes its source from a device context: the pixels go to a bitmap of ours.
		const std::size_t rowBytes = static_cast<std::size_t>(width) * 4;
		for (int row = 0; row < height; row++)
		{
			std::memcpy(static_cast<unsigned char*>(bits) + rowBytes * row, pixels.data + pixels.rowBytes * row, rowBytes);
		}
		const BLENDFUNCTION blend{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		::AlphaBlend(target, pixels.left, pixels.top, width, height, memory, 0, 0, width, height, blend);
	}

private:
	bool Size(int newWidth, int newHeight)
	{
		if (bitmap && newWidth == width && newHeight == height)
		{
			return true;
		}
		Release();
		if (newWidth <= 0 || newHeight <= 0)
		{
			return false;
		}
		BITMAPINFO info{};
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth = newWidth;
		info.bmiHeader.biHeight = -newHeight;   // top row first, as the frame has it
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		memory = ::CreateCompatibleDC(nullptr);
		bitmap = memory ? ::CreateDIBSection(memory, &info, DIB_RGB_COLORS, &bits, nullptr, 0) : nullptr;
		if (!bitmap || !bits)
		{
			Release();
			return false;
		}
		previous = ::SelectObject(memory, bitmap);
		width = newWidth;
		height = newHeight;
		return true;
	}

	void Release()
	{
		if (memory)
		{
			if (previous)
			{
				::SelectObject(memory, previous);
			}
			::DeleteDC(memory);
			memory = nullptr;
		}
		if (bitmap)
		{
			::DeleteObject(bitmap);
			bitmap = nullptr;
		}
		previous = nullptr;
		bits = nullptr;
		width = 0;
		height = 0;
	}

	HDC target = nullptr;
	HDC memory = nullptr;
	HBITMAP bitmap = nullptr;
	HGDIOBJ previous = nullptr;
	void* bits = nullptr;
	int width = 0;
	int height = 0;
};

// The target of the process, as the frame of the process (SkiaViewFrame::Shared): its bitmap is
// kept from one refresh to the next.
inline DeviceContextTarget& SharedDeviceContextTarget(HDC deviceContext)
{
	static DeviceContextTarget target;
	target.On(deviceContext);
	return target;
}

// Draws a view on a device context with the frame of the process. Everything a refresh draws is
// one scene: the layers are rasterised together and put on the device context once.
inline void DrawOnDeviceContext(CGISLibraryApp& view, HDC deviceContext, int offset = 0)
{
	auto& frame = GISLibrary::SkiaViewFrame::Shared();
	const GISLibrary::SkiaViewFrame::Scene scene(frame, &SharedDeviceContextTarget(deviceContext));
	view.Draw(frame, offset);
}
