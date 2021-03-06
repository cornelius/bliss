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
#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QLineEdit>

class SearchEdit : public QLineEdit
{
    Q_OBJECT
  public:
    SearchEdit();

    void setEmpty();

  signals:
    void search( const QString & );
    void stopSearch();

  protected:
    void focusInEvent( QFocusEvent * );
    void focusOutEvent( QFocusEvent * );

  protected slots:
    void slotTextChanged( const QString & );

  private:
    bool m_empty;
};

#endif
