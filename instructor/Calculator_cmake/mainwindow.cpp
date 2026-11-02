#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QTextBlockFormat>
#include <QSignalBlocker>

/**
 * @brief Constructs the calculator window and wires UI controls to slots.
 * @param parent Optional Qt parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_strExpression("")
    , m_isError(false)
    , m_isInverted(false)
    , m_hpos(0)
{
    ui->setupUi(this);

    // Keep the internal expression synchronized when the user edits the display directly.
    connect(ui->textEdit, &QTextEdit::textChanged, this, [this]() {
        m_strExpression = ui->textEdit->toPlainText();
        m_isError = false;
    });

    // Use button IDs to identify digit buttons without relying on sender().
    auto *digitButtons = new QButtonGroup(this);
    digitButtons->addButton(ui->pushButton_0, 0);
    digitButtons->addButton(ui->pushButton_1, 1);
    digitButtons->addButton(ui->pushButton_2, 2);
    digitButtons->addButton(ui->pushButton_3, 3);
    digitButtons->addButton(ui->pushButton_4, 4);
    digitButtons->addButton(ui->pushButton_5, 5);
    digitButtons->addButton(ui->pushButton_6, 6);
    digitButtons->addButton(ui->pushButton_7, 7);
    digitButtons->addButton(ui->pushButton_8, 8);
    digitButtons->addButton(ui->pushButton_9, 9);
    connect(digitButtons, &QButtonGroup::idClicked,
            this, &MainWindow::digit_pressed);


    connect(ui->pushButton_plusminus, &QPushButton::released, this, &MainWindow::unary_operation_pressed);

    // Keep arithmetic operator selection mutually exclusive.
    auto *operatorButtons = new QButtonGroup(this);
    operatorButtons->setExclusive(true);
    operatorButtons->addButton(ui->pushButton_plus, 0);
    operatorButtons->addButton(ui->pushButton_minus, 1);
    operatorButtons->addButton(ui->pushButton_mul, 2);
    operatorButtons->addButton(ui->pushButton_divided, 3);
    for (auto *button : operatorButtons->buttons()) {
        button->setCheckable(true);
    }
    connect(operatorButtons, &QButtonGroup::idClicked, this,
            [this, operatorButtons](int id) {
                static const QString operators[] = {"+", "-", "*", "/"};
                insertRightAlignedText(ui->textEdit, operators[id]);
                operatorButtons->button(id)->setChecked(false);
            });

    connect(ui->pushButton_bracket_left, &QPushButton::released, this, &MainWindow::left_bracked_pressed);
    connect(ui->pushButton_bracket_right, &QPushButton::released, this, &MainWindow::right_bracked_pressed);
    connect(ui->pushButton_sqrt, &QPushButton::released, this, &MainWindow::sqrt_pressed);
    connect(ui->pushButton_x2, &QPushButton::released, this, &MainWindow::power_pressed);
    connect(ui->pushButton_erase_left, &QPushButton::released, this, &MainWindow::erase_left_pressed);

    connect(ui->pushButton_cos, &QPushButton::released, this, &MainWindow::cos_pressed);
    connect(ui->pushButton_sin, &QPushButton::released, this, &MainWindow::sin_pressed);
    connect(ui->pushButton_tan, &QPushButton::released, this, &MainWindow::tan_pressed);
    connect(ui->pushButton_log, &QPushButton::released, this, &MainWindow::log_pressed);
    connect(ui->pushButton_ln, &QPushButton::released, this, &MainWindow::ln_pressed);
    connect(ui->pushButton_inv, &QPushButton::released, this, &MainWindow::inv_pressed);
    connect(ui->pushButton_mod, &QPushButton::released, this, &MainWindow::mod_pressed);
    connect(ui->pushButton_edx, &QPushButton::released, this, &MainWindow::edx_pressed);
    connect(ui->pushButton_percent, &QPushButton::released, this, &MainWindow::percent_pressed);

    connect(ui->pushButton_pi, &QPushButton::released, this, &MainWindow::pi_pressed);
    connect(ui->pushButton_cursor_left, &QPushButton::clicked, this, &MainWindow::move_left_pressed);
    connect(ui->pushButton_cursor_right, &QPushButton::clicked, this, &MainWindow::move_right_pressed);

    connect(ui->pushButton_history_back, &QPushButton::clicked, this, &MainWindow::history_back_pressed);
    connect(ui->pushButton_history_forward, &QPushButton::clicked, this, &MainWindow::history_forward_pressed);

    connect(ui->pushButton_changeDegreesMode, &QPushButton::clicked, this, &MainWindow::degreesMode_pressed);


    connect(ui->pushButton_decimal, &QPushButton::clicked, this, &MainWindow::decimal_pressed);

    connect(ui->pushButton_clear, &QPushButton::clicked, this, &MainWindow::clear_pressed);
    connect(ui->pushButton_equals, &QPushButton::clicked, this, &MainWindow::equals_pressed);

    setRightAlignedText(ui->textEdit, m_strExpression);
}

/** @brief Destroys the window's generated UI. */
MainWindow::~MainWindow()
{
    delete ui;
}

