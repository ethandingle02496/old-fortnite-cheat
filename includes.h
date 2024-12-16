#pragma once
#define WIN32_LEAN_AND_MEAN  
#define _CRT_SECURE_NO_WARNINGS_
#define IMGUI_DEFINE_MATH_OPERATORS

#pragma warning(disable  : 4996).
#pragma warning(disable  : 4530).
#pragma warning(disable  : 4244).

#include <Windows.h>
#include <WinUser.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dwmapi.h>
#include <winioctl.h>
#include <bcrypt.h>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <string>
#include <list>
#include <d3d11.h>
#include <algorithm>
#include <commdlg.h>
#include <ShObjIdl.h>
#include <iomanip>
#include <Pdh.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "pdh.lib")

// framework
#include <base/render/framework/imgui.h>
#include <base/render/framework/imgui_impl_win32.h>
#include <base/render/framework/imgui_impl_dx11.h>
#include <base/render/framework/imgui_edited.hpp>
#include <base/render/framework/imgui_freetype.h>

// internal protection
#include <base/misc/encrypt.hpp>
#include <base/misc/xorstr.h>
#include <base/misc/imports.h>

// inits
#include <base/n_inits.h>

// r/w device
#include <base/kernel/driver/driver.hpp>
#include <base/kernel/mouse/interception.h>
#include <globals.h>
#include <base/render/misc/util.h>

// global headers
#include <cheat/sdk/offsets.h>

#include <cheat/sdk/sdk.hpp>

#include <cheat/sdk/structs.h>

#include <n_ue.h>

#include <cheat/aimkeys.h>

// namespaces
using namespace std;
using namespace settings;
using namespace settings::combat;
using namespace settings::visuals;
using namespace settings::radar;
using namespace settings::world;
using namespace settings::other;