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
#ifndef LISTITEM_H
#define LISTITEM_H

#include "bliss/bliss.h"
#include "fanmenu.h"
#include "hidinglineedit.h"
#include "roundedrectitem.h"

#include <QtGui>

class MainModel;
class MenuHandler;
class MenuHandleItem;
class TodoItem;
class ItemPlacer;

class ListItem : public QObject, public RoundedRectItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

  public:
    ListItem( MainModel *, MenuHandler *, const Bliss::Todo &group,
              const Bliss::ViewList & );

    Bliss::ViewList list() const;

    QGraphicsEllipseItem *handleItem() const;
    
    void updateItem( const Bliss::ViewList & );
    void updateTodoItem( const Bliss::Todo &todo );

    void setDefaultPos( const QPointF & );
    QPointF defaultPos() const;

    void rememberPos( const QPointF & );
    QPointF rememberedPos() const;

    void enableMenus( bool enabled );

    void undoMove();

    int textCenterX();

  signals:
    void removeList( ListItem * );

    void removeTodo( const Bliss::Todo & );
    void done( const Bliss::Todo & );

    void itemChecked( const Bliss::Todo &, bool );

    void itemPressed();

    void menuShown();

    void itemDropped( ListItem * );

  protected:
    void init();

    void preparePositions();
    
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected slots:
    void emitRemoveList();

    void editList();
    void editListDone();

    void newTodo();

    void slotItemMoved( TodoItem *todoItem, const QPointF &pos );

  private:
    MainModel *m_model;
    Bliss::Todo m_group;
    Bliss::ViewList m_list;

    QList<TodoItem *> m_todoItems;
    
    QPointF m_defaultPos;
    QPointF m_rememberedPos;

    MenuHandleItem *m_handleItem;

    QGraphicsTextItem *m_nameItem;

    QPointF m_movePos;

    int m_textCenterX;

    bool m_menusEnabled;

    FanMenu *m_fanMenu;
    
    MenuHandler *m_menuHandler;
    
    HidingLineEdit *m_edit;
    QGraphicsProxyWidget *m_editProxy;

    int m_itemSize;
    int m_handleItemSize;
    int m_spacing;
    
    ItemPlacer *m_itemPlacer;
};

#endif
