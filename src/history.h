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
#ifndef HISTORY_H
#define HISTORY_H

#include <QStringList>

class History
{
  public:
    History();

    void setLocationId( const QString & );

    void set( const QStringList & );
    QStringList get() const;
    
    bool isEmpty() const;
    QString last() const;
    void append( const QString &id );
    int size() const;
    QString takeLast();
    
  private:
    QString m_locationId;
    QStringList m_extraData;
    QStringList m_history;
};

#endif
