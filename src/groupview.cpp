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

#include "groupview.h"

#include "mainmodel.h"

GroupView::GroupView( MainModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model )
{
}

MainModel *GroupView::model() const
{
  return m_model;
}

void GroupView::showGroup( const Bliss::Todo &group )
{
  m_previousGroup = m_group;
  m_group = group;

  doShowGroup();
}

Bliss::Todo GroupView::group() const
{
  return m_group;
}

Bliss::Todo GroupView::previousGroup() const
{
  return m_previousGroup;
}
