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

#include "todoitem.h"

#include "mainmodel.h"
#include "fanmenu.h"
#include "roundedrectitem.h"

#include <KLocale>

TodoItem::TodoItem( MainModel *model, const Bliss::Todo &identity )
  : QObject( model ), m_model( model ), m_todo( identity )
{
  init();
}

TodoItem::TodoItem( QGraphicsItem *item, MainModel *model,
  const Bliss::Todo &identity )
  : QObject( model ), QGraphicsEllipseItem( item ), m_model( model ),
    m_todo( identity )
{
  init();
}

void TodoItem::init()
{
  m_itemSize = 60;
  m_menusEnabled = true;

  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );
  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  setAcceptHoverEvents( true );

  setFlags( ItemIsMovable );

  updateItem( m_todo );
}

void TodoItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;
  
  if ( !m_menusEnabled ) hidePopups();
}

void TodoItem::updateItem( const Bliss::Todo &identity )
{
  m_todo = identity;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }
  
  if ( identity.type() == "group" ) {
    int circleSize = m_itemSize + 14;
    QGraphicsEllipseItem *groupCircle = new QGraphicsEllipseItem( this );
    groupCircle->setRect( -circleSize/2, -circleSize/2,
      circleSize, circleSize );
    QPen pen;
    pen.setBrush( Qt::white );
    pen.setWidth( 4 );
    groupCircle->setPen( pen );
  }
  
  QPixmap pixmap = m_model->pixmap( identity );

  QGraphicsItem *item = new QGraphicsPixmapItem( pixmap, this );  
  item->setPos( -pixmap.width() / 2, -pixmap.height() / 2 );

  m_nameItem = new QGraphicsTextItem( identity.summary().value(), this );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;
  
  m_nameItem->setPos( m_itemSize / 2 + 16, - textHeight / 2 );

  m_fanMenu = new FanMenu( this );
  m_fanMenu->setZValue( 50 );

  FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Remove") );
  connect( menuItem, SIGNAL( clicked() ), SLOT( emitRemoveTodo() ) );
  if ( identity.type() == "group" ) {
    menuItem = m_fanMenu->addItem( i18n("Go to") );
  }
  connect( menuItem, SIGNAL( clicked() ), SLOT( emitShowTodo() ) );
  m_fanMenu->setupItems();

  hidePopups();
}

Bliss::Todo TodoItem::todo() const
{
  return m_todo;
}

void TodoItem::setDefaultPos( const QPointF &pos )
{
  m_defaultPos = pos;
}

QPointF TodoItem::defaultPos() const
{
  return m_defaultPos;
}

void TodoItem::rememberPos( const QPointF &pos )
{
  m_rememberedPos = pos;
}

QPointF TodoItem::rememberedPos() const
{
  return m_rememberedPos;
}

void TodoItem::hidePopups()
{
  m_fanMenu->hide();
}

void TodoItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  if ( m_menusEnabled ) {
    m_nameItem->show();
    m_fanMenu->show();
    emit menuShown();
  }
}

void TodoItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  hidePopups();
}

void TodoItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsEllipseItem::mousePressEvent( event );
}

void TodoItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if ( pos() != m_movePos ) {
    emit itemMoved( this, pos() );
  }

  QGraphicsEllipseItem::mouseReleaseEvent( event );
}

void TodoItem::emitShowTodo()
{
  emit showTodo( m_todo );
}

void TodoItem::emitRemoveTodo()
{
  emit removeTodo( m_todo );
}

QVariant TodoItem::itemChange( GraphicsItemChange change,
  const QVariant &value )
{
  return QGraphicsEllipseItem::itemChange( change, value );
}

void TodoItem::undoMove()
{
  setPos( m_movePos );
}

int TodoItem::textCenterX()
{
  return m_textCenterX;
}
