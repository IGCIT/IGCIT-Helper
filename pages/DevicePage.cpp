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
#include <QVBoxLayout>
#include <QLabel>

#include "DevicePage.h"

namespace IGCIT {
    DevicePage::DevicePage() {
        const QList<std::pair<QString, QString>> gpuList = getGPUList();
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *osLyt = new QHBoxLayout();
        QHBoxLayout *cpuLyt = new QHBoxLayout();
        QHBoxLayout *ramLyt = new QHBoxLayout();
        QHBoxLayout *modelLyt = new QHBoxLayout();
        QHBoxLayout *manufLyt = new QHBoxLayout();
        QLabel *osInfoLbl = new QLabel("System");
        QLabel *gpusInfoLbl = new QLabel("GPUs");
        QLabel *osLbl = new QLabel(QString("%1 (build %2)").arg(QSysInfo::prettyProductName(), QSysInfo::kernelVersion()));
        QLabel *cpuLbl = new QLabel(getProcessorString());
        QLabel *modelLbl = new QLabel(getProductName());
        QLabel *manufLbl = new QLabel(getManufacturer());
        QFont osInfoF = osInfoLbl->font();
        QFont gpusInfoF = gpusInfoLbl->font();

        logger = Logger::getInstance();

        osLbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        cpuLbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        modelLbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        manufLbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        osInfoF.setBold(true);
        osInfoF.setPointSize(11);
        osInfoLbl->setAlignment(Qt::AlignCenter);
        osInfoLbl->setFont(osInfoF);
        gpusInfoF.setBold(true);
        gpusInfoF.setPointSize(11);
        gpusInfoLbl->setAlignment(Qt::AlignCenter);
        gpusInfoLbl->setFont(gpusInfoF);

        osLyt->addWidget(new QLabel("OS version:"));
        osLyt->addStretch();
        osLyt->addWidget(osLbl);

        cpuLyt->addWidget(new QLabel("Processor:"));
        cpuLyt->addStretch();
        cpuLyt->addWidget(cpuLbl);

        ramLyt->addWidget(new QLabel("RAM:"));
        ramLyt->addStretch();
        ramLyt->addWidget(new QLabel(getRAMString()));

        modelLyt->addWidget(new QLabel("Model:"));
        modelLyt->addStretch();
        modelLyt->addWidget(modelLbl);

        manufLyt->addWidget(new QLabel("Manufacturer:"));
        manufLyt->addStretch();
        manufLyt->addWidget(manufLbl);

        lyt->addWidget(osInfoLbl);
        lyt->addSpacing(4);
        lyt->addLayout(osLyt);
        lyt->addLayout(cpuLyt);
        lyt->addLayout(ramLyt);
        lyt->addLayout(modelLyt);
        lyt->addLayout(manufLyt);
        lyt->addWidget(gpusInfoLbl);
        lyt->addSpacing(4);

        if (gpuList.isEmpty()) {
            QLabel *noGpuLbl = new QLabel("no GPU found");

            noGpuLbl->setAlignment(Qt::AlignCenter);
            lyt->addWidget(noGpuLbl);
        }

        for (const auto &[gpu, driver]: gpuList) {
            QHBoxLayout *gpuLyt = new QHBoxLayout();
            QLabel *gpuLbl = new QLabel(QString("%1:").arg(gpu));

            gpuLbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

            gpuLyt->addWidget(gpuLbl);
            gpuLyt->addStretch();
            gpuLyt->addWidget(new QLabel(driver));

            lyt->addLayout(gpuLyt);
        }

        setLayout(lyt);
    }

    QString DevicePage::getProcessorString() const {
        return getRegSZValue(HKEY_LOCAL_MACHINE, LR"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", L"ProcessorNameString");
    }

    QString DevicePage::getManufacturer() const {
        return getRegSZValue(HKEY_LOCAL_MACHINE, LR"(HARDWARE\DESCRIPTION\System\BIOS)", L"SystemManufacturer");
    }

    QString DevicePage::getProductName() const {
        return getRegSZValue(HKEY_LOCAL_MACHINE, LR"(HARDWARE\DESCRIPTION\System\BIOS)", L"SystemProductName");
    }

