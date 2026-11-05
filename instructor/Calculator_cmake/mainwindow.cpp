#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextBlockFormat>
#include <QSignalBlocker>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QMap>
#include <QWidget>



namespace {
class AlpineHeader final : public QWidget
{
public:
    explicit AlpineHeader(QWidget *parent = nullptr) : QWidget(parent)
    {
        setObjectName("alpineHeader");
        setFixedHeight(42);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = rect();

        QLinearGradient sky(0, 0, 0, height());
        sky.setColorAt(0.0, QColor("#e9eee8"));
        sky.setColorAt(1.0, QColor("#d5dfd5"));
        painter.fillRect(area, sky);

        QPainterPath distant;
        distant.moveTo(0, height() * 0.78);
        distant.lineTo(width() * 0.16, height() * 0.20);
        distant.lineTo(width() * 0.27, height() * 0.55);
        distant.lineTo(width() * 0.43, height() * 0.08);
        distant.lineTo(width() * 0.61, height() * 0.70);
        distant.lineTo(width() * 0.78, height() * 0.28);
        distant.lineTo(width(), height() * 0.72);
        distant.lineTo(width(), height());
        distant.lineTo(0, height());
        distant.closeSubpath();
        painter.fillPath(distant, QColor(119, 139, 126, 75));

        QPainterPath foreground;
        foreground.moveTo(0, height() * 0.88);
        foreground.lineTo(width() * 0.22, height() * 0.50);
        foreground.lineTo(width() * 0.37, height() * 0.82);
        foreground.lineTo(width() * 0.58, height() * 0.40);
        foreground.lineTo(width() * 0.76, height() * 0.83);
        foreground.lineTo(width(), height() * 0.54);
        foreground.lineTo(width(), height());
        foreground.lineTo(0, height());
        foreground.closeSubpath();
        painter.fillPath(foreground, QColor(88, 111, 94, 95));

        painter.setPen(QColor("#34483b"));
        QFont titleFont("Sans Serif", 11, QFont::DemiBold);
        titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.5);
        painter.setFont(titleFont);
        painter.drawText(QRect(12, 3, width() - 24, 20),
                         Qt::AlignLeft | Qt::AlignVCenter, "ALLGÄU RECHNER");

        painter.setPen(QColor("#53675a"));
        QFont subtitleFont("Sans Serif", 7);
        subtitleFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.0);
        painter.setFont(subtitleFont);
        painter.drawText(QRect(13, 21, width() - 26, 14),
                         Qt::AlignLeft | Qt::AlignVCenter, "RECHNEN MIT WEITBLICK");
        
        // A tiny, understated edelweiss mark.
        const QPointF flower(width() - 18, 13);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#fffdf4"));
        for (int i = 0; i < 5; ++i) {
            painter.save();
            painter.translate(flower);
            painter.rotate(i * 72.0);
            painter.drawEllipse(QRectF(-2.2, -7.0, 4.4, 7.0));
            painter.restore();
        }
        painter.setBrush(QColor("#c7a75a"));
        painter.drawEllipse(QRectF(flower.x() - 2.0, flower.y() - 2.0, 4.0, 4.0));
    }
};

