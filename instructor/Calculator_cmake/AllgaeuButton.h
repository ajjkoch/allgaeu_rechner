/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#ifndef ALLGAEU_BUTTON_H
#define ALLGAEU_BUTTON_H

#include <QPushButton>
#include <QIcon>

class QPainter;

class AllgaeuButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(WoodStyle woodStyle READ woodStyle WRITE setWoodStyle NOTIFY woodStyleChanged)
    Q_PROPERTY(QString alternateText READ alternateText WRITE setAlternateText NOTIFY alternateTextChanged)
    Q_PROPERTY(QIcon alternateIcon READ alternateIcon WRITE setAlternateIcon NOTIFY alternateIconChanged)
    Q_PROPERTY(bool alternateState READ alternateState WRITE setAlternateState NOTIFY alternateStateChanged)

public:
    enum WoodStyle { NaturalWood, LightWood, DarkWood, GreenWood, RedWood, OliveWood, BlueWood, HoneyWood };
    Q_ENUM(WoodStyle)

    explicit AllgaeuButton(QWidget *parent = nullptr);
    WoodStyle woodStyle() const;
    void setWoodStyle(WoodStyle style);

    QString alternateText() const;
    void setAlternateText(const QString &text);
    QIcon alternateIcon() const;
    void setAlternateIcon(const QIcon &icon);
    bool alternateState() const;
    void setAlternateState(bool enabled);

signals:
    void woodStyleChanged(WoodStyle style);
    void alternateTextChanged(const QString &text);
    void alternateIconChanged(const QIcon &icon);
    void alternateStateChanged(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    WoodStyle m_woodStyle = NaturalWood;
    QString m_alternateText;
    QIcon m_alternateIcon;
    bool m_alternateState = false;
};

#endif // ALLGAEU_BUTTON_H
