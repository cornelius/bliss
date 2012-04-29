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

#include "groupgraphicsview.h"

#include "mainmodel.h"
#include "blissitemmodel.h"
#include "todoitem.h"
#include "labelitem.h"
#include "trackinggraphicsview.h"
#include "mainmenuitem.h"
#include "magicmenuitem.h"
#include "settings.h"
#include "groupadderitem.h"
#include "menuhandler.h"
#include "todohandleitem.h"

#include <KLocale>
#include <KInputDialog>
#include <KRandom>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

GroupGraphicsView::GroupGraphicsView( MainModel *model, QWidget *parent )
  : GroupView( model, parent ), m_mainMenu( 0 ), m_magicMenu( 0 ),
    m_groupAdderItem( 0 ),
    m_morphToAnimation( 0 ), m_morphFromAnimation( 0 ),
    m_removeItemsAnimation( 0 ), m_placeItemsAnimation( 0 ), 
    m_unplaceItemsAnimation( 0 ), m_unhideItemsAnimation( 0 ), m_globalMenu( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_scene = new QGraphicsScene;
  m_scene->setBackgroundBrush( QColor( 231,228,211 ) );
  m_scene->setSceneRect( -1000, -2000, 2000, 4000 );

  m_menuHandler = new MenuHandler( m_scene );

  m_view = new TrackingGraphicsView( m_scene );
  m_view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( m_view );
  m_view->show();
  connect( m_view, SIGNAL( mouseMoved( const QPoint & ) ),
    SLOT( slotMouseMoved( const QPoint & ) ) );
  connect( m_view, SIGNAL( viewportMoved() ), SLOT( positionAbsoluteItems() ) );

  connect( model, SIGNAL( todoAdded( const Bliss::Todo & ) ),
    SLOT( slotTodoAdded( const Bliss::Todo & ) ) );
  connect( model, SIGNAL( todoChanged( const Bliss::Todo & ) ),
    SLOT( slotTodoChanged( const Bliss::Todo & ) ) );
  connect( model, SIGNAL( todoRemoved( const Bliss::Todo & ) ),
    SLOT( slotTodoRemoved( const Bliss::Todo & ) ) );

  createMenuItems();
  positionMenuItems();

  setMinimumWidth( 50 );
}

void GroupGraphicsView::readConfig()
{
  if ( Settings::groupAdderExpanded() ) m_groupAdderItem->expand();
  if ( Settings::groupAdderGroupsExpanded() ) {
    m_groupAdderItem->expandGroupItems( false );
  }
}

void GroupGraphicsView::writeConfig()
{
  Settings::setGroupAdderExpanded( m_groupAdderItem->isExpanded() );
  Settings::setGroupAdderGroupsExpanded( m_groupAdderItem->shownAsSidebar() );
  Settings::setAdderGroup( m_groupAdderItem->group().id() );
}

void GroupGraphicsView::slotTodoChanged( const Bliss::Todo &todo )
{
  TodoItem *i = item( todo );
    
  if ( todo.groups().findGroup( group().id() ).isValid() && i ) {
    i->updateItem( todo );
  } else {
    slotTodoRemoved( todo);
  }
}

void GroupGraphicsView::slotTodoAdded( const Bliss::Todo &identity )
{
  Q_UNUSED( identity )

  recreateItems();
}

void GroupGraphicsView::slotTodoRemoved( const Bliss::Todo &todo )
{
  TodoItem *todoItem = item( todo );
  if ( todoItem ) {
    delete todoItem;
    m_items.removeAll( todoItem );
  }
  
  preparePlaceItemsAnimation();
  
  preparePositions( m_items, true );

  m_placeItemsAnimation->start();
}

void GroupGraphicsView::recreateItems()
{
  m_previousItem = 0;
  placeItems();
}

