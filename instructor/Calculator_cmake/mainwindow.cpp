/*
 * Copyright (c) 2026 Koch, Achim Johannes Josef.
 * SPDX-License-Identifier: MIT
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextBlockFormat>
#include <QSignalBlocker>
#include <QKeyEvent>
#include <QToolButton>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>
#include <QGraphicsOpacityEffect>
#include <cmath>





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
    , m_decimalPlaces(6)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    QSettings settings("AllgaeuRechner", "Calculator");
    m_decimalPlaces = qBound(0, settings.value("decimalPlaces", 6).toInt(), 15);
    setWindowTitle(QStringLiteral("AllgäuRechner — © 2026 Koch, Achim Johannes Josef"));

    // A short press moves one character; holding a cursor key for one second
    // moves directly to the beginning or end of the expression.
    m_cursorLongPressTimer = new QTimer(this);
    m_cursorLongPressTimer->setSingleShot(true);
    m_cursorLongPressTimer->setTimerType(Qt::PreciseTimer);
    m_cursorLongPressTimer->setInterval(1000);
    connect(m_cursorLongPressTimer, &QTimer::timeout, this, [this]() {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(m_pressedCursorAction == CalculatorAction::CursorLeft
                                ? QTextCursor::Start
                                : QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->setFocus();
        ui->textEdit->ensureCursorVisible();
        ui->textEdit->setCursorWidth(1);
        ui->textEdit->repaint();
        m_cursorLongPressTriggered = true;
    });

    // Discreet support button in the existing alpine header.
    auto *headerLayout = new QHBoxLayout(ui->alpineHeader);
    headerLayout->setContentsMargins(0, 7, 8, 0);
    headerLayout->setSpacing(0);
    headerLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    auto *donationButton = new QToolButton(ui->alpineHeader);
    donationButton->setObjectName(QStringLiteral("donationButton"));
    donationButton->setText(tr("Unterstützen"));
    donationButton->setToolTip(tr("AllgäuRechner unterstützen"));
    donationButton->setAccessibleName(tr("AllgäuRechner unterstützen"));
    donationButton->setFixedSize(112, 34);
    donationButton->setCursor(Qt::PointingHandCursor);
    auto *donationOpacity = new QGraphicsOpacityEffect(donationButton);
    donationOpacity->setOpacity(0.4); // 60 % transparent
    donationButton->setGraphicsEffect(donationOpacity);
    donationButton->setStyleSheet(QStringLiteral(
        "QToolButton { background: rgba(246, 240, 217, 225); color: #51351f;"
        " border: 1px solid #6b431f; border-radius: 9px; font-size: 14px;"
        " font-weight: bold; }"
        "QToolButton:pressed { background: rgba(218, 198, 158, 245); }"));
    headerLayout->addWidget(donationButton);

    connect(donationButton, &QToolButton::clicked, this, [this]() {
        if (!m_donationOverlay) {
            m_donationOverlay = new QWidget(ui->centralwidget);
            m_donationOverlay->setObjectName(QStringLiteral("donationOverlay"));
            m_donationOverlay->setStyleSheet(QStringLiteral(
                "QWidget#donationOverlay { background: rgba(20, 12, 6, 170); }"));

            auto *overlayLayout = new QVBoxLayout(m_donationOverlay);
            overlayLayout->setContentsMargins(16, 16, 16, 16);
            overlayLayout->addStretch();

            auto *panel = new QFrame(m_donationOverlay);
            panel->setObjectName(QStringLiteral("donationPanel"));
            panel->setMaximumWidth(360);
            panel->setStyleSheet(QStringLiteral(
                "QFrame#donationPanel { background: #f6f0d9; border: 2px solid #6b431f; border-radius: 10px; }"
                "QFrame#donationPanel QLabel { color: #28190c; border: none; font-size: 15px; }"
                "QFrame#donationPanel QPushButton { color: #28190c; background: #e3c99e; border: 1px solid #6b431f; border-radius: 5px; padding: 10px 6px; font-size: 14px; font-weight: bold; }"));

            auto *panelLayout = new QVBoxLayout(panel);
            panelLayout->setSpacing(14);

            auto *message = new QLabel(
                tr("Der AllgäuRechner bleibt kostenlos und werbefrei.\n\n"
                   "Seine Weiterentwicklung erfordert jedoch viel Zeit und finanzielle "
                   "Mittel – zum Beispiel für Entwicklungs-Hardware und KI-Werkzeuge.\n\n"
                   "Deine freiwillige Unterstützung hilft mir, diese Arbeit und die "
                   "Weiterentwicklung überhaupt erst möglich zu machen, statt meine "
                   "Zeit vollständig für eine andere Erwerbstätigkeit aufbringen zu müssen.\n\n"
                   "Wenn dir der AllgäuRechner gefällt und du seine Zukunft unterstützen "
                   "möchtest, freue ich mich über einen Beitrag. Jeder Beitrag zählt. "
                   "Vielen Dank!"), panel);
            message->setWordWrap(true);
            panelLayout->addWidget(message);

            auto *buttonLayout = new QHBoxLayout();
            auto *cancelButton = new QPushButton(tr("Abbrechen"), panel);
            auto *donateButton = new QPushButton(tr("Mit PayPal unterstützen"), panel);
            buttonLayout->addWidget(cancelButton);
            buttonLayout->addWidget(donateButton);
            panelLayout->addLayout(buttonLayout);

            overlayLayout->addWidget(panel, 0, Qt::AlignHCenter);
            overlayLayout->addStretch();

            connect(cancelButton, &QPushButton::clicked,
                    m_donationOverlay, &QWidget::hide);
            connect(donateButton, &QPushButton::clicked, this, [this]() {
                m_donationOverlay->hide();
                const QUrl paypalUrl(QStringLiteral("https://www.paypal.me/AchimKoch430"));
                QTimer::singleShot(0, this, [paypalUrl]() {
                    QDesktopServices::openUrl(paypalUrl);
                });
            });
        }

        m_donationOverlay->setGeometry(ui->centralwidget->rect());
        m_donationOverlay->raise();
        m_donationOverlay->show();
    });

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
        {"pushButton_bracket_right_end", Action::EndCloseParenthesis},
        {"pushButton_bracket_left_start", Action::StartOpenParenthesis},
        {"pushButton_sqrt", Action::SquareRoot}, {"pushButton_x2", Action::Power},
        {"pushButton_sin", Action::Sine}, {"pushButton_cos", Action::Cosine},
        {"pushButton_tan", Action::Tangent}, {"pushButton_log", Action::Log10},
        {"pushButton_ln", Action::NaturalLog}, {"pushButton_inv", Action::Inverse},
        {"pushButton_mod", Action::Modulo}, {"pushButton_fact", Action::Factorial},
        {"pushButton_placeholder3_3", Action::BinomialCoefficient},
        {"pushButton_edx", Action::Reciprocal},
        {"pushButton_percent", Action::Percent}, {"pushButton_pi", Action::Pi},
        {"pushButton_changeDegreesMode", Action::ToggleAngleMode},
        {"pushButton_cursor_left", Action::CursorLeft},
        {"pushButton_cursor_right", Action::CursorRight},
        {"pushButton_history_back", Action::HistoryBack},
        {"pushButton_history_forward", Action::HistoryForward}
    };

    ui->pushButton_e->setProperty("calculatorActionId", static_cast<int>(Action::Euler));
    connect(ui->pushButton_e, &QPushButton::clicked, this, [this]() {
        dispatchAction(static_cast<CalculatorAction>(
            ui->pushButton_e->property("calculatorActionId").toInt()));
    });

    for (auto it = actions.cbegin(); it != actions.cend(); ++it) {
        auto *button = findChild<QPushButton *>(it.key());
        if (!button)
            continue;

        button->setProperty("calculatorActionId", static_cast<int>(it.value()));
        button->setCheckable(false);

        const CalculatorAction action = it.value();
        const bool isCursorButton = action == CalculatorAction::CursorLeft
                                 || action == CalculatorAction::CursorRight;
        if (isCursorButton) {
            connect(button, &QPushButton::pressed, this, [this, action]() {
                m_pressedCursorAction = action;
                m_cursorLongPressTriggered = false;
                m_cursorLongPressTimer->start();
            });
            connect(button, &QPushButton::released, this, [this]() {
                m_cursorLongPressTimer->stop();
            });
        }

        connect(button, &QPushButton::clicked, this, [this, button, isCursorButton]() {
            if (isCursorButton && m_cursorLongPressTriggered) {
                // The long-press action already moved the cursor. Do not also
                // execute the ordinary one-character click action on release.
                m_cursorLongPressTriggered = false;
                return;
            }
            const auto action = static_cast<CalculatorAction>(
                button->property("calculatorActionId").toInt());
            dispatchAction(action);
        });
    }

    // The intended free key is in the second keypad row.
    auto *precisionButton = ui->pushButton_placeholder3_4;
    precisionButton->setEnabled(true);
    precisionButton->setText("fix");
    precisionButton->setToolTip("Anzahl der Nachkommastellen einstellen");
    connect(precisionButton, &QPushButton::clicked, this, &MainWindow::decimalPlaces_pressed);

    ui->pushButton_placeholder3_3->setToolTip(tr("Binomialkoeffizient C(n, k) einfügen"));

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
    case CalculatorAction::EndCloseParenthesis: {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);
        right_bracked_pressed();
        break;
    }
    case CalculatorAction::StartOpenParenthesis: {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        ui->textEdit->setTextCursor(cursor);
        left_bracked_pressed();
        break;
    }
    case CalculatorAction::SquareRoot: sqrt_pressed(); break;
    case CalculatorAction::Power: power_pressed(); break;
    case CalculatorAction::Sine: sin_pressed(); break;
    case CalculatorAction::Cosine: cos_pressed(); break;
    case CalculatorAction::Tangent: tan_pressed(); break;
    case CalculatorAction::Log10: log_pressed(); break;
    case CalculatorAction::NaturalLog: ln_pressed(); break;
    case CalculatorAction::Inverse: inv_pressed(); break;
    case CalculatorAction::Modulo: mod_pressed(); break;
    case CalculatorAction::Factorial: insertRightAlignedText(ui->textEdit, "!"); break;
    case CalculatorAction::BinomialCoefficient:
        insertRightAlignedText(ui->textEdit, " nCr ");
        break;
    case CalculatorAction::Reciprocal: edx_pressed(); break;
    case CalculatorAction::Percent: percent_pressed(); break;
    case CalculatorAction::Pi: pi_pressed(); break;
    case CalculatorAction::Euler: insertRightAlignedText(ui->textEdit, "e"); break;
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
    if (watched == ui->centralwidget && event->type() == QEvent::Resize && m_precisionOverlay) {
        m_precisionOverlay->setGeometry(ui->centralwidget->rect());
    }
    if (watched == ui->centralwidget && event->type() == QEvent::Resize && m_donationOverlay) {
        m_donationOverlay->setGeometry(ui->centralwidget->rect());
    }
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
        // Switch to scientific notation for very large or very small values.
        // This keeps results such as 50! readable in the calculator display.
        m_strExpression = formatResult(result);

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
    insertRightAlignedText(ui->textEdit, m_isInverted ? "^2" : "sqrt(");
}

/** @brief Inserts the exponentiation operator. */
void MainWindow::power_pressed()
{
    insertRightAlignedText(ui->textEdit, m_isInverted ? " root " : "^");
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
        ui->pushButton_x2->setAlternateState(true);
        ui->pushButton_x2->setToolTip("x-te Wurzel aus y");
        ui->pushButton_x2->update();
        ui->pushButton_sqrt->setAlternateState(true);
        ui->pushButton_sqrt->setToolTip("Quadrat x² berechnen");
        ui->pushButton_sqrt->update();
        ui->pushButton_sin->setAlternateState(true);
        ui->pushButton_cos->setAlternateState(true);
        ui->pushButton_tan->setAlternateState(true);
        ui->pushButton_percent->setAlternateState(true);
        m_isInverted = true;
    }else{
        ui->pushButton_x2->setAlternateState(false);
        ui->pushButton_x2->setToolTip("Potenz x hoch y");
        ui->pushButton_x2->update();
        ui->pushButton_sqrt->setAlternateState(false);
        ui->pushButton_sqrt->setToolTip("Quadratwurzel √x berechnen");
        ui->pushButton_sqrt->update();
        ui->pushButton_sin->setAlternateState(false);
        ui->pushButton_cos->setAlternateState(false);
        ui->pushButton_tan->setAlternateState(false);
        ui->pushButton_percent->setAlternateState(false);
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
        m_strExpression = formatResult(result);
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

    // Update the document as one edit and always leave the caret at the end.
    // This avoids the visible clear/reinsert cycle while typing or evaluating.
    QTextCursor cursor(textEdit->document());
    cursor.beginEditBlock();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
    cursor.endEditBlock();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}



