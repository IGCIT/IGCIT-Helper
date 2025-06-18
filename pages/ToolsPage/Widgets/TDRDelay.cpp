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
#include <QPushButton>

#include "TDRDelay.h"
#include "../../../Logger.h"
#include "../../../include/registryUtils.h"

namespace IGCIT {
    TDRDelay::TDRDelay(): QGroupBox("TDR Delay") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *tdrLyt = new QHBoxLayout();
        QHBoxLayout *tdrDdiLyt = new QHBoxLayout();
        QHBoxLayout *btnLyt = new QHBoxLayout();
        QPushButton *applyBtn = new QPushButton("Apply");
        QPushButton *resetBtn = new QPushButton("Reset");
        QLabel *note = new QLabel("A reboot is required for the changes to take effect");
        QFont noteFont = note->font();

        tdr = new QSpinBox();
        tdrDdi = new QSpinBox();

        tdr->setValue(getRegDword(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDelay", 2));
        tdrDdi->setValue(getRegDword(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDdiDelay", 5));
        noteFont.setItalic(true);
        noteFont.setPointSize(9);
        note->setFont(noteFont);

        tdrLyt->addWidget(new QLabel("TdrDelay (seconds)"));
        tdrLyt->addWidget(tdr);
        tdrDdiLyt->addWidget(new QLabel("TdrDdiDelay (seconds)"));
        tdrDdiLyt->addWidget(tdrDdi);
        btnLyt->addWidget(resetBtn);
        btnLyt->addWidget(applyBtn);
        lyt->addLayout(tdrLyt);
        lyt->addLayout(tdrDdiLyt);
        lyt->addLayout(btnLyt);
        lyt->addItem(new QSpacerItem(1, 4, QSizePolicy::Expanding, QSizePolicy::Fixed));
        lyt->addWidget(note);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(applyBtn, &QPushButton::clicked, this, &TDRDelay::onApplyBtnClicked);
        QObject::connect(resetBtn, &QPushButton::clicked, this, &TDRDelay::onResetBtnClicked);
    }

    void TDRDelay::onApplyBtnClicked() const {
        const bool tdrRes = setRegDword(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDelay", tdr->value());
        const bool tdrDdiRes = setRegDword(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDdiDelay", tdrDdi->value());

        if (tdrRes && tdrDdiRes)
            Logger::getInstance()->writeLog("TDRDelay::apply: no errors");
    }

    void TDRDelay::onResetBtnClicked() const {
        const bool delTdrRes = deleteRegValue(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDelay");
        const bool delTdrDdiRes = deleteRegValue(HKEY_LOCAL_MACHINE, tdrPath, L"TdrDdiDelay");

        if (delTdrRes && delTdrDdiRes)
            Logger::getInstance()->writeLog("TDRDelay::reset: no errors");

        tdr->setValue(2);
        tdrDdi->setValue(5);
    }
}
