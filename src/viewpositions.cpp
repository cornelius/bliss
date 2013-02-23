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

#include "viewpositions.h"

ViewPositions::ViewPositions()
{
}

void ViewPositions::set( const QStringList &data )
{
  foreach( QString position, data ) {
    QStringList entries = position.split( ";" );
    if ( entries.size() == 3 ) {
      m_positions.insert( entries[0],
                          QPoint( entries[1].toInt(), entries[2].toInt() ) );
    }
  }
}

QStringList ViewPositions::get() const
{
  QStringList results;
  foreach( QString id, m_positions.keys() ) {
    QPoint pos = m_positions[ id ];
    results.append( id + ";" + QString::number( pos.x() )  + ";" +
                    QString::number( pos.y() ) );
  }
  return results;
}

bool ViewPositions::hasPosition( const Bliss::Todo &group )
{
  return m_positions.find( group.id() ) != m_positions.end();
}

QPoint ViewPositions::position( const Bliss::Todo &group )
{
  return m_positions.value( group.id() );
}

void ViewPositions::setPosition( const Bliss::Todo &group, const QPoint &pos )
{
  m_positions.insert( group.id(), pos );
}
