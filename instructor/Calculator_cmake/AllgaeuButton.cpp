/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#include "AllgaeuButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QStyleOptionButton>
#include <QStyle>
#include <QResource>

static void ensureAlpineResources()
{
    // The Designer plugin is a separate module. Explicitly initialize its
    // compiled-in .qrc before any QPixmap/QFontDatabase resource lookup.
    if (!QResource(QStringLiteral(":/alpine/mid.jpg")).isValid())
        Q_INIT_RESOURCE(alpine_resources);
}

AllgaeuButton::AllgaeuButton(QWidget *parent)
    : QPushButton(parent)
{
    ensureAlpineResources();
    setAttribute(Qt::WA_Hover, true);
    setMinimumSize(32, 30);
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

QString AllgaeuButton::alternateText() const
{
    return m_alternateText;
}

void AllgaeuButton::setAlternateText(const QString &text)
{
    if (m_alternateText == text)
        return;
    m_alternateText = text;
    update();
    emit alternateTextChanged(text);
}

QIcon AllgaeuButton::alternateIcon() const
{
    return m_alternateIcon;
}

void AllgaeuButton::setAlternateIcon(const QIcon &icon)
{
    if (m_alternateIcon.cacheKey() == icon.cacheKey())
        return;
    m_alternateIcon = icon;
    update();
    emit alternateIconChanged(icon);
}

bool AllgaeuButton::alternateState() const
{
    return m_alternateState;
}

void AllgaeuButton::setAlternateState(bool enabled)
{
    if (m_alternateState == enabled)
        return;
    m_alternateState = enabled;
    update();
    emit alternateStateChanged(enabled);
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

    // Let Qt's style engine render the label. Text, font, palette and icon
    // come from the QPushButton properties configured in mainwindow.ui.
    QStyleOptionButton option;
    initStyleOption(&option);
    option.rect = face.toRect();
    if (m_alternateState && !m_alternateText.isEmpty())
        option.text = m_alternateText;
    if (m_alternateState && !m_alternateIcon.isNull())
        option.icon = m_alternateIcon;
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
}
