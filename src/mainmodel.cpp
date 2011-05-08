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

#include "gitdir.h"
#include "gitremote.h"
#include "settings.h"
#include "blissallitemmodel.h"
#include "blisstodoitemmodel.h"

#include <KRandom>
#include <KLocale>
#include <KStandardDirs>

#include <QDir>
#include <QDebug>

MainModel::MainModel( QObject *parent )
  : QObject( parent ), m_allItemModel( 0 ), m_personsItemModel( 0 ),
    m_groupItemModel( 0 ),
    m_commitCommand( 0 )
{
  m_defaultGroupPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_group.png" );
  m_defaultGroupPixmap = QPixmap( m_defaultGroupPixmapPath );
  m_defaultPersonPixmapPath = KStandardDirs::locate( "appdata",
    "bliss_todo.png" );
  m_defaultPersonPixmap = QPixmap( m_defaultPersonPixmapPath );

  m_gitDir = new GitDir( QDir::homePath() + "/.bliss" );

  m_gitRemote = new GitRemote( m_gitDir );
  connect( m_gitRemote, SIGNAL( pulled() ), SLOT( readData() ) );
  connect( m_gitRemote, SIGNAL( pushed() ), SLOT( slotPushed() ) );

  connect( m_gitDir, SIGNAL( commandExecuted( const GitCommand & ) ),
    SLOT( slotCommandExecuted( const GitCommand & ) ) );
}

MainModel::~MainModel()
{
  delete m_gitDir;
}

GitRemote *MainModel::gitRemote() const
{
  return m_gitRemote;
}

GitDir *MainModel::gitDir() const
{
  return m_gitDir;
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
  return m_groupMap[ id ];
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

bool MainModel::readData()
{
  QString dataFile = m_gitDir->filePath( "std.bliss" );

  if ( QFile::exists( dataFile ) ) {
    m_bliss = Bliss::Bliss::parseFile( dataFile, &m_dataIsValid );

    if ( !m_dataIsValid ) {
      return false;
    }
  } else {
    m_dataIsValid = true;
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

  if ( !QFile::exists( dataFile ) ) {
    createFirstStartData();
  }

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
  if ( !m_dataIsValid) {
    emit dataWritten();
    return;
  }

  // FIXME: Queue commands instead of silently failing them.
  if ( m_commitCommand > 0 ) {
    qDebug() << "ERROR" << "Commit command still running";
    return;
  }

  m_bliss.writeFile( m_gitDir->filePath( "std.bliss" ) );
  m_gitDir->addFile( "std.bliss", msg );
  m_commitCommand = m_gitDir->commitData( i18n("Saving pending changes") );
}

void MainModel::slotCommandExecuted( const GitCommand &cmd )
{
  if ( cmd.id() == m_commitCommand ) {
    m_commitCommand = 0;
    if ( !Settings::remoteSyncingEnabled() ) {
      emit dataWritten();
    }
  }
}

void MainModel::slotPushed()
{
  emit dataWritten();
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

  if ( added ) emit todoAdded( todo );
  else emit todoChanged( todo );

  return todo;
}

Bliss::Todo MainModel::addTodo( const Bliss::Todo &person,
  const Bliss::Todo &group )
{
  Bliss::Todo p = person;

  Bliss::Groups groups = p.groups();

  if ( !groups.findGroup( group.id() ).isValid() ) {
    Bliss::Group g;
    g.setId( group.id() );
    groups.addGroup( g );
    p.setGroups( groups );

    return insert( p, i18n("Add %1 to group %2").arg( person.summary().value() )
      .arg( group.summary().value() ) );
  }
  
  return Bliss::Todo();
}

void MainModel::removeTodo( const Bliss::Todo &todo,
  const Bliss::Todo &group )
{
  Bliss::Group::List groups = todo.groups().groupList();
  Bliss::Group::List newGroups;
  
  foreach( Bliss::Group g, groups ) {
    if ( g.id() != group.id() ) {
      newGroups.append( g );
    }
  }
  
  if ( todo.type() != "group" && newGroups.isEmpty() ) {
    m_bliss.remove( todo );
    
    setupGroups();
    
    emit todoRemoved( todo );
  } else {
    Bliss::Todo newtodo = todo;
  
    Bliss::Groups gg;
    gg.setGroupList( newGroups );
    newtodo.setGroups( gg );
    m_bliss.insert( newtodo );
    
    setupGroups();
    
    emit todoChanged( newtodo );
  }
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
  
  emit todoRemoved( group );
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
  return m_bliss.findGroupView( group.id() );
}

void MainModel::createFirstStartData()
{
}

void MainModel::saveViewSequence( const Bliss::Todo &group,
                                  const QStringList &ids )
{
  Bliss::GroupView v = m_bliss.findGroupView( group.id(),
                                              Bliss::Bliss::AutoCreate );
                                              
  Bliss::TodoSequence sequence;
  foreach( QString id, ids ) {
    qDebug() << "ID" << id;
    Bliss::TodoId todoId;
    todoId.setValue( id );
    sequence.addTodoId( todoId );
  }
  
  v.setTodoSequence( sequence );

  m_bliss.insert( v );

  writeData( i18n("Updated sequence in group %1").
    arg( group.summary().value() ) );
}
