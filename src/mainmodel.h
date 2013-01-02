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
#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "bliss/bliss.h"
#include "blissitemmodel.h"
#include "gitcommand.h"

#include <QObject>
#include <QPixmap>

class StorageGit;
class StorageFile;

class MainModel : public QObject
{
    Q_OBJECT
  public:
    MainModel( QObject *parent = 0 );
    ~MainModel();

    void writeData( const QString &msg );

    // FIXME: create create,read,update,delete identity functions
    Bliss::Todo findTodo( const QString &id );
    Bliss::Todo insert( Bliss::Todo, const QString &msg ); // create/update

    Bliss::Todo rootGroup();

    Bliss::Todo::List groups();

    // FIXME: consistent API for querying lists of identities
    Bliss::Todo::List todos();
    Bliss::Todo::List todosOfGroup( const Bliss::Todo &group );
    Bliss::Todo::List todosOfGroup( const QString &id );
    // FIXME: maybe replace by read-only Bliss access
    Bliss::Todo::List allTodos();

    Bliss::Todo addTodo( const Bliss::Todo &todo,
      const Bliss::Todo &group );
    void removeTodo( const Bliss::Todo &todo,
      const Bliss::Todo &group );
    void moveTodo( const Bliss::Todo &todo, const Bliss::Todo &fromGroup,
                   const Bliss::Todo &toGroup );
    void deleteTodo( const Bliss::Todo &todo );
      
    void removeGroup( const Bliss::Todo &group );

    void addList( const Bliss::TodoList &list,
      const Bliss::Todo &group );
    
    BlissItemModel *allItemModel();
    BlissItemModel *todoItemModel();
    BlissItemModel *groupItemModel();
    BlissItemModel *itemModel( const QString &id = QString() );

    void saveViewLabel( const Bliss::Todo &group,
      const Bliss::ViewLabel &label );
    void removeViewLabel( const Bliss::Todo &group,
      const Bliss::ViewLabel &label );

    void saveViewPosition( const Bliss::Todo &group,
      const Bliss::Todo &identity,
      const QPointF &pos );
    void clearViewPositions( const Bliss::Todo &group );

    void saveViewSequence( const Bliss::Todo &group, const QStringList &ids );
    
    Bliss::GroupView groupView( const Bliss::Todo &group );
    Bliss::GroupView groupView( const QString &groupId );

    QPixmap pixmap( const Bliss::Todo & ) const;

    void retrieveLog();

    void pullData();
    void pushData();

  public slots:
    bool readData( const QString &file = QString() );

  signals:
    void dataWritten();

    void logRetrieved( const QStringList & );

    void todoAdded( const Bliss::Todo & );
    void todoChanged( const Bliss::Todo & );
    void todoRemoved( const Bliss::Todo & );

    void syncingStatusChanged( const QString & );

  protected:
    void doAddTodo( Bliss::Todo &todo, const Bliss::Todo &group );
    void doRemoveTodo( Bliss::Todo &todo, const Bliss::Todo &group );
    
    void setupGroups();

    QPixmap defaultPixmap( const Bliss::Todo &identity ) const;

  private:
    StorageGit *m_storageGit;
    StorageFile *m_storageFile;
  
    QString m_dataFile;
  
    Bliss::Bliss m_bliss;

    Bliss::Todo m_rootGroup;
    Bliss::Todo::List m_groups;
    QMap<QString,Bliss::Todo::List> m_groupMap;

    BlissItemModel *m_allItemModel;
    BlissItemModel *m_personsItemModel;
    BlissItemModel *m_groupItemModel;
    QMap<QString,BlissItemModel *> m_itemModels;
    
    QPixmap m_defaultGroupPixmap;
    QPixmap m_defaultPersonPixmap;

    QString m_defaultGroupPixmapPath;
    QString m_defaultPersonPixmapPath;
};

#endif
