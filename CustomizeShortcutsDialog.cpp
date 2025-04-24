#include "CustomizeShortcutsDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

CustomizeShortcutsDialog::CustomizeShortcutsDialog(QAction* actionNew, QAction* actionOpen, QAction* actionSave, QAction* actionSaveAs,
                                                   QAction* actionExit, QAction* actionFont, QAction* actionFindReplace, QWidget* parent)
    : QDialog(parent),
      m_actionNew(actionNew), m_actionOpen(actionOpen), m_actionSave(actionSave), m_actionSaveAs(actionSaveAs),
      m_actionExit(actionExit), m_actionFont(actionFont), m_actionFindReplace(actionFindReplace)
{
    setWindowTitle("自定义快捷键");
    QGridLayout *layout = new QGridLayout(this);
    
    QLabel *labelNew = new QLabel("新建:", this);
    m_editNew = new QKeySequenceEdit(actionNew->shortcut(), this);
    QLabel *labelOpen = new QLabel("打开:", this);
    m_editOpen = new QKeySequenceEdit(actionOpen->shortcut(), this);
    QLabel *labelSave = new QLabel("保存:", this);
    m_editSave = new QKeySequenceEdit(actionSave->shortcut(), this);
    QLabel *labelSaveAs = new QLabel("另存为:", this);
    m_editSaveAs = new QKeySequenceEdit(actionSaveAs->shortcut(), this);
    QLabel *labelExit = new QLabel("退出:", this);
    m_editExit = new QKeySequenceEdit(actionExit->shortcut(), this);
    QLabel *labelFont = new QLabel("字体设置:", this);
    m_editFont = new QKeySequenceEdit(actionFont->shortcut(), this);
    QLabel *labelFindReplace = new QLabel("查找替换:", this);
    m_editFindReplace = new QKeySequenceEdit(actionFindReplace->shortcut(), this);
    
    layout->addWidget(labelNew, 0, 0);
    layout->addWidget(m_editNew, 0, 1);
    layout->addWidget(labelOpen, 1, 0);
    layout->addWidget(m_editOpen, 1, 1);
    layout->addWidget(labelSave, 2, 0);
    layout->addWidget(m_editSave, 2, 1);
    layout->addWidget(labelSaveAs, 3, 0);
    layout->addWidget(m_editSaveAs, 3, 1);
    layout->addWidget(labelExit, 4, 0);
    layout->addWidget(m_editExit, 4, 1);
    layout->addWidget(labelFont, 5, 0);
    layout->addWidget(m_editFont, 5, 1);
    layout->addWidget(labelFindReplace, 6, 0);
    layout->addWidget(m_editFindReplace, 6, 1);
    
    QPushButton *btnOk = new QPushButton("确定", this);
    QPushButton *btnCancel = new QPushButton("取消", this);
    layout->addWidget(btnOk, 7, 0);
    layout->addWidget(btnCancel, 7, 1);
    
    connect(btnOk, &QPushButton::clicked, this, &CustomizeShortcutsDialog::onAccept);
    connect(btnCancel, &QPushButton::clicked, this, &CustomizeShortcutsDialog::reject);
}

void CustomizeShortcutsDialog::onAccept() {
    m_actionNew->setShortcut(m_editNew->keySequence());
    m_actionOpen->setShortcut(m_editOpen->keySequence());
    m_actionSave->setShortcut(m_editSave->keySequence());
    m_actionSaveAs->setShortcut(m_editSaveAs->keySequence());
    m_actionExit->setShortcut(m_editExit->keySequence());
    m_actionFont->setShortcut(m_editFont->keySequence());
    m_actionFindReplace->setShortcut(m_editFindReplace->keySequence());
    accept();
}