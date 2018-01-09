// This file is generated by kxml_compiler from bliss.xml.
// All changes you do to this file will be lost.
/*
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

#include "bliss.h"

#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QtCore/QtDebug>
#include <QtCore/QFile>

namespace Bliss {

void TodoId::setValue( const QString &v )
{
  mValue = v;
}

QString TodoId::value() const
{
  return mValue;
}

TodoId TodoId::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "todo_id" ) {
    qCritical() << "Expected 'todo_id', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return TodoId();
  }

  TodoId result = TodoId();

  result.setValue( element.text() );

  if ( ok ) *ok = true;
  return result;
}

void TodoId::writeElement( QXmlStreamWriter &xml )
{
  if ( !value().isEmpty() ) {
    xml.writeStartElement( "todo_id" );
    xml.writeCharacters( value() );
    xml.writeEndElement();
  }
}


void TodoSequence::addTodoId( const TodoId &v )
{
  mTodoIdList.append( v );
}

void TodoSequence::setTodoIdList( const TodoId::List &v )
{
  mTodoIdList = v;
}

TodoId::List TodoSequence::todoIdList() const
{
  return mTodoIdList;
}

TodoSequence TodoSequence::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "todo_sequence" ) {
    qCritical() << "Expected 'todo_sequence', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return TodoSequence();
  }

  TodoSequence result = TodoSequence();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "todo_id" ) {
      bool ok;
      TodoId o = TodoId::parseElement( e, &ok );
      if ( ok ) result.addTodoId( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void TodoSequence::writeElement( QXmlStreamWriter &xml )
{
  if ( !todoIdList().isEmpty() ) {
    xml.writeStartElement( "todo_sequence" );
    foreach( TodoId e, todoIdList() ) {
      e.writeElement( xml );
    }
    xml.writeEndElement();
  }
}


bool ViewList::isValid() const
{
  return !mId.isEmpty();
}

void ViewList::setId( const QString &v )
{
  mId = v;
}

QString ViewList::id() const
{
  return mId;
}

void ViewList::setName( const QString &v )
{
  mName = v;
}

QString ViewList::name() const
{
  return mName;
}

void ViewList::setX( int v )
{
  mX = v;
}

int ViewList::x() const
{
  return mX;
}

void ViewList::setY( int v )
{
  mY = v;
}

int ViewList::y() const
{
  return mY;
}

void ViewList::setTodoSequence( const TodoSequence &v )
{
  mTodoSequence = v;
}

TodoSequence ViewList::todoSequence() const
{
  return mTodoSequence;
}

ViewList ViewList::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "view_list" ) {
    qCritical() << "Expected 'view_list', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return ViewList();
  }

  ViewList result = ViewList();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "name" ) {
      result.setName( e.text() );
    }
    else if ( e.tagName() == "x" ) {
      result.setX( e.text().toInt() );
    }
    else if ( e.tagName() == "y" ) {
      result.setY( e.text().toInt() );
    }
    else if ( e.tagName() == "todo_sequence" ) {
      bool ok;
      TodoSequence o = TodoSequence::parseElement( e, &ok );
      if ( ok ) result.setTodoSequence( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void ViewList::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "view_list" );
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  if ( !name().isEmpty() ) {
    xml.writeTextElement(  "name", name() );
  }
  xml.writeTextElement(  "x", QString::number( x() ) );
  xml.writeTextElement(  "y", QString::number( y() ) );
  todoSequence().writeElement( xml );
  xml.writeEndElement();
}


bool ViewLabel::isValid() const
{
  return !mId.isEmpty();
}

void ViewLabel::setId( const QString &v )
{
  mId = v;
}

QString ViewLabel::id() const
{
  return mId;
}

void ViewLabel::setText( const QString &v )
{
  mText = v;
}

QString ViewLabel::text() const
{
  return mText;
}

void ViewLabel::setX( int v )
{
  mX = v;
}

int ViewLabel::x() const
{
  return mX;
}

void ViewLabel::setY( int v )
{
  mY = v;
}

int ViewLabel::y() const
{
  return mY;
}

ViewLabel ViewLabel::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "view_label" ) {
    qCritical() << "Expected 'view_label', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return ViewLabel();
  }

  ViewLabel result = ViewLabel();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "text" ) {
      result.setText( e.text() );
    }
    else if ( e.tagName() == "x" ) {
      result.setX( e.text().toInt() );
    }
    else if ( e.tagName() == "y" ) {
      result.setY( e.text().toInt() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void ViewLabel::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "view_label" );
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  if ( !text().isEmpty() ) {
    xml.writeTextElement(  "text", text() );
  }
  xml.writeTextElement(  "x", QString::number( x() ) );
  xml.writeTextElement(  "y", QString::number( y() ) );
  xml.writeEndElement();
}


bool TodoPosition::isValid() const
{
  return !mId.isEmpty();
}

void TodoPosition::setId( const QString &v )
{
  mId = v;
}

QString TodoPosition::id() const
{
  return mId;
}

void TodoPosition::setX( int v )
{
  mX = v;
}

int TodoPosition::x() const
{
  return mX;
}

void TodoPosition::setY( int v )
{
  mY = v;
}

int TodoPosition::y() const
{
  return mY;
}

TodoPosition TodoPosition::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "todo_position" ) {
    qCritical() << "Expected 'todo_position', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return TodoPosition();
  }

  TodoPosition result = TodoPosition();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "x" ) {
      result.setX( e.text().toInt() );
    }
    else if ( e.tagName() == "y" ) {
      result.setY( e.text().toInt() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void TodoPosition::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "todo_position" );
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  xml.writeTextElement(  "x", QString::number( x() ) );
  xml.writeTextElement(  "y", QString::number( y() ) );
  xml.writeEndElement();
}


bool GroupView::isValid() const
{
  return !mId.isEmpty();
}

void GroupView::setId( const QString &v )
{
  mId = v;
}

QString GroupView::id() const
{
  return mId;
}

void GroupView::addTodoPosition( const TodoPosition &v )
{
  mTodoPositionList.append( v );
}

void GroupView::setTodoPositionList( const TodoPosition::List &v )
{
  mTodoPositionList = v;
}

TodoPosition::List GroupView::todoPositionList() const
{
  return mTodoPositionList;
}

TodoPosition GroupView::findTodoPosition( const QString &id, Flags flags )
{
  foreach( TodoPosition v, mTodoPositionList ) {
    if ( v.id() == id ) return v;
  }
  TodoPosition v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool GroupView::insert( const TodoPosition &v )
{
  int i = 0;
  for( ; i < mTodoPositionList.size(); ++i ) {
    if ( mTodoPositionList[i].id() == v.id() ) {
      mTodoPositionList[i] = v;
      return true;
    }
  }
  if ( i == mTodoPositionList.size() ) {
    addTodoPosition( v );
  }
  return true;
}

bool GroupView::remove( const TodoPosition &v )
{
  TodoPosition::List::Iterator it;
  for( it = mTodoPositionList.begin(); it != mTodoPositionList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mTodoPositionList.end() ) {
    mTodoPositionList.erase( it );
  }
  return true;
}

void GroupView::addViewLabel( const ViewLabel &v )
{
  mViewLabelList.append( v );
}

void GroupView::setViewLabelList( const ViewLabel::List &v )
{
  mViewLabelList = v;
}

ViewLabel::List GroupView::viewLabelList() const
{
  return mViewLabelList;
}

ViewLabel GroupView::findViewLabel( const QString &id, Flags flags )
{
  foreach( ViewLabel v, mViewLabelList ) {
    if ( v.id() == id ) return v;
  }
  ViewLabel v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool GroupView::insert( const ViewLabel &v )
{
  int i = 0;
  for( ; i < mViewLabelList.size(); ++i ) {
    if ( mViewLabelList[i].id() == v.id() ) {
      mViewLabelList[i] = v;
      return true;
    }
  }
  if ( i == mViewLabelList.size() ) {
    addViewLabel( v );
  }
  return true;
}

bool GroupView::remove( const ViewLabel &v )
{
  ViewLabel::List::Iterator it;
  for( it = mViewLabelList.begin(); it != mViewLabelList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mViewLabelList.end() ) {
    mViewLabelList.erase( it );
  }
  return true;
}

void GroupView::setTodoSequence( const TodoSequence &v )
{
  mTodoSequence = v;
}

TodoSequence GroupView::todoSequence() const
{
  return mTodoSequence;
}

void GroupView::addViewList( const ViewList &v )
{
  mViewListList.append( v );
}

void GroupView::setViewListList( const ViewList::List &v )
{
  mViewListList = v;
}

ViewList::List GroupView::viewListList() const
{
  return mViewListList;
}

ViewList GroupView::findViewList( const QString &id, Flags flags )
{
  foreach( ViewList v, mViewListList ) {
    if ( v.id() == id ) return v;
  }
  ViewList v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool GroupView::insert( const ViewList &v )
{
  int i = 0;
  for( ; i < mViewListList.size(); ++i ) {
    if ( mViewListList[i].id() == v.id() ) {
      mViewListList[i] = v;
      return true;
    }
  }
  if ( i == mViewListList.size() ) {
    addViewList( v );
  }
  return true;
}

bool GroupView::remove( const ViewList &v )
{
  ViewList::List::Iterator it;
  for( it = mViewListList.begin(); it != mViewListList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mViewListList.end() ) {
    mViewListList.erase( it );
  }
  return true;
}

GroupView GroupView::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "group_view" ) {
    qCritical() << "Expected 'group_view', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return GroupView();
  }

  GroupView result = GroupView();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "todo_position" ) {
      bool ok;
      TodoPosition o = TodoPosition::parseElement( e, &ok );
      if ( ok ) result.addTodoPosition( o );
    }
    else if ( e.tagName() == "view_label" ) {
      bool ok;
      ViewLabel o = ViewLabel::parseElement( e, &ok );
      if ( ok ) result.addViewLabel( o );
    }
    else if ( e.tagName() == "todo_sequence" ) {
      bool ok;
      TodoSequence o = TodoSequence::parseElement( e, &ok );
      if ( ok ) result.setTodoSequence( o );
    }
    else if ( e.tagName() == "view_list" ) {
      bool ok;
      ViewList o = ViewList::parseElement( e, &ok );
      if ( ok ) result.addViewList( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void GroupView::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "group_view" );
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  foreach( TodoPosition e, todoPositionList() ) {
    e.writeElement( xml );
  }
  foreach( ViewLabel e, viewLabelList() ) {
    e.writeElement( xml );
  }
  todoSequence().writeElement( xml );
  foreach( ViewList e, viewListList() ) {
    e.writeElement( xml );
  }
  xml.writeEndElement();
}


bool Group::isValid() const
{
  return !mId.isEmpty();
}

void Group::setId( const QString &v )
{
  mId = v;
}

QString Group::id() const
{
  return mId;
}

Group Group::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "group" ) {
    qCritical() << "Expected 'group', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Group();
  }

  Group result = Group();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void Group::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "group" );
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  xml.writeEndElement();
}


void Groups::addGroup( const Group &v )
{
  mGroupList.append( v );
}

void Groups::setGroupList( const Group::List &v )
{
  mGroupList = v;
}

Group::List Groups::groupList() const
{
  return mGroupList;
}

Group Groups::findGroup( const QString &id, Flags flags )
{
  foreach( Group v, mGroupList ) {
    if ( v.id() == id ) return v;
  }
  Group v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool Groups::insert( const Group &v )
{
  int i = 0;
  for( ; i < mGroupList.size(); ++i ) {
    if ( mGroupList[i].id() == v.id() ) {
      mGroupList[i] = v;
      return true;
    }
  }
  if ( i == mGroupList.size() ) {
    addGroup( v );
  }
  return true;
}

bool Groups::remove( const Group &v )
{
  Group::List::Iterator it;
  for( it = mGroupList.begin(); it != mGroupList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mGroupList.end() ) {
    mGroupList.erase( it );
  }
  return true;
}

Groups Groups::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "groups" ) {
    qCritical() << "Expected 'groups', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Groups();
  }

  Groups result = Groups();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "group" ) {
      bool ok;
      Group o = Group::parseElement( e, &ok );
      if ( ok ) result.addGroup( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void Groups::writeElement( QXmlStreamWriter &xml )
{
  if ( !groupList().isEmpty() ) {
    xml.writeStartElement( "groups" );
    foreach( Group e, groupList() ) {
      e.writeElement( xml );
    }
    xml.writeEndElement();
  }
}


Title::Title()
{
  QDateTime now = QDateTime::currentDateTime();
  setCreatedAt( now );
  setUpdatedAt( now );
}

void Title::setCreatedAt( const QDateTime &v )
{
  mCreatedAt = v;
}

QDateTime Title::createdAt() const
{
  return mCreatedAt;
}

void Title::setUpdatedAt( const QDateTime &v )
{
  mUpdatedAt = v;
}

QDateTime Title::updatedAt() const
{
  return mUpdatedAt;
}

void Title::setValue( const QString &v )
{
  mValue = v;
  setUpdatedAt( QDateTime::currentDateTime() );
}

QString Title::value() const
{
  return mValue;
}

Title Title::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "title" ) {
    qCritical() << "Expected 'title', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Title();
  }

  Title result = Title();

  result.setValue( element.text() );
  result.setCreatedAt( QDateTime::fromString( element.attribute( "created_at" ), "yyyyMMddThhmmssZ" ) );
  result.setUpdatedAt( QDateTime::fromString( element.attribute( "updated_at" ), "yyyyMMddThhmmssZ" ) );

  if ( ok ) *ok = true;
  return result;
}

void Title::writeElement( QXmlStreamWriter &xml )
{
  if ( !value().isEmpty() ) {
    xml.writeStartElement( "title" );
    if ( !createdAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "created_at", createdAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    if ( !updatedAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "updated_at", updatedAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    xml.writeCharacters( value() );
    xml.writeEndElement();
  }
}


Summary::Summary()
{
  QDateTime now = QDateTime::currentDateTime();
  setCreatedAt( now );
  setUpdatedAt( now );
}

void Summary::setCreatedAt( const QDateTime &v )
{
  mCreatedAt = v;
}

QDateTime Summary::createdAt() const
{
  return mCreatedAt;
}

void Summary::setUpdatedAt( const QDateTime &v )
{
  mUpdatedAt = v;
}

QDateTime Summary::updatedAt() const
{
  return mUpdatedAt;
}

void Summary::setValue( const QString &v )
{
  mValue = v;
  setUpdatedAt( QDateTime::currentDateTime() );
}

QString Summary::value() const
{
  return mValue;
}

Summary Summary::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "summary" ) {
    qCritical() << "Expected 'summary', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Summary();
  }

  Summary result = Summary();

  result.setValue( element.text() );
  result.setCreatedAt( QDateTime::fromString( element.attribute( "created_at" ), "yyyyMMddThhmmssZ" ) );
  result.setUpdatedAt( QDateTime::fromString( element.attribute( "updated_at" ), "yyyyMMddThhmmssZ" ) );

  if ( ok ) *ok = true;
  return result;
}

void Summary::writeElement( QXmlStreamWriter &xml )
{
  if ( !value().isEmpty() ) {
    xml.writeStartElement( "summary" );
    if ( !createdAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "created_at", createdAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    if ( !updatedAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "updated_at", updatedAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    xml.writeCharacters( value() );
    xml.writeEndElement();
  }
}


Postpone::Postpone()
{
  QDateTime now = QDateTime::currentDateTime();
  setCreatedAt( now );
  setUpdatedAt( now );
}

void Postpone::setCreatedAt( const QDateTime &v )
{
  mCreatedAt = v;
}

QDateTime Postpone::createdAt() const
{
  return mCreatedAt;
}

void Postpone::setUpdatedAt( const QDateTime &v )
{
  mUpdatedAt = v;
}

QDateTime Postpone::updatedAt() const
{
  return mUpdatedAt;
}

void Postpone::setValue( const QString &v )
{
  mValue = v;
  setUpdatedAt( QDateTime::currentDateTime() );
}

QString Postpone::value() const
{
  return mValue;
}

Postpone Postpone::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "postpone" ) {
    qCritical() << "Expected 'postpone', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Postpone();
  }

  Postpone result = Postpone();

  result.setValue( element.text() );
  result.setCreatedAt( QDateTime::fromString( element.attribute( "created_at" ), "yyyyMMddThhmmssZ" ) );
  result.setUpdatedAt( QDateTime::fromString( element.attribute( "updated_at" ), "yyyyMMddThhmmssZ" ) );

  if ( ok ) *ok = true;
  return result;
}

void Postpone::writeElement( QXmlStreamWriter &xml )
{
  if ( !value().isEmpty() ) {
    xml.writeStartElement( "postpone" );
    if ( !createdAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "created_at", createdAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    if ( !updatedAt().toString( "yyyyMMddThhmmssZ" ).isEmpty() ) {
      xml.writeAttribute( "updated_at", updatedAt().toString( "yyyyMMddThhmmssZ" ) );
    }
    xml.writeCharacters( value() );
    xml.writeEndElement();
  }
}


bool Todo::isValid() const
{
  return !mId.isEmpty();
}

void Todo::setType( const QString &v )
{
  mType = v;
}

QString Todo::type() const
{
  return mType;
}

void Todo::setId( const QString &v )
{
  mId = v;
}

QString Todo::id() const
{
  return mId;
}

void Todo::setGroups( const Groups &v )
{
  mGroups = v;
}

Groups Todo::groups() const
{
  return mGroups;
}

void Todo::setTitle( const Title &v )
{
  mTitle = v;
}

Title Todo::title() const
{
  return mTitle;
}

void Todo::setSummary( const Summary &v )
{
  mSummary = v;
}

Summary Todo::summary() const
{
  return mSummary;
}

void Todo::setPostpone( const Postpone &v )
{
  mPostpone = v;
}

Postpone Todo::postpone() const
{
  return mPostpone;
}

Todo Todo::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "todo" ) {
    qCritical() << "Expected 'todo', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Todo();
  }

  Todo result = Todo();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "groups" ) {
      bool ok;
      Groups o = Groups::parseElement( e, &ok );
      if ( ok ) result.setGroups( o );
    }
    else if ( e.tagName() == "title" ) {
      bool ok;
      Title o = Title::parseElement( e, &ok );
      if ( ok ) result.setTitle( o );
    }
    else if ( e.tagName() == "summary" ) {
      bool ok;
      Summary o = Summary::parseElement( e, &ok );
      if ( ok ) result.setSummary( o );
    }
    else if ( e.tagName() == "postpone" ) {
      bool ok;
      Postpone o = Postpone::parseElement( e, &ok );
      if ( ok ) result.setPostpone( o );
    }
  }

  result.setType( element.attribute( "type" ) );

  if ( ok ) *ok = true;
  return result;
}

void Todo::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "todo" );
    if ( !type().isEmpty() ) {
      xml.writeAttribute( "type", type() );
    }
  if ( !id().isEmpty() ) {
    xml.writeTextElement(  "id", id() );
  }
  groups().writeElement( xml );
  title().writeElement( xml );
  summary().writeElement( xml );
  postpone().writeElement( xml );
  xml.writeEndElement();
}


void Root::setGroup( const Group &v )
{
  mGroup = v;
}

Group Root::group() const
{
  return mGroup;
}

Root Root::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "root" ) {
    qCritical() << "Expected 'root', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Root();
  }

  Root result = Root();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "group" ) {
      bool ok;
      Group o = Group::parseElement( e, &ok );
      if ( ok ) result.setGroup( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

void Root::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "root" );
  group().writeElement( xml );
  xml.writeEndElement();
}


void Bliss::setSchemaVersion( int v )
{
  mSchemaVersion = v;
}

int Bliss::schemaVersion() const
{
  return mSchemaVersion;
}

void Bliss::setRoot( const Root &v )
{
  mRoot = v;
}

Root Bliss::root() const
{
  return mRoot;
}

void Bliss::addTodo( const Todo &v )
{
  mTodoList.append( v );
}

void Bliss::setTodoList( const Todo::List &v )
{
  mTodoList = v;
}

Todo::List Bliss::todoList() const
{
  return mTodoList;
}

Todo Bliss::findTodo( const QString &id, Flags flags )
{
  foreach( Todo v, mTodoList ) {
    if ( v.id() == id ) return v;
  }
  Todo v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool Bliss::insert( const Todo &v )
{
  int i = 0;
  for( ; i < mTodoList.size(); ++i ) {
    if ( mTodoList[i].id() == v.id() ) {
      mTodoList[i] = v;
      return true;
    }
  }
  if ( i == mTodoList.size() ) {
    addTodo( v );
  }
  return true;
}

bool Bliss::remove( const Todo &v )
{
  Todo::List::Iterator it;
  for( it = mTodoList.begin(); it != mTodoList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mTodoList.end() ) {
    mTodoList.erase( it );
  }
  return true;
}

void Bliss::addGroupView( const GroupView &v )
{
  mGroupViewList.append( v );
}

void Bliss::setGroupViewList( const GroupView::List &v )
{
  mGroupViewList = v;
}

GroupView::List Bliss::groupViewList() const
{
  return mGroupViewList;
}

GroupView Bliss::findGroupView( const QString &id, Flags flags )
{
  foreach( GroupView v, mGroupViewList ) {
    if ( v.id() == id ) return v;
  }
  GroupView v;
  if ( flags == AutoCreate ) {
    v.setId( id );
  }
  return v;
}

bool Bliss::insert( const GroupView &v )
{
  int i = 0;
  for( ; i < mGroupViewList.size(); ++i ) {
    if ( mGroupViewList[i].id() == v.id() ) {
      mGroupViewList[i] = v;
      return true;
    }
  }
  if ( i == mGroupViewList.size() ) {
    addGroupView( v );
  }
  return true;
}

bool Bliss::remove( const GroupView &v )
{
  GroupView::List::Iterator it;
  for( it = mGroupViewList.begin(); it != mGroupViewList.end(); ++it ) {
    if ( (*it).id() == v.id() ) break;
  }
  if ( it != mGroupViewList.end() ) {
    mGroupViewList.erase( it );
  }
  return true;
}

Bliss Bliss::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "bliss" ) {
    qCritical() << "Expected 'bliss', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Bliss();
  }

  Bliss result = Bliss();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "root" ) {
      bool ok;
      Root o = Root::parseElement( e, &ok );
      if ( ok ) result.setRoot( o );
    }
    else if ( e.tagName() == "todo" ) {
      bool ok;
      Todo o = Todo::parseElement( e, &ok );
      if ( ok ) result.addTodo( o );
    }
    else if ( e.tagName() == "group_view" ) {
      bool ok;
      GroupView o = GroupView::parseElement( e, &ok );
      if ( ok ) result.addGroupView( o );
    }
  }

  result.setSchemaVersion( element.attribute( "schemaVersion" ).toInt() );

  if ( ok ) *ok = true;
  return result;
}

void Bliss::writeElement( QXmlStreamWriter &xml )
{
  xml.writeStartElement( "bliss" );
    if ( !QString::number( schemaVersion() ).isEmpty() ) {
      xml.writeAttribute( "schemaVersion", QString::number( schemaVersion() ) );
    }
  root().writeElement( xml );
  foreach( Todo e, todoList() ) {
    e.writeElement( xml );
  }
  foreach( GroupView e, groupViewList() ) {
    e.writeElement( xml );
  }
  xml.writeEndElement();
}

Bliss Bliss::parseFile( const QString &filename, bool *ok )
{
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    if ( ok ) *ok = false;
    return Bliss();
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {
    qCritical() << errorMsg << " at " << errorLine << "," << errorCol;
    if ( ok ) *ok = false;
    return Bliss();
  }

  bool documentOk;
  Bliss c = parseElement( doc.documentElement(), &documentOk );
  if ( ok ) {
    *ok = documentOk;
  }
  return c;
}

Bliss Bliss::parseString( const QString &xml, bool *ok )
{
  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( xml, false, &errorMsg, &errorLine, &errorCol ) ) {
    qCritical() << errorMsg << " at " << errorLine << "," << errorCol;
    if ( ok ) *ok = false;
    return Bliss();
  }

  bool documentOk;
  Bliss c = parseElement( doc.documentElement(), &documentOk );
  if ( ok ) {
    *ok = documentOk;
  }
  return c;
}

bool Bliss::writeFile( const QString &filename )
{
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    return false;
  }

  QXmlStreamWriter xml( &file );
  xml.setAutoFormatting( true );
  xml.setAutoFormattingIndent( 2 );
  xml.writeStartDocument( "1.0" );
  writeElement( xml );
  xml.writeEndDocument();
  file.close();

  return true;
}


}
