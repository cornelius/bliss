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

#include "buttonitem.h"

ButtonItem::ButtonItem( QGraphicsItem *parentItem )
  : QGraphicsEllipseItem( parentItem ), m_defaultItemSize( 30 )
{
  setItemSize( m_defaultItemSize );
 
  setBrush( QColor( 250, 250, 250 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  m_decoration = new QGraphicsPolygonItem( this );
  m_decoration->setBrush( QColor( 200, 200, 200 ) );
  m_decoration->setPen( pen );

  int inner = 3;
  int outer = 11;

  m_plusPolygon.append( QPointF( + inner, - inner ) );
  m_plusPolygon.append( QPointF( + outer, - inner ) );
  m_plusPolygon.append( QPointF( + outer, + inner ) );
  m_plusPolygon.append( QPointF( + inner, + inner ) );
  m_plusPolygon.append( QPointF( + inner, + outer ) );
  m_plusPolygon.append( QPointF( - inner, + outer ) );
  m_plusPolygon.append( QPointF( - inner, + inner ) );
  m_plusPolygon.append( QPointF( - outer, + inner ) );
  m_plusPolygon.append( QPointF( - outer, - inner ) );
  m_plusPolygon.append( QPointF( - inner, - inner ) );
  m_plusPolygon.append( QPointF( - inner, - outer ) );
  m_plusPolygon.append( QPointF( + inner, - outer ) );

  m_minusPolygon.append( QPointF( + outer, - inner ) );
  m_minusPolygon.append( QPointF( + outer, + inner ) );
  m_minusPolygon.append( QPointF( - outer, + inner ) );
  m_minusPolygon.append( QPointF( - outer, - inner ) );

  int arrow = 10;

  m_arrowPolygon.append( QPointF( - outer, - inner ) );
  m_arrowPolygon.append( QPointF( outer - arrow, - inner ) );
  m_arrowPolygon.append( QPointF( outer - arrow, - arrow ) );
  m_arrowPolygon.append( QPointF( outer, 0 ) );
  m_arrowPolygon.append( QPointF( outer - arrow, arrow ) );
  m_arrowPolygon.append( QPointF( outer - arrow, inner ) );
  m_arrowPolygon.append( QPointF( - outer, inner ) );

  setPlus();
}

void ButtonItem::setPlus()
{
  m_decoration->setPolygon( m_plusPolygon );  
}

void ButtonItem::setMinus()
{
  m_decoration->setPolygon( m_minusPolygon );
}

void ButtonItem::setNext()
{
  m_decoration->setPolygon( m_arrowPolygon );
  m_decoration->setRotation( 45 );
}

void ButtonItem::setPrevious()
{
  m_decoration->setPolygon( m_arrowPolygon );
  m_decoration->setRotation( -135 );
}

void ButtonItem::setItemSize( int size )
{
  setRect( -size/2, -size/2, size, size );
}

void ButtonItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void ButtonItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void ButtonItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  emit clicked();

  // Don't call event handler from parent, so mouse press does nothing.
}

void ButtonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}
