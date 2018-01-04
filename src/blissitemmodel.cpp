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

#include "blissitemmodel.h"

#include "mainmodel.h"

#include <QDebug>

BlissItemModel::BlissItemModel( MainModel *blissModel, const QString &groupId )
  : QAbstractListModel( blissModel ), m_model( blissModel ),
    m_groupId( groupId )
{
}

BlissItemModel::~BlissItemModel()
{
}

MainModel *BlissItemModel::model() const
{
  return m_model;
}

int BlissItemModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent );

  return m_model->todosOfGroup( m_groupId ).count();
}

QVariant BlissItemModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if ( index.row() >= rowCount( QModelIndex() ) ) return QVariant();

  Bliss::Todo todo = getTodoData( index );

  if (role == Qt::DisplayRole) {
    if ( todo.type() == "group" ) return todo.title().value();
    else return todo.summary().value();
  } else if ( role == Qt::UserRole ) {
    return todo.id();
  } else if ( role == Qt::DecorationRole ) {
    return m_model->pixmap( todo );
  } else {
    return QVariant();
  }
}

Bliss::Todo BlissItemModel::getTodoData( const QModelIndex &index ) const
{
  return m_model->todosOfGroup( m_groupId ).at( index.row() );
}

QVariant BlissItemModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
    return QString("Column %1").arg(section);
  else
    return QString("Row %1").arg(section);
}

void BlissItemModel::updateData()
{
// TODO: port
//  reset();
}

Bliss::Todo BlissItemModel::todo( const QModelIndex &index )
{
  return m_model->todosOfGroup( m_groupId ).at( index.row() );
}
