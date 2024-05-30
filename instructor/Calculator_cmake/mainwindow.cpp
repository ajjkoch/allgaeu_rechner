#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_strExpression("")
    , m_isError(false)
    , m_isEvaluated(false)
    , m_isInverted(false)
{
    ui->setupUi(this);

    connect(ui->pushButton_0,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_1,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_6,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_7,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_8,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_9,SIGNAL(released()),this,SLOT(digit_pressed()));

    connect(ui->pushButton_plusminus,SIGNAL(released()),this,SLOT(unary_operation_pressed()));

    connect(ui->pushButton_plus,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->pushButton_minus,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->pushButton_divided,SIGNAL(released()),this,SLOT(binary_operation_pressed()));
    connect(ui->pushButton_mul,SIGNAL(released()),this,SLOT(binary_operation_pressed()));

    connect(ui->pushButton_bracket_left,SIGNAL(released()),this,SLOT(left_bracked_pressed()));
    connect(ui->pushButton_bracket_right,SIGNAL(released()),this,SLOT(right_bracked_pressed()));
    connect(ui->pushButton_sqrt,SIGNAL(released()),this,SLOT(sqrt_pressed()));
    connect(ui->pushButton_x2, SIGNAL(released()), this, SLOT(power_pressed()));
    connect(ui->pushButton_erase_left,SIGNAL(released()),this,SLOT(erase_left_pressed()));

    connect(ui->pushButton_cos,SIGNAL(released()),this,SLOT(cos_pressed()));
    connect(ui->pushButton_sin,SIGNAL(released()),this,SLOT(sin_pressed()));
    connect(ui->pushButton_tan,SIGNAL(released()),this,SLOT(tan_pressed()));
    connect(ui->pushButton_log,SIGNAL(released()),this,SLOT(log_pressed()));
    connect(ui->pushButton_ln ,SIGNAL(released()),this,SLOT(ln_pressed()));
    connect(ui->pushButton_inv,SIGNAL(released()),this,SLOT(inv_pressed()));
    connect(ui->pushButton_mod,SIGNAL(released()),this,SLOT(mod_pressed()));
    connect(ui->pushButton_percent,SIGNAL(released()),this,SLOT(percent_pressed()));

    connect(ui->pushButton_pi          , SIGNAL(released()), this, SLOT(pi_pressed()));
    connect(ui->pushButton_cursor_left , SIGNAL(clicked()), this, SLOT(move_left_pressed()));
    connect(ui->pushButton_cursor_right, SIGNAL(clicked()), this, SLOT(move_right_pressed()));

    connect(ui->pushButton_changeDegreesMode, SIGNAL(clicked()), this, SLOT(on_degreesMode_pressed()));


    ui->pushButton_plus->setCheckable(true);
    ui->pushButton_minus->setCheckable(true);
    ui->pushButton_divided->setCheckable(true);
    ui->pushButton_mul->setCheckable(true);

    setRightAlignedText(ui->textEdit, m_strExpression);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_degreesMode_pressed()
{
    QString strMode = ui->pushButton_changeDegreesMode->text();
    if( strMode == "grad"){
        Calculator::setDegMode(false);
        ui->pushButton_changeDegreesMode->setText("rad");
    } else {
        Calculator::setDegMode(true);
        ui->pushButton_changeDegreesMode->setText("grad");
    }
}

void MainWindow::digit_pressed()
{
    if(m_isError) on_pushButton_clear_released();
    insertRightAlignedText(ui->textEdit,((QPushButton*)sender())->text());
}

void MainWindow::on_pushButton_decimal_released()
{
    if (m_isError) on_pushButton_clear_released();
    insertRightAlignedText(ui->textEdit,".");
}

void MainWindow::unary_operation_pressed()
{
    if (m_isError) on_pushButton_clear_released();

    QPushButton*button = (QPushButton*)sender();
    if(button->text() == "+/-")
    {
        if(!m_strExpression.isEmpty())
        {
            int pos = m_strExpression.lastIndexOf("-");
            if( pos != -1)
            {
                m_strExpression.remove(pos, 1);
            }
            else
            {
                m_strExpression.insert(0, "-");
            }
        }
        setRightAlignedText(ui->textEdit,m_strExpression);
    }
}

void MainWindow::on_pushButton_clear_released()
{
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_mul->setChecked(false);
    ui->pushButton_divided->setChecked(false);

    m_isError = false;
    m_strExpression = "";
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,m_strExpression);
}

void MainWindow::pi_pressed()
{
    insertRightAlignedText(ui->textEdit,"pi");
}

