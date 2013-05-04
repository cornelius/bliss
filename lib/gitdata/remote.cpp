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

#include "remote.h"

#include "dir.h"

#include <KLocale>

using namespace GitData;

Remote::Remote( Dir *dir )
  : m_gitDir( dir ), m_pullCommand( -1 ), m_pushCommand( -1 ),
    m_sshAdded( false )
{
  connect( m_gitDir, SIGNAL( commandExecuted( const GitData::Command & ) ),
    SLOT( slotCommandExecuted( const GitData::Command & ) ) );

  setStatus( i18n("Unsynced") );
}

void Remote::pull()
{
  checkSshAdd();

  Command cmd = Command( "pull" );
  
  m_pullCommand = m_gitDir->executeCommand( cmd );

  setStatus( i18n("Pulling...") );
}

void Remote::push()
{
  checkSshAdd();

  Command cmd = Command( "push" );
  
  m_pushCommand = m_gitDir->executeCommand( cmd );

  setStatus( i18n("Pushing...") );
}

void Remote::checkSshAdd()
{
  if ( !m_sshAdded ) {
    if ( system( "ssh-add -l" ) != 0 ) {
      system( "ssh-add < /dev/null" );
    }
  }
  m_sshAdded = true;
}

void Remote::slotCommandExecuted( const Command &cmd )
{
  if ( cmd.id() == m_pullCommand ) {
    m_pullCommand = -1;
    setStatus( i18n("Synced") );
    emit pulled();
  } else if ( cmd.id() == m_pushCommand ) {
    m_pushCommand = -1;
    setStatus( i18n("Synced") );
    emit pushed();
  }
}

QString Remote::status() const
{
  return m_status;
}

void Remote::setStatus( const QString &status )
{
  m_status = status;
  emit statusChanged( status );
}
