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
#ifndef GROUPVIEW_H
#define GROUPVIEW_H

#include "bliss/bliss.h"

#include "fanmenu.h"

#include <QtGui>

class MainModel;
class TodoItem;
class ListItem;
class LabelItem;
class QAnimationGroup;
class MainMenuItem;
class MagicMenuItem;
class GroupAdderItem;
class MenuHandler;
class ItemPlacer;
class ButtonItem;

struct TodoItemGroup {

  TodoItemGroup() : previousGroup( 0 ) {}

  QList<TodoItem *> items;
  QPointF center;
  TodoItem *previousGroup;
};

class GroupView : public QWidget
{
    Q_OBJECT
  public:
    GroupView( MainModel *, QWidget *parent = 0 );

    void readConfig();
    void writeConfig();
    
    void setAdderGroup( const Bliss::Todo &group );

    void setBackButtonEnabled( bool );

    void showGroup( const Bliss::Todo & );

  signals:
    void goBack();
    void showOverview();
    
    void newTodo();
    void showTodo( const Bliss::Todo & );
    void requestShowGroup( const Bliss::Todo & );
    
    void showSettings();

    void newGroup();
    void newList();
    void removeGroup( const Bliss::Todo &group );
    
    void closeRequested();

  protected:
    TodoItemGroup prepareTodoItems( ItemPlacer * );
    void createMenuItems();
    void createLabelItems();
    void createListItems();

    TodoItem *createTodoItem( const Bliss::Todo & );
    
    ListItem *createListItem( const Bliss::ViewList &list );
    LabelItem *createLabelItem( const Bliss::ViewLabel &label );

    TodoItem *item( const Bliss::Todo & ) const;
    ListItem *listItem( TodoItem * ) const;

    Bliss::ViewList viewList( const Bliss::Todo &todo );

    void positionMenuItems();

    void clearItems();
    void clearListItems();

    QPointF preparePositions( const QList<TodoItem *> &todoItems,
      ItemPlacer * );
    
  protected slots:
    void resetLayout();
  
    void hideItems();
    void placeItems();
    void unplaceItems();
    void unhideItems();
    void recreateItems();

    void slotDone( const Bliss::Todo & );

    void addList();
    void removeList( ListItem * );
    
    void addLabel();
    void addLabel( const QPointF & );
    void removeLabel( LabelItem * );
    void renameLabel( LabelItem * );

    void slotItemMoved( TodoItem *item, const QPointF &pos );
 
    void addLabelClicked();

    void hideGlobalMenu();

    void slotMouseMoved( const QPoint &pos );

    void positionAbsoluteItems();

    void finishPlaceItems();

    void slotTodoAdded( const Bliss::Todo & );
    void slotTodoChanged( const Bliss::Todo & );
    void slotTodoRemoved( const Bliss::Todo & );
    
  private:
    MainModel *m_model;

    Bliss::Todo m_group;
    Bliss::Todo m_previousGroup;

    QList<TodoItem *> m_items;
    QList<LabelItem *> m_labelItems;
    QList<ListItem *> m_listItems;

    TodoItem *m_previousItem;

    TodoItemGroup m_newItems;

    QGraphicsTextItem *m_titleItem;
    
    ButtonItem *m_backButton;
    MainMenuItem *m_mainMenu;
    MagicMenuItem *m_magicMenu;
    GroupAdderItem *m_groupAdderItem;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    QAnimationGroup *m_morphToAnimation;
    QAnimationGroup *m_morphFromAnimation;

    QAnimationGroup *m_removeItemsAnimation;
    QAnimationGroup *m_unhideItemsAnimation;

    ItemPlacer *m_itemPlacer;
    ItemPlacer *m_itemUnplacer;
    ItemPlacer *m_removeItemPlacer;

    FanMenu *m_globalMenu;
    FanMenuItem *m_addLabelItem;

    MenuHandler *m_menuHandler;
};

#endif
