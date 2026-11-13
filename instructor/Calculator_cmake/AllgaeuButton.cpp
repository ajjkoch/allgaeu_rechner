/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#include "AllgaeuButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QFontMetricsF>
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


    // Keep the existing Designer-configured labels and SVG icons, but give
    // text labels a warm, engraved appearance instead of Qt's flat text.
    QStyleOptionButton option;
    initStyleOption(&option);
    option.rect = face.toRect();
    if (m_alternateState && !m_alternateText.isEmpty())
        option.text = m_alternateText;
    if (m_alternateState && !m_alternateIcon.isNull())
        option.icon = m_alternateIcon;

    if (!option.text.isEmpty() && option.icon.isNull()) {
        QRectF textRect = style()->subElementRect(
            QStyle::SE_PushButtonContents, &option, this);
        textRect.adjust(3.0, 1.0, -3.0, -1.0);

        QFont labelFont = font();
        const qreal originalPointSize = labelFont.pointSizeF();
        if (originalPointSize > 0.0)
            labelFont.setPointSizeF(originalPointSize + 3.0);
        labelFont.setBold(true);

        // Longer captions keep the same visual weight while being reduced
        // only as much as needed to stay comfortably inside the wooden key.
        if (originalPointSize > 0.0) {
            QFontMetricsF metrics(labelFont);
            while (metrics.horizontalAdvance(option.text) > textRect.width() - 4.0
                   && labelFont.pointSizeF() > originalPointSize) {
                labelFont.setPointSizeF(
                    qMax(originalPointSize, labelFont.pointSizeF() - 0.5));
                metrics = QFontMetricsF(labelFont);
            }
        }

        const QFontMetricsF metrics(labelFont);
        const qreal x = textRect.center().x()
                      - metrics.horizontalAdvance(option.text) / 2.0;
        const qreal baseline = textRect.center().y()
                             + (metrics.ascent() - metrics.descent()) / 2.0;

        QPainterPath lettering;
        lettering.addText(QPointF(x, baseline), labelFont, option.text);
        const QRectF letteringBounds = lettering.boundingRect();

        if (!lettering.isEmpty()) {
            painter.save();
            painter.setRenderHint(QPainter::TextAntialiasing, true);

            const bool darkWood = m_woodStyle == DarkWood;
            const QColor lowerEdge = darkWood
                ? QColor(18, 8, 3, 245)
                : QColor(30, 15, 7, 235);
            const QColor upperEdge = darkWood
                ? QColor(255, 225, 164, 245)
                : QColor(247, 218, 168, 235);

            // The offset edges create the shallow cut: a warm rim catches
            // the light at the upper-left, while the lower-right stays dark.
            painter.fillPath(lettering.translated(1.25, 1.35), lowerEdge);
            painter.fillPath(lettering.translated(-0.8, -0.8), upperEdge);

            QLinearGradient carvedInk(letteringBounds.topLeft(),
                                      letteringBounds.bottomRight());
            if (darkWood) {
                carvedInk.setColorAt(0.0, QColor(255, 211, 126));
                carvedInk.setColorAt(0.55, QColor(220, 151, 65));
                carvedInk.setColorAt(1.0, QColor(157, 91, 37));
            } else {
                carvedInk.setColorAt(0.0, QColor(91, 56, 31));
                carvedInk.setColorAt(0.52, QColor(61, 35, 19));
                carvedInk.setColorAt(1.0, QColor(39, 21, 12));
            }
            painter.fillPath(lettering, carvedInk);
            painter.restore();
        }
    } else {
        // SVG artwork remains rendered by Qt, completely unchanged.
        style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
    }
}
