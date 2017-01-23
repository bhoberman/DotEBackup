#ifndef DOTEBACKUP_H
#define DOTEBACKUP_H

#include <QObject>
#include <QDir>

class DotEBackup : public QObject
{
    Q_OBJECT

private:
    //Direction of the DotE folder which contains "Users"
    QDir doteFolder;
    const QString backupName = "Users-Backup";

public:
    explicit DotEBackup(QObject *parent = 0);
    ~DotEBackup();

signals:

public slots:
    void makeBackup();
    void restoreBackup();

};

#endif // DOTEBACKUP_H
