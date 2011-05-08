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
#ifndef GROUPGRAPHICSVIEW_H
#define GROUPGRAPHICSVIEW_H

#include "fanmenu.h"
#include "groupview.h"

#include <QtGui>

class MainModel;
class TodoItem;
class LabelItem;
class QAnimationGroup;
class MainMenuItem;
class MagicMenuItem;
class GroupAdderItem;

struct TodoItemGroup {

  TodoItemGroup() : previousGroup( 0 ) {}

  QList<TodoItem *> items;
  QPointF center;
  TodoItem *previousGroup;
};

class GroupGraphicsView : public GroupView
{
    Q_OBJECT
  public:
    GroupGraphicsView( MainModel *, QWidget *parent = 0 );

    void setAdderGroup( const Bliss::Todo &group );

  signals:
    void newGroup();
    void removeTodo( const Bliss::Todo &person,
      const Bliss::Todo &group );
    void cloneGroup( const Bliss::Todo &group );
    void removeGroup( const Bliss::Todo &group );

    void closeRequested();

  protected:
    void doShowGroup();

    TodoItemGroup prepareTodoItems( bool doAnimation );
    void createMenuItems();
    void createLabelItems();
  
    LabelItem *createLabelItem( const Bliss::ViewLabel &label );

    TodoItem *item( const Bliss::Todo & ) const;

    bool eventFilter( QObject * watched, QEvent * event );

    void positionMenuItems();

    void clearItems();

  protected slots:
    void resetLayout();
  
    void hideItems();
    void placeItems();
    void unplaceItems();
    void unhideItems();
    void recreateItems();

    void slotRemoveTodo( const Bliss::Todo & );

    void addLabel();
    void addLabel( const QPointF & );
    void removeLabel( LabelItem * );
    void renameLabel( LabelItem * );

    void savePosition( TodoItem *, const QPointF & );
    void saveLabel( const Bliss::ViewLabel &, const QPointF & );
 
    void emitCloneGroup();
    void emitRemoveGroup();

    void addLabelClicked();

    void hideGlobalMenu();

    void slotMouseMoved( const QPoint &pos );

    void positionAbsoluteItems();

    void finishPlaceItems();

    void slotTodoAdded( const Bliss::Todo & );
    void slotTodoChanged( const Bliss::Todo & );
    void slotTodoRemoved( const Bliss::Todo & );

  private:
    QList<TodoItem *> m_items;
    QList<LabelItem *> m_labelItems;

    TodoItem *m_previousItem;

    TodoItemGroup m_newItems;

    MainMenuItem *m_mainMenu;
    MagicMenuItem *m_magicMenu;
    GroupAdderItem *m_groupAdderItem;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    QAnimationGroup *m_morphToAnimation;
    QAnimationGroup *m_morphFromAnimation;

    QAnimationGroup *m_removeItemsAnimation;
    QAnimationGroup *m_placeItemsAnimation;
    QList<QPropertyAnimation *> m_placeItemsAnimations;
    QAnimationGroup *m_unplaceItemsAnimation;
    QAnimationGroup *m_unhideItemsAnimation;

    FanMenu *m_globalMenu;
    FanMenuItem *m_addLabelItem;
};

#endif