/** @brief Toggles the trigonometric angle unit between degrees and radians. */
void MainWindow::degreesMode_pressed()
{
    const bool degrees = ui->pushButton_changeDegreesMode->text() == "grad";
    if (degrees) {
        calc.setDegMode(false);
        ui->pushButton_changeDegreesMode->setText("rad");
    } else {
        calc.setDegMode(true);
        ui->pushButton_changeDegreesMode->setText("grad");
    }
}

/** @brief Inserts the digit of the button that emitted the signal. */
void MainWindow::digit_pressed(int digit)
{
    insertRightAlignedText(ui->textEdit, QString::number(digit));
}

/** @brief Inserts a decimal separator at the current cursor position. */
void MainWindow::decimal_pressed()
{
    insertRightAlignedText(ui->textEdit, ".");
}

/** @brief Applies the sign-toggle button action to the current expression. */
void MainWindow::unary_operation_pressed()
{
    if (m_isError) {
        clear_pressed();
    }

    if (!m_strExpression.isEmpty()) {
        const int minusPosition = m_strExpression.lastIndexOf('-');
        if (minusPosition != -1) {
            m_strExpression.remove(minusPosition, 1);
        } else {
            m_strExpression.prepend('-');
        }
    }
    setRightAlignedText(ui->textEdit, m_strExpression);
}

/** @brief Clears the expression, display error state and selected binary operator buttons. */
void MainWindow::clear_pressed()
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

/** @brief Inserts the calculator constant pi. */
void MainWindow::pi_pressed()
{
    insertRightAlignedText(ui->textEdit,"pi");
}

/** @brief Moves the text cursor one position to the left. */
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

/** @brief Moves the text cursor one position to the right. */
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

/** @brief Displays the preceding entry in the expression/result history. */
void MainWindow::history_back_pressed()
{
    if (m_history.isEmpty()) return;
    m_hpos = qMax(0, m_hpos - 1);

    m_isError = false;
    QString hentry = m_history[m_hpos];
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,hentry);
}

/** @brief Displays the following entry in the expression/result history. */
void MainWindow::history_forward_pressed()
{
    if (m_history.isEmpty()) return;
    m_hpos = qMin(m_hpos + 1, m_history.size() - 1);

    m_isError = false;
    QString hentry = m_history[m_hpos];
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,hentry);
}

/** @brief Evaluates the displayed expression, stores history and displays the result or error. */
void MainWindow::equals_pressed()
{
    try
    {
        m_strExpression = ui->textEdit->toPlainText();
        m_history.append(m_strExpression);

        double result = calc.evaluate(m_strExpression.toStdString());
        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }

        m_history.append(m_strExpression);
        m_hpos = m_history.size()-1;
        m_isError = false;
    } catch (const std::exception& e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

/** @brief Inserts an opening parenthesis. */
void MainWindow::left_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,"(");
}

/** @brief Inserts a closing parenthesis. */
void MainWindow::right_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,")");
}

