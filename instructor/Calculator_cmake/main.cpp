/*
 * Copyright (c) 2026 Koch, Achim Johannes Josef.
 * SPDX-License-Identifier: MIT
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
