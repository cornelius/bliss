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
#include "menuhandleitem.h"

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

  setFlags( ItemIsMovable );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  setBrush( QColor( 210, 210, 190 ) );

  updateItem( m_list );
}

void ListItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;

  if ( !m_fanMenu ) m_fanMenu->hideMenu();
}

void ListItem::updateItem( const Bliss::TodoList &list )
{
  m_list = list;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }

  m_fanMenu = m_menuHandler->createMenu();
  connect( m_fanMenu, SIGNAL( menuShown() ), SIGNAL( menuShown() ) );

  FanMenuItem *menuItem = m_fanMenu->addItem( i18n("Remove") );
  connect( menuItem, SIGNAL( clicked() ), SLOT( emitRemoveList() ) );
  
  menuItem = m_fanMenu->addItem( i18n("Edit") );
  connect( menuItem, SIGNAL( clicked() ), SLOT( editTodo() ) );

  m_fanMenu->setupItems();

  m_fanMenu->hideMenu();

  int itemSize = 40;

  m_handleItem = new MenuHandleItem( m_fanMenu, this );
  m_handleItem->setItemSize( itemSize );
  
  m_nameItem = new QGraphicsTextItem( list.name(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;

  int textLeft = 16;
  
  m_nameItem->setPos( itemSize / 2 + textLeft, - textHeight / 2 + 2 );

  int listBorder = 10;
  int extraBottom = 10;
  
  setRect( -itemSize/2 - listBorder, -itemSize/2 - listBorder,
           textWidth + 2*listBorder + textLeft + itemSize,
           itemSize + 2*listBorder + extraBottom );  
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

void ListItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  RoundedRectItem::hoverEnterEvent( event );
}

void ListItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  RoundedRectItem::hoverLeaveEvent( event );
}

void ListItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  RoundedRectItem::mousePressEvent( event );

  m_fanMenu->hideMenu();

  emit itemPressed();
}

void ListItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  RoundedRectItem::mouseReleaseEvent( event );

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
