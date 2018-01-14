/*
    This file is part of KDE.

    Copyright (C) 2011 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QCheckBox>

class MainModel;

class SettingsWidget : public QWidget
{
    Q_OBJECT
  public:
    SettingsWidget( MainModel *model, QWidget *parent = 0 );
    virtual ~SettingsWidget();

    void readConfig();
    void writeConfig();

  signals:
    void showView();

  protected slots:
    void slotSyncingCheckChanged();

  private:
    MainModel *m_model;

    QCheckBox *m_syncingCheck;
};

#endif
