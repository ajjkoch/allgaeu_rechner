/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#include "AllgaeuButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QStyleOptionButton>
#include <QStyle>
#include <QResource>
#include <QResizeEvent>

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
    setMinimumSize(52, 30);
    updateEffectiveIconSize();
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


int AllgaeuButton::iconHeight() const { return m_iconHeight; }
void AllgaeuButton::setIconHeight(int height)
{
    const int value = qMax(1, height);
    if (m_iconHeight == value) return;
    m_iconHeight = value;
    updateEffectiveIconSize();
    emit iconHeightChanged(value);
}
int AllgaeuButton::iconWidthMargin() const { return m_iconWidthMargin; }
void AllgaeuButton::setIconWidthMargin(int margin)
{
    const int value = qMax(0, margin);
    if (m_iconWidthMargin == value) return;
    m_iconWidthMargin = value;
    updateEffectiveIconSize();
    emit iconWidthMarginChanged(value);
}
int AllgaeuButton::minimumIconWidth() const { return m_minimumIconWidth; }
void AllgaeuButton::setMinimumIconWidth(int width)
{
    const int value = qMax(1, width);
    if (m_minimumIconWidth == value) return;
    m_minimumIconWidth = value;
    updateEffectiveIconSize();
    emit minimumIconWidthChanged(value);
}
void AllgaeuButton::updateEffectiveIconSize()
{
    const int effectiveWidth = qMax(m_minimumIconWidth, width() - m_iconWidthMargin);
    const QSize effectiveSize(effectiveWidth, m_iconHeight);
    if (iconSize() != effectiveSize)
        QPushButton::setIconSize(effectiveSize);
}
void AllgaeuButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    updateEffectiveIconSize();
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


    // Let Qt's style engine render the label. Text, font, palette and icon
    // come from the QPushButton properties configured in mainwindow.ui.
    QStyleOptionButton option;
    initStyleOption(&option);
    option.rect = face.toRect();
    if (m_alternateState) {
        if (!m_alternateText.isEmpty())
            option.text = m_alternateText;
        if (!m_alternateIcon.isNull())
            option.icon = m_alternateIcon;
    }
    // Icon takes visual priority. The QPushButton text remains available as
    // the Designer fallback (and accessible name) when an icon is absent.
    if (!option.icon.isNull())
        option.text.clear();
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
}
