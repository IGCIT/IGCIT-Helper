/*
 * This file is part of IGCIT Helper.
 * Copyright (C) 2025 IGCIT
 *
 * PowerMax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PowerMax is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "win.h"

#include <QString>

namespace IGCIT {
        [[nodiscard]] QString getRegSZValue(HKEY key, LPCWSTR subKey, LPCWSTR value);
        [[nodiscard]] DWORD getRegDword(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD defaultValue);
        [[nodiscard]] bool setRegDword(HKEY key, LPCWSTR subKey, LPCWSTR value, DWORD data, REGSAM openSAM = KEY_SET_VALUE);
        [[nodiscard]] bool setRegExpandSZ(HKEY key, LPCWSTR subKey, LPCWSTR value, LPCWSTR data, REGSAM openSAM = KEY_SET_VALUE);
        [[nodiscard]] bool setRegMultiSZ(HKEY key, LPCWSTR subKey, LPCWSTR value, LPCWSTR data, REGSAM openSAM = KEY_SET_VALUE);
        [[nodiscard]] bool deleteRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, REGSAM openSAM = KEY_SET_VALUE);
        [[nodiscard]] bool deleteRegTree(HKEY key, LPCWSTR subKey, LPCWSTR value);
        [[nodiscard]] bool createRegKey(HKEY key, LPCWSTR subKey, LPCWSTR createKey, REGSAM openSAM, REGSAM createSAM);
}
