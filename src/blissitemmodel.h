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
#ifndef BLISSITEMMODEL_H
#define BLISSITEMMODEL_H

#include "bliss/bliss.h"

#include <QAbstractListModel>

class MainModel;

class BlissItemModel : public QAbstractListModel
{
  public:
    BlissItemModel( MainModel *, const QString &groupId = QString() );
    virtual ~BlissItemModel();

    MainModel *model() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

    virtual Bliss::Todo todo( const QModelIndex &index );

  protected:
    virtual Bliss::Todo getTodoData( const QModelIndex &index ) const;

  private:
    MainModel *m_model;
    QString m_groupId;
};

#endif
