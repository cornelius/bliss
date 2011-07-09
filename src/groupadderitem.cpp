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

#include "groupadderitem.h"

#include "mainmodel.h"
#include "fanmenu.h"
#include "buttonitem.h"
#include "todoitem.h"
#include "todohandleitem.h"

#include <KLocale>

GroupAdderItem::GroupAdderItem( MainModel *model )
  : m_model( model ), m_defaultItemSize( 100 ), m_expanded( false )
{
  setItemSize( m_defaultItemSize );
 
  setBrush( QColor( 230,229,229 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  m_expandButton = new ButtonItem( this );
  m_expandButton->setPos( 21, -21 );
  connect( m_expandButton, SIGNAL( clicked() ), SLOT( expand() ) );

  int groupOffset = 85;

  m_groupItem = new TodoItem( this, m_model, m_model->rootGroup() );
  m_groupItem->setPos( groupOffset, -groupOffset );
  m_groupItem->hide();
  m_groupItem->enableMenus( false );

  m_upButton = new ButtonItem( this );
  m_upButton->setPos( 151, -21 );
  m_upButton->hide();
  connect( m_upButton, SIGNAL( clicked() ), SLOT( nextGroup() ) );
  m_upButton->setNext();
  
  m_downButton = new ButtonItem( this );
  m_downButton->setPos( 21, -151 );
  m_downButton->hide();
  m_downButton->setMinus();
  connect( m_downButton, SIGNAL( clicked() ), SLOT( previousGroup() ) );
  m_downButton->setPrevious();
}

void GroupAdderItem::setItemSize( int size )
{
  setRect( -size/2, -size/2, size, size );
}

void GroupAdderItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse press does nothing.
}

void GroupAdderItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}

void GroupAdderItem::expand()
{
  m_expanded = !m_expanded;

  if ( m_expanded ) {
    setItemSize( 400 );
    m_expandButton->setMinus();
    m_groupItem->show();
    m_upButton->show();
    m_downButton->show();
  } else {
    setItemSize( 100 );
    m_expandButton->setPlus();
    m_groupItem->hide();
    m_upButton->hide();
    m_downButton->hide();
  }
}

void GroupAdderItem::setGroup( const Bliss::Todo &group )
{
  m_groupItem->updateItem( group );
}

Bliss::Todo GroupAdderItem::group() const
{
  return m_groupItem->todo();
}

void GroupAdderItem::nextGroup()
{
  Bliss::Todo::List list = m_model->groups();
  Bliss::Todo::List::ConstIterator it;
  for( it = list.constBegin(); it != list.constEnd(); ++it ) {
    if ( (*it).id() == group().id() ) break;
  }
  ++it;
  if ( it != list.constEnd() ) {
    setGroup( *it );
  } else {
    setGroup( list.first() );
  }
}

void GroupAdderItem::previousGroup()
{
  Bliss::Todo::List list = m_model->groups();
  Bliss::Todo::List::ConstIterator it;
  for( it = list.constBegin(); it != list.constEnd(); ++it ) {
    if ( (*it).id() == group().id() ) break;
  }
  if ( it == list.constEnd() ) {
    setGroup( list.first() );
  } else {
    if ( it == list.constBegin() ) {
      setGroup( list.last() );
    } else {
      --it;
      setGroup( *it );
    }
  }
}
