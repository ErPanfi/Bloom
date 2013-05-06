//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "Windows.h"
#include <string>

namespace mcg
{
	int showErrorMessageBox(LPCTSTR lpText, LPCTSTR lpTitle = L"Error", HWND hWnd = nullptr);
	int showWarningMessageBox(LPCTSTR lpText, LPCTSTR lpTitle = L"Waring", HWND hWnd = nullptr);

	HRESULT readTextFile(const std::string& fileName, std::string& outText);

	std::wstring toWString(const std::string& str);
	std::wstring toWString(const char* const str);
}
