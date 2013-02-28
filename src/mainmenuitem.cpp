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

#include "mainmenuitem.h"

#include "mainmodel.h"
#include "fanmenu.h"

#include <KLocale>

MainMenuItem::MainMenuItem()
  : m_defaultItemSize( 55 )
{
  m_timer.setSingleShot( true );
  connect( &m_timer, SIGNAL( timeout() ), SLOT( hideItems() ) );

  setZValue( 200 );

  setItemSize( m_defaultItemSize );
 
  setBrush( QColor( 230,229,229 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  QGraphicsTextItem *textItem = new QGraphicsTextItem( i18n("Menu"), this );

  int textWidth = textItem->boundingRect().width();
  int textHeight = textItem->boundingRect().height();

  textItem->setPos( - textWidth / 2, - textHeight / 2 );

  m_fanMenu = new FanMenu( this );
  m_fanMenu->setZValue( 50 );
  m_fanMenu->hide();
  m_fanMenu->setStartAngle( 170 );
  m_fanMenu->setEndAngle( 280 );
  m_fanMenu->setRadius( 220 );
  m_fanMenu->setSpacing( 5 );

  FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Add\ngroup") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( addGroup() ) );
  menuItem = m_fanMenu->addItem( i18n("Add\nlist") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( addList() ) );
  menuItem = m_fanMenu->addItem( i18n("Add\ntodo") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( addTodo() ) );
  menuItem = m_fanMenu->addItem( i18n("More...") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( showMore() ) );

  m_fanMenu->setupItems( 90 );

  setAcceptHoverEvents( true );
}

void MainMenuItem::setItemSize( int size )
{
  setRect( -size/2, -size/2, size, size );
}

void MainMenuItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_timer.stop();

  setItemSize( m_defaultItemSize + 20 );

  m_fanMenu->show();
  emit menuShown();
}

void MainMenuItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
  
  m_timer.start( 300 );
}

void MainMenuItem::hideItems()
{
  setItemSize( m_defaultItemSize );

  m_fanMenu->hide();
}

void MainMenuItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse press does nothing.
}

void MainMenuItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}
