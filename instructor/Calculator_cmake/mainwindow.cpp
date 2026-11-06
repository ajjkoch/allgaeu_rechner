#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextBlockFormat>
#include <QSignalBlocker>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QMap>
#include <QWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QFontDatabase>
#include <QPixmap>
#include <cmath>



namespace {
QIcon makeFunctionIcon(const QString &function)
{
    const QSize size(36, 24);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF plot(2.0, 2.0, 32.0, 20.0);

    QPen axisPen(QColor("#8b9988"), 0.8);
    painter.setPen(axisPen);
    painter.drawLine(QPointF(plot.left(), plot.center().y()),
                     QPointF(plot.right(), plot.center().y()));
    painter.drawLine(QPointF(plot.center().x(), plot.top()),
                     QPointF(plot.center().x(), plot.bottom()));

    QPen curvePen(QColor("#365b43"), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(curvePen);
    painter.setBrush(Qt::NoBrush);

    auto mapPoint = [&plot](qreal x, qreal y) {
        return QPointF(plot.left() + (x + 1.0) * plot.width() / 2.0,
                       plot.center().y() - y * plot.height() / 2.0);
    };

    if (function == "tan") {
        // Three clipped branches; dashed lines mark the vertical asymptotes.
        const qreal xMin = -2.2;
        const qreal xMax = 2.2;
        const qreal gap = 0.09;
        const qreal asymptote = 1.5707963267948966;
        const qreal yLimit = 3.2;
        const qreal ranges[3][2] = {
            {xMin, -asymptote - gap},
            {-asymptote + gap, asymptote - gap},
            {asymptote + gap, xMax}
        };

        for (const auto &range : ranges) {
            QPainterPath path;
            for (int i = 0; i <= 80; ++i) {
                const qreal x = range[0] + (range[1] - range[0]) * i / 80.0;
                const qreal y = qBound(-yLimit, std::tan(x), yLimit) / yLimit;
                const QPointF point = mapPoint(x / xMax, y);
                if (i == 0) path.moveTo(point);
                else path.lineTo(point);
            }
            painter.drawPath(path);
        }

        QPen asymptotePen(QColor("#a28d68"), 0.9, Qt::DashLine);
        painter.setPen(asymptotePen);
        for (qreal x : {-asymptote, asymptote}) {
            const qreal normalizedX = x / xMax;
            painter.drawLine(mapPoint(normalizedX, -1.0),
                             mapPoint(normalizedX, 1.0));
        }
    } else {
        QPainterPath path;
        for (int i = 0; i <= 100; ++i) {
            const qreal x = -M_PI;
            const qreal angle = x + (2.0 * M_PI * i / 100.0);
            const qreal y = function == "sin" ? std::sin(angle) : std::cos(angle);
            const QPointF point = mapPoint(angle / M_PI, y * 0.82);
            if (i == 0) path.moveTo(point);
            else path.lineTo(point);
        }
        painter.drawPath(path);
    }

    return QIcon(pixmap);
}

class AlpineBackground final : public QWidget
{
public:
    explicit AlpineBackground(QWidget *parent = nullptr) : QWidget(parent)
    {
        setObjectName("alpineBackground");
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_StyledBackground, false);

        m_header.load(":/alpine/header.jpg");
        m_wood.load(":/alpine/mid.jpg");
        m_footer.load(":/alpine/footer.jpg");
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.fillRect(rect(), QColor("#8b552d"));

        if (m_header.isNull() || m_wood.isNull() || m_footer.isNull())
            return;

        // Header and footer keep their original proportions. The middle image
        // fills the remaining area and keeps its framed side rails continuous.
        int headerHeight = qRound(width() * m_header.height()
                                  / qreal(m_header.width()));
        int footerHeight = qRound(width() * m_footer.height()
                                  / qreal(m_footer.width()));

        // On unusually short windows, scale both fixed bands down together
        // rather than allowing them to cover the calculator controls.
        const int availableBandsHeight = qMax(0, height() - 1);
        const int requestedBandsHeight = headerHeight + footerHeight;
        if (requestedBandsHeight > availableBandsHeight && requestedBandsHeight > 0) {
            const qreal scale = availableBandsHeight / qreal(requestedBandsHeight);
            headerHeight = qRound(headerHeight * scale);
            footerHeight = qMax(0, availableBandsHeight - headerHeight);
        }

        const int woodHeight = qMax(0, height() - headerHeight - footerHeight);
        painter.drawPixmap(QRect(0, 0, width(), headerHeight), m_header);
        painter.drawPixmap(QRect(0, headerHeight, width(), woodHeight), m_wood);
        painter.drawPixmap(QRect(0, headerHeight + woodHeight, width(), footerHeight), m_footer);
    }

private:
    QPixmap m_header;
    QPixmap m_wood;
    QPixmap m_footer;
};
class AlpineHeader final : public QWidget
{
public:
    explicit AlpineHeader(QWidget *parent = nullptr) : QWidget(parent)
    {
        setObjectName("alpineHeader");
        setFixedHeight(88);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const QRectF r=rect().adjusted(1,1,-1,-1);
        QLinearGradient wood(r.topLeft(),r.bottomLeft());
        wood.setColorAt(0,QColor("#f5d49a"));
        wood.setColorAt(.22,QColor("#d99a50"));
        wood.setColorAt(.55,QColor("#f0bd75"));
        wood.setColorAt(1,QColor("#985321"));
        p.setPen(QPen(QColor("#4b2b12"),2)); p.setBrush(wood);
        p.drawRoundedRect(r,7,7);
        p.save();
        QPainterPath clip; clip.addRoundedRect(r.adjusted(2,2,-2,-2),6,6); p.setClipPath(clip);
        for(int i=0;i<17;++i) {
            qreal y=5+i*4.7;
            QPainterPath grain; grain.moveTo(0,y);
            grain.cubicTo(width()*.22,y-3,width()*.53,y+3,width(),y-1);
            p.setPen(QPen(QColor(94,48,15,i%2?30:19),i%4==0?1.4:.7)); p.drawPath(grain);
        }
        p.restore();

        // Engraved Allgäu mountain silhouette on the left.
        const QPointF base(10,height()-10);
        QPainterPath ridge; ridge.moveTo(base);
        ridge.lineTo(width()*.06,height()*.55); ridge.lineTo(width()*.10,height()*.62);
        ridge.lineTo(width()*.16,height()*.20); ridge.lineTo(width()*.21,height()*.49);
        ridge.lineTo(width()*.25,height()*.34); ridge.lineTo(width()*.29,height()*.69);
        p.setPen(QPen(QColor("#4a2a12"),3.0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        p.setBrush(QColor(110,67,29,35)); p.drawPath(ridge);
        QPainterPath snow; snow.moveTo(width()*.105,height()*.48);
        snow.lineTo(width()*.19,height()*.15); snow.lineTo(width()*.22,height()*.43);
        snow.lineTo(width()*.19,height()*.34); snow.lineTo(width()*.17,height()*.39);
        snow.lineTo(width()*.15,height()*.34);
        p.setPen(QPen(QColor("#fff0c9"),2)); p.setBrush(QColor("#fff0c9")); p.drawPath(snow);

        // Pine silhouettes along the lower edge.
        p.setPen(QPen(QColor("#392411"),1.2)); p.setBrush(QColor("#314326"));
        for(int i=0;i<5;++i) {
            qreal x=8+i*13, y=height()-7-(i%2)*3;
            QPainterPath tree; tree.moveTo(x,y-23); tree.lineTo(x-7,y-7);
            tree.lineTo(x-3,y-8); tree.lineTo(x-9,y-2); tree.lineTo(x+9,y-2);
            tree.lineTo(x+3,y-8); tree.lineTo(x+7,y-7); tree.closeSubpath(); p.drawPath(tree);
        }

        p.setPen(QColor("#2b1609"));
        const QRect titleRect(int(width()*.245), 5, int(width()*.52), int(height()*.46));
        QFont title("Georgia", 20, QFont::Bold);
        title.setLetterSpacing(QFont::AbsoluteSpacing, 0.0);
        QFontMetrics titleMetrics(title);
        while (title.pointSize() > 10 && titleMetrics.horizontalAdvance("ALLGÄU RECHNER") > titleRect.width()) {
            title.setPointSize(title.pointSize()-1);
            titleMetrics = QFontMetrics(title);
        }
        p.setFont(title);
        p.drawText(titleRect, Qt::AlignCenter | Qt::TextSingleLine, "ALLGÄU RECHNER");
        const QRect subtitleRect(int(width()*.25), int(height()*.53), int(width()*.51), int(height()*.23));
        QFont sub("Georgia", 10, QFont::Bold); p.setFont(sub);
        p.drawText(subtitleRect, Qt::AlignCenter | Qt::TextSingleLine, "RECHNEN MIT WEITBLICK");

        // Layered edelweiss emblem on the right.
        const QPointF flower(width()-27,height()*.48);
        p.setPen(QPen(QColor("#70421c"),.8)); p.setBrush(QColor("#fff9e8"));
        for(int i=0;i<8;++i) {
            p.save(); p.translate(flower); p.rotate(i*45.0);
            p.drawEllipse(QRectF(-4.1,-17,8.2,18)); p.restore();
        }
        p.setBrush(QColor("#e4b52f")); p.drawEllipse(QRectF(flower.x()-6,flower.y()-6,12,12));
        p.setBrush(QColor("#7b4b19")); p.drawEllipse(QRectF(flower.x()-2,flower.y()-2,4,4));
        p.setPen(Qt::NoPen); p.setBrush(QColor("#3c642d"));
        QPainterPath leaf; leaf.moveTo(width()-42,height()*.62);
        leaf.quadTo(width()-53,height()*.49,width()-49,height()*.30);
        leaf.quadTo(width()-32,height()*.39,width()-42,height()*.62); p.drawPath(leaf);
    }
};

class AlpineFooter final : public QWidget
{
public:
    explicit AlpineFooter(QWidget *parent=nullptr):QWidget(parent) {
        setObjectName("alpineFooter"); setFixedHeight(54);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
        QLinearGradient g(0,0,0,height());
        g.setColorAt(0,QColor("#e9b96f")); g.setColorAt(.5,QColor("#c98742")); g.setColorAt(1,QColor("#70401d"));
        p.fillRect(rect(),g);
        for(int i=0;i<9;++i) {
            QPainterPath grain; qreal y=4+i*6;
            grain.moveTo(0,y); grain.cubicTo(width()*.3,y-2,width()*.7,y+2,width(),y);
            p.setPen(QPen(QColor(73,38,15,35),1)); p.drawPath(grain);
        }
        const qreal w=width(), h=height();
        QPainterPath peaks; peaks.moveTo(0,h*.78);
        peaks.lineTo(w*.12,h*.46); peaks.lineTo(w*.18,h*.63);
        peaks.lineTo(w*.31,h*.18); peaks.lineTo(w*.39,h*.55);
        peaks.lineTo(w*.51,h*.08); peaks.lineTo(w*.62,h*.56);
        peaks.lineTo(w*.72,h*.31); peaks.lineTo(w*.82,h*.63);
        peaks.lineTo(w*.91,h*.42); peaks.lineTo(w,h*.75);
        p.setPen(QPen(QColor("#42240f"),3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        p.setBrush(QColor("#9a6738")); p.drawPath(peaks);
        QPainterPath snow; snow.moveTo(w*.25,h*.43); snow.lineTo(w*.31,h*.18);
        snow.lineTo(w*.36,h*.45); snow.lineTo(w*.32,h*.37); snow.lineTo(w*.30,h*.41);
        snow.lineTo(w*.28,h*.34); snow.lineTo(w*.25,h*.43);
        snow.moveTo(w*.45,h*.35); snow.lineTo(w*.51,h*.08); snow.lineTo(w*.57,h*.39);
        snow.lineTo(w*.52,h*.29); snow.lineTo(w*.49,h*.33); snow.closeSubpath();
        p.setPen(QPen(QColor("#fff0ca"),1.5)); p.setBrush(QColor("#f5e2b7")); p.drawPath(snow);
        p.setPen(QPen(QColor("#3b2614"),1)); p.setBrush(QColor("#263d20"));
        for(int side=0;side<2;++side) for(int i=0;i<5;++i) {
            qreal x=side? w-8-i*13:8+i*13, y=h-2-(i%2)*3;
            QPainterPath t; t.moveTo(x,y-22); t.lineTo(x-7,y-8); t.lineTo(x-3,y-9);
            t.lineTo(x-9,y-2); t.lineTo(x+9,y-2); t.lineTo(x+3,y-9);
            t.lineTo(x+7,y-8); t.closeSubpath(); p.drawPath(t);
        }
    }
};

void paintAlpineButton(QPushButton *button)
{
    QPainter painter(button);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF r = button->rect().adjusted(1.5, 1.5, -1.5, -2.5);
    const QString role = button->property("role").toString();
    const bool pressed = button->isDown();

    // Reuse the actual wood photograph from the calculator's middle background.
    // A narrow central crop avoids copying the side frame into the key faces.
    static const QPixmap woodTexture(":/alpine/mid.jpg");
    painter.save();
    QPainterPath faceClip;
    faceClip.addRoundedRect(r, 11, 11);
    painter.setClipPath(faceClip);
    if (!woodTexture.isNull()) {
        const int sourceWidth = qMax(1, woodTexture.width() * 3 / 5);
        const int sourceX = (woodTexture.width() - sourceWidth) / 2;
        painter.drawPixmap(r, woodTexture,
                           QRect(sourceX, 0, sourceWidth, woodTexture.height()));
    } else {
        painter.fillRect(r, QColor("#c58b50"));
    }

    // Retain the existing key categories as subtle stains over the same wood.
    QColor tint(0, 0, 0, 0);
    if (role == "function" || role == "operator")
        tint = QColor(132, 145, 65, 78);
    else if (role == "utility" || role == "constant")
        tint = QColor(246, 207, 145, 65);
    if (button->objectName() == "pushButton_clear")
        tint = QColor(174, 65, 43, 125);
    if (button->objectName() == "pushButton_equals")
        tint = QColor(27, 105, 14, 205);
    if (pressed)
        tint = tint.isValid() ? QColor(tint.red(), tint.green(), tint.blue(),
                                      qMin(220, tint.alpha() + 35))
                              : QColor(45, 25, 12, 75);
    if (tint.alpha() > 0)
        painter.fillRect(r, tint);
    painter.restore();

    painter.setPen(QPen(QColor("#70451f"), 1.2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(r, 11, 11);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255,239,197,225),1.8));
    painter.drawRoundedRect(r.adjusted(2,2,-2,-3),9,9);
    painter.setPen(QPen(QColor(65,37,18,150),1.0));
    painter.drawLine(QPointF(r.left()+8,r.bottom()-3),QPointF(r.right()-8,r.bottom()-3));

    const QString name = button->objectName();
    const bool trig = name=="pushButton_sin" || name=="pushButton_cos" || name=="pushButton_tan";
    const bool function = role=="function" || trig;
    const QColor ink = name=="pushButton_equals" ? QColor("#fff3cf") :
                       name=="pushButton_clear" ? QColor("#431b12") : QColor("#291307");
    // Keep lettering proportional when the reserved background bands reduce key height.
    const qreal contentScale = qBound(0.62, button->height() / 36.0, 1.0);
    static const int alpineFontId = QFontDatabase::addApplicationFont(":/alpine/BreeSerif-Regular.ttf");
    Q_UNUSED(alpineFontId);
    QFont f("Bree Serif", function ? 19 : 26, QFont::Bold);
    if (name=="pushButton_pi" || name=="pushButton_sqrt") f.setPointSize(31);
    if (name=="pushButton_changeDegreesMode") f.setPointSize(23);
    if (name=="pushButton_edx") f.setPointSize(23);
    if (name=="pushButton_equals") f.setPointSize(31);
    if (name=="pushButton_erase_left") f.setPointSize(20);
    f.setPointSizeF(f.pointSizeF() * contentScale);
    f.setItalic(function || name=="pushButton_pi" || name=="pushButton_changeDegreesMode");
    // Keep function labels crisp: no offset lettering or drop shadow.
    painter.setPen(ink);
    painter.setFont(f);

    if (trig) {
        QIcon icon = button->icon();
        const int graphWidth = qRound(52 * contentScale);
        const int graphHeight = qRound(25 * contentScale);
        const QRect graphRect(button->rect().center().x()-graphWidth/2,
                              button->rect().top()+qRound(5 * contentScale),
                              graphWidth, graphHeight);
        icon.paint(&painter, graphRect);
        // Move only the sin/cos/tan label upward; leave the curve untouched.
        painter.drawText(QRectF(r.left()+3,r.center().y()+1,r.width()-6,r.height()/2-3),
                         Qt::AlignHCenter|Qt::AlignVCenter,button->text());
    } else if (name=="pushButton_pi" || name=="pushButton_sqrt" || name=="pushButton_x2" ||
               name=="pushButton_changeDegreesMode") {
        if (name=="pushButton_sqrt") {
            // Standard square-root notation: radical with an x under the vinculum.
            const qreal cx = r.center().x();
            const qreal cy = r.center().y();
            QPainterPath radical;
            radical.moveTo(cx - 19, cy + 2);
            radical.lineTo(cx - 12, cy + 8);
            radical.lineTo(cx - 4, cy - 9);
            radical.lineTo(cx + 19, cy - 9);
            painter.setPen(QPen(ink, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPath(radical);
            QFont radicandFont("Georgia", qMax(10, qRound(17 * contentScale)), QFont::Bold);
            painter.setFont(radicandFont);
            painter.drawText(QRectF(cx-1, cy-8, 17, 18), Qt::AlignCenter, "x");
        } else if (name=="pushButton_pi") {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        } else if (name=="pushButton_x2") {
            if (button->property("inverseRoot").toBool()) {
                const qreal cx = r.center().x();
                const qreal cy = r.center().y();
                QFont indexFont("Georgia", qMax(9, qRound(13 * contentScale)), QFont::Bold);
                painter.setFont(indexFont);
                painter.drawText(QRectF(cx-23, cy-16, 15, 15), Qt::AlignCenter, "x");
                QPainterPath radical;
                radical.moveTo(cx-10, cy+1);
                radical.lineTo(cx-5, cy+7);
                radical.lineTo(cx+1, cy-8);
                radical.lineTo(cx+22, cy-8);
                painter.setPen(QPen(ink, 2.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawPath(radical);
                QFont radicandFont("Georgia", qMax(11, qRound(18 * contentScale)), QFont::Bold);
                painter.setFont(radicandFont);
                painter.drawText(QRectF(cx+1, cy-7, 19, 19), Qt::AlignCenter, "y");
            } else {
                painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, "xʸ");
            }
        } else {
            painter.drawText(r.adjusted(3, 2, -3, -2), Qt::AlignCenter, button->text());
        }
    } else if (name=="pushButton_erase_left") {
        // Shift the complete backspace symbol (outline and cross) 3 px left.
        painter.save();
        painter.translate(-3.0, 0.0);
        // Center the backspace glyph and keep at least 3 px clear space to
        // every button edge, independent of the actual key dimensions.
        const QRectF bounds = QRectF(r).adjusted(4, 4, -4, -6);
        const qreal cx = bounds.center().x() - 2.0;
        const qreal cy = bounds.center().y();
        const qreal halfH = bounds.height() * 0.42;
        const qreal left = bounds.left() + bounds.width() * 0.12;
        const qreal right = bounds.right() - bounds.width() * 0.08;
        const qreal notch = bounds.left() + bounds.width() * 0.42;
        QPainterPath key;
        key.moveTo(left, cy);
        key.lineTo(notch, cy-halfH);
        key.lineTo(right, cy-halfH);
        key.lineTo(right, cy+halfH);
        key.lineTo(notch, cy+halfH);
        key.closeSubpath();
        painter.setPen(QPen(Qt::black,3.0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        painter.drawPath(key);
        painter.setPen(QPen(Qt::black,2.4,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        const qreal cross = qMin(bounds.width(), bounds.height()) * 0.16;
        painter.drawLine(QPointF(cx+cross,cy-cross),QPointF(cx-cross,cy+cross));
        painter.drawLine(QPointF(cx-cross,cy-cross),QPointF(cx+cross,cy+cross));
        painter.restore();
    } else {
        painter.drawText(r.adjusted(3,2,-3,-3),Qt::AlignCenter,button->text());
    }
}

void applyAlpineTheme(QWidget *window)
{
    const QMap<QString, QString> roles = {
        {"pushButton_0", "digit"}, {"pushButton_1", "digit"},
        {"pushButton_2", "digit"}, {"pushButton_3", "digit"},
        {"pushButton_4", "digit"}, {"pushButton_5", "digit"},
        {"pushButton_6", "digit"}, {"pushButton_7", "digit"},
        {"pushButton_8", "digit"}, {"pushButton_9", "digit"},
        {"pushButton_pi", "constant"}, {"pushButton_sqrt", "operator"},
        {"pushButton_changeDegreesMode", "constant"},
        {"pushButton_sin", "function"}, {"pushButton_cos", "function"},
        {"pushButton_tan", "function"}, {"pushButton_log", "function"},
        {"pushButton_ln", "function"}, {"pushButton_edx", "function"},
        {"pushButton_mod", "function"}, {"pushButton_percent", "function"},
        {"pushButton_inv", "function"}, {"pushButton_x2", "function"},
        {"pushButton_plus", "operator"}, {"pushButton_minus", "operator"},
        {"pushButton_mul", "operator"}, {"pushButton_divided", "operator"},
        {"pushButton_equals", "operator"},
        {"pushButton_clear", "utility"}, {"pushButton_erase_left", "utility"},
        {"pushButton_plusminus", "utility"},
        {"pushButton_bracket_left", "utility"}, {"pushButton_bracket_right", "utility"},
        {"pushButton_bracket_right_end", "utility"},
        {"pushButton_decimal", "utility"}, {"pushButton_cursor_left", "utility"},
        {"pushButton_cursor_right", "utility"}, {"pushButton_history_back", "utility"},
        {"pushButton_history_forward", "utility"}
    };

    // Each key gets a concise, engraved-looking symbol where that improves recognition.
    // Object names and calculatorActionId remain the only source of action semantics.
    const QMap<QString, QString> labels = {
        {"pushButton_plusminus", "±"}, {"pushButton_sqrt", "√"},
        {"pushButton_x2", "xʸ"}, {"pushButton_edx", "1/x"},
        {"pushButton_inv", "INV"}, {"pushButton_mul", "×"},
        {"pushButton_bracket_right_end", "→)"},
        {"pushButton_erase_left", ""}, {"pushButton_cursor_left", "←"},
        {"pushButton_cursor_right", "→"}, {"pushButton_history_back", "↑"},
        {"pushButton_history_forward", "↓"}
    };

    const QMap<QString, QString> hints = {
        {"pushButton_pi", "Konstante π einfügen"},
        {"pushButton_sqrt", "Quadratwurzel √x berechnen"},
        {"pushButton_x2", "Potenz x hoch y; mit INV: x-te Wurzel aus y"},
        {"pushButton_edx", "Kehrwert 1/x berechnen"},
        {"pushButton_inv", "Umkehrfunktionen aktivieren"},
        {"pushButton_changeDegreesMode", "Winkelmaß zwischen Grad und Radiant wechseln"},
        {"pushButton_bracket_right_end", "Schließende Klammer am Ende des Ausdrucks einfügen"},
        {"pushButton_clear", "Eingabe vollständig löschen"},
        {"pushButton_erase_left", "Zeichen links vom Cursor löschen"},
        {"pushButton_equals", "Ausdruck berechnen"},
        {"pushButton_cursor_left", "Cursor nach links bewegen"},
        {"pushButton_cursor_right", "Cursor nach rechts bewegen"},
        {"pushButton_history_back", "Vorherigen Verlaufseintrag anzeigen"},
        {"pushButton_history_forward", "Nächsten Verlaufseintrag anzeigen"},
        {"pushButton_percent", "Prozentfunktion"},
        {"pushButton_mod", "Modulo – Rest einer Division"},
        {"pushButton_plusminus", "Vorzeichen wechseln"}
    };

    window->setStyleSheet(R"(
        QMainWindow, QWidget#centralwidget {
            background: transparent;
        }
        QTextEdit#textEdit {
            background-color: #f6f0d9; color: #28190c;
            border: 2px solid #6b431f; border-top: 2px solid #f5d99e;
            border-bottom: 3px solid #39200e; border-radius: 11px;
            padding: 2px 8px;
        }
        QPushButton { background: transparent; border: none; padding: 1px; }
        QWidget#alpineHeader, QWidget#alpineFooter { border: 2px solid #70451f; border-radius: 7px; }
    )");


    const QMap<QString, QString> functionPlots = {
        {"pushButton_sin", "sin"}, {"pushButton_cos", "cos"},
        {"pushButton_tan", "tan"}
    };

    const auto buttons = window->findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        const QString name = button->objectName();
        if (labels.contains(name))
            button->setText(labels.value(name));
        if (roles.contains(name))
            button->setProperty("role", roles.value(name));
        if (hints.contains(name))
            button->setToolTip(hints.value(name));
        if (functionPlots.contains(name)) {
            button->setIcon(makeFunctionIcon(functionPlots.value(name)));
            button->setIconSize(QSize(36, 24));
        }
        if (name == "pushButton_edx")
            button->setText("1/x");
        button->setMinimumHeight(30);
        button->installEventFilter(window);
        button->setFocusPolicy(Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        button->setAutoDefault(false);
        button->setDefault(false);
    }
}
} // namespace

/**
 * @brief Constructs the calculator window and wires UI controls to slots.
 * @param parent Optional Qt parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_strExpression("")
    , m_isError(false)
    , m_isInverted(false)
    , m_hpos(0)
{
    ui->setupUi(this);

    // Split the existing pi key vertically and place Euler's constant below it.
    auto *piButton = ui->pushButton_pi;
    auto *constantStack = new QWidget(piButton->parentWidget());
    constantStack->setObjectName("constantButtonStack");
    auto *constantLayout = new QVBoxLayout(constantStack);
    constantLayout->setContentsMargins(0, 0, 0, 0);
    constantLayout->setSpacing(1);
    ui->gridLayout->removeWidget(piButton);
    piButton->setParent(constantStack);
    constantLayout->addWidget(piButton);
    auto *eButton = new QPushButton(QString::fromUtf8("e"), constantStack);
    eButton->setObjectName("pushButton_e");
    eButton->setFont(piButton->font());
    eButton->setStyleSheet(piButton->styleSheet());
    eButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    eButton->setFocusPolicy(Qt::NoFocus);
    constantLayout->addWidget(eButton);
    ui->gridLayout->addWidget(constantStack, 1, 0);

    // Share the angle-mode key's grid cell with a dedicated end-parenthesis key.
    auto *angleButton = ui->pushButton_changeDegreesMode;
    auto *angleStack = new QWidget(angleButton->parentWidget());
    angleStack->setObjectName("angleButtonStack");
    auto *angleLayout = new QVBoxLayout(angleStack);
    angleLayout->setContentsMargins(0, 0, 0, 0);
    angleLayout->setSpacing(1);
    ui->gridLayout->removeWidget(angleButton);
    angleButton->setParent(angleStack);
    angleLayout->addWidget(angleButton);
    auto *rightEndButton = new QPushButton(")", angleStack);
    rightEndButton->setObjectName("pushButton_bracket_right_end");
    rightEndButton->setFont(angleButton->font());
    rightEndButton->setStyleSheet(angleButton->styleSheet());
    rightEndButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    rightEndButton->setFocusPolicy(Qt::NoFocus);
    angleLayout->addWidget(rightEndButton);
    ui->gridLayout->addWidget(angleStack, 1, 1);

    applyAlpineTheme(this);
    auto *background = new AlpineBackground(ui->centralwidget);
    background->setGeometry(ui->centralwidget->rect());
    background->lower();
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setContentsMargins(7, 7, 7, 7);

    // Shift the complete calculator grid down by roughly one display-row
    // height so the header is more visible. Keeping the combined clearance
    // stretch unchanged preserves the grid's overall height.
    auto *topClearance = new QWidget(ui->centralwidget);
    auto *bottomClearance = new QWidget(ui->centralwidget);
    topClearance->setObjectName("topBackgroundClearance");
    bottomClearance->setObjectName("bottomBackgroundClearance");
    topClearance->setAttribute(Qt::WA_TransparentForMouseEvents);
    bottomClearance->setAttribute(Qt::WA_TransparentForMouseEvents);
    topClearance->setStyleSheet("background: transparent;");
    bottomClearance->setStyleSheet("background: transparent;");
    ui->verticalLayout->insertWidget(0, topClearance, 80);
    ui->verticalLayout->insertWidget(2, bottomClearance, 22);
    ui->verticalLayout->setStretch(0, 80);
    ui->verticalLayout->setStretch(1, 300);
    ui->verticalLayout->setStretch(2, 22);

    // Keep the outer keys clear of the wooden side rails.
    ui->gridLayout->setContentsMargins(5, 0, 5, 0);
    ui->gridLayout->setHorizontalSpacing(5);
    ui->gridLayout->setVerticalSpacing(5);

    // Every occupied grid row uses the tallest control in that row as its
    // minimum height, so neighboring keys line up exactly.
    for (int row = 0; row < ui->gridLayout->rowCount(); ++row) {
        int tallest = 0;
        for (int column = 0; column < ui->gridLayout->columnCount(); ++column) {
            if (QLayoutItem *item = ui->gridLayout->itemAtPosition(row, column)) {
                if (QWidget *widget = item->widget())
                    tallest = qMax(tallest, widget->sizeHint().height());
            }
        }
        if (tallest == 0)
            continue;
        for (int column = 0; column < ui->gridLayout->columnCount(); ++column) {
            if (QLayoutItem *item = ui->gridLayout->itemAtPosition(row, column)) {
                if (QWidget *widget = item->widget()) {
                    widget->setMinimumHeight(tallest);
                    widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
                }
            }
        }
    }
    // Let the grid distribute the available height naturally. A fixed maximum
    // here capped every key at 30 px on common phone sizes and clipped its artwork.
    // Use one stable display font. Resizing individual controls must never
    // change the expression font or cause visible vertical bouncing.
    QFont expressionFont("Georgia", 18, QFont::Bold);
    ui->textEdit->setFont(expressionFont);
    ui->textEdit->setMinimumHeight(38);
    ui->textEdit->setMaximumHeight(42);

    ui->textEdit->installEventFilter(this);
    ui->centralwidget->installEventFilter(this);

    // Keep the internal expression synchronized when the user edits the display directly.
    connect(ui->textEdit, &QTextEdit::textChanged, this, [this]() {
        m_strExpression = ui->textEdit->toPlainText();
        m_isError = false;
    });

    // Every visual control exposes the same action-ID contract. The widget
    // type and its label are deliberately irrelevant to action dispatch.
    using Action = CalculatorAction;
    const QMap<QString, Action> actions = {
        {"pushButton_0", Action::Digit0}, {"pushButton_1", Action::Digit1},
        {"pushButton_2", Action::Digit2}, {"pushButton_3", Action::Digit3},
        {"pushButton_4", Action::Digit4}, {"pushButton_5", Action::Digit5},
        {"pushButton_6", Action::Digit6}, {"pushButton_7", Action::Digit7},
        {"pushButton_8", Action::Digit8}, {"pushButton_9", Action::Digit9},
        {"pushButton_plus", Action::Add}, {"pushButton_minus", Action::Subtract},
        {"pushButton_mul", Action::Multiply}, {"pushButton_divided", Action::Divide},
        {"pushButton_equals", Action::Equals}, {"pushButton_clear", Action::Clear},
        {"pushButton_erase_left", Action::Backspace},
        {"pushButton_plusminus", Action::ToggleSign},
        {"pushButton_decimal", Action::Decimal},
        {"pushButton_bracket_left", Action::OpenParenthesis},
        {"pushButton_bracket_right", Action::CloseParenthesis},
        {"pushButton_bracket_right_end", Action::EndCloseParenthesis},
        {"pushButton_sqrt", Action::SquareRoot}, {"pushButton_x2", Action::Power},
        {"pushButton_sin", Action::Sine}, {"pushButton_cos", Action::Cosine},
        {"pushButton_tan", Action::Tangent}, {"pushButton_log", Action::Log10},
        {"pushButton_ln", Action::NaturalLog}, {"pushButton_inv", Action::Inverse},
        {"pushButton_mod", Action::Modulo}, {"pushButton_edx", Action::Reciprocal},
        {"pushButton_percent", Action::Percent}, {"pushButton_pi", Action::Pi},
        {"pushButton_changeDegreesMode", Action::ToggleAngleMode},
        {"pushButton_cursor_left", Action::CursorLeft},
        {"pushButton_cursor_right", Action::CursorRight},
        {"pushButton_history_back", Action::HistoryBack},
        {"pushButton_history_forward", Action::HistoryForward}
    };

    eButton->setProperty("calculatorActionId", static_cast<int>(Action::Euler));
    connect(eButton, &QPushButton::clicked, this, [this, eButton]() {
        dispatchAction(static_cast<CalculatorAction>(
            eButton->property("calculatorActionId").toInt()));
    });

    for (auto it = actions.cbegin(); it != actions.cend(); ++it) {
        auto *button = findChild<QPushButton *>(it.key());
        if (!button)
            continue;

        button->setProperty("calculatorActionId", static_cast<int>(it.value()));
        button->setCheckable(false);
        button->setFocusPolicy(Qt::NoFocus);
        connect(button, &QPushButton::clicked, this, [this, button]() {
            const auto action = static_cast<CalculatorAction>(
                button->property("calculatorActionId").toInt());
            dispatchAction(action);
        });
    }

    setRightAlignedText(ui->textEdit, m_strExpression);
}



/** @brief Executes an action identified independently of its visual control. */
void MainWindow::dispatchAction(CalculatorAction action)
{
    switch (action) {
    case CalculatorAction::Digit0: digit_pressed(0); break;
    case CalculatorAction::Digit1: digit_pressed(1); break;
    case CalculatorAction::Digit2: digit_pressed(2); break;
    case CalculatorAction::Digit3: digit_pressed(3); break;
    case CalculatorAction::Digit4: digit_pressed(4); break;
    case CalculatorAction::Digit5: digit_pressed(5); break;
    case CalculatorAction::Digit6: digit_pressed(6); break;
    case CalculatorAction::Digit7: digit_pressed(7); break;
    case CalculatorAction::Digit8: digit_pressed(8); break;
    case CalculatorAction::Digit9: digit_pressed(9); break;
    case CalculatorAction::Add: insertRightAlignedText(ui->textEdit, "+"); break;
    case CalculatorAction::Subtract: insertRightAlignedText(ui->textEdit, "-"); break;
    case CalculatorAction::Multiply: insertRightAlignedText(ui->textEdit, "*"); break;
    case CalculatorAction::Divide: insertRightAlignedText(ui->textEdit, "/"); break;
    case CalculatorAction::Equals: equals_pressed(); break;
    case CalculatorAction::Clear: clear_pressed(); break;
    case CalculatorAction::Backspace: erase_left_pressed(); break;
    case CalculatorAction::ToggleSign: unary_operation_pressed(); break;
    case CalculatorAction::Decimal: decimal_pressed(); break;
    case CalculatorAction::OpenParenthesis: left_bracked_pressed(); break;
    case CalculatorAction::CloseParenthesis: right_bracked_pressed(); break;
    case CalculatorAction::EndCloseParenthesis: {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);
        right_bracked_pressed();
        break;
    }
    case CalculatorAction::SquareRoot: sqrt_pressed(); break;
    case CalculatorAction::Power: power_pressed(); break;
    case CalculatorAction::Sine: sin_pressed(); break;
    case CalculatorAction::Cosine: cos_pressed(); break;
    case CalculatorAction::Tangent: tan_pressed(); break;
    case CalculatorAction::Log10: log_pressed(); break;
    case CalculatorAction::NaturalLog: ln_pressed(); break;
    case CalculatorAction::Inverse: inv_pressed(); break;
    case CalculatorAction::Modulo: mod_pressed(); break;
    case CalculatorAction::Reciprocal: edx_pressed(); break;
    case CalculatorAction::Percent: percent_pressed(); break;
    case CalculatorAction::Pi: pi_pressed(); break;
    case CalculatorAction::Euler: insertRightAlignedText(ui->textEdit, "e"); break;
    case CalculatorAction::ToggleAngleMode: degreesMode_pressed(); break;
    case CalculatorAction::CursorLeft: move_left_pressed(); break;
    case CalculatorAction::CursorRight: move_right_pressed(); break;
    case CalculatorAction::HistoryBack: history_back_pressed(); break;
    case CalculatorAction::HistoryForward: history_forward_pressed(); break;
    }
}

/** @brief Evaluates the expression when Return or keypad Enter is pressed in the display. */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->centralwidget && event->type() == QEvent::Resize) {
        if (auto *background = ui->centralwidget->findChild<QWidget *>("alpineBackground"))
            background->setGeometry(ui->centralwidget->rect());
        // Button heights are managed by the expanding grid layout.
    }

    if (auto *button = qobject_cast<QPushButton *>(watched)) {
        if (event->type() == QEvent::Paint) {
            paintAlpineButton(button);
            return true;
        }
    }

    if (watched == ui->textEdit && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            equals_pressed();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

/** @brief Destroys the window's generated UI. */
MainWindow::~MainWindow()
{
    delete ui;
}

/** @brief Toggles the trigonometric angle unit between degrees and radians. */
void MainWindow::degreesMode_pressed()
{
    const bool degrees = ui->pushButton_changeDegreesMode->text() == "grad";
    if (degrees) {
        calc.setDegMode(false);
        ui->pushButton_changeDegreesMode->setText("rad");
    } else {
        calc.setDegMode(true);
        ui->pushButton_changeDegreesMode->setText("grad");
    }
}

/** @brief Inserts the digit of the button that emitted the signal. */
void MainWindow::digit_pressed(int digit)
{
    insertRightAlignedText(ui->textEdit, QString::number(digit));
}

/** @brief Inserts a decimal separator at the current cursor position. */
void MainWindow::decimal_pressed()
{
    insertRightAlignedText(ui->textEdit, ".");
}

/** @brief Applies the sign-toggle button action to the current expression. */
void MainWindow::unary_operation_pressed()
{
    if (m_isError) {
        clear_pressed();
    }

    if (!m_strExpression.isEmpty()) {
        const int minusPosition = m_strExpression.lastIndexOf('-');
        if (minusPosition != -1) {
            m_strExpression.remove(minusPosition, 1);
        } else {
            m_strExpression.prepend('-');
        }
    }
    setRightAlignedText(ui->textEdit, m_strExpression);
}

/** @brief Clears the expression, display error state and selected binary operator buttons. */
void MainWindow::clear_pressed()
{
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_mul->setChecked(false);
    ui->pushButton_divided->setChecked(false);

    m_isError = false;
    m_strExpression = "";
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,m_strExpression);
}

/** @brief Inserts the calculator constant pi. */
void MainWindow::pi_pressed()
{
    insertRightAlignedText(ui->textEdit,"pi");
}

/** @brief Moves the text cursor one position to the left. */
void MainWindow::move_left_pressed()
{
    ui->textEdit->setFocus();

    // Get the current cursor from the text edit
    QTextCursor cursor = ui->textEdit->textCursor();

    // Move the cursor left
    cursor.movePosition(QTextCursor::Left);

    // Set the modified cursor back to the text edit
    ui->textEdit->setTextCursor(cursor);

    // Ensure the cursor is visible
    ui->textEdit->ensureCursorVisible();

    // Set the cursor visible
    ui->textEdit->setCursorWidth(1); // Ensure cursor width is set to a visible size

    // Update the text edit to reflect changes
    ui->textEdit->repaint();

    ui->textEdit->setFocus();
}

/** @brief Moves the text cursor one position to the right. */
void MainWindow::move_right_pressed()
{
    ui->textEdit->setFocus();

    // Get the current cursor from the text edit
    QTextCursor cursor = ui->textEdit->textCursor();

    // Move the cursor right
    cursor.movePosition(QTextCursor::Right);

    // Set the modified cursor back to the text edit
    ui->textEdit->setTextCursor(cursor);

    // Ensure the cursor is visible
    ui->textEdit->ensureCursorVisible();

    // Set the cursor visible
    ui->textEdit->setCursorWidth(1); // Ensure cursor width is set to a visible size

    // Update the text edit to reflect changes
    ui->textEdit->repaint();

    ui->textEdit->setFocus();
}

/** @brief Displays the preceding entry in the expression/result history. */
void MainWindow::history_back_pressed()
{
    if (m_history.isEmpty()) return;
    m_hpos = qMax(0, m_hpos - 1);

    m_isError = false;
    QString hentry = m_history[m_hpos];
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,hentry);
}

/** @brief Displays the following entry in the expression/result history. */
void MainWindow::history_forward_pressed()
{
    if (m_history.isEmpty()) return;
    m_hpos = qMin(m_hpos + 1, m_history.size() - 1);

    m_isError = false;
    QString hentry = m_history[m_hpos];
    ui->textEdit->clear();
    setRightAlignedText(ui->textEdit,hentry);
}

/** @brief Evaluates the displayed expression, stores history and displays the result or error. */
void MainWindow::equals_pressed()
{
    try
    {
        m_strExpression = ui->textEdit->toPlainText();
        m_history.append(m_strExpression);

        double result = calc.evaluate(m_strExpression.toStdString());
        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }

        m_history.append(m_strExpression);
        m_hpos = m_history.size()-1;
        m_isError = false;
    } catch (const std::exception& e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

/** @brief Inserts an opening parenthesis. */
void MainWindow::left_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,"(");
}

/** @brief Inserts a closing parenthesis. */
void MainWindow::right_bracked_pressed()
{
    insertRightAlignedText(ui->textEdit,")");
}

/** @brief Inserts the square-root function prefix. */
void MainWindow::sqrt_pressed()
{
    insertRightAlignedText(ui->textEdit,"sqrt(");
}

/** @brief Inserts the exponentiation operator. */
void MainWindow::power_pressed()
{
    insertRightAlignedText(ui->textEdit, m_isInverted ? " root " : "^");
}

/** @brief Inserts sine or inverse sine, depending on inverse-key state. */
void MainWindow::sin_pressed()
{
    QString name = m_isInverted ? "asin(" : "sin(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts cosine or inverse cosine, depending on inverse-key state. */
void MainWindow::cos_pressed()
{
    QString name = m_isInverted ? "acos(" : "cos(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts tangent or inverse tangent, depending on inverse-key state. */
void MainWindow::tan_pressed()
{
    QString name = m_isInverted ? "atan(" : "tan(";
    insertRightAlignedText(ui->textEdit,name);
}

/** @brief Inserts the base-10 logarithm function prefix. */
void MainWindow::log_pressed()
{
    insertRightAlignedText(ui->textEdit,"log(");
}

/** @brief Inserts the natural logarithm function prefix. */
void MainWindow::ln_pressed()
{
    insertRightAlignedText(ui->textEdit,"ln(");
}

/** @brief Inserts the floating-point remainder operator. */
void MainWindow::mod_pressed()
{
    insertRightAlignedText(ui->textEdit," mod ");
}

/** @brief Toggles inverse trigonometric keys and inverse percentage mode. */
void MainWindow::inv_pressed()
{
    if(m_isInverted == false){
        ui->pushButton_x2->setProperty("inverseRoot", true);
        ui->pushButton_x2->setToolTip("x-te Wurzel aus y");
        ui->pushButton_x2->update();
        ui->pushButton_sin->setText("asin");
        ui->pushButton_cos->setText("acos");
        ui->pushButton_tan->setText("atan");
        ui->pushButton_percent->setText("1/%");
        m_isInverted = true;
    }else{
        ui->pushButton_x2->setProperty("inverseRoot", false);
        ui->pushButton_x2->setToolTip("Potenz x hoch y");
        ui->pushButton_x2->update();
        ui->pushButton_sin->setText("sin");
        ui->pushButton_cos->setText("cos");
        ui->pushButton_tan->setText("tan");
        ui->pushButton_percent->setText("%");
        m_isInverted = false;
    }
    calc.setInverted(m_isInverted);
}

/** @brief Evaluates the reciprocal of the current expression and displays the result or error. */
void MainWindow::edx_pressed()
{
    try
    {
        const std::string argument = ui->textEdit->toPlainText().toStdString();
        const std::string expression = "edx(" + argument + ")";
        double result = calc.evaluate(expression);
        if (std::floor(result) == result) {
            m_strExpression = QString::number(result, 'f', 0); // Display as integer
        }
        else {
            m_strExpression = QString::number(result); // Display with full precision
        }
        m_isError = false;
    } catch (const std::exception& e) {
        m_strExpression = e.what();
        m_isError = true;
    }

    setRightAlignedText(ui->textEdit,m_strExpression);
}

/** @brief Inserts the percentage operator. */
void MainWindow::percent_pressed()
{
    insertRightAlignedText(ui->textEdit,"%");
}

/** @brief Deletes the character immediately before the text cursor, if any. */
void MainWindow::erase_left_pressed()
{
    if (m_isError) {
        clear_pressed();
        return;
    }

    // Get the cursor position
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position();

    // Check if the cursor is not at the beginning
    if (pos > 0) {
        // Move the cursor to the position before the current one and select the character to delete
        cursor.setPosition(pos - 1, QTextCursor::KeepAnchor);

        // Remove the selected text
        cursor.removeSelectedText();

        // Update the text edit with the new cursor
        ui->textEdit->setTextCursor(cursor);

        // Update the internal string representation
        m_strExpression = ui->textEdit->toPlainText();

        // Set the text to be right-aligned if needed
        //setRightAlignedText(ui->textEdit, m_strExpression);
    }
}

/** @brief Inserts text at the editor cursor and updates m_strExpression.
 * @param textEdit Editor receiving the inserted text.
 * @param text Text to insert.
 */
void MainWindow::insertRightAlignedText(QTextEdit *textEdit, const QString &text) {
    if (m_isError) {
        clear_pressed();
    }

    textEdit->insertPlainText(text);
    m_strExpression = textEdit->toPlainText();
}

/** @brief Replaces editor contents and applies right alignment.
 * @param textEdit Editor to update.
 * @param text Replacement text.
 */
void MainWindow::setRightAlignedText(QTextEdit *textEdit, const QString &text) {
    const QSignalBlocker signalBlocker(textEdit);
    m_strExpression = text;

    // Update the document as one edit and always leave the caret at the end.
    // This avoids the visible clear/reinsert cycle while typing or evaluating.
    QTextCursor cursor(textEdit->document());
    cursor.beginEditBlock();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(blockFormat);
    cursor.endEditBlock();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}


