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

#include "history.h"

History::History()
{
}

void History::setLocationId( const QString &id )
{
  m_locationId = id;
}

void History::set( const QStringList &data )
{
  m_extraData.clear();
  
  foreach( QString history, data ) {
    QStringList entries = history.split( ";" );
    if ( !entries.isEmpty() ) {
      if ( entries.first() == m_locationId ) {
        m_history = entries.mid( 1 );
      } else {
        m_extraData.append( history );
      }
    }
  }
}

QStringList History::get() const
{
  QStringList results = m_extraData;
  results.append( m_locationId + ";" + m_history.join( ";" ) );
  return results;
}

bool History::isEmpty() const
{
  return m_history.isEmpty();
}

QString History::last() const
{
  return m_history.last();
}

void History::append( const QString &id )
{
  m_history.append( id );
}

int History::size() const
{
  return m_history.size();
}

QString History::takeLast()
{
  return m_history.takeLast();
}
