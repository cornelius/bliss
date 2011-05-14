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
#include "todohandleitem.h"

#include <KLocale>

TodoItem::TodoItem( MainModel *model, const Bliss::Todo &identity )
  : QObject( model ), m_model( model ), m_todo( identity )
{
  init();
}

TodoItem::TodoItem( QGraphicsItem *item, MainModel *model,
  const Bliss::Todo &identity )
  : QObject( model ), QGraphicsItemGroup( item ), m_model( model ),
    m_todo( identity )
{
  init();
}

void TodoItem::init()
{
  setFlags( ItemIsMovable );

  updateItem( m_todo );
}

void TodoItem::enableMenus( bool enabled )
{
  m_handleItem->enableMenus( enabled );
}

void TodoItem::updateItem( const Bliss::Todo &todo )
{
  m_todo = todo;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  m_handleItem = new TodoHandleItem( this, m_model, m_todo );
  connect( m_handleItem, SIGNAL( removeClicked() ), SLOT( emitRemoveTodo() ) );
  connect( m_handleItem, SIGNAL( showClicked() ), SLOT( emitShowTodo() ) );
  
  int itemSize = m_handleItem->itemSize();
  
  m_nameItem = new QGraphicsTextItem( todo.summary().value(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;
  
  m_nameItem->setPos( itemSize / 2 + 16, - textHeight / 2 + 2 );
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

void TodoItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  QLineF distance = QLineF( QPointF( 0, 0 ), event->pos() );

  if ( distance.length() <= m_handleItem->itemSize() / 2 + 2 ) {
    m_handleItem->showPopups();
  }

  QGraphicsItemGroup::hoverEnterEvent( event );
}

void TodoItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  QGraphicsItemGroup::hoverLeaveEvent( event );

  m_handleItem->hidePopups();
}

void TodoItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsItemGroup::mousePressEvent( event );
}

void TodoItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if ( pos() != m_movePos ) {
    emit itemMoved( this, pos() );
  }

  QGraphicsItemGroup::mouseReleaseEvent( event );
}

void TodoItem::emitShowTodo()
{
  emit showTodo( m_todo );
}

void TodoItem::emitRemoveTodo()
{
  emit removeTodo( m_todo );
}

void TodoItem::undoMove()
{
  setPos( m_movePos );
}

int TodoItem::textCenterX()
{
  return m_textCenterX;
}

void TodoItem::hidePopups()
{
  m_handleItem->hidePopups();
}
