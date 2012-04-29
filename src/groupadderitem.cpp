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

#include "groupadderitem.h"

#include "mainmodel.h"
#include "fanmenu.h"
#include "buttonitem.h"
#include "todohandleitem.h"
#include "groupaddersidebaritem.h"
#include "droptargetitem.h"

#include <KLocale>

GroupAdderItem::GroupAdderItem( MainModel *model )
  : m_model( model ), m_defaultItemSize( 100 ), m_expanded( false ),
    m_expandGroupsAnimation( 0 ), m_collapseGroupsAnimation( 0 ),
    m_groupOffset( 85 ), m_groupSpacing( 140 )
{
  QColor backgroundColor( 230,229,229 );
  
  setBrush( backgroundColor );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  m_sidebarBackground = new GroupAdderSidebarItem( this );
  m_sidebarBackground->setBrush( backgroundColor );
  m_sidebarBackground->setPen( pen );
  showAsSidebar( false );

  setItemSize( m_defaultItemSize );

  m_expandButton = new ButtonItem( this );
  m_expandButton->setPos( 21, -21 );
  connect( m_expandButton, SIGNAL( clicked() ), SLOT( expand() ) );

  for( int i = 0; i < 4; ++i ) {
    createGroupItem( m_groupOffset, -m_groupOffset + i * m_groupSpacing );
  }
    
  m_upButton = new ButtonItem( this );
  m_upButton->setPos( 151, -21 );
  m_upButton->hide();
  connect( m_upButton, SIGNAL( clicked() ), SLOT( nextGroup() ) );
  m_upButton->setNext();
  
  m_downButton = new ButtonItem( this );
  m_downButton->setPos( 21, -151 );
  m_downButton->hide();
  m_downButton->setMinus();
  connect( m_downButton, SIGNAL( clicked() ), SLOT( previousGroup() ) );
  m_downButton->setPrevious();
}

void GroupAdderItem::createGroupItem( int x, int y )
{  
  DropTargetItem *groupItem = new DropTargetItem( this, m_model,
    m_model->rootGroup() );
  groupItem->setPos( x, y );
  groupItem->hide();
  m_groupItems.append( groupItem );
}

void GroupAdderItem::setItemSize( int size )
{
  setRect( -size/2, -size/2, size, size );
  m_sidebarBackground->setRect( 0, -2000, size/2, 2000 );
}

void GroupAdderItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  showAsSidebar( !shownAsSidebar() );
}

void GroupAdderItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}

void GroupAdderItem::expand()
{
  m_expanded = !m_expanded;

  if ( m_expanded ) {
    setItemSize( 400 );
    m_expandButton->setMinus();
    foreach( DropTargetItem *item, m_groupItems ) {
      item->show();
    }
    m_upButton->show();
    m_downButton->show();
  } else {
    setItemSize( 100 );
    m_expandButton->setPlus();
    foreach( DropTargetItem *item, m_groupItems ) {
      item->hide();
    }
    m_upButton->hide();
    m_downButton->hide();
  }
}

bool GroupAdderItem::isExpanded() const
{
  return m_expanded;
}

void GroupAdderItem::expandGroupItems()
{
  m_sidebarBackground->show();

  if ( m_collapseGroupsAnimation ) {
    m_collapseGroupsAnimation->stop();
  }
  
  if ( !m_expandGroupsAnimation ) {
    m_expandGroupsAnimation = new QParallelAnimationGroup( this );
  }
  m_expandGroupsAnimation->clear();

  for( int i = 0; i < m_groupItems.size(); ++i ) {
    QPropertyAnimation *animation =
      new QPropertyAnimation(m_groupItems[i], "pos", this);

    animation->setDuration( 300 );
    animation->setEndValue( QPointF( m_groupOffset,
      -m_groupOffset - ( m_groupItems.size() - 1 - i ) * m_groupSpacing ) );
    animation->setEasingCurve( QEasingCurve::OutCubic );

    m_expandGroupsAnimation->insertAnimation( 0, animation );
  }
  
  m_expandGroupsAnimation->start();
}

void GroupAdderItem::collapseGroupItems()
{
  m_sidebarBackground->hide();

  if ( m_expandGroupsAnimation ) {
    m_expandGroupsAnimation->stop();
  }
  
  if ( !m_collapseGroupsAnimation ) {
    m_collapseGroupsAnimation = new QParallelAnimationGroup( this );
  }
  m_collapseGroupsAnimation->clear();

  for( int i = 0; i < m_groupItems.size(); ++i ) {
    QPropertyAnimation *animation =
      new QPropertyAnimation( m_groupItems[i], "pos", this );
    m_collapseGroupsAnimation->insertAnimation( 0, animation );

    animation->setDuration( 300 );
    animation->setEndValue( QPointF( m_groupOffset,
                                     -m_groupOffset + i * m_groupSpacing ) );
    animation->setEasingCurve( QEasingCurve::OutCubic );
  }
  
  m_collapseGroupsAnimation->start();
}

void GroupAdderItem::setGroup( const Bliss::Todo &group )
{
  Bliss::Todo::List list = m_model->groups();
  Bliss::Todo::List::ConstIterator it;
  for( it = list.constBegin(); it != list.constEnd(); ++it ) {
    if ( (*it).id() == group.id() ) {
      break;
    }
  }
  
  for( int i = 0; i < m_groupItems.size(); ++i ) {
    if ( it == list.constEnd() ) {
      it = list.constBegin();
    }

    m_groupItems[i]->updateItem( *it );

    ++it;
  }
}

Bliss::Todo GroupAdderItem::collidedGroup( QGraphicsItem *dropItem )
{
  if ( shownAsSidebar() ) {
    foreach( DropTargetItem *item, m_groupItems ) {
      if ( dropItem->collidesWithItem( item ) ) {
        qDebug() << "DROPPED ON" << item->todo().summary().value();
        return item->todo();
      }
    }
  } else {
    if ( dropItem->collidesWithItem( this ) ) {
      qDebug() << "DROPPED ON COLLAPSED" <<
        m_groupItems.first()->todo().summary().value();
      return m_groupItems.first()->todo();
    }
  }
  
  return Bliss::Todo();
}

void GroupAdderItem::nextGroup()
{
  Bliss::Todo::List list = m_model->groups();
  Bliss::Todo::List::ConstIterator it;
  for( it = list.constBegin(); it != list.constEnd(); ++it ) {
    if ( (*it).id() == m_groupItems.first()->todo().id() ) break;
  }
  ++it;
  if ( it == list.constEnd() ) {
    setGroup( list.first() );
  } else {
    setGroup( *it );
  }
}

void GroupAdderItem::previousGroup()
{
  Bliss::Todo::List list = m_model->groups();
  Bliss::Todo::List::ConstIterator it;
  for( it = list.constBegin(); it != list.constEnd(); ++it ) {
    if ( (*it).id() == m_groupItems.first()->todo().id() ) break;
  }
  if ( it == list.constBegin() ) {
    setGroup( list.last() );
  } else {
    --it;
    setGroup( *it );
  }
}

bool GroupAdderItem::shownAsSidebar() const
{
  return m_sidebarBackground->isVisible();
}

void GroupAdderItem::showAsSidebar( bool show )
{
  if ( show ) {
    expandGroupItems();
  } else {
    collapseGroupItems();
  }
}
