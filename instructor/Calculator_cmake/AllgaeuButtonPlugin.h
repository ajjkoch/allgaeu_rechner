/* Copyright (c) 2026 Koch, Achim Johannes Josef. SPDX-License-Identifier: MIT */
#ifndef ALLGAEU_BUTTON_PLUGIN_H
#define ALLGAEU_BUTTON_PLUGIN_H

#include <QObject>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class AllgaeuButtonPlugin final : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface"
                      FILE "AllgaeuButtonPlugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit AllgaeuButtonPlugin(QObject *parent = nullptr);
    bool isContainer() const override;
    bool isInitialized() const override;
    QIcon icon() const override;
    QString domXml() const override;
    QString group() const override;
    QString includeFile() const override;
    QString name() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool m_initialized = false;
};

#endif // ALLGAEU_BUTTON_PLUGIN_H
