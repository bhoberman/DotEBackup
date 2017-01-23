#include "dotebackup.h"
#include <iostream>
#include <QSysInfo>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QDebug>

DotEBackup::DotEBackup(QObject *parent) : QObject(parent)
{
    #if (defined(Q_OS_WIN) || defined(Q_OS_WIN32) || defined(Q_OS_WIN64))
        QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QDir userFolder(path);
        if (userFolder.cd("Dungeon of the Endless")) {//True if exists
            if (userFolder.cd("Users")) {
                qDebug() << "Successfully found DotE Users folder";
                doteFolder = userFolder;
                doteFolder.cdUp();
            } else {
                qDebug() << "Can't find User folder. Maybe something's wrong?";
            }
        } else {
            qDebug() << "Can't find DotE folder. Perhaps you don't have it installed?";
        }
    #elif (defined(Q_OS_MAC))
        QString path = "~/Library/Application Support";
        QDir userFolder(path);
        if (userFolder.cd("Dungeon of the Endless")) {//True if exists
            if (userFolder.cd("Users")) {
                qDebug() << "Successfully found DotE Users folder";
                doteFolder = userFolder;
                doteFolder.cdUp();
            } else {
                qDebug() << "Can't find User folder. Maybe something's wrong?";
            }
        } else {
            qDebug() << "Can't find DotE folder. Perhaps you don't have it installed?";
        }
    #else
        qDebug() << "What an error! Turns out that your OS doesn't support DotE.";
        assert(false);
    #endif

}

DotEBackup::~DotEBackup() {

}

void DotEBackup::makeBackup() {
    qDebug() << "Making Backup";

    //Clear out any old backups if they exist
    QDir usersBackup = doteFolder;
    if (usersBackup.cd(backupName)) {//Don't accidentally delete the entire DotE folder if a backup doesn't already exist
        usersBackup.removeRecursively();
    }

    //Create the exit directory
    usersBackup = doteFolder;
    usersBackup.mkdir(backupName);
    usersBackup.cd(backupName);

    //Make an equivalent strangely named folder in the backup
    QDir usersOriginal = doteFolder;
    if (usersOriginal.cd("Users")) {//For safety's sake I guess
        QString strangeName = usersOriginal.entryList()[2];//It's the only real folder in the folder
        usersBackup.mkdir(strangeName);
        if (usersBackup.cd(strangeName) && usersOriginal.cd(strangeName)) {
            //Both exist, copy all sav files
            QDirIterator it(usersOriginal.absolutePath(), QDirIterator::NoIteratorFlags);
            it.next();//. is consigned to the void
            it.next();//.. is vonsigned to the void
            for (int i = 0; it.hasNext(); ++i) {
                QString nextName = it.next();
                QFile thisOne(nextName);
                QString thisName = QString(QFileInfo(thisOne.fileName()).fileName());
                if (!thisOne.copy(usersBackup.absolutePath().append("/").append(thisName))) {
                    qDebug() << "Error copying " << thisOne.fileName();
                }
            }
        }
    }
    qDebug() << "Backup Complete";
}

void DotEBackup::restoreBackup() {
    qDebug() << "Restoring from Backup";

    //Check to make sure there is stuff to restore
    if (!(doteFolder.entryList().contains(QString("Users")) && doteFolder.entryList().contains(QString(backupName)))) {
        qDebug() << "The two folders don't exist. Make sure to make a backup first.";
        return;
    }

    //Make an equivalent strangely named folder in the backup
    QDir usersOriginal = doteFolder;
    QDir usersBackup = doteFolder;

    if (usersOriginal.cd("Users") && usersBackup.cd(backupName)) {//For safety's sake I guess
        QString strangeName = usersOriginal.entryList()[2];//It's the only real folder in the folder
        if (usersBackup.cd(strangeName) && usersOriginal.cd(strangeName)) {
            //Both exist, copy all sav files
            QDirIterator it(usersBackup.absolutePath(), QDirIterator::NoIteratorFlags);
            it.next();//. is consigned to the void
            it.next();//.. is vonsigned to the void
            for (int i = 0; it.hasNext(); ++i) {
                QString nextName = it.next();
                QFile thisOne(nextName);
                QString thisName = QString(QFileInfo(thisOne.fileName()).fileName());
                if (!thisOne.copy(usersOriginal.absolutePath().append("/").append(thisName))) {
                    qDebug() << "Error copying " << thisOne.fileName();
                }
            }
        }
    }
    qDebug() << "Restoration Complete";
}
