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

class Storage;

class MainModel : public QObject
{
    Q_OBJECT
  public:
    MainModel( QObject *parent = 0 );
    ~MainModel();

    void writeData( const QString &msg );

    QString locationId() const;
    
    // FIXME: create create,read,update,delete identity functions
    Bliss::Todo findTodo( const QString &id );
    Bliss::Todo insert( Bliss::Todo, const QString &msg ); // create/update

    Bliss::ViewList insert( const Bliss::ViewList &, const Bliss::Todo &,
                            const QString &msg );
    
    Bliss::Todo rootGroup();

    Bliss::Todo::List groups();

    // FIXME: consistent API for querying lists of identities
    Bliss::Todo::List todos();
    Bliss::Todo::List todosOfGroup( const Bliss::Todo &group );
    Bliss::Todo::List todosOfGroup( const QString &id );
    // FIXME: maybe replace by read-only Bliss access
    Bliss::Todo::List allTodos();

    Bliss::Todo::List unlistedTodosOfGroup( const Bliss::Todo &group );
    Bliss::Todo::List todosOfList( const Bliss::ViewList &list );

    Bliss::Todo addTodo( const Bliss::Todo &todo,
      const Bliss::Todo &group );
    Bliss::Todo addTodo( const Bliss::Todo &todo,
      const Bliss::Todo &group, const Bliss::ViewList &list );
    void moveTodo( const Bliss::Todo &todo, const Bliss::Todo &fromGroup,
                   const Bliss::Todo &toGroup );
    void deleteTodo( const Bliss::Todo &todo, const Bliss::Todo &group );
      
    void removeGroup( const Bliss::Todo &group );

    Bliss::ViewList::List lists( const Bliss::Todo &group );

    void addList( const Bliss::ViewList &list,
      const Bliss::Todo &group );
    
    BlissItemModel *allItemModel();
    BlissItemModel *todoItemModel();
    BlissItemModel *groupItemModel();
    BlissItemModel *itemModel( const QString &id = QString() );

    void saveViewList( const Bliss::Todo &group,
      const Bliss::ViewList &list );
    void removeViewList( const Bliss::Todo &group,
      const Bliss::ViewList &list );

    void saveViewLabel( const Bliss::Todo &group,
      const Bliss::ViewLabel &label );
    void removeViewLabel( const Bliss::Todo &group,
      const Bliss::ViewLabel &label );

    void saveViewPosition( const Bliss::Todo &group,
      const Bliss::Todo &identity,
      const QPointF &pos );
    void clearViewPositions( const Bliss::Todo &group );

    void saveViewSequence( const Bliss::Todo &group, const QStringList &ids );

    void saveMoveFromListToList( const Bliss::Todo &group,
       const Bliss::Todo &todo, const Bliss::ViewList &fromList,
       const Bliss::ViewList &toList );
    void saveMoveFromCanvasToList( const Bliss::Todo &group,
       const Bliss::Todo &todo,
       const Bliss::ViewList &toList, const QStringList &sortedIds );
    void saveMoveFromListToCanvas( const Bliss::Todo &group,
       const Bliss::Todo &todo,
       const Bliss::ViewList &toList, const QStringList &sortedIds );

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
    void createWelcomeData();
    
    void doAddTodo( Bliss::Todo &todo, const Bliss::Todo &group );
    void doDeleteTodo( const Bliss::Todo &todo, const Bliss::Todo &group );
    void doRemoveFromView( const Bliss::Todo &todo, const Bliss::Todo &group );

    void doSaveViewList( const Bliss::Todo &group,
      const Bliss::ViewList &list );

    void doSaveViewSequence( const Bliss::Todo &group, const QStringList &ids );

    void cleanupGroups();
    
    void setupGroups();

    QPixmap defaultPixmap( const Bliss::Todo &identity ) const;
    
  private:
    Storage *m_storage;
  
    QString m_locationId;
    
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
