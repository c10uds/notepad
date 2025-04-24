#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QStringList>
#include "AutoSaveManager.h"

class QAction;
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
    void chooseFormat();
    void showFindReplaceDialog();
    void showCustomizeShortcutsDialog();
    void showAboutDialog();  // 新增：关于对话框槽函数

private:
    QTabWidget *tabWidget;
    QLabel *statusLabel;
    QStringList currentFiles;
    AutoSaveManager *autoSaveManager;

    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExit;
    QAction *actionFont;
    QAction *actionFindReplace;
    QAction *actionCustomizeShortcuts;
    QAction *actionAbout;  // 新增：关于对话框动作

    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupEditor(QTextEdit *editor);
};

#endif // MAINWINDOW_H