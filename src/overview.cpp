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

#include "overview.h"

#include "searchedit.h"
#include "searchresultview.h"

#include <KLocale>
#include <KStandardDirs>

Overview::Overview( MainModel *model )
  : m_model( model )
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  
  QBoxLayout *searchLayout = new QVBoxLayout;
  topLayout->addLayout( searchLayout, 1 );
  
  m_searchEdit = new SearchEdit;
  searchLayout->addWidget( m_searchEdit );

  m_searchResultView = new SearchResultView( m_model );
  searchLayout->addWidget( m_searchResultView );

  connect( m_searchEdit, SIGNAL( search( const QString & ) ),
    m_searchResultView, SLOT( search( const QString & ) ) );

  
  QBoxLayout *rightLayout = new QVBoxLayout;
  topLayout->addLayout( rightLayout, 1 );

  rightLayout->addStretch( 1 );
  
  QBoxLayout *buttonLayout = new QVBoxLayout;
  rightLayout->addLayout( buttonLayout );

  buttonLayout->addStretch( 1 );
  
  QPushButton *button = new QPushButton( i18n("Group view") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( showGroupView() ) );

  button = new QPushButton( i18n("History") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( showHistory() ) );

  rightLayout->addStretch( 1 );

  QString logoPath = KStandardDirs::locate( "appdata", "bliss-logo.png" );
  QPixmap logoPixmap = QPixmap( logoPath );

  QLabel *logo = new QLabel;
  logo->setPixmap( logoPixmap );
  rightLayout->addWidget( logo );

  QLabel *about = new QLabel;
  rightLayout->addWidget( about );
  
  QString text = "<qt>";
  text += i18n("Bliss - the humane todo list");
  text += "<br/>";
  text += "<br/>";
  text += "Copyright (c) 2013 Cornelius Schumacher";
  text += "<br/>";
  text += "This program is distributed under the terms of the ";
  text += "<a href=\"http://gpl.org\">GPL</a>";
  text += "</qt>";

  about->setText( text );

  rightLayout->addStretch( 1 );
}
