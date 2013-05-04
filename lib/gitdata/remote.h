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
#ifndef GITDATA_REMOTE_H
#define GITDATA_REMOTE_H

#include <gitdata/gitdata_export.h>

#include <gitdata/command.h>

#include <QtCore>

namespace GitData {

class Dir;

class GITDATA_EXPORT Remote : public QObject
{
    Q_OBJECT
  public:
    Remote( Dir * );

    void pull();
    void push();
    
    QString status() const;
    
  signals:
    void pulled();
    void pushed();

    void statusChanged( const QString & );

  protected:
    void setStatus( const QString & );

    void checkSshAdd();

  protected slots:
    void slotCommandExecuted( const GitData::Command & );

  private:
    Dir *m_gitDir;
    int m_pullCommand;
    int m_pushCommand;
    QString m_status;
    bool m_sshAdded;
};

}

#endif
