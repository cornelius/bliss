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

#include "storagegit.h"

#include "gitdata/gitdir.h"
#include "gitdata/gitremote.h"
#include "settings.h"

#include <KLocale>
#include <KStandardDirs>

#include <QDir>
#include <QDebug>

StorageGit::StorageGit( QObject *parent )
  : Storage( parent ), m_gitDir( 0 ), m_gitRemote( 0 ),m_commitCommand( 0 ),
    m_logCommand( -1 )
{
}

StorageGit::~StorageGit()
{
  delete m_gitDir;
  delete m_gitRemote;
}

void StorageGit::setLocation( const QString &location )
{
  delete m_gitDir;
  delete m_gitRemote;
  
  m_gitDir = new GitData::GitDir( location );

  m_gitRemote = new GitData::GitRemote( m_gitDir );
  connect( m_gitRemote, SIGNAL( pulled() ), SLOT( slotPulled() ) );
  connect( m_gitRemote, SIGNAL( pushed() ), SLOT( slotPushed() ) );
  connect( m_gitRemote, SIGNAL( statusChanged( const QString & ) ),
    SIGNAL( syncingStatusChanged( const QString & ) ) );

  connect( m_gitDir, SIGNAL( commandExecuted( const GitData::GitCommand & ) ),
    SLOT( slotCommandExecuted( const GitData::GitCommand & ) ) );  
}

Bliss::Bliss StorageGit::readData()
{
  Bliss::Bliss bliss;

  QString dataFile = m_gitDir->filePath( "std.bliss" );

  if ( QFile::exists( dataFile ) ) {
    bliss = Bliss::Bliss::parseFile( dataFile, &m_dataIsValid );
  } else {
    m_dataIsValid = true;
  }

  return bliss;
}

void StorageGit::pushData()
{
  m_gitRemote->push();
}

void StorageGit::pullData()
{
  m_gitRemote->pull();
}

void StorageGit::writeData( const Bliss::Bliss &b, const QString &msg )
{
  // FIXME: Make Bliss::writeFile const, so copy is not needed
  Bliss::Bliss bliss = b;

  if ( !m_dataIsValid) {
    emit dataWritten();
    return;
  }

  // FIXME: Queue commands instead of silently failing them.
  if ( m_commitCommand > 0 ) {
    qDebug() << "ERROR" << "Commit command still running";
    return;
  }

  bliss.writeFile( m_gitDir->filePath( "std.bliss" ) );
  m_gitDir->addFile( "std.bliss", msg );
  m_commitCommand = m_gitDir->commitData( i18n("Saving pending changes") );
}

void StorageGit::slotCommandExecuted( const GitData::GitCommand &cmd )
{
  if ( cmd.id() == m_commitCommand ) {
    m_commitCommand = 0;
    if ( !Settings::remoteSyncingEnabled() ) {
      emit dataWritten();
    }
  } else if ( cmd.id() == m_logCommand ) {
    m_logCommand = -1;
    
    QStringList log;
    
    foreach( QString line, cmd.result() ) {
      int pos = line.indexOf( " " );
      QString revision = line.left( pos );
//      qDebug() << revision;
      QString message = line.mid( pos + 2 );
      log.append( message );
    }
    
    emit logRetrieved( log );
  }
}

void StorageGit::slotPushed()
{
  emit dataWritten();
}

void StorageGit::slotPulled()
{
  emit dataRead( readData() );
}

void StorageGit::retrieveLog()
{
  m_logCommand = m_gitDir->getLog();
}
