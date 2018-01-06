/*
    This file is part of KDE.

    Copyright (C) 2009-2011 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "mainwindow.h"

#include "mainview.h"
#include "settings.h"

#include <QAction>

#include <KLocale>

MainWindow::MainWindow()
  : QMainWindow(), m_closing( false )
{
  m_view = new MainView( this );
  setCentralWidget( m_view );

  setupActions();

  connect( m_view, SIGNAL( dataWritten() ), SLOT( slotDataWritten() ) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
  QAction *quitAction = new QAction(i18n("Quit"), this);
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()), SLOT(close()));
  addAction(quitAction);
}

void MainWindow::readData( const QString &file )
{
  m_view->readData( file );
}

bool MainWindow::queryClose()
{
  qDebug() << "queryClose";
  m_closing = true;
  m_view->writeData( i18n("Closing") );
  return false;
}

void MainWindow::slotDataWritten()
{
  qDebug() << "DATA WRITTEN";
  if ( m_closing ) deleteLater();
  m_view->writeConfig();
}
