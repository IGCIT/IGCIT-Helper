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

#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "pages/DevicePage.h"
#include "pages/ToolsPage/ToolsPage.h"

MainWindow::MainWindow() {
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	QVBoxLayout *lyt = new QVBoxLayout();
	QHBoxLayout *bottomLyt = new QHBoxLayout();
	QLabel *wikiLink = new QLabel(R"(<a href="https://github.com/IGCIT/Intel-GPU-Community-Issue-Tracker-IGCIT/wiki/IGCIT-Helper">IGCIT Wiki</a>)");

	tabWidget = new QTabWidget();
	logTab = new IGCIT::LogPage();

	tabWidget->addTab(new IGCIT::DevicePage, "Device");
	tabWidget->addTab(new IGCIT::ToolsPage, "Tools");
	logPageIdx = tabWidget->addTab(logTab, "Logs");
	tabWidget->setCurrentIndex(0);
	wikiLink->setTextFormat(Qt::RichText);
	wikiLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
	wikiLink->setOpenExternalLinks(true);
	bottomLyt->setAlignment(Qt::AlignRight);

	bottomLyt->addWidget(wikiLink);
	bottomLyt->addWidget(new QLabel("- GPLv3 - IGCIT"));
	lyt->addWidget(tabWidget);
	lyt->addLayout(bottomLyt);

	ui->centralwidget->setLayout(lyt);

	QObject::connect(IGCIT::Logger::getInstance().get(), &IGCIT::Logger::logSent, this, &MainWindow::onLogSent);
	QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetTabChange);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLogSent(const QString &msg) const {
	logTab->writeLog(msg);

	if (tabWidget->currentIndex() != logPageIdx)
		tabWidget->setTabText(logPageIdx, "Logs*");
}

void MainWindow::onTabWidgetTabChange(const int idx) const {
	if (idx == logPageIdx)
		tabWidget->setTabText(idx, "Logs");
}
