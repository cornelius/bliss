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
#ifndef GITDIR_H
#define GITDIR_H

#include "gitcommand.h"

#include <QString>
#include <QProcess>

class GitDir : public QObject
{
    Q_OBJECT
  public:
    GitDir( const QString &dirPath );
    ~GitDir();

    void init();

    /**
      Directories in
      the file name are created automatically. Call this function before you
      write to the file.
    */
    void createPath( const QString &fileName );
    /**
      Add file to git repo.
    */
    void addFile( const QString &fileName, const QString &msg );

  
    QString filePath( const QString &fileName );
  
    int commitData( const QString &msg );

    int getLog();

    /**
      Returns id of executed command.
    */
    int executeCommit( const QString &arg, const QString &msg );
  
    /**
      Returns id of executed command.
    */
    int executeCommand( const QString &command );
    /**
      Returns id of executed command.
    */
    int executeCommand( const QString &command, const QString &arg );
  
    /**
      Returns id of executed command.
    */
    int executeCommand( const GitCommand & );

  signals:
    void commandExecuted( const GitCommand & );

  protected:
    void processQueue();
  
  protected slots:
    void slotProcessFinished( int exitCode, QProcess::ExitStatus exitStatus );

  private:
    QString m_dirPath;
    QProcess *m_process;
    
    GitCommand::List m_queue;

    int m_commit;
};

#endif
