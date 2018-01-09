#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include <QString>

class Settings {
  public:
    static Settings *self();
    static Settings *m_self;

    void readConfig();
    void writeConfig();

    static bool remoteSyncingEnabled();
    static void setRemoteSyncingEnabled(bool);

    static QStringList history();
    static void setHistory(const QStringList &);

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

    bool m_remoteSyncingEnabled;
    QStringList m_history;
    bool m_groupAdderExpanded;
    bool m_groupAdderGroupsExpanded;
    QString m_adderGroup;
    bool m_magic;
};

#endif
