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
#include "grouplistview.h"
#include "groupgraphicsview.h"
#include "newtododialog.h"
#include "newgroupdialog.h"
#include "newlistdialog.h"
#include "settings.h"
#include "gitremote.h"
#include "settingswidget.h"
#include "overview.h"
#include "historyview.h"
#include "searchedit.h"
#include "searchresultview.h"

#include <KMessageBox>
#include <KLocale>
#include <KInputDialog>

MainView::MainView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new MainModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  
  QBoxLayout *topLayout = new QVBoxLayout( this );

  
  QBoxLayout *buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );

  // FIXME: Use proper icon
  m_backButton = new QPushButton( "<" );
  buttonLayout->addWidget( m_backButton );
  connect( m_backButton, SIGNAL( clicked() ), SLOT( goBack() ) );
  m_backButton->setEnabled( false );

  buttonLayout->addStretch( 1 );

  m_groupNameLabel = new QLabel;
  buttonLayout->addWidget( m_groupNameLabel );

  buttonLayout->addStretch( 1 );

  m_searchEdit = new SearchEdit;
  buttonLayout->addWidget( m_searchEdit );
  connect( m_searchEdit, SIGNAL( search( const QString & ) ),
    SLOT( showSearch( const QString & ) ) );
  connect( m_searchEdit, SIGNAL( stopSearch() ), SLOT( stopSearch() ) );

  QPushButton *button = new QPushButton( i18n("...") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( showOverview() ) );

  button->setFocus();

  QBoxLayout *viewLayout = new QHBoxLayout;
  topLayout->addLayout( viewLayout );

  m_groupWidget = new QWidget;
  viewLayout->addWidget( m_groupWidget );
  
  m_listLayout = new QStackedLayout( m_groupWidget );

  m_overview = new Overview;
  m_listLayout->addWidget( m_overview );
  connect( m_overview, SIGNAL( showGroupView() ), SLOT( showGroupView() ) );
  connect( m_overview, SIGNAL( showListView() ), SLOT( showListView() ) );
  connect( m_overview, SIGNAL( showHistory() ), SLOT( showHistory() ) );

  m_groupListView = new GroupListView( m_model );
  m_listLayout->addWidget( m_groupListView );
  connectGroupView( m_groupListView );

  m_groupGraphicsView = new GroupGraphicsView( m_model );
  m_listLayout->addWidget( m_groupGraphicsView );
  connectGroupView( m_groupGraphicsView );
  connect( m_groupGraphicsView, SIGNAL( newGroup() ), SLOT( newSubGroup() ) );
  connect( m_groupGraphicsView, SIGNAL( newList() ), SLOT( newList() ) );
  connect( m_groupGraphicsView, SIGNAL( removeGroup( const Bliss::Todo & ) ),
    SLOT( removeGroup( const Bliss::Todo & ) ) );

  m_historyView = new HistoryView( m_model );
  m_listLayout->addWidget( m_historyView );

  m_searchResultView = new SearchResultView( m_model );
  m_listLayout->addWidget( m_searchResultView );

  m_settingsWidget = new SettingsWidget( m_model );
  topLayout->addWidget( m_settingsWidget );
  connect( m_settingsWidget, SIGNAL( showView() ), SLOT( showView() ) );

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
}

void MainView::readConfig()
{
  Settings::self()->readConfig();

  m_groupGraphicsView->readConfig();
  m_settingsWidget->readConfig();
}

void MainView::writeConfig()
{
  m_settingsWidget->writeConfig();
  m_groupGraphicsView->writeConfig();

  Settings::setHistory( m_history.get() );

  Settings::self()->writeConfig();
}

void MainView::readData( const QString &file )
{
  if ( !m_model->readData( file ) ) {
    KMessageBox::error( this, i18n("Error reading data file") );
    return;
  }

  m_groupListView->setItemModel( m_model->itemModel() );

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

  m_groupGraphicsView->setAdderGroup( adderGroup );
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

  m_backButton->setEnabled( m_history.size() > 1 );
  m_groupGraphicsView->setBackButtonEnabled( m_history.size() > 1 );
  
  m_groupNameLabel->setText( "<b>" + m_group.summary().value() + "</b>" );

  if ( m_settingsWidget->fancyMode() ) {
    m_groupGraphicsView->showGroup( m_group );
    m_listLayout->setCurrentWidget( m_groupGraphicsView );
  } else {
    m_groupListView->showGroup( m_group );
    m_listLayout->setCurrentWidget( m_groupListView );
  }
}

void MainView::showView()
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
  if ( m_searchResultView->isVisible() ) {
    m_searchEdit->setEmpty();
    showGroupView();
    return;
  }

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
  m_backButton->hide();
  m_groupNameLabel->setText( QString() );
  m_listLayout->setCurrentWidget( m_overview );
}

void MainView::showGroupView()
{
  m_backButton->show();
  showView();
}

void MainView::showListView()
{
  m_backButton->show();
  showView();
}

void MainView::showHistory()
{
  m_backButton->hide();
  m_groupNameLabel->setText( i18n("<b>History</b>") );
  m_listLayout->setCurrentWidget( m_historyView );

  m_historyView->loadHistory();
}

void MainView::showSearch( const QString &text )
{
  m_groupNameLabel->setText( i18n("<b>Search Results</b>") );
  m_backButton->setEnabled( true );

  m_listLayout->setCurrentWidget( m_searchResultView );
  m_searchResultView->search( text );
}

void MainView::stopSearch()
{
  showGroupView();
}
