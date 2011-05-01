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
#ifndef GROUPLISTVIEW_H
#define GROUPLISTVIEW_H

#include "groupview.h"

#include <QtGui>

class BlissItemModel;

class GroupListView : public GroupView
{
    Q_OBJECT
  public:
    GroupListView( MainModel *, QWidget *parent = 0 );

    void setItemModel( BlissItemModel * );

  protected:
    void doShowGroup();

  protected slots:
    void slotItemClicked( const QModelIndex &index );

  private:
    BlissItemModel *m_itemModel;

    QListView *m_flatView;
};

#endif
