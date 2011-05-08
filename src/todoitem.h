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

class TodoItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

  public:
    TodoItem( MainModel *, const Bliss::Todo & );
    TodoItem( QGraphicsItem *, MainModel *, const Bliss::Todo & );

    Bliss::Todo todo() const;

    void updateItem( const Bliss::Todo & );

    void setDefaultPos( const QPointF & );
    QPointF defaultPos() const;

    void rememberPos( const QPointF & );
    QPointF rememberedPos() const;

    void hidePopups();

    void enableMenus( bool enabled );

    void undoMove();

    int textCenterX();
    
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

    QVariant itemChange( GraphicsItemChange change, const QVariant &value );

  protected slots:
    void emitShowTodo();
    void emitRemoveTodo();

  private:
    MainModel *m_model;
    Bliss::Todo m_todo;

    bool m_menusEnabled;

    QPointF m_defaultPos;
    QPointF m_rememberedPos;

    QGraphicsTextItem *m_nameItem;

    FanMenu *m_fanMenu;

    QPointF m_movePos;

    int m_itemSize;
    
    int m_textCenterX;
};

#endif