void MainWindow::move_left_pressed()
{
    ui->textEdit->setFocus();

    // Get the current cursor from the text edit
    QTextCursor cursor = ui->textEdit->textCursor();

    // Move the cursor left
    cursor.movePosition(QTextCursor::Left);

    // Set the modified cursor back to the text edit
    ui->textEdit->setTextCursor(cursor);

    // Ensure the cursor is visible
    ui->textEdit->ensureCursorVisible();

    // Set the cursor visible
    ui->textEdit->setCursorWidth(1); // Ensure cursor width is set to a visible size

    // Update the text edit to reflect changes
    ui->textEdit->repaint();

    ui->textEdit->setFocus();
}

void MainWindow::move_right_pressed()
{
    ui->textEdit->setFocus();

    // Get the current cursor from the text edit
    QTextCursor cursor = ui->textEdit->textCursor();

    // Move the cursor right
    cursor.movePosition(QTextCursor::Right);

    // Set the modified cursor back to the text edit
    ui->textEdit->setTextCursor(cursor);

    // Ensure the cursor is visible
    ui->textEdit->ensureCursorVisible();

    // Set the cursor visible
    ui->textEdit->setCursorWidth(1); // Ensure cursor width is set to a visible size

    // Update the text edit to reflect changes
    ui->textEdit->repaint();

    ui->textEdit->setFocus();
}


void MainWindow::on_pushButton_equals_released()
{
    try
    {
        m_strExpression = ui->textEdit->toPlainText();

        double result = calc.evaluate(m_strExpression.toStdString());
        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }

        //m_strExpression = QString::asprintf("%0.2f", calc.evaluate(m_strExpression.toStdString()) );

        m_isError = false;
    } catch (std::exception e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

void MainWindow::binary_operation_pressed()
{
    if (m_isError) on_pushButton_clear_released();

    if(ui->pushButton_plus->isChecked())
    {
        insertRightAlignedText(ui->textEdit,"+");
        ui->pushButton_plus->setChecked(false);
    }
    else if(ui->pushButton_minus->isChecked())
    {
        insertRightAlignedText(ui->textEdit,"-");
        ui->pushButton_minus->setChecked(false);
    }
    else if(ui->pushButton_mul->isChecked())
    {
        insertRightAlignedText(ui->textEdit,"*");
        ui->pushButton_mul->setChecked(false);
    }
    else if(ui->pushButton_divided->isChecked())
    {
        insertRightAlignedText(ui->textEdit,"/");
        ui->pushButton_divided->setChecked(false);
    }
}

void MainWindow::left_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,"(");
}

void MainWindow::right_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,")");
}

void MainWindow::sqrt_pressed()
{
    insertRightAlignedText(ui->textEdit,"sqrt(");
}

void MainWindow::power_pressed()
{
    insertRightAlignedText(ui->textEdit,"^");
}

void MainWindow::sin_pressed()
{
    QString name = m_isInverted ? "asin(" : "sin(";
    insertRightAlignedText(ui->textEdit,name);
}
void MainWindow::cos_pressed()
{
    QString name = m_isInverted ? "acos(" : "cos(";
    insertRightAlignedText(ui->textEdit,name);
}
void MainWindow::tan_pressed()
{
    QString name = m_isInverted ? "atan(" : "tan(";
    insertRightAlignedText(ui->textEdit,name);
}
void MainWindow::log_pressed()
{
    insertRightAlignedText(ui->textEdit,"log(");
}
void MainWindow::ln_pressed()
{
    insertRightAlignedText(ui->textEdit,"ln(");
}
void MainWindow::mod_pressed()
{
    insertRightAlignedText(ui->textEdit," mod ");
}
void MainWindow::inv_pressed()
{
    if(m_isInverted == false){
        ui->pushButton_sin->setText("asin");
        ui->pushButton_cos->setText("acos");
        ui->pushButton_tan->setText("atan");
        m_isInverted = true;
    }else{
        ui->pushButton_sin->setText("sin");
        ui->pushButton_cos->setText("cos");
        ui->pushButton_tan->setText("tan");
        m_isInverted = false;
    }
}

void MainWindow::percent_pressed()
{
    try
    {
        std::string argu = ui->textEdit->toPlainText().toStdString();

        size_t size = argu.length();
        double dVal = std::stod(argu,&size);

        double result = dVal / 100;

        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }
        m_isError = false;
    } catch (std::exception e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

void MainWindow::erase_left_pressed()
{
    m_strExpression.remove(m_strExpression.length()-1,1);
    setRightAlignedText(ui->textEdit,m_strExpression);
}

void MainWindow::insertRightAlignedText(QTextEdit *textEdit, const QString &text) {
    textEdit->insertPlainText(text);
    m_strExpression = textEdit->toPlainText();
}

// Append text with right alignment
void MainWindow::setRightAlignedText(QTextEdit *textEdit, const QString &text) {
    // Clear the existing content
    textEdit->clear();

    // Set the alignment of the entire text to right
    QTextCursor cursor(textEdit->document());
    cursor.insertText(text);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
}