void applyAlpineTheme(QWidget *window)
{
    window->setStyleSheet(R"(
        QMainWindow, QWidget#centralwidget {
            background-color: #e8e3d7;
        }
        QTextEdit#textEdit {
            background: #f8f6ef;
            color: #26352d;
            border: 1px solid #9a9b87;
            border-bottom: 3px solid #788675;
            border-radius: 8px;
            padding: 5px 9px;
            selection-background-color: #a8b8a5;
            font: 22pt "DejaVu Sans";
        }
        QPushButton {
            color: #344137;
            background-color: #f1eee5;
            border: 1px solid #b8b5a7;
            border-bottom: 2px solid #a19d8e;
            border-radius: 6px;
            padding: 2px 3px;
            font: 11pt "DejaVu Sans";
        }
        QPushButton:hover {
            background-color: #faf8f0;
            border-color: #899786;
        }
        QPushButton:pressed {
            background-color: #d8dfd3;
            border: 1px solid #778b79;
            padding-top: 3px;
            padding-bottom: 1px;
        }
        QPushButton[role="digit"] {
            background-color: #f7f5ee;
            color: #26352d;
            font: 15pt "DejaVu Sans";
            border-color: #c4c0b2;
        }
        QPushButton[role="function"] {
            background-color: #e1e7df;
            color: #405746;
            font: 10pt "DejaVu Sans";
            border-color: #aab7a7;
        }
        QPushButton[role="constant"] {
            background-color: #eee6d3;
            color: #665638;
            font: 11pt "DejaVu Sans";
            border-color: #c9b994;
        }
        QPushButton[role="operator"] {
            background-color: #e7d9c7;
            color: #654b35;
            font: 15pt "DejaVu Sans";
            border-color: #c4aa8e;
        }
        QPushButton[role="utility"] {
            background-color: #e7e4db;
            color: #596257;
            font: 9pt "DejaVu Sans";
        }
        QPushButton#pushButton_equals {
            background-color: #657d68;
            color: #fffdf4;
            border: 1px solid #506653;
            border-bottom: 3px solid #405643;
            font: bold 17pt "DejaVu Sans";
        }
        QPushButton#pushButton_equals:hover {
            background-color: #748d77;
        }
        QPushButton#pushButton_clear {
            background-color: #ead8d0;
            color: #70483d;
            border-color: #c9a99d;
            font: bold 11pt "DejaVu Sans";
        }
        QPushButton#pushButton_inv:checked,
        QPushButton:checked {
            background-color: #cbd8c9;
            border-color: #748b76;
        }
        QWidget#alpineHeader {
            border: 1px solid #c2c7b9;
            border-radius: 7px;
        }
    )");

    const QMap<QString, QString> roles = {
        {"pushButton_0", "digit"}, {"pushButton_1", "digit"},
        {"pushButton_2", "digit"}, {"pushButton_3", "digit"},
        {"pushButton_4", "digit"}, {"pushButton_5", "digit"},
        {"pushButton_6", "digit"}, {"pushButton_7", "digit"},
        {"pushButton_8", "digit"}, {"pushButton_9", "digit"},
        {"pushButton_pi", "constant"}, {"pushButton_sqrt", "constant"},
        {"pushButton_changeDegreesMode", "constant"},
        {"pushButton_sin", "function"}, {"pushButton_cos", "function"},
        {"pushButton_tan", "function"}, {"pushButton_log", "function"},
        {"pushButton_ln", "function"}, {"pushButton_edx", "function"},
        {"pushButton_mod", "function"}, {"pushButton_percent", "function"},
        {"pushButton_inv", "function"}, {"pushButton_x2", "function"},
        {"pushButton_plus", "operator"}, {"pushButton_minus", "operator"},
        {"pushButton_mul", "operator"}, {"pushButton_divided", "operator"},
        {"pushButton_equals", "operator"},
        {"pushButton_clear", "utility"}, {"pushButton_erase_left", "utility"},
        {"pushButton_plusminus", "utility"},
        {"pushButton_bracket_left", "utility"}, {"pushButton_bracket_right", "utility"},
        {"pushButton_decimal", "utility"}, {"pushButton_cursor_left", "utility"},
        {"pushButton_cursor_right", "utility"}, {"pushButton_history_back", "utility"},
        {"pushButton_history_forward", "utility"}
    };

    const QMap<QString, QString> hints = {
        {"pushButton_pi", "Konstante π einfügen"},
        {"pushButton_sqrt", "Quadratwurzel"},
        {"pushButton_x2", "Potenz x hoch y"},
        {"pushButton_edx", "Kehrwert 1/x berechnen"},
        {"pushButton_inv", "Umkehrfunktionen aktivieren"},
        {"pushButton_changeDegreesMode", "Winkelmaß zwischen Grad und Radiant wechseln"},
        {"pushButton_clear", "Eingabe löschen"},
        {"pushButton_erase_left", "Zeichen links vom Cursor löschen"},
        {"pushButton_equals", "Ausdruck berechnen"}
    };

    const auto buttons = window->findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        button->setStyleSheet(QString());
        const QString name = button->objectName();
        if (roles.contains(name))
            button->setProperty("role", roles.value(name));
        if (hints.contains(name))
            button->setToolTip(hints.value(name));
        button->setMinimumHeight(24);
        // Touch keys should not consume the first Android tap for keyboard focus.
        button->setFocusPolicy(Qt::NoFocus);
    }
}
} // namespace

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
    applyAlpineTheme(this);
    ui->verticalLayout->insertWidget(0, new AlpineHeader(ui->centralwidget));
    ui->verticalLayout->setSpacing(5);
    ui->verticalLayout->setContentsMargins(7, 7, 7, 7);
    ui->gridLayout->setHorizontalSpacing(4);
    ui->gridLayout->setVerticalSpacing(4);
    ui->textEdit->installEventFilter(this);

    // Keep the internal expression synchronized when the user edits the display directly.
    connect(ui->textEdit, &QTextEdit::textChanged, this, [this]() {
        m_strExpression = ui->textEdit->toPlainText();
        m_isError = false;
    });

    // Every visual control exposes the same action-ID contract. The widget
    // type and its label are deliberately irrelevant to action dispatch.
    using Action = CalculatorAction;
    const QMap<QString, Action> actions = {
        {"pushButton_0", Action::Digit0}, {"pushButton_1", Action::Digit1},
        {"pushButton_2", Action::Digit2}, {"pushButton_3", Action::Digit3},
        {"pushButton_4", Action::Digit4}, {"pushButton_5", Action::Digit5},
        {"pushButton_6", Action::Digit6}, {"pushButton_7", Action::Digit7},
        {"pushButton_8", Action::Digit8}, {"pushButton_9", Action::Digit9},
        {"pushButton_plus", Action::Add}, {"pushButton_minus", Action::Subtract},
        {"pushButton_mul", Action::Multiply}, {"pushButton_divided", Action::Divide},
        {"pushButton_equals", Action::Equals}, {"pushButton_clear", Action::Clear},
        {"pushButton_erase_left", Action::Backspace},
        {"pushButton_plusminus", Action::ToggleSign},
        {"pushButton_decimal", Action::Decimal},
        {"pushButton_bracket_left", Action::OpenParenthesis},
        {"pushButton_bracket_right", Action::CloseParenthesis},
        {"pushButton_sqrt", Action::SquareRoot}, {"pushButton_x2", Action::Power},
        {"pushButton_sin", Action::Sine}, {"pushButton_cos", Action::Cosine},
        {"pushButton_tan", Action::Tangent}, {"pushButton_log", Action::Log10},
        {"pushButton_ln", Action::NaturalLog}, {"pushButton_inv", Action::Inverse},
        {"pushButton_mod", Action::Modulo}, {"pushButton_edx", Action::Reciprocal},
        {"pushButton_percent", Action::Percent}, {"pushButton_pi", Action::Pi},
        {"pushButton_changeDegreesMode", Action::ToggleAngleMode},
        {"pushButton_cursor_left", Action::CursorLeft},
        {"pushButton_cursor_right", Action::CursorRight},
        {"pushButton_history_back", Action::HistoryBack},
        {"pushButton_history_forward", Action::HistoryForward}
    };

    for (auto it = actions.cbegin(); it != actions.cend(); ++it) {
        auto *button = findChild<QPushButton *>(it.key());
        if (!button)
            continue;

        button->setProperty("calculatorActionId", static_cast<int>(it.value()));
        button->setCheckable(false);
        button->setFocusPolicy(Qt::NoFocus);
        connect(button, &QPushButton::clicked, this, [this, button]() {
            const auto action = static_cast<CalculatorAction>(
                button->property("calculatorActionId").toInt());
            dispatchAction(action);
        });
    }

    setRightAlignedText(ui->textEdit, m_strExpression);
}



