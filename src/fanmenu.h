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
#ifndef FANMENU_H
#define FANMENU_H

#include <QtGui>

#include "fanmenuitem.h"

class FanMenuElement;

class FanMenu : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
  public:
    FanMenu( QGraphicsItem *parent );

    /**
      Setup items. freeSpace is the percentage of the radius the menu covers.
    */
    void setupItems( int coverage = 67 );

    FanMenuItem *addItem( const QString &text ); 

    bool isCloseTo( const QPointF & );

    void setRadius( qreal );
    qreal radius() const;

    void setStartAngle( int angle );
    void setEndAngle( int angle );

    void setSpacing( int );

  private:
    QList<FanMenuItem *> m_items;

    int m_startAngle;
    int m_endAngle;

    int m_spacing;

    qreal m_radius;
};

#endif
