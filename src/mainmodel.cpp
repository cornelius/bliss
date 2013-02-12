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

#include "mainmodel.h"

#include "storagegit.h"
#include "storagefile.h"
#include "settings.h"
#include "blissallitemmodel.h"
#include "blisstodoitemmodel.h"
#include "delayedsignal.h"

#include <KRandom>
#include <KLocale>
#include <KStandardDirs>

#include <QDir>
#include <QDebug>

MainModel::MainModel( QObject *parent )
  : QObject( parent ), m_allItemModel( 0 ), m_personsItemModel( 0 ),
    m_groupItemModel( 0 )
{
  m_defaultGroupPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_group.png" );
  m_defaultGroupPixmap = QPixmap( m_defaultGroupPixmapPath );
  m_defaultPersonPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_todo.png" );
  m_defaultPersonPixmap = QPixmap( m_defaultPersonPixmapPath );

  m_storageGit = new StorageGit( this );
  connect( m_storageGit, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  connect( m_storageGit, SIGNAL( logRetrieved( const QStringList & ) ),
    SIGNAL( logRetrieved( const QStringList & ) ) );
  connect( m_storageGit, SIGNAL( syncingStatusChanged( const QString & ) ),
    SIGNAL( syncingStatusChanged( const QString & ) ) );

  m_storageFile = new StorageFile( this );
}

MainModel::~MainModel()
{
}

void MainModel::retrieveLog()
{
  if ( m_dataFile.isEmpty() ) {
    m_storageGit->retrieveLog();
  }
}

void MainModel::pullData()
{
  m_storageGit->pullData();
}

void MainModel::pushData()
{
  m_storageGit->pushData();
}

Bliss::Todo MainModel::findTodo( const QString &id )
{
  return m_bliss.findTodo( id );
}

Bliss::Todo::List MainModel::allTodos()
{
  return m_bliss.todoList();
}

Bliss::Todo MainModel::rootGroup()
{
  return m_rootGroup;
}

Bliss::Todo::List MainModel::groups()
{
  return m_groups;
}

Bliss::Todo::List MainModel::todos()
{
  // FIXME: Cache it in model.

  Bliss::Todo::List todos;

  foreach( Bliss::Todo todo, m_bliss.todoList() ) {
    if ( todo.type() != "group" ) {
      todos.append( todo );
    }
  }

  return todos;
}

Bliss::Todo::List MainModel::unlistedTodosOfGroup( const Bliss::Todo &group )
{
  Bliss::Todo::List unlistedTodos;
  
  Bliss::Todo::List todos = todosOfGroup( group );
  Bliss::GroupView view = groupView( group.id() );
  
  foreach( Bliss::Todo todo, todos ) {
    bool found = false;
    foreach( Bliss::ViewList list, view.viewListList() ) {
      foreach( Bliss::TodoId id, list.todoSequence().todoIdList() ) {
        if ( todo.id() == id.value() ) {
          found = true;
          break;
        }
      }
      if ( found ) break;
    }
    if ( !found ) {
      unlistedTodos.append( todo );
    }
  }
  
  return unlistedTodos;
}

Bliss::Todo::List MainModel::todosOfGroup( const Bliss::Todo &group )
{
  return todosOfGroup( group.id() );
}

Bliss::Todo::List MainModel::todosOfGroup( const QString &id )
{
  if ( id.isEmpty() ) return m_groups;
  if ( !m_groupMap.contains( id ) ) {
    m_groupMap.insert( id, Bliss::Todo::List() );
  }

  Bliss::Todo::List todos = m_groupMap[ id ];

  Bliss::Todo::List sortedTodos;

  Bliss::GroupView view = groupView( id );
  if ( view.isValid() ) {
    Bliss::TodoId::List todoIdList = view.todoSequence().todoIdList();
    foreach( Bliss::TodoId id, todoIdList ) {
      foreach( Bliss::Todo todo, todos ) {
        if ( todo.id() == id.value() ) {
          sortedTodos.append( todo );
          break;
        }
      }
    }
    foreach( Bliss::Todo todo, todos ) {
      Bliss::TodoId::List::ConstIterator it;
      for( it = todoIdList.begin(); it != todoIdList.end(); ++it ) {
        if ( todo.id() == (*it).value() ) break;
      }
      if ( it == todoIdList.end() ) {
        sortedTodos.append( todo );
      }
    }
  } else {
    sortedTodos = todos;
  }
  
  return sortedTodos;
}

Bliss::Todo::List MainModel::todosOfList( const Bliss::ViewList &list )
{
  Bliss::Todo::List todos;

  foreach( Bliss::TodoId id, list.todoSequence().todoIdList() ) {
    Bliss::Todo todo = m_bliss.findTodo( id.value() );
    if ( todo.isValid() ) todos.append( todo );
  }

  return todos;
}

BlissItemModel *MainModel::allItemModel()
{
  if ( !m_allItemModel ) {
    m_allItemModel = new BlissAllItemModel( this );
  }
  return m_allItemModel;
}

BlissItemModel *MainModel::todoItemModel()
{
  if ( !m_personsItemModel ) {
    m_personsItemModel = new BlissTodoItemModel( this );
  }
  return m_personsItemModel;
}

BlissItemModel *MainModel::itemModel( const QString &id )
{
  if ( !m_itemModels.contains( id ) ) {
    BlissItemModel *itemModel = new BlissItemModel( this, id );
    m_itemModels.insert( id, itemModel );
  }
  return m_itemModels.value( id );
}

BlissItemModel *MainModel::groupItemModel()
{
  if ( !m_groupItemModel ) {
    m_groupItemModel = new BlissItemModel( this );
  }
  return m_groupItemModel;
}

bool MainModel::readData( const QString &location )
{
  QFileInfo fi( location );
  
  if ( location.isEmpty() || fi.isDir() ) {
    if ( location.isEmpty() ) {
      m_storageGit->setLocation( QDir::homePath() + "/.bliss" );
    } else {
      m_storageGit->setLocation( location );
    }
    m_bliss = m_storageGit->readData();
  } else {
    m_dataFile = location;
    qDebug() << "READ FROM" << location;
    m_storageFile->setFileName( m_dataFile );
    m_bliss = m_storageFile->readData();
  }

  foreach( Bliss::Todo todo, m_bliss.todoList() ) {
    if ( todo.id().isEmpty() ) {
      todo.setId( KRandom::randomString( 10 ) );
      m_bliss.insert( todo );
    }
  }

  Bliss::Group rootGroup = m_bliss.root().group();

  m_rootGroup = m_bliss.findTodo( rootGroup.id() );
  if ( !m_rootGroup.isValid() ) {
    m_rootGroup.setId( KRandom::randomString( 10 ) );

    m_rootGroup.setType( "group" );
    Bliss::Summary n;
    n.setValue( i18n("Your todos") );
    m_rootGroup.setSummary( n );

    m_bliss.insert( m_rootGroup );

    Bliss::Root root;
    rootGroup.setId( m_rootGroup.id() );
    root.setGroup( rootGroup );
    m_bliss.setRoot( root );
  }

  setupGroups();

  return true;
}

void MainModel::setupGroups()
{
  m_groups.clear();
  m_groupMap.clear();

  foreach( Bliss::Todo todo, m_bliss.todoList() ) {
    if ( todo.groups().groupList().isEmpty() &&
         todo.type() != "group" ) {
      todo.setType( "group" );
      m_bliss.insert( todo );
    }
      
    if ( todo.type() == "group" ) {
      m_groups.append( todo );
    }
    
    foreach( Bliss::Group group, todo.groups().groupList() ) {
      if ( m_groupMap.contains( group.id() ) ) {
        m_groupMap[ group.id() ].append( todo );
      } else {
        Bliss::Todo::List todoList;
        todoList.append( todo );
        m_groupMap.insert( group.id(), todoList );
      }
    }
  }
}

void MainModel::writeData( const QString &msg )
{
  if ( m_dataFile.isEmpty() ) {
    m_storageGit->writeData( m_bliss, msg );
  } else {
    qDebug() << "WRITE FILE" << msg;
    emit dataWritten();
  }
}

Bliss::Todo MainModel::insert( Bliss::Todo todo,
  const QString &msg )
{
  bool added;

  if ( todo.id().isEmpty() ) {
    todo.setId( KRandom::randomString( 10 ) );
    added = true;
  } else {
    if ( m_bliss.findTodo( todo.id() ).isValid() ) {
      added = false;
    } else {
      added = true;
    }
  }

  m_bliss.insert( todo );

  setupGroups();

  if ( todo.type() == "group" ) {
    groupItemModel()->updateData();
  }

  foreach( Bliss::Group group, todo.groups().groupList() ) {
    itemModel( group.id() )->updateData();
  }

  writeData( msg );

  if ( added ) (new DelayedSignal( this, todo ))->emitTodoAdded();
  else (new DelayedSignal( this, todo ))->emitTodoChanged();

  return todo;
}

void MainModel::moveTodo( const Bliss::Todo &t, const Bliss::Todo &fromGroup,
                          const Bliss::Todo &toGroup )
{
  Bliss::Todo todo = t;

  if ( todo.groups().findGroup( toGroup.id() ).isValid() ) {
    return;
  }

  doAddTodo( todo, toGroup );
  doRemoveTodo( todo, fromGroup );

  insert( todo, i18n("Moved %1 from group %2 to group %3")
    .arg( todo.summary().value() )
    .arg( fromGroup.summary().value() )
    .arg( toGroup.summary().value() ) );
}

void MainModel::moveTodo( const Bliss::Todo &t, const Bliss::Todo &fromGroup,
                          const Bliss::ViewList &list,
                          const Bliss::Todo &toGroup )
{
  Bliss::Todo todo = t;

  if ( todo.groups().findGroup( toGroup.id() ).isValid() ) {
    return;
  }

  doAddTodo( todo, toGroup );
  doRemoveTodo( todo, fromGroup );
  doSaveViewList( fromGroup, list );

  insert( todo, i18n("Moved %1 from group %2 to group %3")
    .arg( todo.summary().value() )
    .arg( fromGroup.summary().value() )
    .arg( toGroup.summary().value() ) );
}

Bliss::Todo MainModel::addTodo( const Bliss::Todo &t,
  const Bliss::Todo &group, const Bliss::ViewList &list )
{
  Bliss::Todo todo = t;
  
  doSaveViewList( group, list );
  doAddTodo( todo, group );
  return insert( todo, i18n("Created todo %1 in list %2")
    .arg( todo.summary().value() )
    .arg( list.name() ) );
}

Bliss::Todo MainModel::addTodo( const Bliss::Todo &t,
  const Bliss::Todo &group )
{
  Bliss::Todo todo = t;

  if ( todo.groups().findGroup( group.id() ).isValid() ) return Bliss::Todo();
  
  doAddTodo( todo, group );

  return insert( todo, i18n("Add %1 to group %2").arg( todo.summary().value() )
      .arg( group.summary().value() ) );
}

void MainModel::doAddTodo( Bliss::Todo &todo, const Bliss::Todo &group )
{
  Bliss::Groups groups = todo.groups();

  Bliss::Group g;
  g.setId( group.id() );
  groups.addGroup( g );
  todo.setGroups( groups );
}

void MainModel::removeTodo( const Bliss::Todo &t,
  const Bliss::Todo &group )
{
  Bliss::Todo todo = t;
  
  doRemoveTodo( todo, group );
  
  insert( todo, i18n("Remove %1 from group %2").arg( todo.summary().value() )
    .arg( group.summary().value() ) );
}

void MainModel::doRemoveTodo( Bliss::Todo &todo, const Bliss::Todo &group )
{
  Bliss::Group::List groups = todo.groups().groupList();
  Bliss::Group::List newGroups;
  
  foreach( Bliss::Group g, groups ) {
    if ( g.id() != group.id() ) {
      newGroups.append( g );
    }
  }

  Bliss::Groups gg;
  gg.setGroupList( newGroups );
  todo.setGroups( gg );
}

void MainModel::deleteTodo( const Bliss::Todo &todo )
{
  m_bliss.remove( todo );
  setupGroups();

  writeData( i18n("Deleted %1").arg( todo.summary().value() ) );
  
  (new DelayedSignal( this, todo ))->emitTodoRemoved();
}

void MainModel::removeGroup( const Bliss::Todo &group )
{
  Bliss::Todo::List members = m_groupMap.value( group.id() );
  foreach( Bliss::Todo member, members ) {
    removeTodo( member, group );
  }
  m_bliss.remove( group );
  m_bliss.remove( m_bliss.findGroupView( group.id() ) );

  setupGroups();
  
  (new DelayedSignal( this, group ))->emitTodoRemoved();
}

Bliss::ViewList::List MainModel::lists( const Bliss::Todo &group )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id() );
  return v.viewListList();  
}

