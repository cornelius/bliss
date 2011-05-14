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
#ifndef TODOITEM_H
#define TODOITEM_H

#include "bliss/bliss.h"
#include "fanmenu.h"

#include <QtGui>

class MainModel;
class RoundedRectItem;
class TodoHandleItem;
class MenuHandler;

class TodoItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

  public:
    TodoItem( MainModel *, MenuHandler *, const Bliss::Todo & );
    TodoItem( QGraphicsItem *, MainModel *, const Bliss::Todo & );

    Bliss::Todo todo() const;

    void updateItem( const Bliss::Todo & );

    void setDefaultPos( const QPointF & );
    QPointF defaultPos() const;

    void rememberPos( const QPointF & );
    QPointF rememberedPos() const;

    void enableMenus( bool enabled );

    void undoMove();

    int textCenterX();

    void showPopups();
    void hidePopups();
    
  signals:
    void showGroup( const Bliss::Todo & );
  
    void showTodo( const Bliss::Todo & );
    void removeTodo( const Bliss::Todo & );

    void itemMoved( TodoItem *, const QPointF & );
    
    void itemChecked( const Bliss::Todo &, bool );

    void menuShown();

    void itemDropped( TodoItem * );

  protected:
    void init();
  
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected slots:
    void emitShowTodo();
    void emitRemoveTodo();

    void checkMenuVisibility();

  private:
    MainModel *m_model;
    Bliss::Todo m_todo;

    QPointF m_defaultPos;
    QPointF m_rememberedPos;

    TodoHandleItem *m_handleItem;

    QGraphicsTextItem *m_nameItem;

    QPointF m_movePos;

    int m_textCenterX;

    bool m_menusEnabled;

    FanMenu *m_fanMenu;
    
    MenuHandler *m_menuHandler;
    
    bool m_isHovered;
};

#endif
