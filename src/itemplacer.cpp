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

#include "itemplacer.h"

#include "todoitem.h"

#include <QParallelAnimationGroup>

ItemPlacer::ItemPlacer( QObject *parent )
  : QObject( parent ), m_placeItemsAnimation( 0 )
{
}

void ItemPlacer::prepare()
{
  if ( !m_placeItemsAnimation ) {
    m_placeItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_placeItemsAnimation, SIGNAL( finished() ),
      SLOT( finished() ) );
  } else {
    m_placeItemsAnimation->stop();
  }
  m_placeItemsAnimation->clear();
  m_placeItemsAnimations.clear();
}

void ItemPlacer::addItem( TodoItem *item, qreal itemX, qreal itemY )
{
  QPropertyAnimation *animation = new QPropertyAnimation( item, "pos", this );
  m_placeItemsAnimation->insertAnimation( 0, animation );
  m_placeItemsAnimations.append( animation );

  animation->setDuration( 300 );
  QPointF target( itemX, itemY );
  animation->setEndValue( target );
  animation->setEasingCurve( QEasingCurve::OutCubic );
}

void ItemPlacer::setStartValue( const QPointF &pos )
{
  foreach( QPropertyAnimation *animation, m_placeItemsAnimations ) {
    animation->setStartValue( pos );
  }
}

void ItemPlacer::start()
{
  m_placeItemsAnimation->start();
}

void ItemPlacer::stop()
{
  if ( m_placeItemsAnimation ) m_placeItemsAnimation->stop();
}
