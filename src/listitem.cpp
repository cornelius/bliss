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
#include "todoitem.h"
#include "newtododialog.h"

#include <KLocale>

ListItem::ListItem( MainModel *model, MenuHandler *menuHandler,
  const Bliss::Todo &group, const Bliss::ViewList &list )
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

void ListItem::updateItem( const Bliss::ViewList &list )
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
  connect( menuItem, SIGNAL( clicked() ), SLOT( editList() ) );

  m_fanMenu->setupItems();

  m_fanMenu->hideMenu();

  int handleItemSize = 40;

  m_handleItem = new MenuHandleItem( m_fanMenu, this );
  m_handleItem->setItemSize( handleItemSize );
  
  m_nameItem = new QGraphicsTextItem( list.name(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;

  int textLeft = 16;
  
  m_nameItem->setPos( handleItemSize / 2 + textLeft, - textHeight / 2 + 2 );

  int itemSize = 30;

  int spacing = 50;
  int y = handleItemSize / 2 + itemSize;
  
  Bliss::Todo::List todos = m_model->todosOfList( m_list );
  foreach( Bliss::Todo todo, todos ) {
    TodoItem *item = new TodoItem( m_model, m_menuHandler, todo );
    item->setParentItem( this );
    item->setPos( 0, y );

    y += spacing;
  }
  
  TodoItem *item = new TodoItem( m_model );
  connect( item, SIGNAL( itemPressed() ), SLOT( newTodo() ) );
  item->setParentItem( this );
  item->setPos( 0, y );

  int listBorder = 10;
  
  setRect( -handleItemSize/2 - listBorder, -handleItemSize/2 - listBorder,
           textWidth + 2*listBorder + textLeft + handleItemSize,
           handleItemSize*1.5 + itemSize + 2*listBorder +
           todos.size() * spacing );
}

Bliss::ViewList ListItem::list() const
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
    m_list.setX( pos().x() );
    m_list.setY( pos().y() );
    m_model->saveViewList( m_group, m_list );
  }
}

void ListItem::emitRemoveList()
{
  emit removeList( this );
}

void ListItem::editList()
{
  if ( !m_edit ) {
    m_edit = new HidingLineEdit;
    connect( m_edit, SIGNAL( returnPressed() ), SLOT( editListDone() ) );
    connect( m_edit, SIGNAL( lostFocus() ), SLOT( editListDone() ) );
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

void ListItem::editListDone()
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

void ListItem::newTodo()
{
  NewTodoDialog *dialog = new NewTodoDialog( m_model );
  if ( dialog->exec() == QDialog::Accepted ) {
    Bliss::Todo todo = dialog->todo();

    Bliss::TodoSequence sequence = m_list.todoSequence();
    Bliss::TodoId id;
    id.setValue( todo.id() );
    sequence.addTodoId( id );

    m_list.setTodoSequence( sequence );

    m_model->addTodo( todo, m_group, m_list );
  }
  return;
}
