#include "NumberKeyboard.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QGuiApplication>

NumberKeyboard::NumberKeyboard(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    // 顶部拖动区域
    QLabel *titleBar = new QLabel("拖动此区域移动", this);
    titleBar->setFixedHeight(20);
    titleBar->setAlignment(Qt::AlignCenter);
    titleBar->setStyleSheet(
        "QLabel { background-color: #888888; color: white; font-size: 16px; }"
    );
    mainLayout->addWidget(titleBar);
    dragArea = titleBar->geometry();  // 初始拖动区域

    // 按钮布局
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(4);
    grid->setContentsMargins(4, 4, 4, 4);

    QStringList keys = {
        "1","2","3","4","✕",
        "5","6","7","8","⌫",
        "9","0",".","C","OK"
    };

    int row = 0, col = 0;
    for (const QString &k : keys) {
        QPushButton *btn = new QPushButton(k, this);
        btn->setMinimumSize(40, 40);  // 小按钮
        btn->setStyleSheet(
            "QPushButton {"
            "   font-size: 18px;"
            "   border: 1px solid #444;"
            "   border-radius: 5px;"
            "   background-color: #e0e0e0;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #cccccc;"
            "}"
        );
        grid->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, &NumberKeyboard::onButtonClicked);

        col++;
        if (col >= 5) { col = 0; row++; }
    }

    mainLayout->addLayout(grid);

    // 根据屏幕自适应大小
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    setFixedSize(rect.width() / 4, rect.height() / 2); // 可微调
}

void NumberKeyboard::setTarget(QLineEdit *edit)
{
    m_target = edit;
}

void NumberKeyboard::onButtonClicked()
{
    if (!m_target) return;
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    QString text = btn->text();

    if (text == "⌫") {
        QString t = m_target->text();
        t.chop(1);
        m_target->setText(t);
    }
    else if (text == "C") {
        m_target->clear();
    }
    else if (text == "OK" || text == "✕") {
        accept();
    }
    else {
        m_target->insert(text);
    }
}

// 仅在 titleBar 区域响应拖动
void NumberKeyboard::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (e->y() <= 20) {  // 顶部 20px 可拖动
            dragPos = e->globalPos() - frameGeometry().topLeft();
            e->accept();
        }
    }
}

void NumberKeyboard::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && e->y() <= 20) {
        // 移动窗口，使窗口左上角跟随手指位置减去偏移
        move(e->globalPos() - dragPos);
        e->accept();
    }
}
