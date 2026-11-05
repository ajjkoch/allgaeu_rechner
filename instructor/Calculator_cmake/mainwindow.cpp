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
#include <cmath>



namespace {
QIcon makeFunctionIcon(const QString &function)
{
    const QSize size(36, 24);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF plot(2.0, 2.0, 32.0, 20.0);

    QPen axisPen(QColor("#8b9988"), 0.8);
    painter.setPen(axisPen);
    painter.drawLine(QPointF(plot.left(), plot.center().y()),
                     QPointF(plot.right(), plot.center().y()));
    painter.drawLine(QPointF(plot.center().x(), plot.top()),
                     QPointF(plot.center().x(), plot.bottom()));

    QPen curvePen(QColor("#365b43"), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(curvePen);
    painter.setBrush(Qt::NoBrush);

    auto mapPoint = [&plot](qreal x, qreal y) {
        return QPointF(plot.left() + (x + 1.0) * plot.width() / 2.0,
                       plot.center().y() - y * plot.height() / 2.0);
    };

    if (function == "tan") {
        // Three clipped branches; dashed lines mark the vertical asymptotes.
        const qreal xMin = -2.2;
        const qreal xMax = 2.2;
        const qreal gap = 0.09;
        const qreal asymptote = 1.5707963267948966;
        const qreal yLimit = 3.2;
        const qreal ranges[3][2] = {
            {xMin, -asymptote - gap},
            {-asymptote + gap, asymptote - gap},
            {asymptote + gap, xMax}
        };

        for (const auto &range : ranges) {
            QPainterPath path;
            for (int i = 0; i <= 80; ++i) {
                const qreal x = range[0] + (range[1] - range[0]) * i / 80.0;
                const qreal y = qBound(-yLimit, std::tan(x), yLimit) / yLimit;
                const QPointF point = mapPoint(x / xMax, y);
                if (i == 0) path.moveTo(point);
                else path.lineTo(point);
            }
            painter.drawPath(path);
        }

        QPen asymptotePen(QColor("#a28d68"), 0.9, Qt::DashLine);
        painter.setPen(asymptotePen);
        for (qreal x : {-asymptote, asymptote}) {
            const qreal normalizedX = x / xMax;
            painter.drawLine(mapPoint(normalizedX, -1.0),
                             mapPoint(normalizedX, 1.0));
        }
    } else {
        QPainterPath path;
        for (int i = 0; i <= 100; ++i) {
            const qreal x = -M_PI;
            const qreal angle = x + (2.0 * M_PI * i / 100.0);
            const qreal y = function == "sin" ? std::sin(angle) : std::cos(angle);
            const QPointF point = mapPoint(angle / M_PI, y * 0.82);
            if (i == 0) path.moveTo(point);
            else path.lineTo(point);
        }
        painter.drawPath(path);
    }

    return QIcon(pixmap);
}

class AlpineHeader final : public QWidget
{
public:
    explicit AlpineHeader(QWidget *parent = nullptr) : QWidget(parent)
    {
        setObjectName("alpineHeader");
        setFixedHeight(50);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const qreal w = width();
        const qreal h = height();

        QLinearGradient sky(0, 0, 0, h);
        sky.setColorAt(0.0, QColor("#edf0e8"));
        sky.setColorAt(1.0, QColor("#cbd7ca"));
        painter.fillRect(rect(), sky);

        // Distant limestone peaks.
        QPainterPath distant;
        distant.moveTo(0, h * 0.77);
        distant.lineTo(w * 0.13, h * 0.26);
        distant.lineTo(w * 0.20, h * 0.43);
        distant.lineTo(w * 0.34, h * 0.08);
        distant.lineTo(w * 0.43, h * 0.34);
        distant.lineTo(w * 0.57, h * 0.17);
        distant.lineTo(w * 0.70, h * 0.52);
        distant.lineTo(w * 0.84, h * 0.24);
        distant.lineTo(w, h * 0.68);
        distant.lineTo(w, h);
        distant.lineTo(0, h);
        distant.closeSubpath();
        painter.fillPath(distant, QColor(119, 137, 125, 82));

        // A darker, nearer ridge gives the header depth without competing with the display.
        QPainterPath foreground;
        foreground.moveTo(0, h * 0.83);
        foreground.lineTo(w * 0.18, h * 0.53);
        foreground.lineTo(w * 0.31, h * 0.77);
        foreground.lineTo(w * 0.49, h * 0.43);
        foreground.lineTo(w * 0.65, h * 0.80);
        foreground.lineTo(w * 0.82, h * 0.49);
        foreground.lineTo(w, h * 0.75);
        foreground.lineTo(w, h);
        foreground.lineTo(0, h);
        foreground.closeSubpath();
        painter.fillPath(foreground, QColor(76, 101, 83, 105));

        painter.setPen(QColor("#293d31"));
        QFont titleFont("Sans Serif", 11, QFont::DemiBold);
        titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.6);
        painter.setFont(titleFont);
        painter.drawText(QRect(12, 5, width() - 40, 20),
                         Qt::AlignLeft | Qt::AlignVCenter, "ALLGÄU RECHNER");

        painter.setPen(QColor("#465d4b"));
        QFont subtitleFont("Sans Serif", 7);
        subtitleFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.1);
        painter.setFont(subtitleFont);
        painter.drawText(QRect(13, 26, width() - 40, 13),
                         Qt::AlignLeft | Qt::AlignVCenter, "RECHNEN MIT WEITBLICK");

        // Small edelweiss emblem.
        const QPointF flower(width() - 19, 15);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#fffdf4"));
        for (int i = 0; i < 6; ++i) {
            painter.save();
            painter.translate(flower);
            painter.rotate(i * 60.0);
            painter.drawEllipse(QRectF(-2.0, -6.5, 4.0, 6.5));
            painter.restore();
        }
        painter.setBrush(QColor("#c5a45c"));
        painter.drawEllipse(QRectF(flower.x() - 1.8, flower.y() - 1.8, 3.6, 3.6));
    }
};


