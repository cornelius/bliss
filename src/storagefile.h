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

#include "storage.h"

class StorageFile : public Storage
{
    Q_OBJECT
  public:
    StorageFile( QObject *parent = 0 );
    ~StorageFile();

    void setLocation( const QString & );

    Bliss::Bliss readData();
    void writeData( const Bliss::Bliss &, const QString &msg );

  protected:
    void createTodo( Bliss::Bliss &bliss, const Bliss::Todo &group,
                     const QString &title );

  private:
    QString m_fileName;
};

#endif
