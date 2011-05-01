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

#include "gitcommand.h"

int GitCommand::m_nextId = 1;

GitCommand::GitCommand( const QString &command )
  : m_command( command )
{
  setId();
}

GitCommand::GitCommand( const QString &command, const QString &arg )
  : m_command( command )
{
  addArg( arg );
  setId();
}

void GitCommand::setId()
{
  m_id = m_nextId++;
}

int GitCommand::id() const
{
  return m_id;
}

void GitCommand::setCommand( const QString &command )
{
  m_command = command;
}

void GitCommand::addArg( const QString &arg )
{
  m_args << arg;
}

void GitCommand::addOption( const QString &option )
{
  addArg( "-" + option );
}

void GitCommand::addOption( const QString &option, const QString &value )
{
  addArg( "-" + option + " " + value );
}

void GitCommand::addLongOption( const QString &option, const QString &value )
{
  addArg( "--" + option + "=" + value );
}

void GitCommand::setArgs( const QStringList &args )
{
  m_args = args;
}

QString GitCommand::command() const
{
  return m_command;
}

QStringList GitCommand::args() const
{
  return m_args;
}

void GitCommand::setResult( const QStringList &t )
{
  m_result = t;
}

QStringList GitCommand::result() const
{
  return m_result;
}
