/*
    Copyright (c) 2011 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QtGui>

class FanMenu;

class MenuHandler
{
  public:
    MenuHandler( QGraphicsScene * );

    FanMenu *createMenu();
    void showMenu( FanMenu *, const QPointF &pos );

  private:
    QGraphicsScene *m_scene;
};

#endif
