#include "settings.h"

Settings::Settings()
{
}

Settings *Settings::self()
{
  return new Settings;
}

void Settings::readConfig()
{

}

void Settings::writeConfig()
{

}

bool Settings::remoteSyncingEnabled()
{
  return false;
}

void Settings::setRemoteSyncingEnabled(bool)
{

}

QStringList Settings::history()
{
  return QStringList();
}

void Settings::setHistory(const QStringList &)
{

}

QStringList Settings::viewPositions()
{
  return QStringList();
}

void Settings::setViewPositions(const QStringList &)
{

}

bool Settings::fancyMode()
{
  return true;
}

void Settings::setFancyMode(bool)
{

}

bool Settings::groupAdderExpanded()
{
  return false;
}

void Settings::setGroupAdderExpanded(bool)
{

}

bool Settings::groupAdderGroupsExpanded()
{
  return false;
}

void Settings::setGroupAdderGroupsExpanded(bool)
{

}

QString Settings::adderGroup()
{
  return QString();
}

void Settings::setAdderGroup(const QString &)
{

}

bool Settings::enableMagic()
{
  return false;
}

void Settings::setEnableMagic(bool)
{

}
