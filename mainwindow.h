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

#include <QMainWindow>
#include <QTabWidget>

#include "pages/LogPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final: public QMainWindow {
    Q_OBJECT

private:
	Ui::MainWindow *ui = nullptr;
	QTabWidget *tabWidget = nullptr;
	IGCIT::LogPage *logTab = nullptr;
	int logPageIdx = 0;

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

private slots:
	void onLogSent(const QString &msg) const;
	void onTabWidgetTabChange(int idx) const;
};
