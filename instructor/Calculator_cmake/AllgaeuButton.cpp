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
    } else {
        painter.drawText(content, Qt::AlignCenter | Qt::TextWordWrap, text());
    }
}
