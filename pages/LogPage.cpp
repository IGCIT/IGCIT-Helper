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

#include "LogPage.h"

namespace IGCIT {
    LogPage::LogPage() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *clearLyt = new QHBoxLayout();
        QPushButton *clearBtn = new QPushButton("Clear");

        logTx = new QTextEdit();

        logTx->setWordWrapMode(QTextOption::NoWrap);
        logTx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        logTx->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        logTx->setReadOnly(true);
        clearBtn->setMinimumWidth(60);

        clearLyt->addStretch();
        clearLyt->addWidget(clearBtn);
        lyt->addWidget(logTx, 1);
        lyt->addLayout(clearLyt);

        setLayout(lyt);

        QObject::connect(clearBtn, &QPushButton::clicked, this, &LogPage::onClearBtnClicked);
    }

    void LogPage::onClearBtnClicked() const {
        logTx->clear();
    }
}
