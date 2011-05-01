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

#include "settingswidget.h"

#include "settings.h"
#include "mainmodel.h"
#include "gitremote.h"

#include <KLocale>

SettingsWidget::SettingsWidget( MainModel *model, QWidget *parent)
  : QWidget( parent ), m_model( model )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *controlLayout = new QHBoxLayout;
  topLayout->addLayout( controlLayout );

  m_graphicsModeCheck = new QCheckBox( i18n("Use fancy view") );
  controlLayout->addWidget( m_graphicsModeCheck );
  connect( m_graphicsModeCheck, SIGNAL( stateChanged( int ) ),
    SIGNAL( showView() ) );

  m_syncingCheck = new QCheckBox( i18n("Syncing enabled") );
  controlLayout->addWidget( m_syncingCheck );
  connect( m_syncingCheck, SIGNAL( stateChanged( int ) ),
    SLOT( slotSyncingCheckChanged() ) );

  QLabel *label = new QLabel;
  controlLayout->addWidget( label );
  connect( m_model->gitRemote(), SIGNAL( statusChanged( const QString & ) ),
    label, SLOT( setText( const QString & ) ) );

  controlLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("Hide Settings") );
  controlLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( hide() ) );

  readConfig();
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::readConfig()
{
  m_syncingCheck->setChecked( Settings::remoteSyncingEnabled() );
  m_graphicsModeCheck->setChecked( Settings::fancyMode() );
}

void SettingsWidget::writeConfig()
{
  Settings::setFancyMode( m_graphicsModeCheck->isChecked() );
}

void SettingsWidget::slotSyncingCheckChanged()
{
  Settings::setRemoteSyncingEnabled( m_syncingCheck->isChecked() );
}

bool SettingsWidget::fancyMode()
{
  return m_graphicsModeCheck->isChecked();
}
