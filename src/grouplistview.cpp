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

#include "grouplistview.h"

#include "mainmodel.h"
#include "blissitemmodel.h"
#include "settings.h"

#include <KLocale>

GroupListView::GroupListView( MainModel *model, QWidget *parent )
  : GroupView( model, parent ), m_itemModel( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_flatView = new QListView;
  topLayout->addWidget( m_flatView );
  connect( m_flatView, SIGNAL( clicked( const QModelIndex & ) ),
    SLOT( slotItemClicked( const QModelIndex & ) ) );

  if ( Settings::enableMagic() ) {
    QBoxLayout *buttonLayout = new QHBoxLayout;
    topLayout->addLayout( buttonLayout );

    buttonLayout->addStretch( 1 );

    QPushButton *button = new QPushButton( i18n("Magic") );
    buttonLayout->addWidget( button );
    connect( button, SIGNAL( clicked() ), SIGNAL( showSettings() ) );    
  }
}

void GroupListView::doShowGroup()
{
  setItemModel( model()->itemModel( group().id() ) );
}

void GroupListView::setItemModel( BlissItemModel *itemModel )
{
  m_itemModel = itemModel;

  m_flatView->setModel( m_itemModel );
}

void GroupListView::slotItemClicked( const QModelIndex &index )
{
  Q_UNUSED( index )
}
