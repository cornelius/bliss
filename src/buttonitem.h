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
#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QtGui>
#include <QGraphicsEllipseItem>

class ButtonItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

  public:
    ButtonItem();
    ButtonItem( QGraphicsItem * );

    void setItemSize( int size );

    void setPlus();
    void setMinus();

    void setNext();
    void setPrevious();

    void setBack();
    
    void setClickEnabled( bool enabled );

  signals:
    void clicked();

  protected:
    void init();
    
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  private:
    int m_defaultItemSize;

    bool m_clickEnabled;

    QGraphicsPolygonItem *m_decoration;
    
    QPolygonF m_plusPolygon;
    QPolygonF m_minusPolygon;
    QPolygonF m_arrowPolygon;
};

#endif