/** @brief Inserts the square-root function prefix. */
void MainWindow::sqrt_pressed()
{
    insertRightAlignedText(ui->textEdit,"sqrt(");
}

/** @brief Inserts the exponentiation operator. */
void MainWindow::power_pressed()
{
    insertRightAlignedText(ui->textEdit,"^");
}

/** @brief Inserts sine or inverse sine, depending on inverse-key state. */
void MainWindow::sin_pressed()
{
    QString name = m_isInverted ? "asin(" : "sin(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts cosine or inverse cosine, depending on inverse-key state. */
void MainWindow::cos_pressed()
{
    QString name = m_isInverted ? "acos(" : "cos(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts tangent or inverse tangent, depending on inverse-key state. */
void MainWindow::tan_pressed()
{
    QString name = m_isInverted ? "atan(" : "tan(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts the base-10 logarithm function prefix. */
void MainWindow::log_pressed()
{
    insertRightAlignedText(ui->textEdit,"log(");
}

/** @brief Inserts the natural logarithm function prefix. */
void MainWindow::ln_pressed()
{
    insertRightAlignedText(ui->textEdit,"ln(");
}

/** @brief Inserts the floating-point remainder operator. */
void MainWindow::mod_pressed()
{
    insertRightAlignedText(ui->textEdit," mod ");
}

/** @brief Toggles inverse trigonometric keys and inverse percentage mode. */
void MainWindow::inv_pressed()
{
    if(m_isInverted == false){
        ui->pushButton_sin->setText("asin");
        ui->pushButton_cos->setText("acos");
        ui->pushButton_tan->setText("atan");
        ui->pushButton_percent->setText("1/%");
        m_isInverted = true;
    }else{
        ui->pushButton_sin->setText("sin");
        ui->pushButton_cos->setText("cos");
        ui->pushButton_tan->setText("tan");
        ui->pushButton_percent->setText("%");
        m_isInverted = false;
    }
    calc.setInverted(m_isInverted);
}

/** @brief Evaluates the reciprocal of the current expression and displays the result or error. */
void MainWindow::edx_pressed()
{
    try
    {
        const std::string argument = ui->textEdit->toPlainText().toStdString();
        const std::string expression = "edx(" + argument + ")";
        double result = calc.evaluate(expression);
        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }
        m_isError = false;
    } catch (const std::exception& e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

/** @brief Inserts the percentage operator. */
void MainWindow::percent_pressed()
{
    insertRightAlignedText(ui->textEdit,"%");
}

/** @brief Deletes the character immediately before the text cursor, if any. */
void MainWindow::erase_left_pressed()
{
    if (m_isError) {
        clear_pressed();
        return;
    }

    // Get the cursor position
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position();

    // Check if the cursor is not at the beginning
    if (pos > 0) {
        // Move the cursor to the position before the current one and select the character to delete
        cursor.setPosition(pos - 1, QTextCursor::KeepAnchor);

        // Remove the selected text
        cursor.removeSelectedText();

        // Update the text edit with the new cursor
        ui->textEdit->setTextCursor(cursor);

        // Update the internal string representation
        m_strExpression = ui->textEdit->toPlainText();

        // Set the text to be right-aligned if needed
        //setRightAlignedText(ui->textEdit, m_strExpression);
    }
}

/** @brief Inserts text at the editor cursor and updates m_strExpression.
 * @param textEdit Editor receiving the inserted text.
 * @param text Text to insert.
 */
void MainWindow::insertRightAlignedText(QTextEdit *textEdit, const QString &text) {
    if (m_isError) {
        clear_pressed();
    }

    textEdit->insertPlainText(text);
    m_strExpression = textEdit->toPlainText();
}

/** @brief Replaces editor contents and applies right alignment.
 * @param textEdit Editor to update.
 * @param text Replacement text.
 */
void MainWindow::setRightAlignedText(QTextEdit *textEdit, const QString &text) {
    const QSignalBlocker signalBlocker(textEdit);
    m_strExpression = text;
    textEdit->clear();

    // Set the alignment of the entire text to right
    QTextCursor cursor(textEdit->document());
    cursor.insertText(text);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
}


