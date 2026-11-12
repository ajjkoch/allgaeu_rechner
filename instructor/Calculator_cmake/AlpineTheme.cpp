/*
 * Copyright (c) 2026 Koch, Achim Johannes Josef.
 * SPDX-License-Identifier: MIT
 */

#include "AlpineTheme.h"
#include "AllgaeuButton.h"

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
        if (qobject_cast<AllgaeuButton *>(watched))
            return QObject::eventFilter(watched, event);
        if (auto *button = qobject_cast<QPushButton *>(watched)) {
            if (event->type() == QEvent::Paint) {
                paintAlpineButton(button);
                return true;
            }
        }
        return QObject::eventFilter(watched, event);
    }
};



void paintAlpineButton(QPushButton *button)
{
    QPainter painter(button);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF r = QRectF(button->rect()).adjusted(1.5, 1.5, -1.5, -2.5);
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

    // Distinguish key groups through restrained stains on the same wood texture.
    // Numeric keys receive a brighter warm glaze; their lettering remains black.
    // Operators are sage green, functions olive, constants warm honey,
    // and navigation/control keys a muted blue-grey.
    QColor tint(0, 0, 0, 0);
    if (role == "digit")
        tint = QColor(255, 238, 207, 165);
    else if (role == "operator")
        tint = QColor(91, 128, 62, 112);
    else if (role == "function")
        tint = QColor(143, 145, 69, 88);
    else if (role == "constant")
        tint = QColor(246, 190, 105, 92);
    else if (role == "control")
        tint = QColor(92, 119, 133, 105);
    else if (role == "utility")
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
                       name=="pushButton_clear" ? QColor("#431b12") :
                       role=="digit" ? QColor("#000000") : QColor("#291307");
    // Keep button labels large and consistent; only apply a small reduction
    // when a button is unusually short.
    const qreal contentScale = qBound(0.90, button->height() / 32.0, 1.0);
    static const int alpineFontId = QFontDatabase::addApplicationFont(":/alpine/BreeSerif-Regular.ttf");
    Q_UNUSED(alpineFontId);
    QFont f("Bree Serif", 18, QFont::Bold);
    f.setPointSizeF(f.pointSizeF() * contentScale);
    painter.setPen(ink);
    painter.setFont(f);

    if (trig) {
        // Trigonometric labels sit in the true visual center of their key.
        painter.drawText(r.adjusted(3, 2, -3, -2),
                         Qt::AlignHCenter | Qt::AlignVCenter, button->text());
    } else if (name=="pushButton_pi" || name=="pushButton_sqrt" || name=="pushButton_x2" ||
               name=="pushButton_changeDegreesMode") {
        if (name=="pushButton_sqrt" && button->property("inverseSquare").toBool()) {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, "x²");
        } else if (name=="pushButton_sqrt") {
            // Standard square-root notation: radical with an x under the vinculum.
            const qreal cx = r.center().x();
            const qreal cy = r.center().y();
            QPainterPath radical;
            // Keep the radical, including its vinculum, within 26 px height.
            constexpr qreal radicalScale = 0.61; // approx. 22 px including the vinculum
            painter.save();
            painter.translate(cx, cy);
            painter.scale(radicalScale, radicalScale);
            radical.moveTo(-19, 2);
            radical.lineTo(-12, 8);
            radical.lineTo(-4, -9);
            radical.lineTo(19, -9);
            painter.setPen(QPen(ink, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPath(radical);
            painter.restore();
            QFont radicandFont("Bree Serif", 14, QFont::Bold);
            painter.setFont(radicandFont);
            painter.drawText(QRectF(cx-1, cy-8, 17, 18), Qt::AlignCenter, "x");
        } else if (name=="pushButton_pi") {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        } else if (name=="pushButton_x2") {
            if (button->property("inverseRoot").toBool()) {
                const qreal cx = r.center().x();
                const qreal cy = r.center().y();
                constexpr qreal inverseRadicalScale = 0.61; // approx. 22 px
                QFont indexFont("Bree Serif", 14, QFont::Bold);
                painter.setFont(indexFont);
                painter.drawText(QRectF(cx-23, cy-16, 15, 15), Qt::AlignCenter, "x");
                QPainterPath radical;
                painter.save();
                painter.translate(cx, cy);
                painter.scale(inverseRadicalScale, inverseRadicalScale);
                radical.moveTo(-10, 1);
                radical.lineTo(-5, 7);
                radical.lineTo(1, -8);
                radical.lineTo(22, -8);
                painter.setPen(QPen(ink, 2.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawPath(radical);
                painter.restore();
                QFont radicandFont("Bree Serif", 14, QFont::Black);
                painter.setFont(radicandFont);
                painter.drawText(QRectF(cx+1, cy-7, 19, 19), Qt::AlignCenter, "y");
            } else {
                painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, "xʸ");
            }
        } else {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        }
    } else {
        painter.drawText(r.adjusted(3,2,-3,-3),Qt::AlignCenter,button->text());
    }
}

void apply(QWidget *window)
{
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
        {"pushButton_placeholder3_3", "Binomialkoeffizient C(n, k) berechnen"},
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
            font-family: "Bree Serif";
            font-size: 20pt;
            font-weight: 700;
        }
    )");
    // Visual properties (wood style, text, font, colors and geometry) belong
    // exclusively to mainwindow.ui. This function only supplies non-visual hints
    // and interaction affordances without changing the Designer configuration.
    const auto buttons = window->findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        const QString name = button->objectName();
        if (hints.contains(name))
            button->setToolTip(hints.value(name));
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        button->setAutoDefault(false);
        button->setDefault(false);
}
} // namespace AlpineTheme
