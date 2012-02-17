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

#include "droptargetitem.h"

#include "mainmodel.h"

DropTargetItem::DropTargetItem( QGraphicsItem *item, MainModel *model,
  const Bliss::Todo &todo )
  : QGraphicsEllipseItem( item ), m_model( model ),
    m_todo( todo ), m_itemSize( 100 )

{
  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );

  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  updateItem( m_todo );
}

void DropTargetItem::updateItem( const Bliss::Todo &todo )
{
  m_todo = todo;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  m_nameItem = new QGraphicsTextItem( todo.summary().value(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_nameItem->setPos( -textWidth / 2, -textHeight / 2 + 2 );  

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

Bliss::Todo DropTargetItem::todo() const
{
  return m_todo;
}

void DropTargetItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mousePressEvent( event );
}

void DropTargetItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mouseReleaseEvent( event );
}
