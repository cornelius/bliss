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
#ifndef LABELITEM_H
#define LABELITEM_H

#include "bliss/bliss.h"
#include "fanmenu.h"
#include "roundedrectitem.h"

#include <QtGui>

class MainModel;

class LabelItem : public QObject, public RoundedRectItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

  public:
    LabelItem( MainModel *, const Bliss::ViewLabel & );

    Bliss::ViewLabel label() const;
    void setLabel( const Bliss::ViewLabel & );

  signals:
    void itemMoved( const Bliss::ViewLabel &, const QPointF & );
    void removeLabel( LabelItem * );
    void renameLabel( LabelItem * );

    void menuShown();
    
  protected:
    void setText( const QString & );
  
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

  protected slots:
    void emitRemoveLabel();
    void emitRenameLabel();

  private:
    MainModel *m_model;
    Bliss::ViewLabel m_label;

    QGraphicsTextItem *m_textItem;

    FanMenu *m_fanMenu;

    QPointF m_movePos;
};

#endif
