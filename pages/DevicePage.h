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

#include <QWidget>

#include "../include/registryUtils.h"
#include "../Logger.h"

namespace IGCIT {
    class DevicePage final: public QWidget {
    private:
        QSharedPointer<Logger> logger;

        [[nodiscard]] QString getProcessorString() const;
        [[nodiscard]] QString getManufacturer() const;
        [[nodiscard]] QString getProductName() const;
        [[nodiscard]] QString getRAMString() const;
        [[nodiscard]] QList<std::pair<QString, QString>> getGPUList() const;

    public:
        DevicePage();
    };
}
