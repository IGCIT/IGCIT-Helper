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
#include <QScrollArea>
#include <QVBoxLayout>

#include "ToolsPage.h"
#include "Widgets/TDRDelay.h"
#include "Widgets/SSU.h"
#include "Widgets/CrashDumps.h"

namespace IGCIT {
    ToolsPage::ToolsPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QVBoxLayout *scrollWidgLyt = new QVBoxLayout();
        QScrollArea *scrollArea = new QScrollArea();

        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(new QWidget);

        scrollWidgLyt->addWidget(new TDRDelay);
        scrollWidgLyt->addWidget(new SSU);
        scrollWidgLyt->addWidget(new CrashDumps);
        scrollWidgLyt->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
        lyt->addWidget(scrollArea);

        scrollArea->widget()->setLayout(scrollWidgLyt);
        setLayout(lyt);
    }
}
