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

#include "mainview.h"

#include "mainmodel.h"
#include "groupview.h"
#include "newtododialog.h"
#include "newgroupdialog.h"
#include "newlistdialog.h"
#include "settings.h"
#include "settingswidget.h"
#include "overview.h"
#include "historyview.h"
#include "searchedit.h"
#include "searchresultview.h"

#include <QMessageBox>

#include <KLocale>

MainView::MainView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new MainModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );

  QBoxLayout *topLayout = new QVBoxLayout( this );
  topLayout->setContentsMargins(0,0,0,0);

  QBoxLayout *viewLayout = new QHBoxLayout;
  topLayout->addLayout( viewLayout );

  m_groupWidget = new QWidget;
  viewLayout->addWidget( m_groupWidget );

  m_listLayout = new QStackedLayout( m_groupWidget );

  m_overview = new Overview( m_model );
  m_listLayout->addWidget( m_overview );
  connect( m_overview, SIGNAL( showGroupView() ), SLOT( showGroupView() ) );
  connect( m_overview, SIGNAL( showHistory() ), SLOT( showHistory() ) );

  m_groupView = new GroupView( m_model );
  m_listLayout->addWidget( m_groupView );
  connectGroupView( m_groupView );
  connect( m_groupView, SIGNAL( newGroup() ), SLOT( newSubGroup() ) );
  connect( m_groupView, SIGNAL( newList() ), SLOT( newList() ) );
  connect( m_groupView, SIGNAL( removeGroup( const Bliss::Todo & ) ),
    SLOT( removeGroup( const Bliss::Todo & ) ) );

  m_historyView = new HistoryView( m_model );
  m_listLayout->addWidget( m_historyView );
  connect( m_historyView, SIGNAL( showGroupView() ), SLOT( showGroupView() ) );

  m_settingsWidget = new SettingsWidget( m_model );
  topLayout->addWidget( m_settingsWidget );
  connect( m_settingsWidget, SIGNAL( showView() ), SLOT( showGroupView() ) );

  m_settingsWidget->hide();

  readConfig();
}

MainView::~MainView()
{
}

void MainView::connectGroupView( GroupView *groupView )
{
  connect( groupView, SIGNAL( goBack() ), SLOT( goBack() ) );
  connect( groupView, SIGNAL( newTodo() ), SLOT( newTodo() ) );
  connect( groupView, SIGNAL( requestShowGroup( const Bliss::Todo & ) ),
           SLOT( showGroup( const Bliss::Todo & ) ) );
  connect( groupView, SIGNAL( showSettings() ),
    SLOT( showSettings() ) );
  connect( groupView, SIGNAL( showOverview() ), SLOT( showOverview() ) );
}

void MainView::readConfig()
{
  Settings::self()->readConfig();

  m_groupView->readConfig();
  m_settingsWidget->readConfig();
}

void MainView::writeConfig()
{
  m_settingsWidget->writeConfig();
  m_groupView->writeConfig();

  Settings::setHistory( m_history.get() );

  Settings::self()->writeConfig();
}

void MainView::readData( const QString &file )
{
  if ( !m_model->readData( file ) ) {
    QMessageBox::critical( this, i18n("Error"), i18n("Error reading data file") );
    return;
  }

  m_history.setLocationId( m_model->locationId() );
  m_history.set( Settings::history() );

  if ( m_history.isEmpty() ) {
    showRoot();
  } else {
    Bliss::Todo group = m_model->findTodo( m_history.last() );
    showGroup( group );
  }

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->pullData();
  }

  Bliss::Todo adderGroup = m_model->findTodo( Settings::adderGroup() );
  if ( !adderGroup.isValid() ) adderGroup = m_model->rootGroup();

  m_groupView->setAdderGroup( adderGroup );
}

void MainView::writeData( const QString &msg )
{
  m_model->writeData( msg );

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->pushData();
  }
}

void MainView::newSubGroup()
{
  NewGroupDialog *dialog = new NewGroupDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Bliss::Todo group = dialog->todo();
    m_model->addTodo( group, m_group );
  }
  return;
}

void MainView::newList()
{
  NewListDialog *dialog = new NewListDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Bliss::ViewList list = dialog->list();
    m_model->addList( list, m_group );
  }
  return;
}

void MainView::removeGroup( const Bliss::Todo &group )
{
  m_model->removeGroup( group );
}

void MainView::newTodo()
{
  NewTodoDialog *dialog = new NewTodoDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Bliss::Todo todo = dialog->todo();

    m_model->addTodo( todo, m_group );
  }
  return;
}

void MainView::showRoot()
{
  showGroup( m_model->rootGroup() );
}

void MainView::showGroup( const Bliss::Todo &group )
{
  m_groupWidget->setMaximumWidth( QWIDGETSIZE_MAX );

  m_group = group;

  // Return to event loop, so layout gets adjusted before new group is rendered.
  QTimer::singleShot( 0, this, SLOT( continueShowGroup() ) );
}

void MainView::continueShowGroup()
{
  if ( m_history.isEmpty() || m_history.last() != m_group.id() ) {
    m_history.append( m_group.id() );
  }

  m_groupView->setBackButtonEnabled( m_history.size() > 1 );

  m_groupView->showGroup( m_group );
  m_listLayout->setCurrentWidget( m_groupView );
}

void MainView::showGroupView()
{
  if ( m_group.id().isEmpty() ) {
    showRoot();
  } else {
    showGroup( m_group );
  }
}

void MainView::showSettings()
{
  if ( m_settingsWidget->isVisible() ) {
    m_settingsWidget->hide();
  } else {
    m_settingsWidget->show();
  }
}

void MainView::goBack()
{
  m_history.takeLast();
  while ( !m_history.isEmpty() ) {
    QString id = m_history.last();
    Bliss::Todo group = m_model->findTodo( id );
    if ( group.isValid() && group.type() == "group" ) {
      showGroup( group );
      return;
    }
    m_history.takeLast();
  }
  showRoot();
}

void MainView::showOverview()
{
  m_listLayout->setCurrentWidget( m_overview );
}

void MainView::showHistory()
{
  m_listLayout->setCurrentWidget( m_historyView );

  m_historyView->loadHistory();
}
