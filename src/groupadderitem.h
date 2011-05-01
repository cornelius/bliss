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
#ifndef GROUPADDERITEM_H
#define GROUPADDERITEM_H

#include "mainmodel.h"
#include "fanmenu.h"

#include <QtGui>

class ButtonItem;
class TodoItem;

class GroupAdderItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
  public:
    GroupAdderItem( MainModel * );

    void setItemSize( int size );

    void setGroup( const Bliss::Todo &group );
    Bliss::Todo group() const;

  protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected slots:
    void expand();

    void nextGroup();
    void previousGroup();
    
  private:
    MainModel *m_model;
  
    int m_defaultItemSize;

    bool m_expanded;

    ButtonItem *m_expandButton;

    ButtonItem *m_upButton;
    ButtonItem *m_downButton;

    TodoItem *m_groupItem;
};

#endif
