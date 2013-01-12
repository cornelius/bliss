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

#include "delayedsignal.h"

#include <QTimer>

DelayedSignal::DelayedSignal( QObject *object, const Bliss::Todo &todo )
  : m_todo( todo )
{
  connect( this, SIGNAL( todoAdded( const Bliss::Todo & ) ),
           object, SIGNAL( todoAdded( const Bliss::Todo & ) ) );
  connect( this, SIGNAL( todoChanged( const Bliss::Todo & ) ),
           object, SIGNAL( todoChanged( const Bliss::Todo & ) ) );
  connect( this, SIGNAL( todoRemoved( const Bliss::Todo & ) ),
           object, SIGNAL( todoRemoved( const Bliss::Todo & ) ) );
}

void DelayedSignal::emitTodoAdded()
{
  QTimer::singleShot( 0, this, SLOT( slotEmitTodoAdded() ) );
}

void DelayedSignal::emitTodoChanged()
{
  QTimer::singleShot( 0, this, SLOT( slotEmitTodoChanged() ) );
}

void DelayedSignal::emitTodoRemoved()
{
  QTimer::singleShot( 0, this, SLOT( slotEmitTodoRemoved() ) );
}

void DelayedSignal::slotEmitTodoAdded()
{
  emit todoAdded( m_todo );
  deleteLater();
}

void DelayedSignal::slotEmitTodoChanged()
{
  emit todoChanged( m_todo );
  deleteLater();
}

void DelayedSignal::slotEmitTodoRemoved()
{
  emit todoRemoved( m_todo );
  deleteLater();
}