/** @brief Executes an action identified independently of its visual control. */
void MainWindow::dispatchAction(CalculatorAction action)
{
    switch (action) {
    case CalculatorAction::Digit0: digit_pressed(0); break;
    case CalculatorAction::Digit1: digit_pressed(1); break;
    case CalculatorAction::Digit2: digit_pressed(2); break;
    case CalculatorAction::Digit3: digit_pressed(3); break;
    case CalculatorAction::Digit4: digit_pressed(4); break;
    case CalculatorAction::Digit5: digit_pressed(5); break;
    case CalculatorAction::Digit6: digit_pressed(6); break;
    case CalculatorAction::Digit7: digit_pressed(7); break;
    case CalculatorAction::Digit8: digit_pressed(8); break;
    case CalculatorAction::Digit9: digit_pressed(9); break;
    case CalculatorAction::Add: insertRightAlignedText(ui->textEdit, "+"); break;
    case CalculatorAction::Subtract: insertRightAlignedText(ui->textEdit, "-"); break;
    case CalculatorAction::Multiply: insertRightAlignedText(ui->textEdit, "*"); break;
    case CalculatorAction::Divide: insertRightAlignedText(ui->textEdit, "/"); break;
    case CalculatorAction::Equals: equals_pressed(); break;
    case CalculatorAction::Clear: clear_pressed(); break;
    case CalculatorAction::Backspace: erase_left_pressed(); break;
    case CalculatorAction::ToggleSign: unary_operation_pressed(); break;
    case CalculatorAction::Decimal: decimal_pressed(); break;
    case CalculatorAction::OpenParenthesis: left_bracked_pressed(); break;
    case CalculatorAction::CloseParenthesis: right_bracked_pressed(); break;
    case CalculatorAction::SquareRoot: sqrt_pressed(); break;
    case CalculatorAction::Power: power_pressed(); break;
    case CalculatorAction::Sine: sin_pressed(); break;
    case CalculatorAction::Cosine: cos_pressed(); break;
    case CalculatorAction::Tangent: tan_pressed(); break;
    case CalculatorAction::Log10: log_pressed(); break;
    case CalculatorAction::NaturalLog: ln_pressed(); break;
    case CalculatorAction::Inverse: inv_pressed(); break;
    case CalculatorAction::Modulo: mod_pressed(); break;
    case CalculatorAction::Reciprocal: edx_pressed(); break;
    case CalculatorAction::Percent: percent_pressed(); break;
    case CalculatorAction::Pi: pi_pressed(); break;
    case CalculatorAction::ToggleAngleMode: degreesMode_pressed(); break;
    case CalculatorAction::CursorLeft: move_left_pressed(); break;
    case CalculatorAction::CursorRight: move_right_pressed(); break;
    case CalculatorAction::HistoryBack: history_back_pressed(); break;
    case CalculatorAction::HistoryForward: history_forward_pressed(); break;
    }
}

/** @brief Evaluates the expression when Return or keypad Enter is pressed in the display. */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->textEdit && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            equals_pressed();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
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


