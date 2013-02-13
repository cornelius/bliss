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
#include "itemplacer.h"

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
  m_itemSize = 30;
  m_handleItemSize = 40;
  m_spacing = 50;
  m_listBorder = 10;
  m_textLeft = 16;
  
  
  m_itemPlacer = new ItemPlacer( this );
  
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

  m_todoItems.clear();
  
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

  m_handleItem = new MenuHandleItem( m_fanMenu, this );
  m_handleItem->setItemSize( m_handleItemSize );
  
  m_nameItem = new QGraphicsTextItem( list.name(), this );
  m_nameItem->setAcceptHoverEvents( false );

  int textWidth = m_nameItem->boundingRect().width();
  int textHeight = m_nameItem->boundingRect().height();

  m_textCenterX = textWidth / 2;

  m_nameItem->setPos( m_handleItemSize / 2 + m_textLeft, - textHeight / 2 + 2 );

  Bliss::Todo::List todos = m_model->todosOfList( m_list );
  foreach( Bliss::Todo todo, todos ) {
    TodoItem *item = createItem( todo );
    m_todoItems.append( item );
  }
  
  m_adderItem = new TodoItem( m_model );
  connect( m_adderItem, SIGNAL( itemPressed() ), SLOT( newTodo() ) );
  m_adderItem->setParentItem( this );
  m_todoItems.append( m_adderItem );

  m_itemPlacer->prepare( false );
  preparePositions();
  m_itemPlacer->start();
  
  setListBox();
}

void ListItem::setListBox()
{
  int listWidth = m_nameItem->boundingRect().width() + m_textLeft +
    m_handleItemSize;
  foreach( TodoItem *item, m_todoItems ) {
    int itemWidth = item->itemWidth();
    if ( itemWidth > listWidth ) {
      listWidth = itemWidth;
    }
  }

  setRect( -m_handleItemSize/2 - m_listBorder,
           -m_handleItemSize/2 - m_listBorder,
           listWidth + 2*m_listBorder,
           m_handleItemSize*1.5 + m_itemSize + 2*m_listBorder +
           ( m_todoItems.size() -1 ) * m_spacing );
}

TodoItem *ListItem::createItem( const Bliss::Todo &todo )
{
  TodoItem *item = new TodoItem( m_model, m_menuHandler, todo );

  connect( item, SIGNAL( removeTodo( const Bliss::Todo & ) ),
            SIGNAL( removeTodo( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( done( const Bliss::Todo & ) ),
            SIGNAL( done( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( itemMoved( TodoItem *, const QPointF & ) ),
            SLOT( slotItemMoved( TodoItem *, const QPointF & ) ) );

  item->setParentItem( this );
  
  return item;
}

void ListItem::updateTodoItem( const Bliss::Todo &todo )
{
  foreach( TodoItem *item, m_todoItems ) {
    if ( item->todo().id() == todo.id() ) {
      item->updateItem( todo );
    }
  }
}

void ListItem::preparePositions()
{
  int y = m_handleItemSize / 2 + m_itemSize;

  foreach( TodoItem *item, m_todoItems ) {
    m_itemPlacer->addItem( item, 0, y );
    y += m_spacing;
  }
}


void ListItem::setList( const Bliss::ViewList &list )
{
  m_list = list;
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

void ListItem::repositionItems()
{
  m_itemPlacer->prepare();

  QMap <qreal, TodoItem *> map;
  foreach( TodoItem *i, m_todoItems ) {
    if ( i != m_adderItem ) map.insert( i->pos().y(), i );
  }

  Bliss::TodoSequence sequence;
  foreach( TodoItem *i, map.values() ) {
    Bliss::TodoId todoId;
    todoId.setValue( i->todo().id() );
    sequence.addTodoId( todoId );
  }

  m_list.setTodoSequence( sequence );

  m_todoItems = map.values();
  m_todoItems.append( m_adderItem );
  
  preparePositions();

  m_itemPlacer->start();
}

bool ListItem::hasItem( TodoItem *item ) const
{
  return m_todoItems.contains( item );
}

void ListItem::slotItemMoved( TodoItem *item, const QPointF &pos )
{
  emit itemMoved( item, mapToScene( pos ) );
}

void ListItem::addTodo( const Bliss::Todo &todo )
{
  TodoItem *item = createItem( todo );
  addItem( item );
}

void ListItem::addItem( TodoItem *item )
{
  m_todoItems.insert( m_todoItems.size() - 1, item );

  m_itemPlacer->prepare();
  preparePositions();
  m_itemPlacer->start();

  setListBox();
}

void ListItem::removeItem( TodoItem *item )
{
  m_todoItems.removeOne( item );
  repositionItems();

  setListBox();
}

TodoItem *ListItem::item( const Bliss::Todo &todo )
{
  foreach( TodoItem *i, m_todoItems ) {
    if ( i->todo().id() == todo.id() ) return i;
  }
  return 0;
}