    QString DevicePage::getRAMString() const {
        constexpr char sizeStr[][8] = {"bytes", "KB", "MB", "GB"};
        MEMORYSTATUSEX memStat {};
        int sizeIdx = 0;
        double sz;

        memStat.dwLength = sizeof(memStat);

        if (GlobalMemoryStatusEx(&memStat) == 0) {
            logger->writeLog(QStringLiteral("getRAMString: failed to get status"));
            return "";
        }

        sz = memStat.ullTotalPhys;

        while (static_cast<int>(sz / 1024.f) > 0) {
            sz /= 1024.f;
            ++sizeIdx;

            if (sizeIdx >= 4)
                return "Unknown";
        }

        return QString("%1 %2").arg(QString::number(sz, 'g', 3), sizeStr[sizeIdx]);
    }

    QList<std::pair<QString, QString>> DevicePage::getGPUList() const {
        constexpr TCHAR regMachineStr[] = LR"(\Registry\Machine\)";
        constexpr TCHAR basicDisplayStr[] = L"BasicDisplay";
        constexpr size_t regMachineStrLen = std::wstring_view(regMachineStr).size();
        QList<std::pair<QString, QString>> gpuList;
        std::unique_ptr<TCHAR[]> nameBuf;
        std::unique_ptr<BYTE[]> valueBuf;
        DWORD maxValueLen;
        DWORD maxNameLen;
        DWORD valueCount;
        HKEY hkey;
        LSTATUS ret;

	    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, LR"(HARDWARE\DEVICEMAP\VIDEO)", 0, KEY_READ, &hkey);
        if (ret != ERROR_SUCCESS) {
            logger->writeLog(QString("getGPUList: failed to open key, code %1").arg(ret));
            return {};
        }

	    ret = RegQueryInfoKey(hkey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &valueCount, &maxNameLen, &maxValueLen, nullptr, nullptr);
        if (ret != ERROR_SUCCESS) {
            logger->writeLog(QString("getGPUList: failed to query key, code %1").arg(ret));
		    RegCloseKey(hkey);
            return {};
        }

        nameBuf = std::make_unique<TCHAR[]>(++maxNameLen);
        valueBuf = std::make_unique<BYTE[]>(++maxValueLen);

        for (int i=0; i<valueCount; ++i) {
            DWORD nameLen = maxNameLen;
            DWORD valueLen = maxValueLen;
            int valueBufOfft = 0;
            std::pair<QString, QString> gpu;
            LPWSTR valueBufW;
            DWORD keyType;

            wmemset(nameBuf.get(), 0, maxNameLen);
            memset(valueBuf.get(), 0, maxValueLen);

            ret = RegEnumValue(hkey, i, nameBuf.get(), &nameLen, nullptr, &keyType, valueBuf.get(), &valueLen);
            if (ret != ERROR_SUCCESS) {
                logger->writeLog(QString("getGPUList: failed to enum value, code %1").arg(ret));
                continue;
            }

            valueBufW = reinterpret_cast<LPWSTR>(valueBuf.get());
            if (keyType != REG_SZ || wcsstr(nameBuf.get(), L"Video") == nullptr || wcsstr(valueBufW, basicDisplayStr) != nullptr)
                continue;

            if (wcsncmp(valueBufW, regMachineStr, regMachineStrLen) == 0)
                valueBufOfft = regMachineStrLen;

            gpu = std::make_pair(
                getRegSZValue(HKEY_LOCAL_MACHINE, valueBufW + valueBufOfft, L"DriverDesc"),
                getRegSZValue(HKEY_LOCAL_MACHINE, valueBufW + valueBufOfft, L"DriverVersion")
            );

            if (gpu.first.isEmpty() || gpu.second.isEmpty()) {
                logger->writeLog(QString("GPU name or driver version is empty [%1, %2], skip..").arg(gpu.first, gpu.second));
                continue;
            }

            gpuList.append(gpu);
        }

		RegCloseKey(hkey);
        return gpuList;
    }
}
