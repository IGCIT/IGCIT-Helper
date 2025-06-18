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

#include <QGroupBox>
#include <QSpinBox>

namespace IGCIT {
    class TDRDelay final: public QGroupBox {
        Q_OBJECT

    private:
        static constexpr wchar_t tdrPath[] = LR"(SYSTEM\CurrentControlSet\Control\GraphicsDrivers)";
        QSpinBox *tdr = nullptr;
        QSpinBox *tdrDdi = nullptr;

    public:
        TDRDelay();

    private slots:
        void onApplyBtnClicked() const;
        void onResetBtnClicked() const;
    };
}
