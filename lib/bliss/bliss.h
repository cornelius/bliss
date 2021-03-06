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
#ifndef BLISS_BLISS_H
#define BLISS_BLISS_H

#include <bliss/bliss_export.h>
#include <QString>
#include <QDomElement>
#include <QList>
#include <QDateTime>
#include <QXmlStreamWriter>

namespace Bliss {

class BLISS_EXPORT TodoId
{
  public:
    typedef QList<TodoId> List;

  public:
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static TodoId parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mValue;
};

class BLISS_EXPORT TodoSequence
{
  public:
    void addTodoId( const TodoId &v );
    void setTodoIdList( const TodoId::List &v );
    TodoId::List todoIdList() const;
    /**
      Parse XML object from DOM element.
     */
    static TodoSequence parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    TodoId::List mTodoIdList;
};

class BLISS_EXPORT ViewList
{
  public:
    typedef QList<ViewList> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setName( const QString &v );
    QString name() const;
    void setX( int v );
    int x() const;
    void setY( int v );
    int y() const;
    void setTodoSequence( const TodoSequence &v );
    TodoSequence todoSequence() const;
    /**
      Parse XML object from DOM element.
     */
    static ViewList parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QString mName;
    int mX;
    int mY;
    TodoSequence mTodoSequence;
};

class BLISS_EXPORT ViewLabel
{
  public:
    typedef QList<ViewLabel> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setText( const QString &v );
    QString text() const;
    void setX( int v );
    int x() const;
    void setY( int v );
    int y() const;
    /**
      Parse XML object from DOM element.
     */
    static ViewLabel parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QString mText;
    int mX;
    int mY;
};

class BLISS_EXPORT TodoPosition
{
  public:
    typedef QList<TodoPosition> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setX( int v );
    int x() const;
    void setY( int v );
    int y() const;
    /**
      Parse XML object from DOM element.
     */
    static TodoPosition parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    int mX;
    int mY;
};

class BLISS_EXPORT GroupView
{
  public:
    typedef QList<GroupView> List;

  public:
    enum Flags { None, AutoCreate };

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void addTodoPosition( const TodoPosition &v );
    void setTodoPositionList( const TodoPosition::List &v );
    TodoPosition::List todoPositionList() const;
    TodoPosition findTodoPosition( const QString &id, Flags flags = None );
    bool insert( const TodoPosition &v );
    bool remove( const TodoPosition &v );
    void addViewLabel( const ViewLabel &v );
    void setViewLabelList( const ViewLabel::List &v );
    ViewLabel::List viewLabelList() const;
    ViewLabel findViewLabel( const QString &id, Flags flags = None );
    bool insert( const ViewLabel &v );
    bool remove( const ViewLabel &v );
    void setTodoSequence( const TodoSequence &v );
    TodoSequence todoSequence() const;
    void addViewList( const ViewList &v );
    void setViewListList( const ViewList::List &v );
    ViewList::List viewListList() const;
    ViewList findViewList( const QString &id, Flags flags = None );
    bool insert( const ViewList &v );
    bool remove( const ViewList &v );
    /**
      Parse XML object from DOM element.
     */
    static GroupView parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    TodoPosition::List mTodoPositionList;
    ViewLabel::List mViewLabelList;
    TodoSequence mTodoSequence;
    ViewList::List mViewListList;
};

class BLISS_EXPORT Group
{
  public:
    typedef QList<Group> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    /**
      Parse XML object from DOM element.
     */
    static Group parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
};

class BLISS_EXPORT Groups
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addGroup( const Group &v );
    void setGroupList( const Group::List &v );
    Group::List groupList() const;
    Group findGroup( const QString &id, Flags flags = None );
    bool insert( const Group &v );
    bool remove( const Group &v );
    /**
      Parse XML object from DOM element.
     */
    static Groups parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Group::List mGroupList;
};

class BLISS_EXPORT Title
{
  public:
    Title();
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static Title parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mValue;
};

class BLISS_EXPORT Summary
{
  public:
    Summary();
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static Summary parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mValue;
};

class BLISS_EXPORT Postpone
{
  public:
    Postpone();
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static Postpone parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mValue;
};

class BLISS_EXPORT Todo
{
  public:
    typedef QList<Todo> List;

  public:
    bool isValid() const;
    void setType( const QString &v );
    QString type() const;
    void setId( const QString &v );
    QString id() const;
    void setGroups( const Groups &v );
    Groups groups() const;
    void setTitle( const Title &v );
    Title title() const;
    void setSummary( const Summary &v );
    Summary summary() const;
    void setPostpone( const Postpone &v );
    Postpone postpone() const;
    /**
      Parse XML object from DOM element.
     */
    static Todo parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mType;
    QString mId;
    Groups mGroups;
    Title mTitle;
    Summary mSummary;
    Postpone mPostpone;
};

class BLISS_EXPORT Root
{
  public:
    void setGroup( const Group &v );
    Group group() const;
    /**
      Parse XML object from DOM element.
     */
    static Root parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Group mGroup;
};

class BLISS_EXPORT Bliss
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void setSchemaVersion( int v );
    int schemaVersion() const;
    void setRoot( const Root &v );
    Root root() const;
    void addTodo( const Todo &v );
    void setTodoList( const Todo::List &v );
    Todo::List todoList() const;
    Todo findTodo( const QString &id, Flags flags = None );
    bool insert( const Todo &v );
    bool remove( const Todo &v );
    void addGroupView( const GroupView &v );
    void setGroupViewList( const GroupView::List &v );
    GroupView::List groupViewList() const;
    GroupView findGroupView( const QString &id, Flags flags = None );
    bool insert( const GroupView &v );
    bool remove( const GroupView &v );
    /**
      Parse XML object from DOM element.
     */
    static Bliss parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );
    static Bliss parseFile( const QString &filename, bool *ok );
    static Bliss parseString( const QString &xml, bool *ok );
    bool writeFile( const QString &filename );

  private:
    int mSchemaVersion;
    Root mRoot;
    Todo::List mTodoList;
    GroupView::List mGroupViewList;
};

}

#endif
