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

#include <KConfig>
#include <KMessageBox>
#include <KPushButton>
#include <KGlobal>

NewTodoDialog::NewTodoDialog( MainModel *model, QWidget *parent )
  : KDialog( parent ), m_model( model ), m_proxyModel( 0 ), m_matchList( 0 )
{
  setCaption( "New Todo" );
  setButtons( KDialog::Ok | KDialog::Cancel );
  setModal( true );

  QWidget *topWidget = new QWidget;
  
  QBoxLayout *topLayout = new QVBoxLayout( topWidget );

  QLabel *label = new QLabel( "Enter summary of todo" );
  topLayout->addWidget( label );
  
  m_nameInput = new QLineEdit;
  topLayout->addWidget( m_nameInput );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    SLOT( checkOkButton() ) );

  m_matchList = new MatchList( m_model );
  topLayout->addWidget( m_matchList );
  
  connect( m_matchList, SIGNAL( activated() ), SLOT( accept() ) );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    m_matchList, SLOT( filter( const QString & ) ) );

  setMainWidget( topWidget );

  restoreDialogSize( KGlobal::config()->group("newtododialog") );

  m_nameInput->setFocus();

  checkOkButton();
}

NewTodoDialog::~NewTodoDialog()
{
  KConfigGroup cg( KGlobal::config(), "newtododialog" );
  saveDialogSize( cg );
}

Bliss::Todo NewTodoDialog::todo()
{
  if ( !m_matchList || !m_matchList->todo().isValid() ) {
    Bliss::Todo todo;
    Bliss::Summary name;
    name.setValue( m_nameInput->text() );
    todo.setSummary( name );
    return todo;
  } else {
    return m_matchList->todo();
  }
}

void NewTodoDialog::checkOkButton()
{
  button( Ok )->setEnabled( !m_nameInput->text().isEmpty() );
}
