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
#ifndef MAINMENUITEM_H
#define MAINMENUITEM_H

#include "fanmenu.h"

#include <QtGui>

class MainMenuItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
  public:
    MainMenuItem();

    void setItemSize( int size );

  signals:
    void addTodo();
    void addGroup();
    void addList();
    void showMore();
  
    void menuShown();

  protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected slots:
    void hideItems();

  private:
    int m_defaultItemSize;
  
    FanMenu *m_fanMenu;
    
    QTimer m_timer;
};

#endif
