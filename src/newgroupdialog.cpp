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

#include "newgroupdialog.h"

#include <KConfig>
#include <KMessageBox>
#include <KPushButton>
#include <KGlobal>

NewGroupDialog::NewGroupDialog( MainModel *model, QWidget *parent )
  : KDialog( parent ), m_model( model )
{
  setCaption( "New Group" );
  setButtons( KDialog::Ok | KDialog::Cancel );
  setModal( true );

  QWidget *topWidget = new QWidget;
  
  QBoxLayout *topLayout = new QVBoxLayout( topWidget );

  QLabel *label = new QLabel( "Enter name of new group" );
  topLayout->addWidget( label );
  
  m_nameInput = new QLineEdit;
  topLayout->addWidget( m_nameInput );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    SLOT( checkOkButton() ) );

  setMainWidget( topWidget );

  m_nameInput->setFocus();

  checkOkButton();
}

NewGroupDialog::~NewGroupDialog()
{
}

Bliss::Todo NewGroupDialog::todo()
{
  Bliss::Todo identity;
  identity.setType( "group" );
  Bliss::Summary name;
  name.setValue( m_nameInput->text() );
  identity.setSummary( name );
  return identity;
}

void NewGroupDialog::checkOkButton()
{
  button( Ok )->setEnabled( !m_nameInput->text().isEmpty() );
}
