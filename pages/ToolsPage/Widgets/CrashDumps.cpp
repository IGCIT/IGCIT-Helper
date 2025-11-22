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
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QProgressDialog>
#include <QDirListing>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDir>
#include <QLabel>

#include "CrashDumps.h"
#include "../../../Logger.h"
#include "../../../include/registryUtils.h"

namespace IGCIT {
    CrashDumps::CrashDumps(): QGroupBox("Crash Dumps") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *row1 = new QHBoxLayout();
        QHBoxLayout *row2 = new QHBoxLayout();
        QPushButton *enableBtn = new QPushButton("Enable");
        QPushButton *fixWatchdogBtn = new QPushButton("Fix Watchdog dumps");
        QPushButton *clearDumpsBtn = new QPushButton("Clear dump folders");
        QPushButton *resetBtn = new QPushButton("Restore defaults");
        QPushButton *collectBtn = new QPushButton("Extract dumps");
        QLabel *note = new QLabel("Enable/Restore: a reboot is required for the changes to take effect");
        QFont noteFont = note->font();

        resetBtn->setToolTip("Reset Windows dump settings to default, you must enable them again to generate all dumps");
        noteFont.setItalic(true);
        noteFont.setPointSize(9);
        note->setFont(noteFont);

        row1->addWidget(enableBtn);
        row1->addWidget(resetBtn);
        row1->addWidget(clearDumpsBtn);
        row2->addWidget(fixWatchdogBtn);
        row2->addWidget(collectBtn);