void GroupGraphicsView::doShowGroup()
{
  m_previousItem = 0;

  if ( m_removeItemsAnimation ) m_removeItemsAnimation->stop();
  if ( m_placeItemsAnimation ) m_placeItemsAnimation->stop();
  if ( m_unplaceItemsAnimation ) m_unplaceItemsAnimation->stop();
  if ( m_unhideItemsAnimation ) m_unhideItemsAnimation->stop();

  if ( group().isValid() ) {
    m_previousItem = item( group() );
  }

  if ( m_previousItem ) {
    hideItems();
  } else {
    if ( m_items.isEmpty() ) {
      placeItems();
    } else {
      unplaceItems();
    }
  }
}

void GroupGraphicsView::hideItems()
{
  if ( !m_removeItemsAnimation ) {
    m_removeItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_removeItemsAnimation, SIGNAL( finished() ),
      SLOT( placeItems() ) );
  }
  m_removeItemsAnimation->clear();

  foreach( TodoItem *item, m_items ) {
    if ( item == m_previousItem ) continue;

    QPropertyAnimation *animation = new QPropertyAnimation(item, "opacity", this);
    m_removeItemsAnimation->insertAnimation( 0, animation );
    animation->setStartValue( 1 );
    animation->setEndValue( 0 );
    animation->setDuration( 200 );
  }

  m_removeItemsAnimation->start();  
}

void GroupGraphicsView::clearItems()
{
  foreach( TodoItem *item, m_items ) {
    delete item;
  }
  m_items.clear();

  foreach( LabelItem *item, m_labelItems ) {
    delete item;
  }
  m_labelItems.clear();

  if ( m_globalMenu ) delete m_globalMenu;
  m_globalMenu = 0;
}

void GroupGraphicsView::preparePlaceItemsAnimation()
{
  if ( !m_placeItemsAnimation ) {
    m_placeItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_placeItemsAnimation, SIGNAL( finished() ),
      SLOT( finishPlaceItems() ) );
  } else {
    m_placeItemsAnimation->stop();
  }
  m_placeItemsAnimation->clear();
  m_placeItemsAnimations.clear();
}

void GroupGraphicsView::placeItems()
{
  bool doAnimation = false;
  QPoint previousItemPos;

  if ( m_previousItem ) {
    doAnimation = true;

    preparePlaceItemsAnimation();

    previousItemPos = m_view->mapFromScene( m_previousItem->pos() );
  }

  clearItems();

  TodoItemGroup items = prepareTodoItems( doAnimation );

  m_items = items.items;
  foreach( TodoItem *item, m_items ) {
    m_scene->addItem( item );
  }

  m_view->centerOn( items.center );

  if ( doAnimation ) {
    foreach( QPropertyAnimation *animation, m_placeItemsAnimations ) {
      animation->setStartValue( m_view->mapToScene( previousItemPos ) );
    }
    
    m_placeItemsAnimation->start();
  } else {
    createLabelItems();
  }

  foreach( TodoItem *item, m_items ) {
    item->hidePopups();
  }
}

void GroupGraphicsView::finishPlaceItems()
{
  createLabelItems();
}

void GroupGraphicsView::unplaceItems()
{
  foreach( LabelItem *item, m_labelItems ) {
    delete item;
  }
  m_labelItems.clear();

  if ( !m_unplaceItemsAnimation ) {
    m_unplaceItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_unplaceItemsAnimation, SIGNAL( finished() ),
      SLOT( unhideItems() ) );
  }
  m_unplaceItemsAnimation->clear();

  m_newItems = prepareTodoItems( false );

  if ( !m_newItems.previousGroup ) {
    recreateItems();
    return;
  }

  QPointF target = m_newItems.previousGroup->pos();

  QRect viewportRect = m_view->viewport()->rect();
  QPoint currentViewportCenter( viewportRect.width() / 2,
    viewportRect.height() / 2 );
  QPointF currentCenter = m_view->mapToScene( currentViewportCenter );
  
  target.setX( target.x() - m_newItems.center.x() + currentCenter.x() );
  target.setY( target.y() - m_newItems.center.y() + currentCenter.y() );
  
  foreach( TodoItem *item, m_items ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_unplaceItemsAnimation->insertAnimation( 0, animation );

    animation->setDuration( 300 );
    animation->setEndValue( target );
    animation->setEasingCurve( QEasingCurve::OutCubic );    
  }

  m_unplaceItemsAnimation->start();
}

