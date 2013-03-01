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
  : QObject( parent ), m_storage( 0 ), m_allItemModel( 0 ),
    m_personsItemModel( 0 ), m_groupItemModel( 0 )
{
  m_defaultGroupPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_group.png" );
  m_defaultGroupPixmap = QPixmap( m_defaultGroupPixmapPath );
  m_defaultPersonPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_todo.png" );
  m_defaultPersonPixmap = QPixmap( m_defaultPersonPixmapPath );
}

MainModel::~MainModel()
{
}


QString MainModel::locationId() const
{
  return m_locationId;
}

void MainModel::retrieveLog()
{
  m_storage->retrieveLog();
}

void MainModel::pullData()
{
  m_storage->pullData();
}

void MainModel::pushData()
{
  m_storage->pushData();
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
    m_storage = new StorageGit( this );

    if ( location.isEmpty() ) {
      m_storage->setLocation( QDir::homePath() + "/.bliss" );
      m_locationId = "std:";
    } else {
      m_storage->setLocation( location );
      m_locationId = "git:" + fi.absoluteFilePath();
    }
  } else {
    m_storage = new StorageFile( this );

    m_storage->setLocation( location );
    m_locationId = "file:" + fi.absoluteFilePath();
  }

  connect( m_storage, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  connect( m_storage, SIGNAL( logRetrieved( const QStringList & ) ),
    SIGNAL( logRetrieved( const QStringList & ) ) );
  connect( m_storage, SIGNAL( syncingStatusChanged( const QString & ) ),
    SIGNAL( syncingStatusChanged( const QString & ) ) );

  m_bliss = m_storage->readData();

  foreach( Bliss::Todo todo, m_bliss.todoList() ) {
    if ( todo.id().isEmpty() ) {
      todo.setId( KRandom::randomString( 10 ) );
      m_bliss.insert( todo );
    }
  }

  m_rootGroup = m_bliss.findTodo( m_bliss.root().group().id() );
  if ( !m_rootGroup.isValid() ) {
    createWelcomeData();
  }

  cleanupGroups();

  setupGroups();

  return true;
}

void MainModel::cleanupGroups()
{
  foreach( Bliss::Todo todo, m_bliss.todoList() ) {
    Bliss::Group::List groupList = todo.groups().groupList();
    if ( groupList.size() > 1 ) {
      Bliss::Group firstGroup = groupList.first();
      groupList.clear();
      groupList.append( firstGroup );

      Bliss::Groups groups;
      groups.setGroupList( groupList );

      todo.setGroups( groups );

      m_bliss.insert( todo );
    }
  }  
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
  m_storage->writeData( m_bliss, msg );
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

  Bliss::Groups groups;
  Bliss::Group g;
  g.setId( toGroup.id() );
  groups.addGroup( g );
  todo.setGroups( groups );
  m_bliss.insert( todo );
  
  doRemoveFromView( todo, fromGroup );
  
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

void MainModel::deleteTodo( const Bliss::Todo &todo, const Bliss::Todo &group )
{
  doDeleteTodo( todo, group );

  writeData( i18n("Deleted %1 from group %2").arg( todo.summary().value() )
    .arg( group.summary().value() ) );
  
  (new DelayedSignal( this, todo ))->emitTodoRemoved();
}

void MainModel::doDeleteTodo( const Bliss::Todo &todo,
                              const Bliss::Todo &group )
{
  m_bliss.remove( todo );
 
  doRemoveFromView( todo, group );
  
  setupGroups();
}

void MainModel::doRemoveFromView( const Bliss::Todo &todo,
                                  const Bliss::Todo &group )
{
  Bliss::GroupView view = groupView( group );
  
  Bliss::TodoId::List newIdList;
  foreach( Bliss::TodoId id, view.todoSequence().todoIdList() ) {
    if ( id.value() != todo.id() ) newIdList.append( id ); 
  }
  Bliss::TodoSequence sequence = view.todoSequence();
  sequence.setTodoIdList( newIdList );
  view.setTodoSequence( sequence );
  
  Bliss::ViewList::List lists;
  foreach( Bliss::ViewList list, view.viewListList() ) {
    Bliss::TodoId::List newIdList;
    foreach( Bliss::TodoId id, list.todoSequence().todoIdList() ) {
      if ( id.value() != todo.id() ) newIdList.append( id ); 
    }
    Bliss::TodoSequence sequence = list.todoSequence();
    sequence.setTodoIdList( newIdList );
    list.setTodoSequence( sequence );
    
    lists.append( list );
  }
  view.setViewListList( lists );
  
  m_bliss.insert( view );  
}

void MainModel::removeGroup( const Bliss::Todo &group )
{
  Bliss::Todo::List members = m_groupMap.value( group.id() );
  foreach( Bliss::Todo member, members ) {
    doDeleteTodo( member, group );
  }
  m_bliss.remove( group );
  m_bliss.remove( m_bliss.findGroupView( group.id() ) );

  setupGroups();

  writeData( i18n("Deleted group %1").arg( group.summary().value() ) );
  
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

void MainModel::saveMoveFromListToList( const Bliss::Todo &group,
  const Bliss::Todo &todo, const Bliss::ViewList &fromList,
  const Bliss::ViewList &toList )
{
  doSaveViewList( group, fromList );
  doSaveViewList( group, toList );
  writeData( i18n("Moved '%1' from '%2' to '%3'").arg( todo.summary().value() )
    .arg( fromList.name() ).arg( toList.name() ) );
}

void MainModel::saveMoveFromCanvasToList( const Bliss::Todo &group,
  const Bliss::Todo &todo,
  const Bliss::ViewList &toList, const QStringList &sortedIds )
{
  doSaveViewList( group, toList );
  doSaveViewSequence( group, sortedIds );
  writeData( i18n("Moved '%1' from canvas to '%2'")
    .arg( todo.summary().value() )
    .arg( toList.name() ) );
}

void MainModel::saveMoveFromListToCanvas( const Bliss::Todo &group,
  const Bliss::Todo &todo,
  const Bliss::ViewList &fromList, const QStringList &sortedIds )
{
  doSaveViewList( group, fromList );
  doSaveViewSequence( group, sortedIds );
  writeData( i18n("Moved '%1' to from '%2' to canvas")
    .arg( todo.summary().value() )
    .arg( fromList.name() ) );
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
  doSaveViewSequence( group, ids );

  writeData( i18n("Updated sequence in group %1").
    arg( group.summary().value() ) );
}

void MainModel::doSaveViewSequence( const Bliss::Todo &group,
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
}

void MainModel::createWelcomeData()
{
  m_rootGroup.setId( KRandom::randomString( 10 ) );

  m_rootGroup.setType( "group" );
  Bliss::Summary n;
  n.setValue( i18n("Stuff to do") );
  m_rootGroup.setSummary( n );

  m_bliss.insert( m_rootGroup );

  Bliss::Root root;
  Bliss::Group rootGroup;
  rootGroup.setId( m_rootGroup.id() );
  root.setGroup( rootGroup );
  m_bliss.setRoot( root );
}
