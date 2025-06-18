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
#include <QRegularExpression>

namespace IGCIT {
    class SSU final: public QGroupBox {
        Q_OBJECT

    private:
        const QList<QRegularExpression> rexList {
            QRegularExpression {R"((\s+MAC\sAddress:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+IP\s(Address|Subnet):\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Default\sIP\sGateway:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Access\sPoint:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Serial\sNumber:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Machine\sname:.*$))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Machine\sId:.*$))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Network\sName:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Profile:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption},
            QRegularExpression {R"((\s+Net\sConnection\sID:\".*\"))", QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption}
        };

    public:
        SSU();

    private slots:
        void onAnonBtnClick();
    };
}
