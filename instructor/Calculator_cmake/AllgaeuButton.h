/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#ifndef ALLGAEU_BUTTON_H
#define ALLGAEU_BUTTON_H

#include <QPushButton>

class QPainter;

class AllgaeuButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(WoodStyle woodStyle READ woodStyle WRITE setWoodStyle NOTIFY woodStyleChanged)

public:
    enum WoodStyle { NaturalWood, LightWood, DarkWood };
    Q_ENUM(WoodStyle)

    explicit AllgaeuButton(QWidget *parent = nullptr);
    WoodStyle woodStyle() const;
    void setWoodStyle(WoodStyle style);

signals:
    void woodStyleChanged(WoodStyle style);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    WoodStyle m_woodStyle = NaturalWood;
};

#endif // ALLGAEU_BUTTON_H
