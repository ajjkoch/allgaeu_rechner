/*
 * Copyright (c) 2026 Koch, Achim Johannes Josef.
 * SPDX-License-Identifier: MIT
 */

#include "AlpineTheme.h"
#include "AllgaeuButton.h"

#include <QPushButton>
#include <QMap>
#include <QWidget>

namespace AlpineTheme {

void apply(QWidget *window)
{
    // Semantic roles remain runtime metadata for tests and accessibility logic.
    // They must never be used here to overwrite Designer-owned appearance.
    const QMap<QString, QString> roles = {
        {"pushButton_e", "constant"}, {"pushButton_pi", "constant"},
        {"pushButton_fact", "function"}, {"pushButton_placeholder3_3", "function"},
        {"pushButton_0", "digit"}, {"pushButton_1", "digit"},
        {"pushButton_2", "digit"}, {"pushButton_3", "digit"},
        {"pushButton_4", "digit"}, {"pushButton_5", "digit"},
        {"pushButton_6", "digit"}, {"pushButton_7", "digit"},
        {"pushButton_8", "digit"}, {"pushButton_9", "digit"},
        {"pushButton_sqrt", "function"}, {"pushButton_x2", "function"},
        {"pushButton_changeDegreesMode", "control"},
        {"pushButton_sin", "function"}, {"pushButton_cos", "function"},
        {"pushButton_tan", "function"}, {"pushButton_log", "function"},
        {"pushButton_ln", "function"}, {"pushButton_edx", "function"},
        {"pushButton_mod", "function"}, {"pushButton_percent", "function"},
        {"pushButton_inv", "function"},
        {"pushButton_plus", "operator"}, {"pushButton_minus", "operator"},
        {"pushButton_mul", "operator"}, {"pushButton_divided", "operator"},
        {"pushButton_equals", "operator"},
        {"pushButton_clear", "utility"}, {"pushButton_erase_left", "utility"},
        {"pushButton_plusminus", "utility"}, {"pushButton_placeholder3_4", "utility"},
        {"pushButton_bracket_left", "utility"}, {"pushButton_bracket_right", "utility"},
        {"pushButton_decimal", "utility"},
        {"pushButton_bracket_right_end", "control"},
        {"pushButton_bracket_left_start", "control"},
        {"pushButton_cursor_left", "control"}, {"pushButton_cursor_right", "control"},
        {"pushButton_history_back", "control"}, {"pushButton_history_forward", "control"}
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
        {"pushButton_placeholder3_3", "Binomialkoeffizient C(n, k) berechnen"},
        {"pushButton_plusminus", "Vorzeichen wechseln"}
    };

    // Visual properties (wood style, text, font, colors and geometry) belong
    // exclusively to mainwindow.ui. This function only supplies non-visual hints
    // and interaction affordances without changing the Designer configuration.
    const auto buttons = window->findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        const QString name = button->objectName();
        if (roles.contains(name))
            button->setProperty("role", roles.value(name));
        if (hints.contains(name))
            button->setToolTip(hints.value(name));
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        button->setAutoDefault(false);
        button->setDefault(false);
    }
}
} // namespace AlpineTheme
