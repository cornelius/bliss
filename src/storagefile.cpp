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
#include "krandom.h"

#include <KLocale>
#include <KStandardDirs>

#include <QtCore>

StorageFile::StorageFile( QObject *parent )
  : QObject( parent )
{
}

StorageFile::~StorageFile()
{
}

void StorageFile::setFileName( const QString &fileName )
{
  m_fileName = fileName;
}

Bliss::Bliss StorageFile::readData()
{
  Bliss::Bliss bliss;

  QFile file( m_fileName );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug() << "ERROR READING FILE" << m_fileName;
  } else {
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
  
    QTextStream ts( &file );
    QString line;
    do {
      line = ts.readLine();
      
      if ( line.startsWith( "*" ) ) {
        Bliss::Todo todo;
        todo.setId( KRandom::randomString( 10 ) );
        
        Bliss::Summary title;
        title.setValue( line.mid( 1 ).trimmed() );
        todo.setSummary( title );

        Bliss::Group group;
        group.setId( rootGroup.id() );
        Bliss::Groups groups;
        groups.addGroup( group );
        todo.setGroups( groups );
        
        bliss.addTodo( todo );
      }
      
    } while ( !line.isNull() );
  }

  if ( !bliss.writeFile( "out.bliss" ) ) {
    qDebug() << "ERROR WRITING out.bliss";
  }

  return bliss;
}

void StorageFile::writeData( const Bliss::Bliss &b, const QString &msg )
{
  Q_UNUSED( msg )

  // FIXME: Make Bliss::writeFile const, so copy is not needed
  Bliss::Bliss bliss = b;

  qDebug() << "TODO: write data to file";
}

void StorageFile::retrieveLog()
{
  QStringList log;
  log.append( i18n("No log available") );

  emit logRetrieved( log );
}
