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

HistoryView::HistoryView( MainModel *model )
  : m_model( model ), m_logCommand( -1 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_list = new QListWidget;
  topLayout->addWidget( m_list );

  connect( m_model->gitDir(), SIGNAL( commandExecuted( const GitCommand & ) ),
    SLOT( slotCommandExecuted( const GitCommand & ) ) );
}

void HistoryView::loadHistory()
{
  m_list->clear();
  m_list->addItem( "Loading..." );

  m_logCommand = m_model->gitDir()->getLog();
}

void HistoryView::slotCommandExecuted( const GitCommand &cmd )
{
  if ( m_logCommand == cmd.id() ) {
    m_logCommand = -1;
    m_list->clear();
    
    foreach( QString line, cmd.result() ) {
      int pos = line.indexOf( " " );
      QString revision = line.left( pos );
//      qDebug() << revision;
      QString message = line.mid( pos + 2 );
      m_list->addItem( message );
    }
  }
}