void GroupGraphicsView::unhideItems()
{
  clearItems();

  m_items = m_newItems.items;
  foreach( TodoItem *item, m_items ) {
    item->setOpacity( 0 );
    m_scene->addItem( item );
  }

  createLabelItems();

  m_view->centerOn( m_newItems.center );

  if ( !m_unhideItemsAnimation ) {
    m_unhideItemsAnimation = new QParallelAnimationGroup( this );
  }
  m_unhideItemsAnimation->clear();

  foreach( TodoItem *item, m_items ) {
    if ( item == m_newItems.previousGroup ) {
      item->setOpacity( 1 );
    } else {
      QPropertyAnimation *animation = new QPropertyAnimation(item, "opacity", this);
      m_unhideItemsAnimation->insertAnimation( 0, animation );
      animation->setStartValue( 0 );
      animation->setEndValue( 1 );
      animation->setDuration( 200 );
    }
  }

  m_newItems = TodoItemGroup();

  m_unhideItemsAnimation->start();
}

TodoItemGroup GroupGraphicsView::prepareTodoItems( bool doAnimation )
{
  TodoItemGroup result;
  
  Bliss::Todo::List todos = model()->todosOfGroup( group() );

  foreach( Bliss::Todo todo, todos ) {
    TodoItem *item = new TodoItem( model(), m_menuHandler, todo );
    result.items.append( item );

    connect( item, SIGNAL( showGroup( const Bliss::Todo & ) ),
      SIGNAL( requestShowGroup( const Bliss::Todo & ) ) );
    connect( item, SIGNAL( removeTodo( const Bliss::Todo & ) ),
      SLOT( slotRemoveTodo( const Bliss::Todo & ) ) );
    connect( item, SIGNAL( done( const Bliss::Todo & ) ),
      SLOT( slotDone( const Bliss::Todo & ) ) );
      
    connect( item, SIGNAL( itemMoved( TodoItem *, const QPointF & ) ),
      SLOT( slotItemMoved( TodoItem *, const QPointF & ) ) );

    connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );

    if ( previousGroup().isValid() &&
         item->todo().id() == previousGroup().id() ) {
      result.previousGroup = item;
    }
  }

  // Create handle item for adding new todos
  TodoItem *item = new TodoItem( model() );
  connect( item, SIGNAL( itemPressed() ), SIGNAL( newTodo() ) );
  result.items.append( item );
  
  result.center = preparePositions( result.items, doAnimation );

  return result;
}

QPointF GroupGraphicsView::preparePositions( const QList<TodoItem *> &todoItems,
  bool doAnimation )
{
  int spacing = 50;

  int x = 0;
  int y = 0;

  qreal minY = 0;
  qreal maxY = 0;

  qreal centerX = 0;

  bool firstItem = true;

  Bliss::GroupView view = model()->groupView( group() );

  foreach( TodoItem *item, todoItems ) {
    qreal posX = x;
    qreal posY = y * spacing;

    item->setDefaultPos( QPointF( posX, posY ) );

    qreal itemX;
    qreal itemY;

    Bliss::TodoPosition p = view.findTodoPosition( item->todo().id() );
    if ( p.isValid() ) {
      itemX = p.x();
      itemY = p.y();
    } else {
      itemX = posX;
      itemY = posY;
    }

    preparePos( item, itemX, itemY, doAnimation );

    if ( firstItem ) {
      firstItem = false;
    
      centerX = item->textCenterX();
      minY = itemY;
      maxY = itemY;
    } else {
      if ( item->textCenterX() > centerX ) centerX = item->textCenterX();
      if ( itemY < minY ) minY = itemY;
      if ( itemY > maxY ) maxY = itemY;
    }

    y++;    
  }

  qreal centerY = ( minY + maxY ) / 2;

  return QPointF( centerX, centerY );
}
  