void MainModel::addList( const Bliss::ViewList &list,
  const Bliss::Todo &group )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );

  v.insert( list );
  m_bliss.insert( v );
  writeData( i18n("Inserted list %1 into group %2").arg( list.name() )
    .arg( group.summary().value() ) );
}

QPixmap MainModel::pixmap( const Bliss::Todo &todo ) const
{
  return defaultPixmap( todo );
}

QPixmap MainModel::defaultPixmap( const Bliss::Todo &todo ) const
{
  if ( todo.type() == "group" ) return m_defaultGroupPixmap;
  else return m_defaultPersonPixmap;
}

void MainModel::saveItemMove( const Bliss::Todo &group,
  const Bliss::Todo &todo, const Bliss::ViewList &fromList,
  const Bliss::ViewList &toList )
{
  doSaveViewList( group, fromList );
  doSaveViewList( group, toList );
  writeData( i18n("Moved '%1' from '%2' to '%3'").arg( todo.summary().value() )
    .arg( fromList.name() ).arg( toList.name() ) );
}

void MainModel::saveViewList( const Bliss::Todo &group,
  const Bliss::ViewList &list )
{
  doSaveViewList( group, list );
  writeData( i18n("Inserted list %1").arg( list.name() ) );
}

void MainModel::doSaveViewList( const Bliss::Todo &group,
  const Bliss::ViewList &list )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );
  v.insert( list );
  m_bliss.insert( v );
}

