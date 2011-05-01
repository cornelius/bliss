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
#ifndef FANMENUITEM_H
#define FANMENUITEM_H

#include <QtGui>

class FanMenuElement;

class FanMenuItem : public QObject
{
    Q_OBJECT
  public:
    FanMenuItem( const QString &text );

    void setText( const QString &text );        
    QString text() const;
        
    void setElement( FanMenuElement * );

    void emitClicked();

  signals:
    void clicked();
        
  private:
    QString m_text;
    FanMenuElement *m_element;
};

#endif
