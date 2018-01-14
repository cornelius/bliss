#include "settings.h"

#include <QSettings>
#include <QDebug>

Settings *Settings::m_self = 0;

Settings::Settings()
{
}

Settings *Settings::self()
{
  if (!m_self) m_self = new Settings;
  return m_self;
}

void Settings::readConfig()
{
  QSettings settings;

  setRemoteSyncingEnabled(settings.value("remoteSyncingEnabled", false).toBool());
  setHistory(settings.value("history").toStringList());
  setGroupAdderExpanded(settings.value("groupAdderExpanded", false).toBool());
  setGroupAdderGroupsExpanded(settings.value("groupAdderGroupsExpanded", false).toBool());
  setAdderGroup(settings.value("adderGroup").toString());
  setEnableMagic(settings.value("magic", false).toBool());
}

void Settings::writeConfig()
{
  QSettings settings;

  settings.setValue("remoteSyncingEnabled", remoteSyncingEnabled());
  settings.setValue("history", history());
  settings.setValue("groupAdderExpanded", groupAdderExpanded());
  settings.setValue("groupAdderGroupsExpanded", groupAdderGroupsExpanded());
  settings.setValue("adderGroup", adderGroup());
  settings.setValue("magic", enableMagic());
}

bool Settings::remoteSyncingEnabled()
{
  return self()->m_remoteSyncingEnabled;
}

void Settings::setRemoteSyncingEnabled(bool v)
{
  self()->m_remoteSyncingEnabled = v;
}

QStringList Settings::history()
{
  return self()->m_history;
}

void Settings::setHistory(const QStringList &v)
{
  self()->m_history = v;
}

bool Settings::groupAdderExpanded()
{
  return self()->m_groupAdderExpanded;
}

void Settings::setGroupAdderExpanded(bool v)
{
  self()->m_groupAdderExpanded = v;
}

bool Settings::groupAdderGroupsExpanded()
{
  return self()->m_groupAdderGroupsExpanded;
}

void Settings::setGroupAdderGroupsExpanded(bool v)
{
  self()->m_groupAdderGroupsExpanded = v;
}

QString Settings::adderGroup()
{
  return self()->m_adderGroup;
}

void Settings::setAdderGroup(const QString &v)
{
  self()->m_adderGroup = v;
}

bool Settings::enableMagic()
{
  return self()->m_magic;
}

void Settings::setEnableMagic(bool v)
{
  self()->m_magic = v;
}
