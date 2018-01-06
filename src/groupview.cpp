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

#include "groupview.h"

#include "mainmodel.h"
#include "blissitemmodel.h"
#include "todoitem.h"
#include "trackinggraphicsview.h"
#include "mainmenuitem.h"
#include "magicmenuitem.h"
#include "settings.h"
#include "groupadderitem.h"
#include "menuhandler.h"
#include "todohandleitem.h"
#include "listitem.h"
#include "newlistdialog.h"
#include "itemplacer.h"
#include "buttonitem.h"

#include <KLocale>
#include <KRandom>

GroupView::GroupView( MainModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_backButton( 0 ), m_mainMenu( 0 ),
    m_magicMenu( 0 ),
    m_groupAdderItem( 0 ),
    m_morphToAnimation( 0 ), m_morphFromAnimation( 0 ),
    m_removeItemsAnimation( 0 ),
    m_unhideItemsAnimation( 0 ),
    m_itemPlacer( 0 ), m_globalMenu( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_scene = new QGraphicsScene;
  m_scene->setBackgroundBrush( QColor( 231,228,211 ) );
  m_scene->setSceneRect( -2000, -2000, 4000, 4000 );

  m_itemPlacer = new ItemPlacer( this );
  connect( m_itemPlacer, SIGNAL( finished() ), SLOT( finishPlaceItems() ) );

  m_itemUnplacer = new ItemPlacer( this );
  connect( m_itemUnplacer, SIGNAL( finished() ), SLOT( unhideItems() ) );

  m_removeItemPlacer = new ItemPlacer( this );

  m_menuHandler = new MenuHandler( m_scene );

  m_view = new TrackingGraphicsView( m_scene );
  m_view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( m_view );
  m_view->show();
  connect( m_view, SIGNAL( mouseMoved( const QPoint & ) ),
    SLOT( slotMouseMoved( const QPoint & ) ) );
  connect( m_view, SIGNAL( viewportMoved() ), SLOT( positionAbsoluteItems() ) );
  connect( m_view, SIGNAL( movementStopped( const QPoint & ) ),
    SLOT( rememberPosition( const QPoint & ) ) );

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

void GroupView::readConfig()
{
  if ( Settings::groupAdderExpanded() ) m_groupAdderItem->expand();
  if ( Settings::groupAdderGroupsExpanded() ) {
    m_groupAdderItem->expandGroupItems( false );
  }
  m_viewPositions.set( Settings::viewPositions() );
}

void GroupView::writeConfig()
{
  Settings::setGroupAdderExpanded( m_groupAdderItem->isExpanded() );
  Settings::setGroupAdderGroupsExpanded( m_groupAdderItem->shownAsSidebar() );
  Settings::setAdderGroup( m_groupAdderItem->group().id() );
  Settings::setViewPositions( m_viewPositions.get() );
}

void GroupView::setBackButtonEnabled( bool enabled )
{
  if ( enabled ) {
    m_backButton->show();
  } else {
    m_backButton->hide();
  }
}

void GroupView::slotTodoChanged( const Bliss::Todo &todo )
{
  TodoItem *i = item( todo );
  if ( i ) {
    if ( todo.groups().findGroup( m_group.id() ).isValid() ) {
      i->updateItem( todo );
    } else {
      slotTodoRemoved( todo);
    }
  }

  foreach ( ListItem *listItem, m_listItems ) {
    listItem->updateTodoItem( todo );
  }
}

Bliss::ViewList GroupView::viewList( const Bliss::Todo &todo )
{
  Bliss::ViewList list;
  Bliss::ViewList::List lists = m_model->lists( m_group );
  foreach( Bliss::ViewList l, lists ) {
    foreach( Bliss::TodoId id, l.todoSequence().todoIdList() ) {
      if ( id.value() == todo.id() ) {
        list = l;
        break;
      }
    }
    if ( list.isValid() ) break;
  }
  return list;
}

void GroupView::slotTodoAdded( const Bliss::Todo &todo )
{
  Bliss::ViewList list = viewList( todo );

  if ( list.isValid() ) {
    foreach( ListItem *listItem, m_listItems ) {
      if ( listItem->list().id() == list.id() ) {
        listItem->addTodo( todo );
        listItem->setList( list );
        listItem->repositionSortedItems();
        break;
      }
    }
  } else {
    TodoItem *item = createTodoItem( todo );
    m_items.insert( m_items.size() - 1, item );
    m_scene->addItem( item );

    m_itemPlacer->prepare();
    preparePositions( m_items, m_itemPlacer );
    m_itemPlacer->start();
  }
}

void GroupView::slotTodoRemoved( const Bliss::Todo &todo )
{
  TodoItem *todoItem = item( todo );
  if ( todoItem ) {
    delete todoItem;
    m_items.removeAll( todoItem );

    m_removeItemPlacer->prepare();
    preparePositions( m_items, m_removeItemPlacer );
    m_removeItemPlacer->start();
  }

  foreach( ListItem *listItem, m_listItems ) {
    todoItem = listItem->item( todo );
    if ( todoItem ) {
      listItem->removeItem( todoItem );
      delete todoItem;

      Bliss::ViewList list = m_model->groupView( m_group )
        .findViewList( listItem->list().id() );
      listItem->setList( list );
    }
  }
}

void GroupView::recreateItems()
{
  m_previousItem = 0;
  placeItems();
}

void GroupView::showGroup( const Bliss::Todo &group )
{
  m_previousGroup = m_group;
  m_group = group;

  m_previousItem = 0;

  if ( m_removeItemsAnimation ) m_removeItemsAnimation->stop();
  if ( m_unhideItemsAnimation ) m_unhideItemsAnimation->stop();

  m_itemPlacer->stop();
  m_itemUnplacer->stop();

  if ( m_group.isValid() ) {
    m_previousItem = item( m_group );
    if ( m_previousItem ) {
      m_previousItemPos = m_previousItem->pos();
    } else {
      foreach( ListItem *list, m_listItems ) {
        TodoItem *item = list->item( m_group );
        if ( item ) {
          m_previousItem = item;
          m_previousItemPos = list->mapToScene( item->pos() );
          break;
        }
      }
    }

    m_titleItem->setPlainText( m_group.summary().value() );
    positionMenuItems();
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

void GroupView::hideItems()
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

void GroupView::clearItems()
{
  foreach( TodoItem *item, m_items ) {
    delete item;
  }
  m_items.clear();

  clearListItems();

  if ( m_globalMenu ) delete m_globalMenu;
  m_globalMenu = 0;
}

void GroupView::clearListItems()
{
  foreach( ListItem *item, m_listItems ) {
    delete item;
  }
  m_listItems.clear();
}

void GroupView::placeItems()
{
  bool doAnimation = false;

  if ( m_previousItem ) {
    doAnimation = true;
  }

  m_itemPlacer->prepare( doAnimation );

  clearItems();

  TodoItemGroup items = prepareTodoItems( m_itemPlacer );

  m_items = items.items;
  foreach( TodoItem *item, m_items ) {
    m_scene->addItem( item );
  }

  if ( m_viewPositions.hasPosition( m_group ) ) {
    QTimer::singleShot( 0, this, SLOT( setViewPosition() ) );
    m_previousItemPos += m_viewPositions.position( m_group ) -
                       m_view->position();
  } else {
    QRect viewportRect = m_view->viewport()->rect();
    QPoint currentViewportCenter( viewportRect.width() / 2,
      viewportRect.height() / 2 );
    QPointF currentCenter = m_view->mapToScene( currentViewportCenter );

    m_previousItemPos += items.center - currentCenter;

    m_view->centerOn( items.center );
  }

  foreach( TodoItem *item, m_items ) {
    item->enableMenus( false );
  }

  m_itemPlacer->setStartValue( m_previousItemPos );

  m_itemPlacer->start();
}

void GroupView::finishPlaceItems()
{
  createListItems();

  foreach( TodoItem *item, m_items ) {
    item->enableMenus( true );
  }
}

void GroupView::unplaceItems()
{
  foreach( ListItem *item, m_listItems ) {
    delete item;
  }
  m_listItems.clear();

  m_itemUnplacer->prepare();

  m_newItems = prepareTodoItems( m_itemUnplacer );

  m_previousItem = 0;
  if ( m_previousGroup.isValid() ) {
    foreach( TodoItem *item, m_newItems.items ) {
      if ( item->todo().id() == m_previousGroup.id() ) {
        m_previousItem = item;
        m_previousItemPos = m_itemUnplacer->targetPosition( item );
        break;
      }
    }
  }

  if ( !m_previousItem ) {
    // Temporary create list items to get positions. There certainly is a more
    // efficient way to do this.
    createListItems();
    foreach( ListItem *list, m_listItems ) {
      if ( list->item( m_previousGroup ) ) {
        m_previousItem = list->item( m_previousGroup );
        m_previousItemPos = list->mapToScene( m_previousItem->pos() );
        break;
      }
    }
    clearListItems();
  }

  if ( !m_previousItem ) {
    recreateItems();
    return;
  }

  QPointF target = m_previousItemPos;

  if ( m_viewPositions.hasPosition( m_group ) ) {
    target += m_view->position() - m_viewPositions.position( m_group );
  } else {
    QRect viewportRect = m_view->viewport()->rect();
    QPoint currentViewportCenter( viewportRect.width() / 2,
      viewportRect.height() / 2 );
    QPointF currentCenter = m_view->mapToScene( currentViewportCenter );

    target += currentCenter - m_newItems.center;
  }

  foreach( TodoItem *item, m_items ) {
    m_itemUnplacer->addItem( item, target );
  }

  m_itemUnplacer->start();
}

void GroupView::unhideItems()
{
  clearItems();

  m_items = m_newItems.items;
  foreach( TodoItem *item, m_items ) {
    item->setOpacity( 0 );
    m_scene->addItem( item );
  }

  createListItems();

  if ( m_viewPositions.hasPosition( m_group ) ) {
    QTimer::singleShot( 0, this, SLOT( setViewPosition() ) );
  } else {
    m_view->centerOn( m_newItems.center );
  }

  if ( !m_unhideItemsAnimation ) {
    m_unhideItemsAnimation = new QParallelAnimationGroup( this );
  }
  m_unhideItemsAnimation->clear();

  foreach( TodoItem *item, m_items ) {
    if ( item == m_previousItem ) {
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

TodoItemGroup GroupView::prepareTodoItems( ItemPlacer *placer )
{
  TodoItemGroup result;

  Bliss::Todo::List todos = m_model->unlistedTodosOfGroup( m_group );

  foreach( Bliss::Todo todo, todos ) {
    TodoItem *item = createTodoItem( todo );
    result.items.append( item );
  }

  // Create handle item for adding new todos
  TodoItem *item = new TodoItem( m_model );
  connect( item, SIGNAL( itemPressed() ), SIGNAL( newTodo() ) );
  result.items.append( item );

  result.center = preparePositions( result.items, placer );

  return result;
}

TodoItem *GroupView::createTodoItem( const Bliss::Todo &todo )
{
  TodoItem *item = new TodoItem( m_model, m_menuHandler, todo );

  connect( item, SIGNAL( removeGroup( const Bliss::Todo & ) ),
    SIGNAL( removeGroup( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( showGroup( const Bliss::Todo & ) ),
    SIGNAL( requestShowGroup( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( done( const Bliss::Todo & ) ),
    SLOT( slotDone( const Bliss::Todo & ) ) );

  connect( item, SIGNAL( itemMoved( TodoItem *, const QPointF & ) ),
    SLOT( slotItemMoved( TodoItem *, const QPointF & ) ) );

  connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );

  return item;
}

QPointF GroupView::preparePositions( const QList<TodoItem *> &todoItems,
  ItemPlacer *placer )
{
  int spacing = 50;

  int x = 0;
  int y = 0;

  qreal minY = 0;
  qreal maxY = 0;

  qreal centerX = 0;

  bool firstItem = true;

  Bliss::GroupView view = m_model->groupView( m_group );

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

    placer->addItem( item, itemX, itemY );

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

void GroupView::createListItems()
{
  clearListItems();

  Bliss::GroupView view = m_model->groupView( m_group );

  foreach( Bliss::ViewList list, view.viewListList() ) {
    createListItem( list );
  }
}

void GroupView::createMenuItems()
{
  m_titleItem = new QGraphicsTextItem();
  m_scene->addItem( m_titleItem );
  QFont font = m_titleItem->font();
  font.setBold( true );
  font.setPointSizeF( font.pointSizeF() * 1.5 );
  m_titleItem->setFont( font );

  m_backButton = new ButtonItem;
  m_scene->addItem( m_backButton );
  m_backButton->setItemSize( 50 );
  m_backButton->setBack();
  connect( m_backButton, SIGNAL( clicked() ), SIGNAL( goBack() ) );

  if ( Settings::enableMagic() ) {
    m_magicMenu = new MagicMenuItem();
    m_scene->addItem( m_magicMenu );

    connect( m_magicMenu, SIGNAL( resetLayout() ), SLOT( resetLayout() ) );
    connect( m_magicMenu, SIGNAL( showSettings() ), SIGNAL( showSettings() ) );
  }

  m_mainMenu = new MainMenuItem();
  m_scene->addItem( m_mainMenu );

  connect( m_mainMenu, SIGNAL( addGroup() ), SIGNAL( newGroup() ) );
  connect( m_mainMenu, SIGNAL( addList() ), SLOT( addList() ) );
  connect( m_mainMenu, SIGNAL( addTodo() ), SIGNAL( newTodo() ) );
  connect( m_mainMenu, SIGNAL( showMore() ), SIGNAL( showOverview() ) );

  m_groupAdderItem = new GroupAdderItem( m_model );
  m_scene->addItem( m_groupAdderItem );
  m_groupAdderItem->setZValue( -100 );
}

void GroupView::positionMenuItems()
{
  QRect viewportRect = m_view->viewport()->rect();

  QPoint upperRight( viewportRect.width(), 0 );
  QPointF upperRightScene = m_view->mapToScene( upperRight );

  QPoint lowerLeft( 0, viewportRect.height() );
  QPointF lowerLeftScene = m_view->mapToScene( lowerLeft );

  QPointF upperLeftScene = m_view->mapToScene( 0, 0 );

  if ( m_titleItem ) {
    m_titleItem->setPos( ( upperRightScene.x() + upperLeftScene.x() ) / 2 -
      m_titleItem->boundingRect().width() / 2,
      upperLeftScene.y() - m_titleItem->boundingRect().height() / 2 + 50 );
  }

  if ( m_backButton ) {
    m_backButton->setPos( upperLeftScene.x() + 50, upperLeftScene.y() + 50 );
  }

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

void GroupView::positionAbsoluteItems()
{
  positionMenuItems();
}

void GroupView::slotDone( const Bliss::Todo &todo )
{
  m_model->deleteTodo( todo, m_group );
}

void GroupView::slotItemMoved( TodoItem *todoItem,
  const QPointF &pos )
{
  ListItem *listSource = listItem( todoItem );

  ListItem *listTarget = 0;
  foreach( ListItem *listItem, m_listItems ) {
    QPointF relativePos = listItem->mapFromScene( pos );
    if ( listItem->contains( relativePos ) ) {
      listTarget = listItem;
      break;
    }
  }

  Bliss::Todo groupTarget =
    m_groupAdderItem->collidedGroup( todoItem->handleItem() );

  if ( groupTarget.isValid() ) {
    // Move to other group
    if ( listSource ) {
      listSource->removeItem( todoItem );
      delete todoItem;
    }
    m_model->moveTodo( todoItem->todo(), m_group, groupTarget );
  } else {
    if ( listTarget ) {
      if ( listSource == listTarget ) {
        // Move on same list
        listTarget->repositionItems();
        m_model->saveViewList( m_group, listTarget->list() );
      } else {
        if ( listSource ) {
          // Move to other list
          TodoItem *newItem = listTarget->addTodo( todoItem->todo() );
          newItem->setPos( listTarget->mapFromItem( listSource, todoItem->pos() ) );
          listTarget->repositionItems();

          listSource->removeItem( todoItem );
          delete todoItem;

          m_model->saveMoveFromListToList( m_group, newItem->todo(),
            listSource->list(), listTarget->list() );
        } else {
          // Move from canvas to list
          TodoItem *newItem = listTarget->addTodo( todoItem->todo() );
          newItem->setPos( listTarget->mapFromScene( todoItem->pos() ) );
          listTarget->repositionItems();

          m_items.removeOne( todoItem );
          delete todoItem;

          QStringList sortedCanvasIds;
          foreach( TodoItem *i, m_items ) {
            sortedCanvasIds.append( i->todo().id() );
          }

          m_model->saveMoveFromCanvasToList( m_group, newItem->todo(),
            listTarget->list(), sortedCanvasIds );

          m_itemPlacer->prepare();
          preparePositions( m_items, m_itemPlacer );
          m_itemPlacer->start();
        }
      }
    } else {
      if ( listSource ) {
        // Move from list to canvas
        TodoItem *newItem = createTodoItem( todoItem->todo() );
        newItem->setPos( listSource->mapToScene( todoItem->pos() ) );
        m_items.insert( m_items.size() - 1, newItem );
        m_scene->addItem( newItem );

        listSource->removeItem( todoItem );
        delete todoItem;

        QMap <qreal, QString> map;
        foreach( TodoItem *i, m_items ) {
          map.insert( i->pos().y(), i->todo().id() );
        }

        m_model->saveMoveFromListToCanvas( m_group, newItem->todo(),
          listSource->list(), map.values() );

        m_itemPlacer->prepare();
        QList<TodoItem *> sortedItems;

        Bliss::Todo::List todos = m_model->unlistedTodosOfGroup( m_group );
        foreach( Bliss::Todo todo, todos ) {
          sortedItems.append( item( todo ) );
        }

        TodoItem *addNewItem = m_items.last();
        m_items = sortedItems;
        m_items.append( addNewItem );

        preparePositions( m_items, m_itemPlacer );

        m_itemPlacer->start();
      } else {
        // Move on canvas
        m_itemPlacer->prepare();

        QMap <qreal, QString> map;
        foreach( TodoItem *i, m_items ) {
          map.insert( i->pos().y(), i->todo().id() );
        }
        m_model->saveViewSequence( m_group, map.values() );

        QList<TodoItem *> sortedItems;

        Bliss::Todo::List todos = m_model->unlistedTodosOfGroup( m_group );
        foreach( Bliss::Todo todo, todos ) {
          sortedItems.append( item( todo ) );
        }

        TodoItem *addNewItem = m_items.last();
        m_items = sortedItems;
        m_items.append( addNewItem );

        preparePositions( m_items, m_itemPlacer );

        m_itemPlacer->start();
      }
    }
  }
}

void GroupView::addList()
{
  NewListDialog *dialog = new NewListDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Bliss::ViewList list = dialog->list();

    QPointF pos = QPoint( 10, 10 );
    list.setX( pos.x() );
    list.setY( pos.y() );

    createListItem( list );

    m_model->saveViewList( m_group, list );
  }
  return;
}


void GroupView::removeList( ListItem *item )
{
  m_listItems.removeAll( item );

  delete item;
  m_model->removeViewList( m_group, item->list() );
}

ListItem *GroupView::createListItem( const Bliss::ViewList &list )
{
  ListItem *item = new ListItem( m_model, m_menuHandler, m_group, list );

  connect( item, SIGNAL( removeList( ListItem * ) ),
    SLOT( removeList( ListItem * ) ) );
  connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );
  connect( item, SIGNAL( done( const Bliss::Todo & ) ),
    SLOT( slotDone( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( itemMoved( TodoItem *, const QPointF & ) ),
    SLOT( slotItemMoved( TodoItem *, const QPointF & ) ) );
  connect( item, SIGNAL( showGroup( const Bliss::Todo & ) ),
    SIGNAL( requestShowGroup( const Bliss::Todo & ) ) );
  connect( item, SIGNAL( removeGroup( const Bliss::Todo & ) ),
    SIGNAL( removeGroup( const Bliss::Todo & ) ) );

  m_scene->addItem( item );

  item->setPos( list.x(), list.y() );

  m_listItems.append( item );

  return item;
}

void GroupView::resetLayout()
{
  m_model->clearViewPositions( m_group );

  foreach( TodoItem *item, m_items ) {
    if ( item->pos() != item->defaultPos() ) {
      QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);

      animation->setDuration(500);
      animation->setStartValue( item->pos() );
      animation->setEndValue( item->defaultPos() );
      animation->setEasingCurve( QEasingCurve::OutCubic );

      animation->start();
    }
  }
}

TodoItem *GroupView::item( const Bliss::Todo &identity ) const
{
  foreach( TodoItem *item, m_items ) {
    if ( item->todo().id() == identity.id() ) return item;
  }
  return 0;
}

ListItem *GroupView::listItem( TodoItem *item ) const
{
  foreach( ListItem *list, m_listItems ) {
    if ( list->hasItem( item ) ) return list;
  }
  return 0;
}

void GroupView::hideGlobalMenu()
{
  if ( m_globalMenu ) {
    m_globalMenu->hide();
  }
}

void GroupView::slotMouseMoved( const QPoint &pos )
{
  if ( !m_globalMenu || !m_globalMenu->isVisible() ) return;

  if ( !m_globalMenu->isCloseTo( m_view->mapToScene( pos ) ) ) {
    hideGlobalMenu();
  }
}

void GroupView::setAdderGroup( const Bliss::Todo &group )
{
  m_groupAdderItem->setGroup( group );
}

void GroupView::rememberPosition( const QPoint &pos )
{
  m_viewPositions.setPosition( m_group, pos );
}

void GroupView::setViewPosition()
{
  QPoint pos = m_viewPositions.position( m_group );
  m_view->setPosition( pos );
}
