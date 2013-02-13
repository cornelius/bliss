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
#include "menuhandler.h"


#include <KLocale>

TodoItem::TodoItem( MainModel *model, MenuHandler *menuHandler,
  const Bliss::Todo &todo )
  : QObject( model ), m_model( model ), m_todo( todo ), m_handleItem( 0 ),
    m_nameItem( 0 ),
    m_fanMenu( 0 ), m_menuHandler( menuHandler ), m_edit( 0 ), m_editProxy( 0 )
{
  init();
}

TodoItem::TodoItem( MainModel *model )
  : QObject( model ), m_model( model ),
    m_handleItem( 0 ), m_nameItem( 0 ), m_textCenterX( 0 ), m_fanMenu( 0 ),
    m_menuHandler( 0 ), m_edit( 0 ), m_editProxy( 0 )
{
  m_menusEnabled = false;

  m_handleItem = new TodoHandleItem( this, m_model );
}

void TodoItem::init()
{
  m_menusEnabled = true;

  m_isHovered = false;

  setFlags( ItemIsMovable );

  updateItem( m_todo );
}

void TodoItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;

  if ( !m_menusEnabled ) hidePopups();
}

void TodoItem::updateItem( const Bliss::Todo &todo )
{
  m_todo = todo;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  m_handleItem = new TodoHandleItem( this, m_model, m_todo );
  m_handleItem->setItemSize( 30 );
  
  int itemSize = m_handleItem->itemSize();
  
  m_nameItem = new QGraphicsTextItem( todo.summary().value(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;
 
  m_nameItem->setPos( itemSize / 2 + 16, - textHeight / 2 + 2 );
  
  if ( m_menuHandler ) {
    m_fanMenu = m_menuHandler->createMenu();
    connect( m_fanMenu, SIGNAL( hoverStateChanged( bool ) ),
      SLOT( checkMenuVisibility() ) );

    FanMenuItem *menuItem;
    
    if ( todo.type() == "group" ) {
      menuItem = m_fanMenu->addItem( i18n("Go to") );
      connect( menuItem, SIGNAL( clicked() ), SLOT( emitShowGroup() ) );
    }

    if ( todo.type() != "group" ) {
      menuItem = m_fanMenu->addItem( i18n("Done") );
      connect( menuItem, SIGNAL( clicked() ), SLOT( emitDone() ) );
    }

    menuItem = m_fanMenu->addItem( i18n("Edit") );
    connect( menuItem, SIGNAL( clicked() ), SLOT( editTodo() ) );

    m_fanMenu->setupItems();

    hidePopups();
  }
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

void TodoItem::showPopups()
{
  if ( m_menusEnabled ) {
    m_fanMenu->showMenu( scenePos() );
    emit menuShown();
  }
}

void TodoItem::hidePopups()
{
  if ( m_menuHandler ) {
    m_fanMenu->hide();
  }
}

void TodoItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  QLineF distance = QLineF( QPointF( 0, 0 ), event->pos() );

  if ( distance.length() <= m_handleItem->itemSize() / 2 + 2 ) {
    showPopups();
  }

  QGraphicsItemGroup::hoverEnterEvent( event );

  m_isHovered = true;
}

void TodoItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  QGraphicsItemGroup::hoverLeaveEvent( event );

  QTimer::singleShot( 0, this, SLOT( checkMenuVisibility() ) );

  m_isHovered = false;
}

void TodoItem::checkMenuVisibility()
{
  if ( m_fanMenu && !m_fanMenu->isHovered() && !m_isHovered ) {
    hidePopups();
  }
}

void TodoItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsItemGroup::mousePressEvent( event );

  hidePopups();

  if ( parentItem() ) parentItem()->setZValue( 100 );
  
  emit itemPressed();
}

void TodoItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsItemGroup::mouseReleaseEvent( event );

  if ( parentItem() ) parentItem()->setZValue( 0 );

  if ( pos() != m_movePos ) {
    emit itemMoved( this, pos() );
  }
}

void TodoItem::emitShowGroup()
{
  emit showGroup( m_todo );
}

void TodoItem::editTodo()
{
  if ( !m_edit ) {
    m_edit = new HidingLineEdit;
    connect( m_edit, SIGNAL( returnPressed() ), SLOT( editTodoDone() ) );
    connect( m_edit, SIGNAL( lostFocus() ), SLOT( editTodoDone() ) );
    m_editProxy = scene()->addWidget( m_edit );
  }

  m_edit->setText( m_todo.summary().value() );
  m_edit->setFocus();

  QRect rect;
  rect.setHeight( m_edit->height() );
  rect.setWidth( m_nameItem->boundingRect().width() + 20 );
  m_edit->setGeometry( rect );

  m_editProxy->show();
  m_editProxy->setPos( mapToScene( m_nameItem->pos() ) );
}

void TodoItem::editTodoDone()
{
  QString newText = m_edit->text();
  
  Bliss::Summary summary = m_todo.summary();

  if ( summary.value() != newText ) {
    summary.setValue( newText );
    m_todo.setSummary( summary );
    m_model->insert( m_todo, i18n("Changed text of todo to '%1'.")
      .arg( newText ) );
  }

  m_editProxy->hide();
}

void TodoItem::emitDone()
{
  emit done( m_todo );
}

void TodoItem::undoMove()
{
  setPos( m_movePos );
}

int TodoItem::itemWidth()
{
  if ( !m_handleItem ) return 0;

  if ( m_nameItem ) {
    return m_handleItem->itemSize() + 16 + m_nameItem->boundingRect().width();
  } else {
    return m_handleItem->itemSize();
  }
}

int TodoItem::textCenterX()
{
  return m_textCenterX;
}

TodoHandleItem *TodoItem::handleItem() const
{
  return m_handleItem;
}