void MainModel::removeViewList( const Bliss::Todo &group,
  const Bliss::ViewList &list )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );
  v.remove( list );
  m_bliss.insert( v );
  writeData( i18n("Removed list %1").arg( list.name() ) );
}

void MainModel::saveViewLabel( const Bliss::Todo &group,
  const Bliss::ViewLabel &label )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );
  v.insert( label );
  m_bliss.insert( v );
  writeData( i18n("Inserted label %1").arg( label.text() ) );
}

void MainModel::removeViewLabel( const Bliss::Todo &group,
  const Bliss::ViewLabel &label )
{
  QString labelText = label.text();

  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );
  v.remove( label );
  m_bliss.insert( v );
  writeData( i18n("Removed label %1").arg( labelText ) );
}

void MainModel::saveViewPosition( const Bliss::Todo &group,
  const Bliss::Todo &todo,
  const QPointF &pos )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
    Bliss::Bliss::AutoCreate );
  Bliss::TodoPosition p = v.findTodoPosition( todo.id(),
    Bliss::GroupView::AutoCreate );
  p.setX( pos.x() );
  p.setY( pos.y() );
  v.insert( p );
  m_bliss.insert( v );
  writeData( i18n("Moved %1 in group %2")
    .arg( todo.summary().value() ).arg( group.summary().value() ) );
}

void MainModel::clearViewPositions( const Bliss::Todo &group )
{
  Bliss::GroupView view = m_bliss.findGroupView( group.id() );
  view.setTodoPositionList( Bliss::TodoPosition::List() );
  m_bliss.insert( view );
  writeData( i18n("Reset positions in group %1").arg( group.summary().value() ) );
}

Bliss::GroupView MainModel::groupView( const Bliss::Todo &group )
{
  return groupView( group.id() );
}

Bliss::GroupView MainModel::groupView( const QString &groupId )
{
  return m_bliss.findGroupView( groupId );
}

void MainModel::saveViewSequence( const Bliss::Todo &group,
                                  const QStringList &ids )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
                                              Bliss::Bliss::AutoCreate );
                                              
  Bliss::TodoSequence sequence;
  foreach( QString id, ids ) {
    Bliss::TodoId todoId;
    todoId.setValue( id );
    sequence.addTodoId( todoId );
  }
  
  v.setTodoSequence( sequence );

  m_bliss.insert( v );

  writeData( i18n("Updated sequence in group %1").
    arg( group.summary().value() ) );
}
