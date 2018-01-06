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

#include "newtododialog.h"

#include "matchlist.h"

#include <KMessageBox>
#include <KRandom>

#include <QLabel>
#include <QBoxLayout>

NewTodoDialog::NewTodoDialog( MainModel *model, QWidget *parent )
  : KDialog( parent ), m_model( model )
{
  setCaption( "New Todo" );
  setButtons( KDialog::Ok | KDialog::Cancel );

  QWidget *topWidget = new QWidget;

  QBoxLayout *topLayout = new QVBoxLayout( topWidget );

  QLabel *label = new QLabel( "Enter summary of todo" );
  topLayout->addWidget( label );

  m_nameInput = new QLineEdit;
  topLayout->addWidget( m_nameInput );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    SLOT( checkOkButton() ) );

  setMainWidget( topWidget );

  m_nameInput->setFocus();

  checkOkButton();
}

NewTodoDialog::~NewTodoDialog()
{
}

Bliss::Todo NewTodoDialog::todo()
{
  Bliss::Todo todo;
  todo.setId( KRandom::randomString( 10 ) );
  Bliss::Summary name;
  name.setValue( m_nameInput->text() );
  todo.setSummary( name );
  return todo;
}

void NewTodoDialog::checkOkButton()
{
  button( Ok )->setEnabled( !m_nameInput->text().isEmpty() );
}
