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
#ifndef STORAGEFILE_H
#define STORAGEFILE_H

#include "bliss/bliss.h"

#include <QObject>

class StorageFile : public QObject
{
    Q_OBJECT
  public:
    StorageFile( QObject *parent = 0 );
    ~StorageFile();

    void setFileName( const QString & );

    void writeData( const Bliss::Bliss &, const QString &msg );

    Bliss::Bliss readData();

    void retrieveLog();

  signals:
    void dataWritten();
    void dataRead( const Bliss::Bliss & );
    
    void logRetrieved( const QStringList & );

  private:
    QString m_fileName;
};

#endif
