#ifndef CSHIGHLIGHTER_H
#define CSHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>


class CsHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

  public:
    CsHighlighter(QTextDocument *parent = 0);

  protected:
    void highlightBlock(const QString &text);

  private:

    QTextCharFormat mKeywordFormat;           //!< Ключевые слова
    QTextCharFormat mSingleLineCommentFormat; //!< Однострочные коментарии
    QTextCharFormat mChordFormat;             //!< Для идентификаторов глобальных
    QTextCharFormat mNoteFormat;              //!< Десятичные числа
    QTextCharFormat mLyricFormat;             //!< Строки и символы
    QTextCharFormat mErrorFormat;             //!< Шестнадцатиричные числа

  signals:
  };

#endif // CSHIGHLIGHTER_H
