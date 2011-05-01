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

#include "trackinggraphicsview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>

TrackingGraphicsView::TrackingGraphicsView( QGraphicsScene *scene )
  : QGraphicsView( scene )
{
  setMouseTracking( true );

  connect( verticalScrollBar(), SIGNAL( valueChanged( int ) ),
    SIGNAL( viewportMoved() ) );
  connect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ),
    SIGNAL( viewportMoved() ) );
}

void TrackingGraphicsView::mouseMoveEvent( QMouseEvent *event )
{
  emit mouseMoved( event->pos() );

  QGraphicsView::mouseMoveEvent( event );
}

void TrackingGraphicsView::scrollContentsBy( int dx, int dy )
{
//  emit viewportMoved();

  QGraphicsView::scrollContentsBy( dx, dy );
}

void TrackingGraphicsView::resizeEvent( QResizeEvent *event )
{
  emit viewportMoved();
  
  QGraphicsView::resizeEvent( event );
}
