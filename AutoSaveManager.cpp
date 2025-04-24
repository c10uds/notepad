#include "AutoSaveManager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

AutoSaveManager::AutoSaveManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent), m_tabWidget(tabWidget) {
    m_timer.setInterval(300000); // 每5分钟自动保存一次
    connect(&m_timer, &QTimer::timeout, this, &AutoSaveManager::autoSave);
}

void AutoSaveManager::start() {
    m_timer.start();
}

void AutoSaveManager::stop() {
    m_timer.stop();
}

void AutoSaveManager::autoSave() {
    int count = m_tabWidget->count();
    QDir tempDir(QDir::tempPath());
    for (int i = 0; i < count; ++i) {
        QWidget *w = m_tabWidget->widget(i);
        QTextEdit *editor = qobject_cast<QTextEdit*>(w);
        if (editor) {
            QString tempFile = tempDir.filePath(QString("qt_notepad_tab_%1.autosave").arg(i));
            QFile file(tempFile);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << editor->toPlainText();
                file.close();
            }
        }
    }
    qDebug() << "Auto saved all tabs.";
}

void AutoSaveManager::recoverFiles() {
    int count = m_tabWidget->count();
    QDir tempDir(QDir::tempPath());
    for (int i = 0; i < count; ++i) {
        QString tempFile = tempDir.filePath(QString("qt_notepad_tab_%1.autosave").arg(i));
        QFile file(tempFile);
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QTextEdit *editor = qobject_cast<QTextEdit*>(m_tabWidget->widget(i));
            if (editor) {
                editor->setPlainText(in.readAll());
            }
            file.close();
        }
    }
}