/** @brief Formats results using the selected decimal precision. */
QString MainWindow::formatResult(double result) const
{
    QString formatted = (std::isfinite(result) && std::abs(result) >= 1.0e15)
        ? QString::number(result, 'e', m_decimalPlaces)
        : QString::number(result, 'f', m_decimalPlaces);

    // Do not show unnecessary trailing zeros; the setting defines precision,
    // not a requirement to fill every decimal place.
    const qsizetype exponentPos = formatted.indexOf('e');
    const qsizetype mantissaEnd = exponentPos >= 0 ? exponentPos : formatted.size();
    qsizetype end = mantissaEnd;
    while (end > 0 && formatted.at(end - 1) == '0')
        --end;
    if (end > 0 && formatted.at(end - 1) == '.')
        --end;
    formatted.remove(end, mantissaEnd - end);
    return formatted;
}

/** @brief Opens and persists the decimal precision setting. */
void MainWindow::decimalPlaces_pressed()
{
    if (!m_precisionOverlay) {
        m_precisionOverlay = new QWidget(ui->centralwidget);
        m_precisionOverlay->setObjectName(QStringLiteral("precisionOverlay"));
        m_precisionOverlay->setStyleSheet(
            QStringLiteral("QWidget#precisionOverlay { background: rgba(20, 12, 6, 170); }"));

        auto *overlayLayout = new QVBoxLayout(m_precisionOverlay);
        overlayLayout->setContentsMargins(16, 16, 16, 16);
        overlayLayout->addStretch();

        auto *panel = new QFrame(m_precisionOverlay);
        panel->setObjectName(QStringLiteral("precisionPanel"));
        panel->setMaximumWidth(320);
        panel->setStyleSheet(QStringLiteral(
            "QFrame#precisionPanel { background: #f6f0d9; border: 2px solid #6b431f; border-radius: 10px; }"
            "QFrame#precisionPanel QLabel { color: #28190c; border: none; font-size: 16px; }"
            "QFrame#precisionPanel QLineEdit { color: #28190c; background: white; border: 1px solid #6b431f; padding: 8px; font-size: 18px; }"
            "QFrame#precisionPanel QPushButton { color: #28190c; background: #e3c99e; border: 1px solid #6b431f; border-radius: 5px; padding: 10px 6px; font-size: 15px; }"));

        auto *panelLayout = new QVBoxLayout(panel);
        panelLayout->setSpacing(14);
        panelLayout->addWidget(new QLabel(tr("Nachkommastellen (0–15)"), panel));
        m_precisionInput = new QLineEdit(panel);
        m_precisionInput->setValidator(new QIntValidator(0, 15, m_precisionInput));
        m_precisionInput->setInputMethodHints(Qt::ImhDigitsOnly);
        panelLayout->addWidget(m_precisionInput);

        auto *buttonLayout = new QHBoxLayout();
        auto *cancelButton = new QPushButton(tr("Abbrechen"), panel);
        auto *applyButton = new QPushButton(tr("Übernehmen"), panel);
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addWidget(applyButton);
        panelLayout->addLayout(buttonLayout);

        overlayLayout->addWidget(panel, 0, Qt::AlignHCenter);
        overlayLayout->addStretch();

        connect(cancelButton, &QPushButton::clicked, m_precisionOverlay, &QWidget::hide);
        connect(applyButton, &QPushButton::clicked, this, [this]() {
            bool valid = false;
            const int places = m_precisionInput->text().toInt(&valid);
            if (!valid || places < 0 || places > 15)
                return;
            m_decimalPlaces = places;
            QSettings settings("AllgaeuRechner", "Calculator");
            settings.setValue("decimalPlaces", m_decimalPlaces);
            m_precisionOverlay->hide();
        });
    }

    m_precisionInput->setText(QString::number(m_decimalPlaces));
    m_precisionOverlay->setGeometry(ui->centralwidget->rect());
    m_precisionOverlay->raise();
    m_precisionOverlay->show();
    m_precisionInput->setFocus(Qt::OtherFocusReason);
}
