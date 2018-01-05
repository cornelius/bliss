#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
  public:
    static Settings *self() { return new Settings; }

    void readConfig() {};
    void writeConfig() {};

    static bool remoteSyncingEnabled() { return false; };
    static void setRemoteSyncingEnabled(bool) {};

    static QStringList history() { return QStringList(); };
    static void setHistory(const QStringList &) {};

    static QStringList viewPositions() { return QStringList(); };
    static void setViewPositions(const QStringList &) {};
    
    static bool fancyMode() { return true; };
    static void setFancyMode(bool) {};

    static bool groupAdderExpanded() { return false; };
    static void setGroupAdderExpanded(bool) {};

    static bool groupAdderGroupsExpanded() { return false; };
    static void setGroupAdderGroupsExpanded(bool) {};

    static QString adderGroup() { return QString(); };
    static void setAdderGroup(const QString &) {};

    static bool enableMagic() { return false; };
    static void setEnableMagic(bool) {};
};

#endif
