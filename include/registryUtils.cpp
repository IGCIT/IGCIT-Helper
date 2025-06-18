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
#include "registryUtils.h"
#include "../Logger.h"

namespace IGCIT {
    static bool setRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, LPCVOID data, const DWORD dataType, const DWORD dataSz, const REGSAM openSAM) {
        HKEY hkey;
        LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, openSAM, &hkey);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("setRegValue: Failed to open %1, code %2").arg(subKey).arg(ret));
            return false;
        }

        ret = RegSetKeyValue(hkey, nullptr, value, dataType, data, dataSz);
        if (ret != ERROR_SUCCESS)
            Logger::getInstance()->writeLog(QString("setRegValue: Failed to set %1, code %2").arg(value).arg(ret));

        RegCloseKey(hkey);
        return ret == ERROR_SUCCESS;
    }

    QString getRegSZValue(HKEY key, LPCWSTR subKey, LPCWSTR value) {
        std::unique_ptr<TCHAR[]> buf;
        DWORD bufLen;
        HKEY hkey;
        LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, KEY_READ, &hkey);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("getRegSZValue: failed to open %1, code %2").arg(subKey).arg(ret));
            return "";
        }

        ret = RegGetValue(hkey, nullptr, value, RRF_RT_REG_SZ, nullptr, nullptr, &bufLen);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("getRegSZValue: failed to get buffer length for %1, code %2").arg(value).arg(ret));
            RegCloseKey(hkey);
            return "";
        }

        buf = std::make_unique<TCHAR[]>(++bufLen);

        ret = RegGetValue(hkey, nullptr, value, RRF_RT_REG_SZ, nullptr, buf.get(), &bufLen);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("getRegSZValue: failed to get %1, code %2").arg(value).arg(ret));
            RegCloseKey(hkey);
            return "";
        }

        RegCloseKey(hkey);
        return QString(buf.get());
    }

    DWORD getRegDword(HKEY key, LPCWSTR subKey, LPCWSTR value, const DWORD defaultValue) {
        DWORD dwSz = sizeof(DWORD);
        DWORD dw;
        HKEY hkey;
        LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, KEY_READ, &hkey);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("getRegDword: Failed to open %1, code %2").arg(subKey).arg(ret));
            return 0;
        }

        ret = RegGetValue(hkey, nullptr, value, RRF_RT_DWORD, nullptr, &dw, &dwSz);
        if (ret != ERROR_SUCCESS && ret != ERROR_FILE_NOT_FOUND) {
            Logger::getInstance()->writeLog(QString("getRegDword: Failed to get %1, code %2").arg(value).arg(ret));
            RegCloseKey(hkey);
            return 0;
        }

        RegCloseKey(hkey);
        return ret == ERROR_FILE_NOT_FOUND ? defaultValue : dw;
    }

    bool setRegDword(HKEY key, LPCWSTR subKey, LPCWSTR value, const DWORD data, const REGSAM openSAM) {
        return setRegValue(key, subKey, value, &data, REG_DWORD, sizeof(DWORD), openSAM);
    }

    bool setRegExpandSZ(HKEY key, LPCWSTR subKey, LPCWSTR value, LPCWSTR data, const REGSAM openSAM) {
        return setRegValue(key, subKey, value, data, REG_EXPAND_SZ, wcslen(data) + sizeof(TCHAR), openSAM);
    }

    bool setRegMultiSZ(HKEY key, LPCWSTR subKey, LPCWSTR value, LPCWSTR data, const REGSAM openSAM) {
        return setRegValue(key, subKey, value, data, REG_MULTI_SZ, wcslen(data) + (sizeof(TCHAR) * 2), openSAM);
    }

    bool deleteRegValue(HKEY key, LPCWSTR subKey, LPCWSTR value, const REGSAM openSAM) {
        HKEY hkey;
        LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, openSAM, &hkey);
        if (ret != ERROR_SUCCESS && ret != ERROR_FILE_NOT_FOUND) {
            Logger::getInstance()->writeLog(QString("deleteRegValue: Failed to open %1, code %2").arg(subKey).arg(ret));
            return false;

        } else if (ret == ERROR_FILE_NOT_FOUND) {
            return true;
        }

        ret = RegDeleteValue(hkey, value);
        if (ret != ERROR_SUCCESS && ret != ERROR_FILE_NOT_FOUND) {
            Logger::getInstance()->writeLog(QString("deleteRegValue: Failed to delete %1, code %2").arg(value).arg(ret));
            RegCloseKey(hkey);
            return false;
        }

        RegCloseKey(hkey);
        return true;
    }

    bool deleteRegTree(HKEY key, LPCWSTR subKey, LPCWSTR value) {
        HKEY hkey;
        LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, KEY_SET_VALUE, &hkey);
        if (ret != ERROR_SUCCESS && ret != ERROR_FILE_NOT_FOUND) {
            Logger::getInstance()->writeLog(QString("deleteRegTree: Failed to open %1, code %2").arg(subKey).arg(ret));
            return false;

        } else if (ret == ERROR_FILE_NOT_FOUND) {
            return true;
        }

        ret = RegDeleteTree(hkey, value);
        if (ret != ERROR_SUCCESS)
            Logger::getInstance()->writeLog(QString("deleteRegTree: Failed to delete %1, code %2").arg(subKey).arg(ret));

        RegCloseKey(hkey);
        return ret == ERROR_SUCCESS;
    }

    bool createRegKey(HKEY key, LPCWSTR subKey, LPCWSTR createKey, const REGSAM openSAM, const REGSAM createSAM) {
        HKEY hkey;
        HKEY ckey;
	    LSTATUS ret;

        ret = RegOpenKeyEx(key, subKey, 0, openSAM, &hkey);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("createRegKey: Failed to open %1, code %2").arg(subKey).arg(ret));
            return false;
        }

        ret = RegCreateKeyEx(hkey, createKey, 0, nullptr, 0, createSAM, nullptr, &ckey, nullptr);
        if (ret != ERROR_SUCCESS) {
            Logger::getInstance()->writeLog(QString("createRegKey: Failed to create %1, code %2").arg(createKey).arg(ret));
            RegCloseKey(hkey);
            return false;
        }

        RegCloseKey(ckey);
        RegCloseKey(hkey);
        return true;
    }
}
