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
#ifndef ITEMPLACER_H
#define ITEMPLACER_H

#include <QObject>
#include <QAnimationGroup>
#include <QPropertyAnimation>

class TodoItem;

class ItemPlacer : public QObject
{
    Q_OBJECT
  public:
    ItemPlacer( QObject *parent = 0 );

    void prepare( bool animate = true );

    void addItem( TodoItem *item, const QPointF & );
    void addItem( TodoItem *item, qreal itemX, qreal itemY );

    void setStartValue( const QPointF & );

    void start();
    void stop();
    
  signals:
    void finished();

  private:
    QAnimationGroup *m_placeItemsAnimation;
    QList<QPropertyAnimation *> m_placeItemsAnimations;

    bool m_animate;    
};

#endif
