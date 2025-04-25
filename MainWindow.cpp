#include "MainWindow.h"
#include "SimpleHighlighter.h"
#include "AutoSaveManager.h"
#include "CustomizeShortcutsDialog.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QTextEdit>
#include <QFontDialog>
#include <QColorDialog>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    setCentralWidget(tabWidget);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    createMenus();
    createToolBars();
    createStatusBar();

    newFile();

    autoSaveManager = new AutoSaveManager(tabWidget, this);
    autoSaveManager->start();
    autoSaveManager->recoverFiles();

    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateCursorPos);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    actionNew = fileMenu->addAction("New");
    actionOpen = fileMenu->addAction("Open");
    actionSave = fileMenu->addAction("Save");
    actionSaveAs = fileMenu->addAction("Save As");
    actionExit = fileMenu->addAction("Exit");

    connect(actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    QMenu *formatMenu = menuBar()->addMenu("格式");
    actionFont = formatMenu->addAction("字体设置");
    connect(actionFont, &QAction::triggered, this, &MainWindow::chooseFormat);

    QMenu *findMenu = menuBar()->addMenu("查找");
    actionFindReplace = findMenu->addAction("查找替换");
    connect(actionFindReplace, &QAction::triggered, this, &MainWindow::showFindReplaceDialog);

    QMenu *shortcutMenu = menuBar()->addMenu("快捷键");
    actionCustomizeShortcuts = shortcutMenu->addAction("自定义快捷键");
    connect(actionCustomizeShortcuts, &QAction::triggered, this, &MainWindow::showCustomizeShortcutsDialog);

    QMenu *helpMenu = menuBar()->addMenu("帮助");
    actionAbout = helpMenu->addAction("关于");
    connect(actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);

    actionNew->setShortcut(QKeySequence("Ctrl+N"));
    actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    actionSave->setShortcut(QKeySequence("Ctrl+S"));
    actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    actionFont->setShortcut(QKeySequence("Ctrl+T"));
    actionFindReplace->setShortcut(QKeySequence("Ctrl+F"));
}

void MainWindow::showAboutDialog() {
    QMessageBox::about(this, "关于", "这是一个简单的记事本应用，支持多标签页、格式设置、查找替换等功能。\n\n版本：1.0\n作者：c10uds");
}
void MainWindow::createToolBars() {
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->addAction("Undo", this, [=](){
        if (QTextEdit *editor = currentEditor())
            editor->undo();
    });
    toolBar->addAction("Redo", this, [=](){
        if (QTextEdit *editor = currentEditor())
            editor->redo();
    });
}

void MainWindow::createStatusBar() {
    statusLabel->setText("Ln 1, Col 1");
}

QTextEdit* MainWindow::currentEditor() const {
    return qobject_cast<QTextEdit*>(tabWidget->currentWidget());
}

void MainWindow::setupEditor(QTextEdit *editor) {
    new SimpleHighlighter(editor->document());
    connect(editor, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPos);
}

void MainWindow::newFile() {
    QTextEdit *editor = new QTextEdit();
    setupEditor(editor);
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
    if(currentFiles.at(index).isEmpty())
        saveAs();
    else {
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
        if (QTextEdit *editor = currentEditor())
            editor->clear();
        tabWidget->setTabText(0, "Untitled");
        currentFiles[0].clear();
        return;
    }
    currentFiles.removeAt(index);
    QWidget *page = tabWidget->widget(index);
    tabWidget->removeTab(index);
    page->deleteLater();
}

void MainWindow::chooseFormat() {
    if (QTextEdit *editor = currentEditor()) {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, editor->font(), this, "选择字体");
        if (ok) {
            editor->setFont(font);
            QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
            if (color.isValid()) {
                QTextCharFormat fmt;
                fmt.setForeground(color);
                QTextCursor cursor = editor->textCursor();
                cursor.select(QTextCursor::Document);
                cursor.mergeCharFormat(fmt);
                editor->setTextCursor(cursor);
            }
        }
    }
}

void MainWindow::showFindReplaceDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("查找替换");
    QGridLayout layout(&dialog);

    QLabel labelFind("查找:");
    QLineEdit editFind;
    QLabel labelReplace("替换为:");
    QLineEdit editReplace;
    QCheckBox checkCase("区分大小写");
    QCheckBox checkWhole("全词匹配");
    QPushButton btnFind("查找下一个");
    QPushButton btnReplace("替换");
    QPushButton btnReplaceAll("替换全部");
    QPushButton btnCancel("取消");

    layout.addWidget(&labelFind, 0, 0);
    layout.addWidget(&editFind, 0, 1, 1, 3);
    layout.addWidget(&labelReplace, 1, 0);
    layout.addWidget(&editReplace, 1, 1, 1, 3);
    layout.addWidget(&checkCase, 2, 0);
    layout.addWidget(&checkWhole, 2, 1);
    layout.addWidget(&btnFind, 3, 0);
    layout.addWidget(&btnReplace, 3, 1);
    layout.addWidget(&btnReplaceAll, 3, 2);
    layout.addWidget(&btnCancel, 3, 3);

    connect(&btnFind, &QPushButton::clicked, [&](){
        if (QTextEdit *editor = currentEditor()) {
            QTextDocument::FindFlags flags;
            if (checkCase.isChecked())
                flags |= QTextDocument::FindCaseSensitively;
            if (checkWhole.isChecked())
                flags |= QTextDocument::FindWholeWords;
            if (!editor->find(editFind.text(), flags))
                QMessageBox::information(this, "查找", "未找到匹配内容");
        }
    });

    connect(&btnReplace, &QPushButton::clicked, [&](){
        if (QTextEdit *editor = currentEditor()) {
            QTextCursor cursor = editor->textCursor();
            if (cursor.hasSelection() && cursor.selectedText() == editFind.text())
                cursor.insertText(editReplace.text());
            QTextDocument::FindFlags flags;
            if (checkCase.isChecked())
                flags |= QTextDocument::FindCaseSensitively;
            if (checkWhole.isChecked())
                flags |= QTextDocument::FindWholeWords;
            if (!editor->find(editFind.text(), flags))
                QMessageBox::information(this, "查找替换", "未找到匹配内容");
        }
    });

    connect(&btnReplaceAll, &QPushButton::clicked, [&](){
        if (QTextEdit *editor = currentEditor()) {
            QTextDocument *doc = editor->document();
            QTextCursor cursor(doc);
            cursor.beginEditBlock();
            QTextDocument::FindFlags flags;
            if (checkCase.isChecked())
                flags |= QTextDocument::FindCaseSensitively;
            if (checkWhole.isChecked())
                flags |= QTextDocument::FindWholeWords;
            int count = 0;
            while (true) {
                cursor = doc->find(editFind.text(), cursor, flags);
                if (cursor.isNull())
                    break;
                cursor.insertText(editReplace.text());
                count++;
            }
            cursor.endEditBlock();
            QMessageBox::information(this, "替换全部", QString("共替换 %1 处").arg(count));
        }
    });

    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::showCustomizeShortcutsDialog() {
    CustomizeShortcutsDialog dlg(actionNew, actionOpen, actionSave, actionSaveAs,
                                   actionExit, actionFont, actionFindReplace, this);
    dlg.exec();
}