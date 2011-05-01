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

#include "gitdir.h"

#include "QDebug"
#include "QDir"

GitDir::GitDir( const QString &dirPath )
  : m_dirPath( dirPath )
{
  // FIXME: Store QDir instead of path

  m_process = new QProcess( this );
  connect( m_process, SIGNAL( finished(int,QProcess::ExitStatus) ),
    SLOT( slotProcessFinished(int,QProcess::ExitStatus ) ) );

  init();
}

GitDir::~GitDir()
{
}

void GitDir::init()
{
  m_process->setWorkingDirectory( m_dirPath );

  QDir dir( m_dirPath );
  if ( !dir.exists() ) {
    dir.mkpath( m_dirPath );
    executeCommand( "init" );
  }
}

void GitDir::slotProcessFinished( int exitCode,
  QProcess::ExitStatus exitStatus )
{
  Q_UNUSED( exitStatus );

  QString output = m_process->readAllStandardOutput();

  qDebug() << "EXIT CODE:" << exitCode;
  qDebug() << "STDOUT:" << output;
  qDebug() << "STDERR:" << m_process->readAllStandardError();

  GitCommand cmd = m_queue.first();
  cmd.setResult( output.split( "\n" ) );

  emit commandExecuted( cmd );

  m_queue.removeFirst();

  processQueue();
}

// FIXME: Return a GitFile object, which can be used as handle.
void GitDir::createPath( const QString &fileName )
{
  QString filePath = GitDir::filePath( fileName );
  if ( !QFile::exists( filePath ) ) {
    QFileInfo fi( filePath );
    QDir dir( fi.absoluteDir() );
    if ( !dir.exists() ) {
      dir.mkpath( dir.path() );
    }
  }
}

// FIXME: Return a GitFile object, which can be used as handle.
void GitDir::addFile( const QString &fileName, const QString &msg )
{
  QString filePath = GitDir::filePath( fileName );
  if ( !QFile::exists( filePath ) ) {
    qDebug() << "ERROR: file" << filePath << "doesn't exist";
  } else {
    executeCommand( "add", fileName );
    executeCommit( QString(), msg );
  }
}

int GitDir::commitData( const QString &msg )
{
  return executeCommit( "-a", msg );
}

int GitDir::executeCommit( const QString &arg, const QString &msg )
{
  GitCommand cmd = GitCommand( "commit" );
  if ( !arg.isEmpty() ) {
    cmd.addArg( arg );
  }
  cmd.addOption( "m", msg );
  
  return executeCommand( cmd ); 
}

QString GitDir::filePath( const QString &fileName )
{
  return m_dirPath + "/" + fileName;
}

int GitDir::getLog()
{
  GitCommand cmd = GitCommand( "log" );
  cmd.addLongOption( "pretty", "oneline" );
  return executeCommand( cmd );
}

int GitDir::executeCommand( const QString &command )
{
  return executeCommand( GitCommand( command ) );
}

int GitDir::executeCommand( const QString &command, const QString &arg )
{
  return executeCommand( GitCommand( command, arg ) );
}

int GitDir::executeCommand( const GitCommand &command )
{
  m_queue.append( command );
  processQueue();
  return command.id();
}

void GitDir::processQueue()
{
  if ( m_queue.isEmpty() ) return;
  if ( m_process->state() != QProcess::NotRunning ) return;

  GitCommand command = m_queue.first();

  qDebug() << "RUNNING GIT" << command.command() << command.args();

  QStringList args = command.args();
  args.prepend( command.command() );

  m_process->start( "git", args );
}
