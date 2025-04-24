#include "MainWindow.h"
#include "SimpleHighlighter.h"    // 语法高亮模块
#include "AutoSaveManager.h"      // 自动保存与恢复模块

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QTabWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 使用 QTabWidget 实现多标签页编辑，同时启用关闭按钮
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    setCentralWidget(tabWidget);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    createMenus();
    createToolBars();
    createStatusBar();

    newFile();

    // 创建并启动自动保存管理器（自动保存间隔为 5 分钟）
    autoSaveManager = new AutoSaveManager(tabWidget, this);
    autoSaveManager->start();
    autoSaveManager->recoverFiles();

    // 当标签页切换时更新光标位置状态
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateCursorPos);
    // 连接标签页关闭请求
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *newAction = fileMenu->addAction("New");
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Save");
    QAction *saveAsAction = fileMenu->addAction("Save As");
    QAction *exitAction = fileMenu->addAction("Exit");

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createToolBars() {
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->addAction("Undo", this, [=](){
        if (QTextEdit *editor = currentEditor()) {
            editor->undo();
        }
    });
    toolBar->addAction("Redo", this, [=](){
        if (QTextEdit *editor = currentEditor()) {
            editor->redo();
        }
    });
}

void MainWindow::createStatusBar() {
    statusLabel->setText("Ln 1, Col 1");
}

QTextEdit* MainWindow::currentEditor() const {
    return qobject_cast<QTextEdit*>(tabWidget->currentWidget());
}

void MainWindow::setupEditor(QTextEdit *editor) {
    // 安装语法高亮
    new SimpleHighlighter(editor->document());
    // 连接光标位置变化信号，以更新状态栏显示
    connect(editor, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPos);
}

void MainWindow::newFile() {
    QTextEdit *editor = new QTextEdit();
    setupEditor(editor);
    // 新建标签页，默认标题 "Untitled"
    int index = tabWidget->addTab(editor, "Untitled");
    if(index >= currentFiles.size())
        currentFiles.append(QString());
    else
        currentFiles[index].clear();
    tabWidget->setCurrentWidget(editor);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Cannot open file");
            return;
        }
        QTextStream in(&file);
        QTextEdit *editor = new QTextEdit();
        setupEditor(editor);
        editor->setPlainText(in.readAll());
        int index = tabWidget->addTab(editor, QFileInfo(fileName).fileName());
        if(index >= currentFiles.size())
            currentFiles.append(fileName);
        else
            currentFiles[index] = fileName;
        tabWidget->setCurrentIndex(index);
        file.close();
    }
}

void MainWindow::saveFile() {
    int index = tabWidget->currentIndex();
    if(index < 0)
        return;
    if(currentFiles.at(index).isEmpty()) {
        saveAs();
    } else {
        QFile file(currentFiles.at(index));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Cannot save file");
            return;
        }
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << currentEditor()->toPlainText();
        file.close();
        setWindowModified(false);
    }
}

void MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        int index = tabWidget->currentIndex();
        if(index < 0)
            return;
        currentFiles[index] = fileName;
        tabWidget->setTabText(index, QFileInfo(fileName).fileName());
        saveFile();
    }
}

void MainWindow::updateCursorPos() {
    QTextEdit *editor = currentEditor();
    if (!editor)
        return;
    QTextCursor cursor = editor->textCursor();
    statusLabel->setText(QString("Ln %1, Col %2")
        .arg(cursor.blockNumber() + 1)
        .arg(cursor.columnNumber() + 1));
}

void MainWindow::closeTab(int index) {
    if (tabWidget->count() <= 1) {
        // 若只剩一个标签，则清空内容，不关闭标签
        if (QTextEdit *editor = currentEditor()) {
            editor->clear();
        }
        tabWidget->setTabText(0, "Untitled");
        currentFiles[0].clear();
        return;
    }
    currentFiles.removeAt(index);
    QWidget *page = tabWidget->widget(index);
    tabWidget->removeTab(index);
    page->deleteLater();
}
