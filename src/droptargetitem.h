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
#ifndef DROPTARGETITEM_H
#define DROPTARGETITEM_H

#include "bliss/bliss.h"

#include <QtGui>

class MainModel;

class DropTargetItem : public QGraphicsEllipseItem
{
  public:
    DropTargetItem( QGraphicsItem *, MainModel *, const Bliss::Todo & );

    Bliss::Todo todo() const;
    
    void updateItem( const Bliss::Todo & );

  protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  private:
    MainModel *m_model;
    Bliss::Todo m_todo;

    int m_itemSize;

    QGraphicsTextItem *m_nameItem;
};

#endif
