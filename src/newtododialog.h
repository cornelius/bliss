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
#ifndef NEWTODODIALOG_H
#define NEWTODODIALOG_H

#include "mainmodel.h"

#include <KDialog>

#include <QtGui>

class MatchList;

class NewTodoDialog : public KDialog
{
    Q_OBJECT
  public:
    NewTodoDialog( MainModel *, QWidget *parent = 0 );
    ~NewTodoDialog();

    Bliss::Todo todo();

  protected slots:
    void checkOkButton();

  private:
    MainModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
  
    QLineEdit *m_nameInput;
    MatchList *m_matchList;
};

#endif
