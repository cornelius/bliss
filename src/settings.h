#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include <QString>

class Settings {
  public:
    static Settings *self();

    void readConfig();
    void writeConfig();

    static bool remoteSyncingEnabled();
    static void setRemoteSyncingEnabled(bool);

    static QStringList history();
    static void setHistory(const QStringList &);

    static QStringList viewPositions();
    static void setViewPositions(const QStringList &);

    static bool fancyMode();
    static void setFancyMode(bool);

    static bool groupAdderExpanded();
    static void setGroupAdderExpanded(bool);

    static bool groupAdderGroupsExpanded();
    static void setGroupAdderGroupsExpanded(bool);

    static QString adderGroup();
    static void setAdderGroup(const QString &);

    static bool enableMagic();
    static void setEnableMagic(bool);

  private:
    Settings();
};

#endif
