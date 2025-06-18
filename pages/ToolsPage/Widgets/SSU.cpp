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
#define SECURITY_WIN32
#include "../../../include/win.h"
#include <security.h>
#include <lmcons.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLabel>

#include "SSU.h"
#include "../../../Logger.h"

namespace IGCIT {
    SSU::SSU(): QGroupBox("SSU") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QPushButton *anonBtn = new QPushButton("Anonymize");
        QLabel *note = new QLabel("This function may not remove all data, please check the output before posting online.");
        QFont noteFont = note->font();

        noteFont.setItalic(true);
        noteFont.setPointSize(9);
        note->setFont(noteFont);
        note->setWordWrap(true);

        lyt->addWidget(anonBtn);
        lyt->addWidget(note);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(anonBtn, &QPushButton::clicked, this, &SSU::onAnonBtnClick);
    }

    void SSU::onAnonBtnClick() {
        const QString ssu = QFileDialog::getOpenFileName(this, "Open SSU Report", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Text files (*.txt)");

        if (ssu.isEmpty())
            return;

        constexpr DWORD nameLen = UNLEN + 1;
        QFile ssuFile {ssu};
        const QString outPath = QString("%1/igcit_ssu.txt").arg(QFileInfo(ssuFile).absolutePath());
        QFile ssuOut {outPath};
        TCHAR username[nameLen] = {0};
        TCHAR samUname[nameLen] = {0};
        DWORD usernameLen = nameLen;
        DWORD samUnameLen = nameLen;
        QString ssuCont;
        BOOL ret;

        if (!ssuFile.open(QIODevice::ReadOnly)) {
            Logger::getInstance()->writeLog("Failed to open ssu file");
            return;
        }

        if (!ssuOut.open(QIODevice::WriteOnly)) {
            Logger::getInstance()->writeLog(QString("Failed to create output file: %1").arg(outPath));
            ssuFile.close();
            return;
        }

        ssuCont = ssuFile.readAll();

        for (const QRegularExpression &rex: rexList)
            ssuCont.replace(rex, "");

        ret = GetUserNameEx(EXTENDED_NAME_FORMAT::NameSamCompatible, samUname, &samUnameLen);
        if (ret > 0)
            ssuCont.replace(QString(samUname), "");

        ret = GetUserName(username, &usernameLen);
        if (ret > 0)
            ssuCont.replace(QString(username), "");

        if (ssuOut.write(ssuCont.toUtf8()) == -1)
            Logger::getInstance()->writeLog(QString("Failed to write %1").arg(outPath));
        else
            Logger::getInstance()->writeLog(QString("ssu saved to: %1").arg(outPath));

        ssuOut.close();
        ssuFile.close();
    }
}
