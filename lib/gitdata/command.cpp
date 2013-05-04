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

#include "command.h"

using namespace GitData;

int Command::m_nextId = 1;

Command::Command( const QString &command )
  : m_command( command )
{
  setId();
}

Command::Command( const QString &command, const QString &arg )
  : m_command( command )
{
  addArg( arg );
  setId();
}

void Command::setId()
{
  m_id = m_nextId++;
}

int Command::id() const
{
  return m_id;
}

void Command::setCommand( const QString &command )
{
  m_command = command;
}

void Command::addArg( const QString &arg )
{
  m_args << arg;
}

void Command::addOption( const QString &option )
{
  addArg( "-" + option );
}

void Command::addOption( const QString &option, const QString &value )
{
  addArg( "-" + option + " " + value );
}

void Command::addLongOption( const QString &option, const QString &value )
{
  addArg( "--" + option + "=" + value );
}

void Command::setArgs( const QStringList &args )
{
  m_args = args;
}

QString Command::command() const
{
  return m_command;
}

QStringList Command::args() const
{
  return m_args;
}

void Command::setResult( const QStringList &t )
{
  m_result = t;
}

QStringList Command::result() const
{
  return m_result;
}
