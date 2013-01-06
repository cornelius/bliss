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

#include "menuhandleitem.h"

#include <KLocale>

MenuHandleItem::MenuHandleItem( FanMenu *menu, QGraphicsItem *item )
  : QGraphicsEllipseItem( item ), m_menu( menu )
{
  init();
}
void MenuHandleItem::init()
{
  setItemSize( 30 );

  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  connect( m_menu, SIGNAL( hoverStateChanged( bool ) ),
           SLOT( checkMenuVisibility() ) );

  setAcceptHoverEvents( true );
}

void MenuHandleItem::setItemSize( int size )
{
  m_itemSize = size;

  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );
}

int MenuHandleItem::itemSize() const
{
  return m_itemSize;
}

void MenuHandleItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_menu->showMenu( scenePos() );
  
  m_isHovered = true;
}

void MenuHandleItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  QTimer::singleShot( 0, this, SLOT( checkMenuVisibility() ) );

  m_isHovered = false;
}

void MenuHandleItem::checkMenuVisibility()
{
  if ( !m_menu->isHovered() && !m_isHovered ) {
    m_menu->hideMenu();
  }
}

void MenuHandleItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mousePressEvent( event );
}

void MenuHandleItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mouseReleaseEvent( event );
}
