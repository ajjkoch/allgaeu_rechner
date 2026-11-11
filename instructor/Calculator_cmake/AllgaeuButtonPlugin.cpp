/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#include "AllgaeuButtonPlugin.h"
#include "AllgaeuButton.h"

AllgaeuButtonPlugin::AllgaeuButtonPlugin(QObject *parent)
    : QObject(parent)
{
}

bool AllgaeuButtonPlugin::isContainer() const { return false; }
bool AllgaeuButtonPlugin::isInitialized() const { return m_initialized; }
QIcon AllgaeuButtonPlugin::icon() const { return {}; }
QString AllgaeuButtonPlugin::group() const { return QStringLiteral("AllgäuRechner"); }
QString AllgaeuButtonPlugin::includeFile() const { return QStringLiteral("AllgaeuButton.h"); }
QString AllgaeuButtonPlugin::name() const { return QStringLiteral("AllgaeuButton"); }
QString AllgaeuButtonPlugin::toolTip() const { return QStringLiteral("Button im Allgäu-Holzstil"); }
QString AllgaeuButtonPlugin::whatsThis() const { return toolTip(); }

QWidget *AllgaeuButtonPlugin::createWidget(QWidget *parent)
{
    return new AllgaeuButton(parent);
}

void AllgaeuButtonPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);
    if (m_initialized)
        return;
    m_initialized = true;
}

QString AllgaeuButtonPlugin::domXml() const
{
    return QStringLiteral(
        "<ui language=\"c++\">"
        " <widget class=\"AllgaeuButton\" name=\"allgaeuButton\">"
        "  <property name=\"text\"><string>Allgäu</string></property>"
        "  <property name=\"woodStyle\"><enum>AllgaeuButton::NaturalWood</enum></property>"
        " </widget>"
        "</ui>");
}
