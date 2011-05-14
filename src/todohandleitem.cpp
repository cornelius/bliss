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

#include <KLocale>

TodoHandleItem::TodoHandleItem( MainModel *model, const Bliss::Todo &identity )
  : QObject( model ), m_model( model ), m_todo( identity )
{
  init();
}

TodoHandleItem::TodoHandleItem( QGraphicsItem *item, MainModel *model,
  const Bliss::Todo &identity )
  : QObject( model ), QGraphicsEllipseItem( item ), m_model( model ),
    m_todo( identity )
{
  init();
}

void TodoHandleItem::init()
{
  m_itemSize = 60;
  m_menusEnabled = true;

  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );
  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  setAcceptHoverEvents( true );

  updateItem( m_todo );
}

int TodoHandleItem::itemSize() const
{
  return m_itemSize;
}

void TodoHandleItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;
  
  if ( !m_menusEnabled ) hidePopups();
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
  
  QPixmap pixmap = m_model->pixmap( todo );

  QGraphicsItem *item = new QGraphicsPixmapItem( pixmap, this );  
  item->setPos( -pixmap.width() / 2, -pixmap.height() / 2 );

  m_fanMenu = new FanMenu( this );
  m_fanMenu->setZValue( 50 );

  FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Remove") );
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( removeClicked() ) );
  if ( todo.type() == "group" ) {
    menuItem = m_fanMenu->addItem( i18n("Go to") );
  }
  connect( menuItem, SIGNAL( clicked() ), SIGNAL( showClicked() ) );
  m_fanMenu->setupItems();

  hidePopups();
}

Bliss::Todo TodoHandleItem::todo() const
{
  return m_todo;
}

void TodoHandleItem::showPopups()
{
  if ( m_menusEnabled ) {
    m_fanMenu->show();
    emit menuShown();
  }
}

void TodoHandleItem::hidePopups()
{
  m_fanMenu->hide();
}

void TodoHandleItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  qDebug() << "TICK";

  showPopups();
}

void TodoHandleItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  hidePopups();
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
