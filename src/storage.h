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
#ifndef STORAGE_H
#define STORAGE_H

#include "bliss/bliss.h"

#include <QObject>

class Storage : public QObject
{
    Q_OBJECT
  public:
    Storage( QObject *parent = 0 );
    ~Storage();

    virtual void setLocation( const QString & ) = 0;
    
    virtual Bliss::Bliss readData() = 0;
    virtual void writeData( const Bliss::Bliss &, const QString &msg ) = 0;

    virtual void retrieveLog();

    virtual void pushData();
    virtual void pullData();
    
  signals:
    void syncingStatusChanged( const QString & );

    void dataWritten();
    void dataRead( const Bliss::Bliss & );
    
    void logRetrieved( const QStringList & );
};

#endif
