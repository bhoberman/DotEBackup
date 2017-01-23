#include "mainwindow.h"
#include "dotebackup.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DotEBackup backup;

    //Create the menu taskbar icon
    QSystemTrayIcon tray;
    tray.setIcon(QIcon(":/img/img/dote16x16.png"));

    //Setup the context menu for the app
    QMenu menu;
    QAction* makeBackupAction = menu.addAction("Make backup");
    QAction* restoreFromBackup = menu.addAction("Restore from backup");
    QAction* quitAction = menu.addAction("Quit");
    a.connect(makeBackupAction, SIGNAL(triggered()), &backup, SLOT(makeBackup()));
    a.connect(restoreFromBackup, SIGNAL(triggered()), &backup, SLOT(restoreBackup()));
    a.connect(quitAction, SIGNAL(triggered(bool)), &a, SLOT(quit()));

    //Use the menu, show the taskbar icon, and run the app
    tray.setContextMenu(&menu);
    tray.show();
    return a.exec();
}
