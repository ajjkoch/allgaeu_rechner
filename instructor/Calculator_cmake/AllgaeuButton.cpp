/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#include "AllgaeuButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QFontDatabase>
#include <QStyleOptionButton>

AllgaeuButton::AllgaeuButton(QWidget *parent)
    : QPushButton(parent)
{
    static const int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/alpine/BreeSerif-Regular.ttf"));
    Q_UNUSED(fontId);
    setAttribute(Qt::WA_Hover, true);
    setMinimumSize(32, 30);
    setFont(QFont("Bree Serif", 21, QFont::Black));
}

AllgaeuButton::WoodStyle AllgaeuButton::woodStyle() const
{
    return m_woodStyle;
}

void AllgaeuButton::setWoodStyle(WoodStyle style)
{
    if (m_woodStyle == style)
        return;
    m_woodStyle = style;
    update();
    emit woodStyleChanged(style);
}

void AllgaeuButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF face = QRectF(rect()).adjusted(1.5, 1.5, -1.5, -2.5);
    if (face.isEmpty())
        return;

    painter.save();
    QPainterPath clip;
    clip.addRoundedRect(face, 10, 10);
    painter.setClipPath(clip);

    static const QPixmap texture(QStringLiteral(":/alpine/mid.jpg"));
    if (!texture.isNull()) {
        const int sourceWidth = qMax(1, texture.width() * 3 / 5);
        const int sourceX = (texture.width() - sourceWidth) / 2;
        painter.drawPixmap(face, texture,
                           QRect(sourceX, 0, sourceWidth, texture.height()));
    } else {
        painter.fillRect(face, QColor("#c58b50"));
    }

    QColor glaze;
    switch (m_woodStyle) {
    case LightWood: glaze = QColor(255, 239, 207, 175); break;
    case DarkWood:  glaze = QColor(67, 37, 19, 145); break;
    case GreenWood: glaze = QColor(91, 128, 62, 112); break;
    case RedWood:   glaze = QColor(174, 65, 43, 125); break;
    case OliveWood: glaze = QColor(143, 145, 69, 88); break;
    case BlueWood:  glaze = QColor(92, 119, 133, 105); break;
    case HoneyWood: glaze = QColor(246, 190, 105, 92); break;
    case NaturalWood:
    default:        glaze = QColor(246, 207, 145, 55); break;
    }
    painter.fillRect(face, glaze);
    if (isDown())
        painter.fillRect(face, QColor(30, 18, 9, 65));
    painter.restore();

    painter.setPen(QPen(QColor("#3b2416"), 1.8));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(face, 10, 10);
    painter.setPen(QPen(QColor("#56351f"), 1.0));
    painter.drawRoundedRect(face.adjusted(2, 2, -2, -3), 8, 8);

    const QColor ink = m_woodStyle == DarkWood ? QColor("#fff3cf") : QColor("#201208");
    painter.setPen(ink);
    QFont buttonFont = font();
    if (buttonFont.family().isEmpty())
        buttonFont = QFont("Bree Serif", 21, QFont::Black);
    painter.setFont(buttonFont);

    const QString name = objectName();
    const QColor ink = name == "pushButton_equals" ? QColor("#fff3cf")
                     : name == "pushButton_clear" ? QColor("#431b12")
                     : m_woodStyle == DarkWood ? QColor("#fff3cf")
                     : QColor("#201208");
    painter.setPen(ink);
    painter.setFont(QFont("Bree Serif", 21, QFont::Black));

    QRect content = face.toRect().adjusted(7, 3, -7, -3);
    if (!icon().isNull()) {
        const QSize actualIconSize = iconSize().isValid() ? iconSize() : QSize(24, 24);
        const QSize fitted = actualIconSize.boundedTo(content.size());
        const QRect iconRect(content.center().x() - fitted.width() / 2,
                             content.center().y() - fitted.height() / 2,
                             fitted.width(), fitted.height());
        icon().paint(&painter, iconRect, Qt::AlignCenter,
                     isEnabled() ? QIcon::Normal : QIcon::Disabled,
                     isDown() ? QIcon::On : QIcon::Off);
    } else if (name == "pushButton_sqrt" && property("inverseSquare").toBool()) {
        painter.drawText(content, Qt::AlignCenter, QStringLiteral("x²"));
    } else if (name == "pushButton_sqrt") {
        const qreal cx = face.center().x();
        const qreal cy = face.center().y();
        QPainterPath radical;
        constexpr qreal scale = 0.61;
        painter.save();
        painter.translate(cx, cy);
        painter.scale(scale, scale);
        radical.moveTo(-19, 2);
        radical.lineTo(-12, 8);
        radical.lineTo(-4, -9);
        radical.lineTo(19, -9);
        painter.setPen(QPen(ink, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(radical);
        painter.restore();
        painter.setPen(ink);
        painter.setFont(QFont("Bree Serif", 14, QFont::Black));
        painter.drawText(QRectF(cx - 1, cy - 8, 17, 18), Qt::AlignCenter, QStringLiteral("x"));
    } else if (name == "pushButton_x2" && property("inverseRoot").toBool()) {
        const qreal cx = face.center().x();
        const qreal cy = face.center().y();
        painter.setFont(QFont("Bree Serif", 14, QFont::Black));
        painter.drawText(QRectF(cx - 23, cy - 16, 15, 15), Qt::AlignCenter, QStringLiteral("x"));
        QPainterPath radical;
        constexpr qreal scale = 0.61;
        painter.save();
        painter.translate(cx, cy);
        painter.scale(scale, scale);
        radical.moveTo(-10, 1);
        radical.lineTo(-5, 7);
        radical.lineTo(1, -8);
        radical.lineTo(22, -8);
        painter.setPen(QPen(ink, 2.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(radical);
        painter.restore();
        painter.setPen(ink);
        painter.setFont(QFont("Bree Serif", 14, QFont::Black));
        painter.drawText(QRectF(cx + 1, cy - 7, 19, 19), Qt::AlignCenter, QStringLiteral("y"));
    } else {
        painter.drawText(content, Qt::AlignCenter | Qt::TextWordWrap, text());
    }
}
