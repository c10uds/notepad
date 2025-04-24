#include "SimpleHighlighter.h"

SimpleHighlighter::SimpleHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    // 简单示例：高亮 if/else/for/while/return 这些关键字
    QStringList keywordPatterns = {"\\bif\\b", "\\belse\\b", "\\bfor\\b", "\\bwhile\\b", "\\breturn\\b"};
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void SimpleHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
