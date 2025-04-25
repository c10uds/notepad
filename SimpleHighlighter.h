#ifndef SIMPLEHIGHLIGHTER_H
#define SIMPLEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class SimpleHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SimpleHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat numberFormat;
};

#endif // SIMPLEHIGHLIGHTER_H