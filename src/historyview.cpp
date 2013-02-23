/*
    This file is part of KDE.

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

#include "historyview.h"

#include "mainmodel.h"
#include "gitdir.h"

#include <KLocale>

HistoryView::HistoryView( MainModel *model )
  : m_model( model )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *titleLayout = new QHBoxLayout;
  topLayout->addLayout( titleLayout );
  
  titleLayout->addWidget( new QLabel( i18n("<b>History</b>") ) );
  
  titleLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("Go to group view") );
  connect( button, SIGNAL( clicked() ), SIGNAL( showGroupView() ) );  
  titleLayout->addWidget( button );
  
  m_list = new QListWidget;
  topLayout->addWidget( m_list );

  connect( m_model, SIGNAL( logRetrieved( const QStringList & ) ),
    SLOT( historyLoaded( const QStringList & ) ) );
}

void HistoryView::loadHistory()
{
  m_list->clear();
  m_list->addItem( "Loading..." );

  m_model->retrieveLog();
}

void HistoryView::historyLoaded( const QStringList &log )
{
  m_list->clear();
    
  foreach( QString line, log ) {
    m_list->addItem( line );
  }
}