        lyt->addLayout(row1);
        lyt->addLayout(row2);
        lyt->addSpacing(4);
        lyt->addWidget(note);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(enableBtn, &QPushButton::clicked, this, &CrashDumps::onEnableBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &CrashDumps::onResetBtnClicked);
        QObject::connect(clearDumpsBtn, &QPushButton::clicked, this, &CrashDumps::onClearBtnClicked);
        QObject::connect(fixWatchdogBtn, &QPushButton::clicked, this, &CrashDumps::onFixWatchdogBtnClicked);
        QObject::connect(collectBtn, &QPushButton::clicked, this, &CrashDumps::onCollectBtnClicked);
    }

    void CrashDumps::onEnableBtnClicked() const {
        const std::tuple<HKEY, LPCWSTR, LPCWSTR, REGSAM, REGSAM> createKeys[] {
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting)", L"LocalDumps", KEY_CREATE_SUB_KEY | KEY_WOW64_64KEY, KEY_SET_VALUE | KEY_WOW64_64KEY),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl)", L"LiveKernelReports", KEY_CREATE_SUB_KEY, KEY_SET_VALUE),
        };
        const std::tuple<HKEY, LPCWSTR, LPCWSTR, REGSAM, DWORD> setKeys[] {
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting)", L"Disabled", KEY_SET_VALUE, 1),
            std::make_tuple(HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\Windows Error Reporting)", L"Disabled", KEY_SET_VALUE, 1),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting\LocalDumps)", L"DumpType", KEY_SET_VALUE | KEY_WOW64_64KEY, 2),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl\LiveKernelReports)", L"DeleteLiveMiniDumps", KEY_SET_VALUE, 0),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl)", L"CrashDumpEnabled", KEY_SET_VALUE, 1),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl)", L"FilterPages", KEY_SET_VALUE, 1)
        };

        for (const auto &[key, subKey, createKey, openSam, createSam]: createKeys) {
            if (!createRegKey(key, subKey, createKey, openSam, createSam))
                return;
        }

        for (const auto &[key, subKey, setKey, openSam, value]: setKeys) {
            if (!setRegDword(key, subKey, setKey, value, openSam))
                return;
        }

        if (!setRegExpandSZ(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting\LocalDumps)", L"DumpFolder", LR"("C:\AppCrashDumps")", KEY_SET_VALUE | KEY_WOW64_64KEY))
            return;

        if (!setRegMultiSZ(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management)", L"PagingFiles", L"C:\\pagefile.sys 17400 17400\0"))
            return;

        Logger::getInstance()->writeLog(QStringLiteral("crash dumps enabled successfully"));
    }

    void CrashDumps::onResetBtnClicked() const {
        const std::tuple<HKEY, LPCWSTR, LPCWSTR, REGSAM> regKeys[] {
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting)", L"Disabled", KEY_SET_VALUE),
            std::make_tuple(HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\Windows Error Reporting)", L"Disabled", KEY_SET_VALUE),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(Software\Microsoft\Windows\Windows Error Reporting\LocalDumps)", L"DumpType", KEY_SET_VALUE | KEY_WOW64_64KEY),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl\LiveKernelReports)", L"DeleteLiveMiniDumps", KEY_SET_VALUE),
            std::make_tuple(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl)", L"FilterPages", KEY_SET_VALUE)
        };

        for (const auto &[key, subKey, value, openSam]: regKeys) {
            if (!deleteRegValue(key, subKey, value, openSam))
                return;
        }

        if (!setRegDword(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl)", L"CrashDumpEnabled", 3))
            Logger::getInstance()->writeLog(QStringLiteral("CrashDumpEnabled not set, skipping.."));

        if (!setRegMultiSZ(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management)", L"PagingFiles", L"?:\\pagefile.sys\0"))
            Logger::getInstance()->writeLog(QStringLiteral("PagingFiles not set, skipping.."));

        Logger::getInstance()->writeLog(QStringLiteral("crash dumps restored successfully"));
    }

    void CrashDumps::onClearBtnClicked() {
        if (QMessageBox::question(this, "Clear dump folders", "Do you want to clear all dumps?") == QMessageBox::No)
            return;

        const QDirListing dumpFolders[] = {
            QDirListing(R"(C:\AppCrashDumps)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive),
            QDirListing(R"(C:\Windows\Minidump)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive),
            QDirListing(R"(C:\Windows\LiveKernelReports\WATCHDOG)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive)
        };
        QProgressDialog progress("Deleting files...", "Abort", 0, 3, this);
        int prog = 1;

        for (const QDirListing &dit: dumpFolders) {
            if (progress.wasCanceled()) {
                progress.close();
                return;
            }

            for (const QDirListing::DirEntry &entry: dit) {
                if (progress.wasCanceled()) {
                    progress.close();
                    return;
                }

                if (!QFile::remove(entry.absoluteFilePath()))
                    Logger::getInstance()->writeLog(QString("failed to delete %1").arg(entry.absoluteFilePath()));
            }

            progress.setValue(prog++);
        }

        Logger::getInstance()->writeLog("all dumps cleared");
    }

    void CrashDumps::onCollectBtnClicked() {
	    const QString outFold {QFileDialog::getExistingDirectory(this, "Select output folder", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))};

        if (outFold.isEmpty())
            return;

        const QDirListing dumpFolders[] = {
            QDirListing(R"(C:\AppCrashDumps)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive),
            QDirListing(R"(C:\Windows\Minidump)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive),
            QDirListing(R"(C:\Windows\LiveKernelReports\WATCHDOG)", QDirListing::IteratorFlag::FilesOnly | QDirListing::IteratorFlag::Recursive)
        };
        QProgressDialog progress("Collecting files...", "Abort", 0, 3, this);
        int prog = 1;
        QDir qdir {};

        for (const QDirListing &dit: dumpFolders) {
            if (progress.wasCanceled()) {
                progress.close();
                return;
            }

            for (const QDirListing::DirEntry &entry: dit) {
                if (progress.wasCanceled()) {
                    progress.close();
                    return;
                }

                if (!qdir.rename(entry.absoluteFilePath(), QString("%1/%2").arg(outFold, entry.fileName())))
                    Logger::getInstance()->writeLog(QString("failed to collect %1").arg(entry.absoluteFilePath()));
            }

            progress.setValue(prog++);
        }

        QDesktopServices::openUrl(QUrl::fromLocalFile(outFold));
    }

    void CrashDumps::onFixWatchdogBtnClicked() const {
        if (deleteRegTree(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\CrashControl\LiveKernelReports)", L"WATCHDOG"))
            Logger::getInstance()->writeLog(QStringLiteral("watchdog fix applied"));
    }
}
