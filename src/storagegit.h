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

#include "storage.h"

#include "gitdata/gitcommand.h"

namespace GitData {
class GitDir;
class GitRemote;
}

class StorageGit : public Storage
{
    Q_OBJECT
  public:
    StorageGit( QObject *parent = 0 );
    ~StorageGit();

    void setLocation( const QString & );
    
    Bliss::Bliss readData();
    void writeData( const Bliss::Bliss &, const QString &msg );

    void retrieveLog();

    void pushData();
    void pullData();

  protected slots:
    void slotCommandExecuted( const GitData::GitCommand & );
    void slotPushed();
    void slotPulled();

  private:
    GitData::GitDir *m_gitDir;
    GitData::GitRemote *m_gitRemote;
  
    bool m_dataIsValid;
    
    int m_commitCommand;
    int m_logCommand;
};

#endif
