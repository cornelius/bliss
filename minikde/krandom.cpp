#include <KRandom>

#include <QUuid>

QString KRandom::randomString(int)
{
  QString uuidStr = QUuid::createUuid().toString();
  return uuidStr.mid(1,uuidStr.length()-2);
}
