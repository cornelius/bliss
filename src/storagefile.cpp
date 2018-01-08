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

#include "storagefile.h"

#include "settings.h"

#include <KRandom>
#include <KLocale>

#include <QtCore>

StorageFile::StorageFile( QObject *parent )
  : Storage( parent )
{
}

StorageFile::~StorageFile()
{
}

void StorageFile::setLocation( const QString &fileName )
{
  m_fileName = fileName;
}

Bliss::Bliss StorageFile::readData()
{
  qDebug() << "READ FROM FILE" << m_fileName;

  Bliss::Bliss bliss;

  QFile file( m_fileName );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug() << "ERROR READING FILE" << m_fileName;
  } else {
    QTextStream ts( &file );
    QString line;
    do {
      line = ts.readLine();

      if ( line.startsWith( "*" ) ) {
        addTodo();
        m_summary = line.mid( 1 ).trimmed();
      } else if ( line.trimmed().isEmpty() ) {
        addTodo();
        m_todoEntries << "";
      } else {
        m_summary += " " + line.trimmed();
      }
    } while ( !line.isNull() );

    addTodo();

    while( !m_todoEntries.isEmpty() && m_todoEntries.first().isEmpty() ) {
      m_todoEntries.removeFirst();
    }
    while( !m_todoEntries.isEmpty() && m_todoEntries.last().isEmpty() ) {
      m_todoEntries.removeLast();
    }

    Bliss::Todo rootGroup;
    rootGroup.setType( "group" );
    rootGroup.setId( "root" );
    Bliss::Summary rootTitle;
    rootTitle.setValue( QFileInfo( file ).baseName() );
    rootGroup.setSummary( rootTitle );
    bliss.insert( rootGroup );

    Bliss::Group group;
    group.setId( rootGroup.id() );
    Bliss::Root root;
    root.setGroup( group );
    bliss.setRoot( root );

    foreach( QString todoEntry, m_todoEntries ) {
      createTodo( bliss, rootGroup, todoEntry );
    }
  }

  if ( !bliss.writeFile( "out.bliss" ) ) {
    qDebug() << "ERROR WRITING out.bliss";
  }

  return bliss;
}

void StorageFile::addTodo()
{
  if ( !m_summary.isEmpty() ) {
    m_todoEntries.append( m_summary );
    m_summary.clear();
  }
}

void StorageFile::createTodo( Bliss::Bliss &bliss, const Bliss::Todo &group,
                              const QString &title )
{
  Bliss::Todo todo;
  todo.setId( KRandom::randomString( 10 ) );

  Bliss::Summary summary;
  summary.setValue( title );
  todo.setSummary( summary );

  Bliss::Group g;
  g.setId( group.id() );
  Bliss::Groups groups;
  groups.addGroup( g );
  todo.setGroups( groups );

  bliss.addTodo( todo );
}

void StorageFile::writeData( const Bliss::Bliss &b, const QString &msg )
{
  Q_UNUSED( msg )

  // FIXME: Make Bliss::writeFile const, so copy is not needed
  Bliss::Bliss bliss = b;

  QFile file( m_fileName );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qDebug() << "ERROR WRITING FILE" << m_fileName;
  } else {
    QTextStream ts( &file );
    Bliss::Todo::List todos = b.todoList();
    foreach( Bliss::Todo todo, todos ) {
      if ( todo.type() != "group" ) {
        QString summary = todo.summary().value();
        if ( summary.isEmpty() ) ts << "\n\n";
        else {
          QString chunk = summary;
          ts << "* ";
          while ( chunk.length() > 78 ) {
            int separatorPos = chunk.lastIndexOf( " ", 77 );
            if ( separatorPos < 0 ) {
              separatorPos = chunk.indexOf( " ", 77 );
            }
            ts << chunk.left( separatorPos ) << "\n ";
            chunk = chunk.mid( separatorPos );
          }
          ts << chunk;
        }
      }
    }
    ts << "\n";
  }

  emit dataWritten();
}
