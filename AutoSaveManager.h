#ifndef AUTOSAVEMANAGER_H
#define AUTOSAVEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QTabWidget>
#include <QTextEdit>

class AutoSaveManager : public QObject {
    Q_OBJECT
public:
    AutoSaveManager(QTabWidget *tabWidget, QObject *parent = nullptr);
    void start();
    void stop();
    // 程序启动时调用，用于恢复上次自动保存的内容
    void recoverFiles();

private slots:
    void autoSave();

private:
    QTabWidget *m_tabWidget;
    QTimer m_timer;
};

#endif // AUTOSAVEMANAGER_H
