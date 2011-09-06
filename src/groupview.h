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
#ifndef GROUPVIEW_H
#define GROUPVIEW_H

#include "bliss/bliss.h"

#include <QtGui>

class BlissItemModel;
class MainModel;

class GroupView : public QWidget
{
    Q_OBJECT
  public:
    GroupView( MainModel *, QWidget *parent = 0 );

    MainModel *model() const;

    Bliss::Todo group() const;
    Bliss::Todo previousGroup() const;

  public slots:
    void showGroup( const Bliss::Todo & );

  signals:
    void goBack();
    void newTodo();
    void showTodo( const Bliss::Todo & );
    void requestShowGroup( const Bliss::Todo & );
    
    void showSettings();

  protected:
    virtual void doShowGroup() = 0;

  private:
    MainModel *m_model;

    Bliss::Todo m_group;
    Bliss::Todo m_previousGroup;
};

#endif