void GroupGraphicsView::preparePos( TodoItem *item, qreal itemX, qreal itemY,
  bool doAnimation )
{
  if ( doAnimation ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_placeItemsAnimation->insertAnimation( 0, animation );
    m_placeItemsAnimations.append( animation );

    animation->setDuration( 300 );
    QPointF target( itemX, itemY );
    animation->setEndValue( target );
    animation->setEasingCurve( QEasingCurve::OutCubic );
  } else {
    item->setPos( itemX, itemY );
  }
}

void GroupGraphicsView::createLabelItems()
{
  Bliss::GroupView view = model()->groupView( group() );

  foreach( Bliss::ViewLabel label, view.viewLabelList() ) {
    createLabelItem( label );
  }
}

void GroupGraphicsView::createMenuItems()
{
  if ( Settings::enableMagic() ) {
    m_magicMenu = new MagicMenuItem();
    m_scene->addItem( m_magicMenu );
  
    connect( m_magicMenu, SIGNAL( resetLayout() ), SLOT( resetLayout() ) );
    connect( m_magicMenu, SIGNAL( showSettings() ), SIGNAL( showSettings() ) );
  }

  m_mainMenu = new MainMenuItem();
  m_scene->addItem( m_mainMenu );

  connect( m_mainMenu, SIGNAL( cloneGroup() ), SLOT( emitCloneGroup() ) );
  connect( m_mainMenu, SIGNAL( removeGroup() ), SLOT( emitRemoveGroup() ) );
  connect( m_mainMenu, SIGNAL( addGroup() ), SIGNAL( newGroup() ) );
  connect( m_mainMenu, SIGNAL( addTodo() ), SIGNAL( newTodo() ) );

  m_groupAdderItem = new GroupAdderItem( model() );
  m_scene->addItem( m_groupAdderItem );
  m_groupAdderItem->setZValue( -100 );
}

void GroupGraphicsView::positionMenuItems()
{
  QRect viewportRect = m_view->viewport()->rect();

  QPoint upperRight( viewportRect.width(), 0 );
  QPointF upperRightScene = m_view->mapToScene( upperRight );

  QPoint lowerLeft( 0, viewportRect.height() );
  QPointF lowerLeftScene = m_view->mapToScene( lowerLeft );

  if ( m_mainMenu ) {
    m_mainMenu->setPos( upperRightScene.x() - 50, upperRightScene.y() + 50 );
  }
  if ( m_magicMenu ) {
    m_magicMenu->setPos( upperRightScene.x() - 50, upperRightScene.y() + 130 );
  }
  if ( m_groupAdderItem ) {
    m_groupAdderItem->setPos( lowerLeftScene );
  }
}

void GroupGraphicsView::positionAbsoluteItems()
{
  positionMenuItems();
}

void GroupGraphicsView::slotRemoveTodo( const Bliss::Todo &identity )
{
  model()->removeTodo( identity, group() );
}

void GroupGraphicsView::slotDone( const Bliss::Todo &todo )
{
  model()->deleteTodo( todo );
}

void GroupGraphicsView::slotItemMoved( TodoItem *todoItem,
  const QPointF &pos )
{
  Q_UNUSED( pos )

  Bliss::Todo target =
    m_groupAdderItem->collidedGroup( todoItem->handleItem() );
  if ( target.isValid() ) {
    model()->moveTodo( todoItem->todo(), group(), target );
  } else {
    preparePlaceItemsAnimation();
    
    QMap <qreal, QString> map;
    foreach( TodoItem *i, m_items ) {
      map.insert( i->pos().y(), i->todo().id() );
    }
    model()->saveViewSequence( group(), map.values() );
  
    QList<TodoItem *> sortedItems;
    
    Bliss::Todo::List todos = model()->todosOfGroup( group() );
    foreach( Bliss::Todo todo, todos ) {
      sortedItems.append( item( todo ) );
    }
    
    TodoItem *addNewItem = m_items.last();
    m_items = sortedItems;
    m_items.append( addNewItem );

    preparePositions( m_items, true );

    m_placeItemsAnimation->start();
  }
}

