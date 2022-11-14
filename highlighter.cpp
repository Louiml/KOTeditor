#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColorConstants::Svg::lightblue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b"), QStringLiteral("\\busing\\b"),
        QStringLiteral("\\breturn\\b"), QStringLiteral("\\bstring\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
//! [0] //! [1]
    }
//! [1]

//! [2]
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);
//! [2]

//! [3]
    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);
//! [3]

//! [4]
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
//! [4]

//! [5]
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [6]
    classFormat.setFontWeight(QFont::Bold);
    quotationFormat.setForeground(QColorConstants::Svg::deepskyblue);
    rule.pattern = QRegularExpression(QStringLiteral("<.*>"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setForeground(QColorConstants::Svg::cyan);
    rule.pattern = QRegularExpression(QStringLiteral("\\binclude"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setForeground(Qt::black);
    functionFormat.setFontItalic(false);
    rule.pattern = QRegularExpression(QStringLiteral("\\bstd"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setForeground(QColorConstants::Svg::deepskyblue);
    functionFormat.setFontItalic(false);
    rule.pattern = QRegularExpression(QStringLiteral("\\bcout"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setForeground(QColorConstants::Svg::deepskyblue);
    functionFormat.setFontItalic(false);
    rule.pattern = QRegularExpression(QStringLiteral("\\bcin"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setForeground(QColorConstants::Svg::deepskyblue);
    functionFormat.setFontItalic(false);
    rule.pattern = QRegularExpression(QStringLiteral("\\bendl"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    functionFormat.setFontItalic(false);
    functionFormat.setForeground(QColorConstants::Svg::cyan);
    rule.pattern = QRegularExpression(QStringLiteral("\\bdefine"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [7]
    classFormat.setFontWeight(QFont::ExtraExpanded);
    keywordFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(QColorConstants::Svg::lightpink);
    rule.pattern = QRegularExpression(QStringLiteral("\\bvoid"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
//! [8]
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}
//! [9]

//! [10]
void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
//! [9] //! [10]
    setCurrentBlockState(0);
//! [10]

//! [11]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

//! [11] //! [12]
    while (startIndex >= 0) {
//! [12] //! [13]
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
