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

#include "matchlist.h"

#include <KMessageBox>

MatchList::MatchList( MainModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_proxyModel( 0 ), m_matchList( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_matchList = new QListView;

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );

  topLayout->addWidget( m_matchList );
  connect( m_matchList, SIGNAL( activated( const QModelIndex & ) ),
    SIGNAL( activated() ) );
}

MatchList::~MatchList()
{
}

void MatchList::filter( const QString &string )
{
  if ( !m_matchList->model() ) {
    m_proxyModel->setSourceModel( m_model->todoItemModel() );
    m_matchList->setModel( m_proxyModel );
  }
  m_proxyModel->setFilterWildcard( string );
}

Bliss::Todo MatchList::todo()
{
  QModelIndexList selectedIndexes =
    m_matchList->selectionModel()->selectedIndexes();
  if ( selectedIndexes.isEmpty() ) {
    Bliss::Todo identity;
    return identity;
  } else {
    if ( selectedIndexes.count() > 1 ) {
      KMessageBox::information( 0, "More than one todo selected" );
    }
    return m_model->findTodo( m_proxyModel->data( selectedIndexes.first(),
      Qt::UserRole ).toString() );
  }
}
