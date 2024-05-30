#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QString>
#include <QList>

#include "../../common/Util.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_strExpression;
    Calculator calc;
    bool m_isError;
    bool m_isEvaluated;
    bool m_isInverted;
    int m_hpos;
    QVector<QString> m_history;
    void setRightAlignedText(QTextEdit *textEdit, const QString &text);
    void insertRightAlignedText(QTextEdit *textEdit, const QString &text);

private slots:
    void degreesMode_pressed();
    void digit_pressed();
    void decimal_pressed();
    void unary_operation_pressed();
    void clear_pressed();
    void equals_pressed();
    void binary_operation_pressed();
    void left_bracked_pressed();
    void right_bracked_pressed();
    void sqrt_pressed();
    void sin_pressed();
    void cos_pressed();
    void tan_pressed();
    void log_pressed();
    void ln_pressed();
    void mod_pressed();
    void edx_pressed();
    void inv_pressed();
    void percent_pressed();
    void erase_left_pressed();
    void power_pressed();
    void pi_pressed();
    void move_left_pressed();
    void move_right_pressed();
    void history_back_pressed();
    void history_forward_pressed();
};
#endif // MAINWINDOW_H
