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

#include <QObject>
#include <QSharedPointer>

namespace IGCIT {
    class Logger final: public QObject {
        Q_OBJECT

    private:
        static inline QSharedPointer<Logger> instance;

        Logger() = default;

    public:
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

        static QSharedPointer<Logger> getInstance() {
            if (instance.isNull())
                instance.reset(new Logger);

            return instance;
        }

        void writeLog(const QString &msg) {
            emit logSent(msg);
        }

    signals:
        void logSent(const QString &msg);
    };
}