void paintAlpineButton(QPushButton *button)
{
    QPainter painter(button);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF r = button->rect().adjusted(1.5, 1.5, -1.5, -2.5);
    const QString role = button->property("role").toString();
    const bool pressed = button->isDown();
    QColor top("#f4c27c"), middle("#dfa45d"), bottom("#b87536");
    if (role == "function") { top=QColor("#d9d78a"); middle=QColor("#b8b96a"); bottom=QColor("#858b3d"); }
    if (role == "operator") { top=QColor("#d9d78a"); middle=QColor("#b8b96a"); bottom=QColor("#858b3d"); }
    if (role == "utility") { top=QColor("#f8e2b7"); middle=QColor("#e8c58d"); bottom=QColor("#b98b52"); }
    if (button->objectName() == "pushButton_clear") { top=QColor("#eaa08a"); middle=QColor("#c96f58"); bottom=QColor("#8e4538"); }
    if (button->objectName() == "pushButton_equals") { top=QColor("#4f9b20"); middle=QColor("#26730d"); bottom=QColor("#124407"); }
    if (pressed) { top=top.darker(115); middle=middle.darker(115); bottom=bottom.darker(115); }

    painter.setPen(QPen(QColor(55,35,18,190), 2.0));
    painter.setBrush(QColor("#4a2b16"));
    painter.drawRoundedRect(r.adjusted(0,1,0,1), 12, 12);
    QLinearGradient wood(r.topLeft(), r.bottomLeft());
    wood.setColorAt(0, top.lighter(118)); wood.setColorAt(0.45, middle); wood.setColorAt(1, bottom);
    painter.setPen(QPen(QColor("#68401f"), 1.2));
    painter.setBrush(wood);
    painter.drawRoundedRect(r, 11, 11);

    painter.save();
    QPainterPath clip; clip.addRoundedRect(r.adjusted(2,2,-2,-2),9,9);
    painter.setClipPath(clip);
    for (int i=0; i<9; ++i) {
        const qreal y = r.top()+5+i*(r.height()-10)/8.0;
        QPainterPath grain;
        grain.moveTo(r.left()-4,y);
        grain.cubicTo(r.left()+r.width()*0.22,y-3+(i%3),r.left()+r.width()*0.65,y+3-(i%2),r.right()+4,y-1);
        painter.setPen(QPen(QColor(92,48,17, i%2 ? 32 : 20), i%3==0 ? 1.4 : 0.8));
        painter.drawPath(grain);
    }
    painter.restore();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255,245,210,175),1.2));
    painter.drawRoundedRect(r.adjusted(2,2,-2,-3),9,9);
    painter.setPen(QPen(QColor(65,37,18,150),1.0));
    painter.drawLine(QPointF(r.left()+8,r.bottom()-3),QPointF(r.right()-8,r.bottom()-3));

    const QString name = button->objectName();
    const bool trig = name=="pushButton_sin" || name=="pushButton_cos" || name=="pushButton_tan";
    const bool function = role=="function" || trig;
    const QColor ink = name=="pushButton_equals" ? QColor("#fff3cf") :
                       name=="pushButton_clear" ? QColor("#431b12") : QColor("#291307");
    painter.setPen(ink);
    QFont f("Georgia", function ? 15 : 20, QFont::Bold);
    if (name=="pushButton_pi" || name=="pushButton_sqrt") f.setPointSize(25);
    if (name=="pushButton_changeDegreesMode") f.setPointSize(18);
    if (name=="pushButton_edx") f.setPointSize(17);
    if (name=="pushButton_equals") f.setPointSize(25);
    f.setItalic(function || name=="pushButton_pi" || name=="pushButton_changeDegreesMode");
    painter.setFont(f);

    if (trig) {
        QIcon icon = button->icon();
        const QRect graphRect(button->rect().center().x()-26, button->rect().top()+5, 52, 25);
        icon.paint(&painter, graphRect);
        painter.drawText(QRectF(r.left()+3,r.center().y()+4,r.width()-6,r.height()/2-3),Qt::AlignHCenter|Qt::AlignVCenter,button->text());
    } else if (name=="pushButton_pi" || name=="pushButton_sqrt" || name=="pushButton_x2" ||
               name=="pushButton_changeDegreesMode") {
        painter.drawText(QRectF(r.left()+3,r.top()+5,r.width()-6,r.height()*0.58),Qt::AlignCenter,button->text());
        QFont sub("Georgia", 12, QFont::Bold); sub.setItalic(true); painter.setFont(sub);
        QString caption = name=="pushButton_pi" ? "Pi" : name=="pushButton_sqrt" ? "Wurzel" :
                          name=="pushButton_x2" ? "Potenz" : "Grad";
        painter.drawText(QRectF(r.left()+3,r.center().y()+3,r.width()-6,r.height()/2-4),Qt::AlignCenter,caption);
    } else {
        painter.drawText(r.adjusted(3,2,-3,-3),Qt::AlignCenter,button->text());
    }
}

