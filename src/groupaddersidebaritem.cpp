/*
    Copyright (c) 2011 Cornelius Schumacher <schumacher@kde.org>

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

#include "groupaddersidebaritem.h"

#include "groupadderitem.h"

#include <QPainter>

GroupAdderSidebarItem::GroupAdderSidebarItem( GroupAdderItem *parent )
  : QGraphicsItemGroup( parent ), m_parent( parent )
{
  m_top = new QGraphicsEllipseItem( this );
  m_bar = new QGraphicsRectItem( this );
  
  QColor color( 230,229,229 );

  m_bar->setBrush( color );
  m_top->setBrush( color );
  
  QPen pen;
  pen.setBrush( Qt::NoBrush );
  m_bar->setPen( pen );
  m_top->setPen( pen );
}

GroupAdderSidebarItem::~GroupAdderSidebarItem()
{
}

void GroupAdderSidebarItem::setSize( int size )
{
  m_bar->setRect( 0, 0, size/2, 2000 );
  m_top->setRect( -size/2, -size/2, size/2 * 2, size/2 * 2 );
}

void GroupAdderSidebarItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )
  
  m_parent->toggleShowAsSidebar();
}
