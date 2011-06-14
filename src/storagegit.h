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
#ifndef STORAGEGIT_H
#define STORAGEGIT_H

#include "bliss/bliss.h"

#include "gitcommand.h"

#include <QObject>

class GitDir;
class GitRemote;

class StorageGit : public QObject
{
    Q_OBJECT
  public:
    StorageGit( QObject *parent = 0 );
    ~StorageGit();

    void writeData( const Bliss::Bliss &, const QString &msg );

    Bliss::Bliss readData();

    void retrieveLog();

    void pushData();
    void pullData();

  signals:
    void syncingStatusChanged( const QString & );
  
    void dataWritten();
    
    void logRetrieved( const QStringList & );

  protected slots:
    void slotCommandExecuted( const GitCommand & );
    void slotPushed();

  private:
    GitDir *m_gitDir;
    GitRemote *m_gitRemote;
  
    bool m_dataIsValid;
    
    int m_commitCommand;
    int m_logCommand;
};

#endif
