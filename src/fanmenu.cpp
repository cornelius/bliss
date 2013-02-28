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

#include "fanmenu.h"

#include "fanmenuelement.h"

FanMenu::FanMenu( QGraphicsItem *parent )
  : QGraphicsLineItem( parent ), m_startAngle( 100 ), m_endAngle( 260 ),
    m_spacing( 10 ), m_radius( 70 ), m_isHovered( false )
{
}

void FanMenu::setRadius( qreal radius )
{
  m_radius = radius;
}

qreal FanMenu::radius() const
{
  return m_radius;
}

void FanMenu::setupItems( int coverage )
{
  int count = m_items.count();
 
  int availableAngle = m_endAngle - m_startAngle - ( count - 1 ) * m_spacing;
  int totalWeight = 0;
  foreach( FanMenuItem *item, m_items ) {
    totalWeight += item->weight();
  }
  int angle = m_startAngle;
  foreach( FanMenuItem *item, m_items ) {
    FanMenuElement *element = new FanMenuElement( this );
    int width = availableAngle / totalWeight * item->weight();
    element->setup( item, angle, angle + width, coverage );
    angle += width + m_spacing;
  }
}

FanMenuItem *FanMenu::addItem( const QString &text, int weight )
{
  FanMenuItem *item = new FanMenuItem( text, weight );
  m_items.append( item );
  return item;
}

bool FanMenu::isCloseTo( const QPointF &point )
{
  return QLineF( point, pos() ).length() <= radius() + 10;
}

void FanMenu::setStartAngle( int angle )
{
  m_startAngle = angle;
}

void FanMenu::setEndAngle( int angle )
{
  m_endAngle = angle;
}

void FanMenu::setSpacing( int spacing )
{
  m_spacing = spacing;
}

void FanMenu::setIsHovered( bool hovered )
{
  m_isHovered = hovered;

  emit hoverStateChanged( m_isHovered );
}

bool FanMenu::isHovered() const
{
  return m_isHovered;
}

void FanMenu::showMenu( const QPointF &pos )
{
  setPos( pos );
  show();
  emit menuShown();
}

void FanMenu::hideMenu()
{
  hide();
}
