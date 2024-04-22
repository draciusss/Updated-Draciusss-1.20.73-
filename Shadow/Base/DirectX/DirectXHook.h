#pragma once

// Include Fonts
#include "MojangLes.h" // MojangLes's font.
#include "ProductSans.h" // ProductSans's font.

void* oPresent;
bool initContext = false;
ID3D11Device* d3d11Device = nullptr;
ID3D12Device5* d3d12Device = nullptr;
int Width = 0;
int Height = 0;

// Include LoadTextureFromFile
#include "LoadTexture.h"

void callRender() { // onImGui CallBack for modules
	ImGui::GetIO().FontGlobalScale = 0.5f;

	// ImGui Render stuff here
	ImGuiRenderEvent event{}; // ImGuiRenderEvent
	event.cancelled = nullptr;
	CallBackEvent(&event); // Call ImGui event for modules to be writen on this hook.
}

void loadFonts() { // load ImGui font.
	// Add ProductSans's font from Memory Compressed TTF
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ProductSansCompressedData, ProductSansCompressedSize, 48.f);
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(MojangLesCompressedData, MojangLesCompressedSize, 48.f);

	ImGui::GetStyle().WindowRounding = 13.f; // Set the default Windows Rounding to 8
	//LoadTextureFromFile("C:\\Users\\Rasky1\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\vope\\otu.png", &Global::RenderInfo::Texture, &Width, &Height);
}

HRESULT D3D12_PresentDetour(IDXGISwapChain3* swapchain, UINT syncInterval, UINT flags) {
	Global::RenderInfo::Window = FindWindowA(nullptr, "Minecraft"); // Find Minecraft's Window 
	if (!Global::RenderInfo::Window) // If Minecraft's title is not "Minecraft"
		Global::RenderInfo::Window = FindWindowA(nullptr, "Minecraft: Developer Edition"); // Check if it's Developer Edition
	if (!Global::RenderInfo::Window) // If Minecraft's window = nullptr
		return Utils::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);

	if (!SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
			swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			static_cast<ID3D12Device5*>(d3d12Device)->RemoveDevice();
		}
		return Utils::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
	}
	ID3D11DeviceContext* ppContext = nullptr;
	d3d11Device->GetImmediateContext(&ppContext);
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	ID3D11RenderTargetView* mainRenderTargetView;
	d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
	pBackBuffer->Release();
	if (!initContext) {
		ImGui::CreateContext();
		loadFonts();
		ImGui_ImplWin32_Init(Global::RenderInfo::Window);
		ImGui_ImplDX11_Init(d3d11Device, ppContext);
		initContext = true;
	};
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	callRender();
	ImGui::EndFrame();
	ImGui::Render();
	ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mainRenderTargetView->Release();
	d3d11Device->Release();
	return Utils::CallFunc<HRESULT, IDXGISwapChain3*, UINT, UINT>(oPresent, swapchain, syncInterval, flags);
}

class DirectXHook : public FuncHook {
public:
	bool Initialize() override {
		if (kiero::init(kiero::RenderType::D3D12) != kiero::Status::Success && kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
			//log("['Hidden', DirectX] Failed to create hook\n");
			return false;
		}
		const int index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8;
		kiero::bind(index, (void**)&oPresent, D3D12_PresentDetour);
		//log("[DirectX] Successfully Enabled hook\n");
		return true;
	}

	static DirectXHook& Instance() {
		static DirectXHook instance;
		return instance;
	}
};