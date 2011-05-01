/*
    This file is part of KDE.

    Copyright (C) 2009-2011 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "bliss/bliss.h"

#include <QtGui>

class QPainter;
class KUrl;
class MainModel;
class GroupListView;
class GroupGraphicsView;
class PersonView;
class SettingsWidget;
class GroupView;
class Overview;
class HistoryView;
class SearchEdit;
class SearchResultView;

class MainView : public QWidget
{
    Q_OBJECT
  public:
    MainView( QWidget *parent = 0 );
    virtual ~MainView();

    void readConfig();
    void writeConfig();

    void readData();
    void writeData( const QString &msg );

  public slots:
    void newTodo();
    void newSubGroup();

    void cloneGroup( const Bliss::Todo &group );
    void removeGroup( const Bliss::Todo &group );

    void showRoot();
    void showView();
    void goBack();
    void showGroup( const Bliss::Todo & );
    void showTodo( const Bliss::Todo & );
    void removeTodo( const Bliss::Todo &person,
      const Bliss::Todo &group );

    void showSettings();

    void showOverview();
    void showGroupView();
    void showListView();
    void showHistory();

  signals:
    void dataWritten();
  
  protected:
    void connectGroupView( GroupView * );
  
  protected slots:
    void continueShowGroup();

    void showSearch( const QString & );
    void stopSearch();

  private:
    MainModel *m_model;

    Bliss::Todo m_group;

    QStringList m_history;

    QPushButton *m_backButton;
    QLabel *m_groupNameLabel;
    SearchEdit *m_searchEdit;
    SettingsWidget *m_settingsWidget;
    QWidget *m_groupWidget;
    QStackedLayout *m_listLayout;
    GroupListView *m_groupListView;
    GroupGraphicsView *m_groupGraphicsView;
    PersonView *m_personView;
    Overview *m_overview;
    HistoryView *m_historyView;
    SearchResultView *m_searchResultView;
};

#endif
