#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QEvent>

#include "../../common/Util.h"

class QTextEdit;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief Creates the calculator window, initializes its state and connects UI signals. */
    MainWindow(QWidget *parent = nullptr);
    /** @brief Releases the generated UI object owned by this window. */
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QString m_strExpression;
    Calculator calc;
    bool m_isError;
    bool m_isInverted;
    int m_hpos;
    QVector<QString> m_history;
    /** @brief Replaces the editor contents and right-aligns the text.
     * @param textEdit Target text editor.
     * @param text Text to display.
     */
    void setRightAlignedText(QTextEdit *textEdit, const QString &text);
    /** @brief Inserts text at the current cursor and synchronizes the expression state.
     * @param textEdit Target text editor.
     * @param text Text to insert.
     */
    void insertRightAlignedText(QTextEdit *textEdit, const QString &text);

private slots:
    /** @brief Toggles the trigonometric angle unit between degrees and radians. */
    void degreesMode_pressed();
    /** @brief Inserts the digit of the button that emitted the signal. */
    void digit_pressed(int digit);
    /** @brief Inserts a decimal separator at the current cursor position. */
    void decimal_pressed();
    /** @brief Applies the sign-toggle button action to the current expression. */
    void unary_operation_pressed();
    /** @brief Clears the expression, display error state and selected binary operator buttons. */
    void clear_pressed();
    /** @brief Evaluates the displayed expression, stores history and displays the result or error. */
    void equals_pressed();
    /** @brief Inserts an opening parenthesis. */
    void left_bracked_pressed();
    /** @brief Inserts a closing parenthesis. */
    void right_bracked_pressed();
    /** @brief Inserts the square-root function prefix. */
    void sqrt_pressed();
    /** @brief Inserts sine or inverse sine, depending on inverse-key state. */
    void sin_pressed();
    /** @brief Inserts cosine or inverse cosine, depending on inverse-key state. */
    void cos_pressed();
    /** @brief Inserts tangent or inverse tangent, depending on inverse-key state. */
    void tan_pressed();
    /** @brief Inserts the base-10 logarithm function prefix. */
    void log_pressed();
    /** @brief Inserts the natural logarithm function prefix. */
    void ln_pressed();
    /** @brief Inserts the floating-point remainder operator. */
    void mod_pressed();
    /** @brief Evaluates the reciprocal of the current expression and displays the result or error. */
    void edx_pressed();
    /** @brief Toggles inverse trigonometric keys and inverse percentage mode. */
    void inv_pressed();
    /** @brief Inserts the percentage operator. */
    void percent_pressed();
    /** @brief Deletes the character immediately before the text cursor, if any. */
    void erase_left_pressed();
    /** @brief Inserts the exponentiation operator. */
    void power_pressed();
    /** @brief Inserts the calculator constant pi. */
    void pi_pressed();
    /** @brief Moves the text cursor one position to the left. */
    void move_left_pressed();
    /** @brief Moves the text cursor one position to the right. */
    void move_right_pressed();
    /** @brief Displays the preceding entry in the expression/result history. */
    void history_back_pressed();
    /** @brief Displays the following entry in the expression/result history. */
    void history_forward_pressed();
};
#endif // MAINWINDOW_H
