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

#include "todohandleitem.h"

#include "mainmodel.h"
#include "fanmenu.h"
#include "roundedrectitem.h"
#include "buttonitem.h"

#include <KLocale>

TodoHandleItem::TodoHandleItem( MainModel *model, const Bliss::Todo &identity )
  : QObject( model ), m_model( model ), m_todo( identity )
{
  init();
  initTodo();
}

TodoHandleItem::TodoHandleItem( QGraphicsItem *item, MainModel *model,
  const Bliss::Todo &identity )
  : QObject( model ), QGraphicsEllipseItem( item ), m_model( model ),
    m_todo( identity )
{
  init();
  initTodo();
}

TodoHandleItem::TodoHandleItem( QGraphicsItem *item, MainModel *model )
  : QObject( model ), QGraphicsEllipseItem( item ), m_model( model )
{
  init();

  ButtonItem *button = new ButtonItem( this );
  button->setPlus();
  button->setClickEnabled( false );
}

void TodoHandleItem::init()
{
  setItemSize( 30 );

  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );
}

void TodoHandleItem::initTodo()
{
  setAcceptHoverEvents( true );

  updateItem( m_todo );
}

void TodoHandleItem::setItemSize( int size )
{
  m_itemSize = size;

  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );

  updateItem( m_todo );
}

int TodoHandleItem::itemSize() const
{
  return m_itemSize;
}

void TodoHandleItem::updateItem( const Bliss::Todo &todo )
{
  m_todo = todo;

  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  if ( todo.type() == "group" ) {
    int circleSize = m_itemSize + 14;
    QGraphicsEllipseItem *groupCircle = new QGraphicsEllipseItem( this );
    groupCircle->setRect( -circleSize/2, -circleSize/2,
      circleSize, circleSize );
    QPen pen;
    pen.setBrush( Qt::white );
    pen.setWidth( 4 );
    groupCircle->setPen( pen );
  }
}

Bliss::Todo TodoHandleItem::todo() const
{
  return m_todo;
}

void TodoHandleItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void TodoHandleItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void TodoHandleItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mousePressEvent( event );
}

void TodoHandleItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mouseReleaseEvent( event );
}

int TodoHandleItem::textCenterX()
{
  return m_textCenterX;
}
