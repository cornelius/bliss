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

#include "newlistdialog.h"

#include <KRandom>

#include <QBoxLayout>
#include <QLabel>

NewListDialog::NewListDialog( MainModel *model, QWidget *parent )
  : KDialog( parent ), m_model( model )
{
  setCaption( "New List" );
  setButtons( KDialog::Ok | KDialog::Cancel );

  QWidget *topWidget = new QWidget;

  QBoxLayout *topLayout = new QVBoxLayout( topWidget );

  QLabel *label = new QLabel( "Enter name of new list" );
  topLayout->addWidget( label );

  m_nameInput = new QLineEdit;
  topLayout->addWidget( m_nameInput );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    SLOT( checkOkButton() ) );

  setMainWidget( topWidget );

  // TODO: port
  // restoreDialogSize( KGlobal::config()->group("NewListDialog") );

  m_nameInput->setFocus();

  checkOkButton();
}

NewListDialog::~NewListDialog()
{
  // TODO: port
  // KConfigGroup cg( KGlobal::config(), "NewListDialog" );
  // saveDialogSize( cg );
}

Bliss::ViewList NewListDialog::list()
{
  Bliss::ViewList list;
  list.setId( KRandom::randomString( 10 ) );
  list.setName( m_nameInput->text() );
  return list;
}

void NewListDialog::checkOkButton()
{
  button( Ok )->setEnabled( !m_nameInput->text().isEmpty() );
}