void applyAlpineTheme(QWidget *window)
{
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

    // Each key gets a concise, engraved-looking symbol where that improves recognition.
    // Object names and calculatorActionId remain the only source of action semantics.
    const QMap<QString, QString> labels = {
        {"pushButton_plusminus", "±"}, {"pushButton_sqrt", "√"},
        {"pushButton_x2", "x²"}, {"pushButton_edx", "1/x"},
        {"pushButton_inv", "INV"}, {"pushButton_mul", "×"},
        {"pushButton_erase_left", "⌫"}, {"pushButton_cursor_left", "←"},
        {"pushButton_cursor_right", "→"}, {"pushButton_history_back", "↑"},
        {"pushButton_history_forward", "↓"}
    };

    const QMap<QString, QString> hints = {
        {"pushButton_pi", "Konstante π einfügen"},
        {"pushButton_sqrt", "Quadratwurzel √x berechnen"},
        {"pushButton_x2", "Potenz x hoch y"},
        {"pushButton_edx", "Kehrwert 1/x berechnen"},
        {"pushButton_inv", "Umkehrfunktionen aktivieren"},
        {"pushButton_changeDegreesMode", "Winkelmaß zwischen Grad und Radiant wechseln"},
        {"pushButton_clear", "Eingabe vollständig löschen"},
        {"pushButton_erase_left", "Zeichen links vom Cursor löschen"},
        {"pushButton_equals", "Ausdruck berechnen"},
        {"pushButton_cursor_left", "Cursor nach links bewegen"},
        {"pushButton_cursor_right", "Cursor nach rechts bewegen"},
        {"pushButton_history_back", "Vorherigen Verlaufseintrag anzeigen"},
        {"pushButton_history_forward", "Nächsten Verlaufseintrag anzeigen"},
        {"pushButton_percent", "Prozentfunktion"},
        {"pushButton_mod", "Modulo – Rest einer Division"},
        {"pushButton_plusminus", "Vorzeichen wechseln"}
    };

    window->setStyleSheet(R"(
        QMainWindow, QWidget#centralwidget {
            background-color: #70421f;
            background-image: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #9b6738,stop:0.5 #633b1e,stop:1 #3c2415);
        }
        QTextEdit#textEdit {
            background-color: #f6f0d9; color: #28190c;
            border: 3px solid #6b431f; border-top: 3px solid #f5d99e;
            border-bottom: 5px solid #39200e; border-radius: 13px;
            padding: 7px 10px; font: bold 23pt "Georgia";
        }
        QPushButton { background: transparent; border: none; padding: 1px; }
        QWidget#alpineHeader { border: 2px solid #70451f; border-radius: 8px; }
    )");


    const QMap<QString, QString> functionPlots = {
        {"pushButton_sin", "sin"}, {"pushButton_cos", "cos"},
        {"pushButton_tan", "tan"}
    };

    const auto buttons = window->findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        const QString name = button->objectName();
        if (labels.contains(name))
            button->setText(labels.value(name));
        if (roles.contains(name))
            button->setProperty("role", roles.value(name));
        if (hints.contains(name))
            button->setToolTip(hints.value(name));
        if (functionPlots.contains(name)) {
            button->setIcon(makeFunctionIcon(functionPlots.value(name)));
            button->setIconSize(QSize(36, 24));
        }
        if (name == "pushButton_edx")
            button->setText(QString::fromUtf8("1⁄x"));
        button->setMinimumHeight(30);
        button->installEventFilter(window);
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        button->setAutoDefault(false);
        button->setDefault(false);
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
    if (qobject_cast<QPushButton *>(watched) && event->type() == QEvent::Paint) {
        paintAlpineButton(qobject_cast<QPushButton *>(watched));
        return true;
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


