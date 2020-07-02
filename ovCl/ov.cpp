#include <string>
#include <iostream>
#include "ov.h"
#include <thread>
#include <dwmapi.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui_internal.h"
#include <ShlObj.h>
#pragma comment(lib, "dwmapi.lib")



using namespace ImGui;

std::string Status;
int	PasswordAttempt = 0;


// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
ImVec4 clear_color;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Menu
{
	/* DirectX Pointers */
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
	ID3D11BlendState* g_pBlendState = NULL;

	/* Menu Colors */
	ImVec4 TabSlider = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 TabTitleText = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 RedText = { 1.f, 0.0f, 0.0f, .95f };
	ImVec4 ActiveButton = { 0.917, 0.223, 0.223, 1.f };
	ImVec4 GreenText = { 0.f, 1.0f, 0.0f, .95f };
	ImVec4 Underline = { 0.f, 0.749f, 1.f, 1.00f };
	ImVec4 ActiveBut = { 0.2f, 0.2f, 0.231f, 1.00f };
	ImVec4 RegButton = { 0.137f, 0.137f, 0.160f, 1.00f };

	/* Fonts*/
	ImFont* Font12 = NULL;
	ImFont* Font14 = NULL;
	ImFont* Font16 = NULL;
	ImFont* Font18 = NULL;
	ImFont* Font24 = NULL;
	ImFont* Icons = NULL;

	/* Handle*/
	WNDCLASSEX wc = { NULL };
	HWND hwnd = NULL;
	bool bDisplayAdmin = false;

	LPCTSTR Logo = "D:\\Everything\\Overflow\\Loader.png";
	ID3D11ShaderResourceView* LogoTexture = NULL;
}

float getFactorFromDistance(float distance)
{
	if (distance > 200.f)
		return 0.f;
	/*
	if (distance > 100.f)
		return 0.2f;
	if (distance > 50.f)
		return 0.5f;
	if (distance > 30.f)
		return 0.8f;
		*/
	return pow(1 - pow(distance / 200.f, 0.4), 2);
}

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	Menu::g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	Menu::g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

void Menu::Init()
{
	/* Colors */
	SetColorEditOptions(ImGuiColorEditFlags_HEX);

	/* Setup Style */
	ImGuiStyle& style = GetStyle();
	Status = " Rainbow Six Seige";

	style.Alpha = 1.f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f; //4.0 for slight curve
	style.WindowTitleAlign = ImVec2(0.5f, 1.5f);
	style.ChildRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 16.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	ImGuiIO& io = GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_FONTS, NULL, NULL, szPath);

	std::string Path = szPath;
	std::string pTahoma{ Path + "\\Tahoma.ttf" };
	//std::string pRuda{ Path + "\\Ruda-Bold.ttf" };

	Font16 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 16.0f);
	Font12 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 12.0f);
	Font14 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 14.0f);
	Font18 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 18.0f);
	Font24 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 24.0f);

	int my_image_width = 370;
	int my_image_height = 90;

	return;
}

void Menu::RenderMenu(const std::vector<data>& ds)
{

	// Style
	auto& style = GetStyle();

	// Menu
	SetNextWindowPos(ImVec2(0, 0));
	SetNextWindowSize(ImVec2(0, 0));
	static const auto dwFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	Begin("o", 0, dwFlags);
	{
		for (auto p : ds)
		{
			if (p.inGameDistance == 0)
				continue;
			char buff[100];
			sprintf(buff, "%s (%.2fm)", p.pName, p.inGameDistance);
			std::string label = buff;
			float realX = (p.x + 1920) / 2;
			float realY = (-p.y + 1080) / 2;
			if (realX == realY && realX == 0)
				continue;
			float radius = 50;
			ImU32 color;
			if (p.isS)
			{
				color = ImGui::GetColorU32({ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f, 255 / 255.0f });
			}
			else
			{
				color = ImGui::GetColorU32({ 255 / 255.0f, 0, 0, 255 / 255.0f });
			}
			ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 10 + 30 * getFactorFromDistance(p.inGameDistance), ImVec2(realX + 1, realY + 1), color, label.c_str());
			ImGui::GetOverlayDrawList()->AddCircle(ImVec2(realX, realY), radius * getFactorFromDistance(p.inGameDistance),
				color);
		}
	}
	End();
}

void Menu::MenuShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return;
}

void Menu::SetWindowLoop()
{
	while (true)
	{
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		Sleep(1);
	}
}

void Menu::BeginDraw()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWindow(hwnd);
}

void Menu::EndDraw()
{
	// Rendering
	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	ImGui::Render();
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0); // Present with vsync
}

void Menu::Setup(HWND* win)
{
	hwnd = *win;
	// Create application window

	wc = { sizeof(WNDCLASSEX), NULL, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("t"), NULL };
	::RegisterClassEx(&wc);
	/*
	hwnd = CreateWindowEx(WS_EX_LAYERED,
		wc.lpszClassName, _T(""), WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);
		*/
	//hwnd = GetActiveWindow();
	// Initialize Direct3D
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return;
	}

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 1.0f, LWA_ALPHA);
	SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

	// Show the window
	::ShowWindow(hwnd, SW_MAXIMIZE);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Curosr
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// Colors
	Init();

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pd3dDevice->CreateBlendState(&desc, &g_pBlendState);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);

	return;
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &Menu::g_pSwapChain, &Menu::g_pd3dDevice, &featureLevel, &Menu::g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (Menu::g_pSwapChain) { Menu::g_pSwapChain->Release(); Menu::g_pSwapChain = NULL; }
	if (Menu::g_pd3dDeviceContext) { Menu::g_pd3dDeviceContext->Release(); Menu::g_pd3dDeviceContext = NULL; }
	if (Menu::g_pd3dDevice) { Menu::g_pd3dDevice->Release(); Menu::g_pd3dDevice = NULL; }
	return;
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	Menu::g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	Menu::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &Menu::g_mainRenderTargetView);
	pBackBuffer->Release();
	return;
}

void CleanupRenderTarget()
{
	if (Menu::g_mainRenderTargetView) { Menu::g_mainRenderTargetView->Release(); Menu::g_mainRenderTargetView = NULL; }
	return;
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (Menu::g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			Menu::g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
