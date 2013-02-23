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
#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "gitcommand.h"

#include <QtGui>

class MainModel;

class HistoryView : public QWidget
{
    Q_OBJECT
  public:
    HistoryView( MainModel * );

    void loadHistory();

  signals:
    void showGroupView();
    
  protected slots:
    void historyLoaded( const QStringList & );

  private:
    MainModel *m_model;

    QListWidget *m_list;
};

#endif
