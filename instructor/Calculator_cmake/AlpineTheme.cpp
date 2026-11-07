#include "AlpineTheme.h"

#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QMap>
#include <QWidget>
#include <QEvent>
#include <QFontDatabase>
#include <QPixmap>
#include <cmath>

namespace AlpineTheme {

void paintAlpineButton(QPushButton *button);

class AlpineButtonPaintFilter final : public QObject
{
public:
    using QObject::QObject;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (auto *button = qobject_cast<QPushButton *>(watched)) {
            if (event->type() == QEvent::Paint) {
                paintAlpineButton(button);
                return true;
            }
        }
        return QObject::eventFilter(watched, event);
    }
};

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

void paintAlpineButton(QPushButton *button)
{
    QPainter painter(button);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF r = button->rect().adjusted(1.5, 1.5, -1.5, -2.5);
    const QString role = button->property("role").toString();
    const bool pressed = button->isDown();

    // Reuse the actual wood photograph from the calculator's middle background.
    // A narrow central crop avoids copying the side frame into the key faces.
    static const QPixmap woodTexture(":/alpine/mid.jpg");
    painter.save();
    QPainterPath faceClip;
    faceClip.addRoundedRect(r, 11, 11);
    painter.setClipPath(faceClip);
    if (!woodTexture.isNull()) {
        const int sourceWidth = qMax(1, woodTexture.width() * 3 / 5);
        const int sourceX = (woodTexture.width() - sourceWidth) / 2;
        painter.drawPixmap(r, woodTexture,
                           QRect(sourceX, 0, sourceWidth, woodTexture.height()));
    } else {
        painter.fillRect(r, QColor("#c58b50"));
    }

    // Retain the existing key categories as subtle stains over the same wood.
    QColor tint(0, 0, 0, 0);
    if (role == "function" || role == "operator")
        tint = QColor(132, 145, 65, 78);
    else if (role == "utility" || role == "constant")
        tint = QColor(246, 207, 145, 65);
    if (button->objectName() == "pushButton_clear")
        tint = QColor(174, 65, 43, 125);
    if (button->objectName() == "pushButton_equals")
        tint = QColor(27, 105, 14, 205);
    if (pressed)
        tint = tint.isValid() ? QColor(tint.red(), tint.green(), tint.blue(),
                                      qMin(220, tint.alpha() + 35))
                              : QColor(45, 25, 12, 75);
    if (tint.alpha() > 0)
        painter.fillRect(r, tint);
    painter.restore();

    // Dark brown contours preserve a clear edge between the wood key face
    // and the surrounding wooden background without the former cream highlight.
    painter.setPen(QPen(QColor("#3b2416"), 1.8));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(r, 11, 11);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#56351f"), 1.2));
    painter.drawRoundedRect(r.adjusted(2,2,-2,-3),9,9);
    painter.setPen(QPen(QColor("#3b2416"), 1.0));
    painter.drawLine(QPointF(r.left()+8,r.bottom()-3),QPointF(r.right()-8,r.bottom()-3));

    const QString name = button->objectName();
    const bool trig = name=="pushButton_sin" || name=="pushButton_cos" || name=="pushButton_tan";
    const bool function = role=="function" || trig;
    const QColor ink = name=="pushButton_equals" ? QColor("#fff3cf") :
                       name=="pushButton_clear" ? QColor("#431b12") : QColor("#291307");
    // Keep lettering proportional when the reserved background bands reduce key height.
    const qreal contentScale = qBound(0.62, button->height() / 36.0, 1.0);
    static const int alpineFontId = QFontDatabase::addApplicationFont(":/alpine/BreeSerif-Regular.ttf");
    Q_UNUSED(alpineFontId);
    // Use one consistent type size for all engraved text labels. The
    // button-specific drawing branches below continue to handle custom symbols.
    QFont f("Bree Serif", 11, QFont::Bold);
    f.setPointSizeF(f.pointSizeF() * contentScale);
    f.setItalic(function || name=="pushButton_changeDegreesMode");
    // Keep function labels crisp: no offset lettering or drop shadow.
    painter.setPen(ink);
    painter.setFont(f);

    if (trig) {
        QIcon icon = button->icon();
        const int graphWidth = qRound(52 * contentScale);
        const int graphHeight = qRound(25 * contentScale);
        const QRect graphRect(button->rect().center().x()-graphWidth/2,
                              button->rect().top()+qRound(5 * contentScale),
                              graphWidth, graphHeight);
        icon.paint(&painter, graphRect);
        // Move only the sin/cos/tan label upward; leave the curve untouched.
        painter.drawText(QRectF(r.left()+3,r.center().y()+1,r.width()-6,r.height()/2-3),
                         Qt::AlignHCenter|Qt::AlignVCenter,button->text());
    } else if (name=="pushButton_pi" || name=="pushButton_sqrt" || name=="pushButton_x2" ||
               name=="pushButton_changeDegreesMode") {
        if (name=="pushButton_sqrt" && button->property("inverseSquare").toBool()) {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, "x²");
        } else if (name=="pushButton_sqrt") {
            // Standard square-root notation: radical with an x under the vinculum.
            const qreal cx = r.center().x();
            const qreal cy = r.center().y();
            QPainterPath radical;
            radical.moveTo(cx - 19, cy + 2);
            radical.lineTo(cx - 12, cy + 8);
            radical.lineTo(cx - 4, cy - 9);
            radical.lineTo(cx + 19, cy - 9);
            painter.setPen(QPen(ink, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPath(radical);
            QFont radicandFont("Georgia", qMax(10, qRound(11 * contentScale)), QFont::Bold);
            painter.setFont(radicandFont);
            painter.drawText(QRectF(cx-1, cy-8, 17, 18), Qt::AlignCenter, "x");
        } else if (name=="pushButton_pi") {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        } else if (name=="pushButton_x2") {
            if (button->property("inverseRoot").toBool()) {
                const qreal cx = r.center().x();
                const qreal cy = r.center().y();
                QFont indexFont("Georgia", qMax(9, qRound(9 * contentScale)), QFont::Bold);
                painter.setFont(indexFont);
                painter.drawText(QRectF(cx-23, cy-16, 15, 15), Qt::AlignCenter, "x");
                QPainterPath radical;
                radical.moveTo(cx-10, cy+1);
                radical.lineTo(cx-5, cy+7);
                radical.lineTo(cx+1, cy-8);
                radical.lineTo(cx+22, cy-8);
                painter.setPen(QPen(ink, 2.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawPath(radical);
                QFont radicandFont("Georgia", qMax(11, qRound(12 * contentScale)), QFont::Bold);
                painter.setFont(radicandFont);
                painter.drawText(QRectF(cx+1, cy-7, 19, 19), Qt::AlignCenter, "y");
            } else {
                painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, "xʸ");
            }
        } else {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        }
    } else if (name=="pushButton_erase_left") {
        // Shift the complete backspace symbol (outline and cross) 3 px left.
        painter.save();
        painter.translate(-3.0, 0.0);
        // Center the backspace glyph and keep at least 3 px clear space to
        // every button edge, independent of the actual key dimensions.
        const QRectF bounds = QRectF(r).adjusted(4, 4, -4, -6);
        const qreal cx = bounds.center().x() - 2.0;
        const qreal cy = bounds.center().y();
        const qreal halfH = bounds.height() * 0.42;
        const qreal left = bounds.left() + bounds.width() * 0.12;
        const qreal right = bounds.right() - bounds.width() * 0.08;
        const qreal notch = bounds.left() + bounds.width() * 0.42;
        QPainterPath key;
        key.moveTo(left, cy);
        key.lineTo(notch, cy-halfH);
        key.lineTo(right, cy-halfH);
        key.lineTo(right, cy+halfH);
        key.lineTo(notch, cy+halfH);
        key.closeSubpath();
        painter.setPen(QPen(Qt::black,3.0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        painter.drawPath(key);
        painter.setPen(QPen(Qt::black,2.4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        const qreal cross = qMin(bounds.width(), bounds.height()) * 0.16;
        painter.drawLine(QPointF(cx+cross,cy-cross),QPointF(cx-cross,cy+cross));
        painter.drawLine(QPointF(cx-cross,cy-cross),QPointF(cx+cross,cy+cross));
        painter.restore();
    } else {
        painter.drawText(r.adjusted(3,2,-3,-3),Qt::AlignCenter,button->text());
    }
}

void apply(QWidget *window)
{
    const QMap<QString, QString> roles = {
        {"pushButton_e", "constant"}, {"pushButton_fact", "function"},
        {"pushButton_0", "digit"}, {"pushButton_1", "digit"},
        {"pushButton_2", "digit"}, {"pushButton_3", "digit"},
        {"pushButton_4", "digit"}, {"pushButton_5", "digit"},
        {"pushButton_6", "digit"}, {"pushButton_7", "digit"},
        {"pushButton_8", "digit"}, {"pushButton_9", "digit"},
        {"pushButton_pi", "digit"}, {"pushButton_sqrt", "operator"},
        {"pushButton_changeDegreesMode", "constant"},
        {"pushButton_sin", "function"}, {"pushButton_cos", "function"},
        {"pushButton_tan", "function"}, {"pushButton_log", "function"},
        {"pushButton_ln", "function"}, {"pushButton_edx", "function"},
        {"pushButton_mod", "function"}, {"pushButton_fact", "function"},
        {"pushButton_percent", "function"},
        {"pushButton_inv", "function"}, {"pushButton_x2", "function"},
        {"pushButton_plus", "operator"}, {"pushButton_minus", "operator"},
        {"pushButton_mul", "operator"}, {"pushButton_divided", "operator"},
        {"pushButton_equals", "operator"},
        {"pushButton_clear", "utility"}, {"pushButton_erase_left", "utility"},
        {"pushButton_plusminus", "utility"},
        {"pushButton_bracket_left", "utility"}, {"pushButton_bracket_right", "utility"},
        {"pushButton_bracket_right_end", "utility"},
        {"pushButton_bracket_left_start", "utility"},
        {"pushButton_decimal", "utility"}, {"pushButton_cursor_left", "utility"},
        {"pushButton_cursor_right", "utility"}, {"pushButton_history_back", "utility"},
        {"pushButton_history_forward", "utility"}
    };

    // Each key gets a concise, engraved-looking symbol where that improves recognition.
    // Object names and calculatorActionId remain the only source of action semantics.
    const QMap<QString, QString> labels = {
        {"pushButton_plusminus", "±"}, {"pushButton_sqrt", "√"},
        {"pushButton_x2", "xʸ"}, {"pushButton_edx", "1/x"},
        {"pushButton_inv", "INV"}, {"pushButton_mul", "×"},
        {"pushButton_bracket_right_end", "->)"},
        {"pushButton_bracket_left_start", "(<-"},
        {"pushButton_erase_left", ""}, {"pushButton_cursor_left", "←"},
        {"pushButton_cursor_right", "→"}, {"pushButton_history_back", "↑"},
        {"pushButton_history_forward", "↓"}
    };

    const QMap<QString, QString> hints = {
        {"pushButton_pi", "Konstante π einfügen"},
        {"pushButton_sqrt", "Quadratwurzel √x berechnen; mit INV: Quadrat x² berechnen"},
        {"pushButton_x2", "Potenz x hoch y; mit INV: x-te Wurzel aus y"},
        {"pushButton_edx", "Kehrwert 1/x berechnen"},
        {"pushButton_inv", "Umkehrfunktionen aktivieren"},
        {"pushButton_e", "Eulersche Zahl e einfügen"},
        {"pushButton_changeDegreesMode", "Winkelmaß zwischen Grad und Radiant wechseln"},
        {"pushButton_bracket_right_end", "Schließende Klammer am Ende des Ausdrucks einfügen"},
        {"pushButton_bracket_left_start", "Öffnende Klammer am Anfang des gesamten Ausdrucks einfügen"},
        {"pushButton_clear", "Eingabe vollständig löschen"},
        {"pushButton_erase_left", "Zeichen links vom Cursor löschen"},
        {"pushButton_equals", "Ausdruck berechnen"},
        {"pushButton_cursor_left", "Cursor nach links bewegen"},
        {"pushButton_cursor_right", "Cursor nach rechts bewegen"},
        {"pushButton_history_back", "Vorherigen Verlaufseintrag anzeigen"},
        {"pushButton_history_forward", "Nächsten Verlaufseintrag anzeigen"},
        {"pushButton_percent", "Prozentfunktion"},
        {"pushButton_mod", "Modulo – Rest einer Division"},
        {"pushButton_fact", "Fakultät n! berechnen"},
        {"pushButton_plusminus", "Vorzeichen wechseln"}
    };

    window->setStyleSheet(R"(
        QMainWindow, QWidget#centralwidget {
            background: transparent;
        }
        QTextEdit#textEdit {
            background-color: #f6f0d9; color: #28190c;
            border: 2px solid #6b431f; border-top: 2px solid #f5d99e;
            border-bottom: 3px solid #39200e; border-radius: 11px;
            padding: 2px 8px;
        }
        QPushButton { background: transparent; border: none; padding: 1px; }
        QWidget#alpineHeader, QWidget#alpineFooter { border: 2px solid #70451f; border-radius: 7px; }
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
            button->setText("1/x");
        button->setMinimumHeight(30);
        button->installEventFilter(new AlpineButtonPaintFilter(button));
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        button->setAutoDefault(false);
        button->setDefault(false);
    }
}
} // namespace AlpineTheme
