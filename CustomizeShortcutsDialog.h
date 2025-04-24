#ifndef CUSTOMIZESHORTCUTSDIALOG_H
#define CUSTOMIZESHORTCUTSDIALOG_H

#include <QDialog>
#include <QKeySequenceEdit>
#include <QAction>

class CustomizeShortcutsDialog : public QDialog {
    Q_OBJECT
public:
    explicit CustomizeShortcutsDialog(QAction* actionNew, QAction* actionOpen, QAction* actionSave, QAction* actionSaveAs,
                                        QAction* actionExit, QAction* actionFont, QAction* actionFindReplace, QWidget* parent = nullptr);
private slots:
    void onAccept();
private:
    QAction* m_actionNew;
    QAction* m_actionOpen;
    QAction* m_actionSave;
    QAction* m_actionSaveAs;
    QAction* m_actionExit;
    QAction* m_actionFont;
    QAction* m_actionFindReplace;

    QKeySequenceEdit *m_editNew;
    QKeySequenceEdit *m_editOpen;
    QKeySequenceEdit *m_editSave;
    QKeySequenceEdit *m_editSaveAs;
    QKeySequenceEdit *m_editExit;
    QKeySequenceEdit *m_editFont;
    QKeySequenceEdit *m_editFindReplace;
};

#endif // CUSTOMIZESHORTCUTSDIALOG_H