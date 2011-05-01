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

#include "magicmenuitem.h"

#include "mainmodel.h"
#include "fanmenu.h"

#include <KLocale>

MagicMenuItem::MagicMenuItem()
{
  int itemSize = 55;

  setRect( -itemSize/2, -itemSize/2, itemSize, itemSize );
  setBrush( QColor( 230,229,229 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  QGraphicsTextItem *textItem = new QGraphicsTextItem( i18n("Magic"), this );

  int textWidth = textItem->boundingRect().width();
  int textHeight = textItem->boundingRect().height();

  textItem->setPos( - textWidth / 2, - textHeight / 2 );

  m_fanMenu = new FanMenu( this );
  m_fanMenu->setZValue( 50 );
  m_fanMenu->hide();
  m_fanMenu->setStartAngle( 80 );
  m_fanMenu->setEndAngle( 280 );

  FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Reset\nlayout") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( resetLayout() ) );
  menuItem = m_fanMenu->addItem( i18n("Settings") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( showSettings() ) );

  m_fanMenu->setupItems( 80 );

  setAcceptHoverEvents( true );
}

void MagicMenuItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->show();
  emit menuShown();
}

void MagicMenuItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->hide();
}

void MagicMenuItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse press does nothing.
}

void MagicMenuItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}
