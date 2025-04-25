#include "SimpleHighlighter.h"

SimpleHighlighter::SimpleHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    // 1. 关键字高亮
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {
        "\\band\\b", "\\bas\\b", "\\bassert\\b", "\\bbreak\\b", "\\bclass\\b", "\\bcontinue\\b",
        "\\bdef\\b", "\\bdel\\b", "\\belif\\b", "\\belse\\b", "\\bexcept\\b", "\\bFalse\\b",
        "\\bfinally\\b", "\\bfor\\b", "\\bfrom\\b", "\\bglobal\\b", "\\bif\\b", "\\bimport\\b",
        "\\bin\\b", "\\bis\\b", "\\blambda\\b", "\\bNone\\b", "\\bnonlocal\\b", "\\bnot\\b",
        "\\bor\\b", "\\bpass\\b", "\\braise\\b", "\\breturn\\b", "\\bTrue\\b", "\\btry\\b",
        "\\bwhile\\b", "\\bwith\\b", "\\byield\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // 2. 内置函数高亮
    builtinFunctionFormat.setForeground(Qt::darkCyan);
    builtinFunctionFormat.setFontItalic(true);
    QStringList builtinFunctionPatterns = {
        "\\bprint\\b", "\\blen\\b", "\\btype\\b", "\\bstr\\b", "\\bint\\b", "\\bfloat\\b",
        "\\binput\\b", "\\bopen\\b", "\\brange\\b", "\\bdir\\b", "\\bhelp\\b", "\\bmap\\b",
        "\\bfilter\\b", "\\bsum\\b", "\\bmin\\b", "\\bmax\\b", "\\babs\\b", "\\bround\\b",
        "\\bzip\\b", "\\benumerate\\b", "\\bany\\b", "\\ball\\b", "\\bsorted\\b"
    };
    for (const QString &pattern : builtinFunctionPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = builtinFunctionFormat;
        highlightingRules.append(rule);
    }

    // 3. 字符串高亮
    stringFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(R"(["']{3}.*?["']{3}|["'].*?["'])");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // 4. 注释高亮
    commentFormat.setForeground(Qt::darkGray);
    commentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // 5. 数字高亮
    numberFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b\\d+\\.?\\d*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);
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