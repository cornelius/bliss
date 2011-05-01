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
#ifndef GITCOMMAND_H
#define GITCOMMAND_H

#include <QString>
#include <QStringList>
#include <QList>

class GitCommand
{
  public:
    typedef QList<GitCommand> List;
  
    GitCommand( const QString &command );
    GitCommand( const QString &command, const QString &arg );

    int id() const;

    void setCommand( const QString & );
    
    void addArg( const QString & );
    void addOption( const QString &option );
    void addOption( const QString &option, const QString &value );
    void addLongOption( const QString &option, const QString &value );
    void setArgs( const QStringList & );

    QString command() const;
    QStringList args() const;

    void setResult( const QStringList & );
    QStringList result() const;

  private:
    void setId();

  private:
    QString m_command;
    QStringList m_args;
    int m_id;
    QStringList m_result;

    static int m_nextId;
};

#endif
