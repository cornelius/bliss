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
#ifndef VIEWPOSITIONS_H
#define VIEWPOSITIONS_H

#include <bliss/bliss.h>

#include <QStringList>
#include <QPoint>
#include <QMap>

class ViewPositions
{
  public:
    ViewPositions();

    void set( const QStringList & );
    QStringList get() const;
    
    bool hasPosition( const Bliss::Todo &group );
    QPoint position( const Bliss::Todo &group );
    void setPosition( const Bliss::Todo &group, const QPoint &pos );
    
  private:
    QMap<QString,QPoint> m_positions;
};

#endif
