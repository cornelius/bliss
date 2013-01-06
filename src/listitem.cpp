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

#include "listitem.h"

#include "mainmodel.h"
#include "fanmenu.h"
#include "roundedrectitem.h"
#include "menuhandler.h"


#include <KLocale>

ListItem::ListItem( MainModel *model, MenuHandler *menuHandler,
  const Bliss::Todo &group, const Bliss::TodoList &list )
  : QObject( model ), m_model( model ), m_group( group ), m_list( list ),
    m_fanMenu( 0 ), m_menuHandler( menuHandler ), m_edit( 0 ), m_editProxy( 0 )
{
  init();
}

void ListItem::init()
{
  m_menusEnabled = true;

  m_isHovered = false;

  setFlags( ItemIsMovable );

  updateItem( m_list );
}

void ListItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;

  if ( !m_menusEnabled ) hidePopups();
}

void ListItem::updateItem( const Bliss::TodoList &list )
{
  m_list = list;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  m_handleItem = new QGraphicsEllipseItem( this );
  m_handleItem->setAcceptHoverEvents( true );

  int itemSize = 40;
  m_handleItem->setRect( -itemSize/2, -itemSize/2, itemSize, itemSize );
  
  m_nameItem = new QGraphicsTextItem( list.name(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;
 
  m_nameItem->setPos( itemSize / 2 + 16, - textHeight / 2 + 2 );
  
  if ( m_menuHandler ) {
    m_fanMenu = m_menuHandler->createMenu();
    connect( m_fanMenu, SIGNAL( hoverStateChanged( bool ) ),
      SLOT( checkMenuVisibility() ) );

    FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Remove") );
    connect( menuItem, SIGNAL( clicked() ), SLOT( emitRemoveList() ) );
    
    menuItem = m_fanMenu->addItem( i18n("Edit") );
    connect( menuItem, SIGNAL( clicked() ), SLOT( editTodo() ) );

    m_fanMenu->setupItems();

    hidePopups();
  }
}

Bliss::TodoList ListItem::list() const
{
  return m_list;
}

void ListItem::setDefaultPos( const QPointF &pos )
{
  m_defaultPos = pos;
}

QPointF ListItem::defaultPos() const
{
  return m_defaultPos;
}

void ListItem::rememberPos( const QPointF &pos )
{
  m_rememberedPos = pos;
}

QPointF ListItem::rememberedPos() const
{
  return m_rememberedPos;
}

void ListItem::showPopups()
{
  if ( m_menusEnabled ) {
    m_menuHandler->showMenu( m_fanMenu, scenePos() );
    emit menuShown();
  }
}

void ListItem::hidePopups()
{
  if ( m_menuHandler ) {
    m_fanMenu->hide();
  }
}

void ListItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  qDebug() << "ListItem::hoverEnterEvent";
  
  QLineF distance = QLineF( QPointF( 0, 0 ), event->pos() );

  if ( distance.length() <= m_handleItem->boundingRect().width() / 2 + 2 ) {
    showPopups();
  }

  QGraphicsItemGroup::hoverEnterEvent( event );

  m_isHovered = true;
}

void ListItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  QGraphicsItemGroup::hoverLeaveEvent( event );

  QTimer::singleShot( 0, this, SLOT( checkMenuVisibility() ) );

  m_isHovered = false;
}

void ListItem::checkMenuVisibility()
{
  if ( m_fanMenu && !m_fanMenu->isHovered() && !m_isHovered ) {
    hidePopups();
  }
}

void ListItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsItemGroup::mousePressEvent( event );

  hidePopups();

  emit itemPressed();
}

void ListItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsItemGroup::mouseReleaseEvent( event );

  if ( pos() != m_movePos ) {
    emit itemMoved( m_list, pos() );
  }
}

void ListItem::emitRemoveList()
{
  emit removeList( this );
}

void ListItem::editTodo()
{
  if ( !m_edit ) {
    m_edit = new HidingLineEdit;
    connect( m_edit, SIGNAL( returnPressed() ), SLOT( editTodoDone() ) );
    connect( m_edit, SIGNAL( lostFocus() ), SLOT( editTodoDone() ) );
    m_editProxy = scene()->addWidget( m_edit );
  }

  m_edit->setText( m_list.name() );
  m_edit->setFocus();

  QRect rect;
  rect.setHeight( m_edit->height() );
  rect.setWidth( m_nameItem->boundingRect().width() + 20 );
  m_edit->setGeometry( rect );

  m_editProxy->show();
  m_editProxy->setPos( mapToScene( m_nameItem->pos() ) );
}

void ListItem::editTodoDone()
{
  QString newText = m_edit->text();

  if ( m_list.name() != newText ) {
    m_list.setName( newText );
    m_model->saveViewList( m_group, m_list );
  }

  m_editProxy->hide();
  
  updateItem( m_list );
}

void ListItem::undoMove()
{
  setPos( m_movePos );
}

int ListItem::textCenterX()
{
  return m_textCenterX;
}

QGraphicsEllipseItem *ListItem::handleItem() const
{
  return m_handleItem;
}
