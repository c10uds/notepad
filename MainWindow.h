#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QStringList>
#include "AutoSaveManager.h"  // 使用外部模块，不要再定义嵌套类

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void newFile();
    void openFile();
    void saveFile();
    void saveAs();
    void updateCursorPos();
    QTextEdit* currentEditor() const;

private slots:
    void closeTab(int index);

private:
    QTabWidget *tabWidget;
    QLabel *statusLabel;
    QStringList currentFiles;
    AutoSaveManager *autoSaveManager;  // 使用外部 AutoSaveManager

    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupEditor(QTextEdit *editor);
};

#endif // MAINWINDOW_H
