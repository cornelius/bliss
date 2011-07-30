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
#ifndef TRACKINGGRAPHICSVIEW_H
#define TRACKINGGRAPHICSVIEW_H

#include <QGraphicsView>

class TrackingGraphicsView : public QGraphicsView
{
    Q_OBJECT
  public:
    TrackingGraphicsView( QGraphicsScene * );

  signals:
    void mouseMoved( const QPoint & );
    void viewportMoved();

  protected:
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void scrollContentsBy( int dx, int dy );
    void resizeEvent( QResizeEvent *event );

  private:
    bool m_mousePressed;
    int m_pressedValueHorizontal;
    int m_pressedValueVertical;
    QPoint m_pressedPos;
};

#endif
