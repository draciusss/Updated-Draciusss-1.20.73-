#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

#include "imgui.h"

#include <optional>
#include <vector>

struct ID3D11Device;
struct IDXGISurface;
struct ID2D1Bitmap1;

namespace ImFX {
	enum class FXMode
	{
		QUALITY,
		NORMAL,
		PERFORMANCE
	};
	void SetFXMode(FXMode mode);
	FXMode GetFXMode();
	bool NewFrame(ID3D11Device* d3d11Device, IDXGISurface* backBuffer, float dpi);
	bool EndFrame();
	bool CleanupFX();
	bool Begin(ImDrawList* drawList, bool dontCopy = false);
	bool End(bool composite = false);
	bool PushLayer(std::optional<ImVec4> clipRect = std::optional<ImVec4>());
	bool PopLayer();
	bool AddBlur(float strength, std::optional<ImVec4> clipRect = std::optional<ImVec4>(), float rounding = ImGui::GetStyle().FrameRounding);
};