void GroupGraphicsView::saveLabel( const Bliss::ViewLabel &label,
  const QPointF &pos )
{
  Bliss::ViewLabel l = label;
  l.setX( pos.x() );
  l.setY( pos.y() );
  model()->saveViewLabel( group(), l );
}


void GroupGraphicsView::addLabel()
{
  addLabel( m_view->mapToScene( QPoint( 10, 10 ) ) );
}

void GroupGraphicsView::addLabel( const QPointF &pos )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Add Label"),
    i18n("Enter text of label"), QString(),
    &ok );
  if ( ok ) {
    Bliss::ViewLabel label;
    label.setId( KRandom::randomString( 10 ) );
    label.setText( name );

    label.setX( pos.x() );
    label.setY( pos.y() );
    
    createLabelItem( label );
    
    model()->saveViewLabel( group(), label );
  }
}

void GroupGraphicsView::removeLabel( LabelItem *item )
{
  m_labelItems.removeAll( item );

  delete item;
  model()->removeViewLabel( group(), item->label() );
}

void GroupGraphicsView::renameLabel( LabelItem *item )
{
  Bliss::ViewLabel label = item->label();

  bool ok;
  QString name = KInputDialog::getText( i18n("Rename Label"),
    i18n("Enter new text of label"), label.text(),
    &ok );
  if ( ok ) {
    label.setText( name );
    item->setLabel( label );
    model()->saveViewLabel( group(), label );
  }
}

LabelItem *GroupGraphicsView::createLabelItem( const Bliss::ViewLabel &label )
{
  LabelItem *item = new LabelItem( model(), label );

  connect( item, SIGNAL( itemMoved( const Bliss::ViewLabel &, const QPointF & ) ),
    SLOT( saveLabel( const Bliss::ViewLabel &, const QPointF & ) ) );
  connect( item, SIGNAL( removeLabel( LabelItem * ) ),
    SLOT( removeLabel( LabelItem * ) ) );
  connect( item, SIGNAL( renameLabel( LabelItem * ) ),
    SLOT( renameLabel( LabelItem * ) ) );
  connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );

  m_scene->addItem( item );

  item->setPos( label.x(), label.y() );

  m_labelItems.append( item );

  return item;
}

void GroupGraphicsView::resetLayout()
{
  model()->clearViewPositions( group() );

  foreach( TodoItem *item, m_items ) {
    if ( item->pos() != item->defaultPos() ) {
#if QT_VERSION >= 0x040600
      QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);

      animation->setDuration(500);
      animation->setStartValue( item->pos() );
      animation->setEndValue( item->defaultPos() );
      animation->setEasingCurve( QEasingCurve::OutCubic );

      animation->start();
#else
      item->setPos( item->defaultPos() );
#endif
    }
  }
}

void GroupGraphicsView::emitCloneGroup()
{
  emit cloneGroup( group() );
}

void GroupGraphicsView::emitRemoveGroup()
{
  emit removeGroup( group() );
}

TodoItem *GroupGraphicsView::item( const Bliss::Todo &identity ) const
{
  foreach( TodoItem *item, m_items ) {
    if ( item->todo().id() == identity.id() ) return item;
  }
  return 0;
}

void GroupGraphicsView::addLabelClicked()
{
  hideGlobalMenu();

  addLabel( m_globalMenu->pos() );
}

void GroupGraphicsView::hideGlobalMenu()
{
  if ( m_globalMenu ) {
    m_globalMenu->hide();
  }
}

void GroupGraphicsView::slotMouseMoved( const QPoint &pos )
{
  if ( !m_globalMenu || !m_globalMenu->isVisible() ) return;

  if ( !m_globalMenu->isCloseTo( m_view->mapToScene( pos ) ) ) {
    hideGlobalMenu();
  }
}

void GroupGraphicsView::setAdderGroup( const Bliss::Todo &group )
{
  m_groupAdderItem->setGroup( group );
}
