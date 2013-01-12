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
#ifndef DELAYEDSIGNAL_H
#define DELAYEDSIGNAL_H

#include "bliss/bliss.h"

#include <QObject>

class DelayedSignal : public QObject
{
    Q_OBJECT
  public:
    DelayedSignal( QObject *, const Bliss::Todo & );
    
    void emitTodoAdded();
    void emitTodoChanged();
    void emitTodoRemoved();

  signals:
    void todoAdded( const Bliss::Todo & );
    void todoChanged( const Bliss::Todo & );
    void todoRemoved( const Bliss::Todo & );
    
  protected slots:
    void slotEmitTodoAdded();
    void slotEmitTodoChanged();
    void slotEmitTodoRemoved();
    
  private:
    QObject m_object;
    Bliss::Todo m_todo;
};

#endif
