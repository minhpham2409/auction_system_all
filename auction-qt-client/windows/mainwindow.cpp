#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QMessageBox>
#include <QStatusBar>
#include <QTime>
#include <QSplitter>
#include <QScrollBar>
#include <QInputDialog>
#include <QTextCursor>
#include "../dialogs/bidplacedialog.h"
#include "../dialogs/createroomdialog.h"
#include "../dialogs/createauctiondialog.h"
#include "../utils/formatters.h"
MainWindow::MainWindow(NetworkManager *net, const User& user, QWidget *parent)
    : QMainWindow(parent), 
      network(net), 
      currentUser(user), 
      countdownTimer(new QTimer(this)),
      warningCheckTimer(new QTimer(this))  // ← DẤU PHẨY, không phải dấu chấm phẩy
{  // ← DẤU { PHẢI Ở ĐÂY
    // Setup countdown timer
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdowns);
    countdownTimer->start(1000);
    
    // Setup warning timer
    connect(warningCheckTimer, &QTimer::timeout, this, &MainWindow::checkAuctionWarnings);
    warningCheckTimer->start(1000);
    
    setupUI();
    
    
    // Connect ALL signals
    connect(network, &NetworkManager::sellerHistoryReceived,  // ← THÊM
        this, &MainWindow::onSellerHistoryReceived);
    connect(network, &NetworkManager::roomHistoryReceived,    // ← THÊM
        this, &MainWindow::onRoomHistoryReceived);
    connect(network, &NetworkManager::auctionDeletedBroadcast, 
        this, &MainWindow::onAuctionDeletedBroadcast);
    connect(network, &NetworkManager::roomListReceived, this, &MainWindow::onRoomListReceived);
    connect(network, &NetworkManager::auctionListReceived, this, &MainWindow::onAuctionListReceived);
    connect(network, &NetworkManager::joinedRoom, this, &MainWindow::onJoinedRoom);
    connect(network, &NetworkManager::leftRoom, this, &MainWindow::onLeftRoom);
    connect(network, &NetworkManager::roomCreated, this, &MainWindow::onRoomCreated);
    connect(network, &NetworkManager::auctionCreated, this, &MainWindow::onAuctionCreated);
    connect(network, &NetworkManager::auctionActivated, this, &MainWindow::onAuctionActivated);
    connect(network, &NetworkManager::bidPlaced, this, &MainWindow::onBidPlaced);
    connect(network, &NetworkManager::buyNowSuccess, this, &MainWindow::onBuyNowSuccess);
    connect(network, &NetworkManager::balanceUpdated, this, &MainWindow::onBalanceUpdated);
    connect(network, &NetworkManager::auctionDeleted, this, &MainWindow::onAuctionDeleted);
    connect(network, &NetworkManager::auctionDetails, this, &MainWindow::onAuctionDetails);
    connect(network, &NetworkManager::bidHistoryReceived, this, &MainWindow::onBidHistoryReceived);
    connect(network, &NetworkManager::auctionHistoryReceived, this, &MainWindow::onAuctionHistoryReceived);
    connect(network, &NetworkManager::notification, this, &MainWindow::onNotification);
    connect(network, &NetworkManager::newBid, this, &MainWindow::onNewBid);
    connect(network, &NetworkManager::newAuction, this, &MainWindow::onNewAuction);
    connect(network, &NetworkManager::auctionWarning, this, &MainWindow::onAuctionWarning);
    connect(network, &NetworkManager::auctionEnded, this, &MainWindow::onAuctionEnded);
    connect(network, &NetworkManager::userJoinedRoom, this, &MainWindow::onUserJoinedRoom);
    connect(network, &NetworkManager::userLeftRoom, this, &MainWindow::onUserLeftRoom);
    connect(network, &NetworkManager::roomError, this, &MainWindow::onRoomError);
    connect(network, &NetworkManager::auctionError, this, &MainWindow::onAuctionError);
    connect(network, &NetworkManager::bidError, this, &MainWindow::onBidError);
    connect(network, &NetworkManager::disconnected, this, &MainWindow::onDisconnected);
    
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdowns);
    countdownTimer->start(1000);
    
    network->sendListRooms();
    
    addLogMessage("Đăng nhập thành công", "SUCCESS");
    addLogMessage(QString("Chào %1! Số dư: %2")
        .arg(user.username)
        .arg(Formatters::formatCurrency(user.balance)), "INFO");
}

MainWindow::~MainWindow() {}

QPushButton* MainWindow::createStyledButton(const QString& text, const QString& color)
{
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(QString(
        "QPushButton { background: %1; color: white; border: none; "
        "padding: 10px 15px; border-radius: 6px; font-weight: bold; font-size: 13px; } "
        "QPushButton:hover { background: %2; } "
        "QPushButton:pressed { background: %3; }"
    ).arg(color).arg(adjustBrightness(color, 110)).arg(adjustBrightness(color, 90)));
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QString MainWindow::adjustBrightness(const QString& color, int percent)
{
    QColor c(color);
    int h, s, v;
    c.getHsv(&h, &s, &v);
    v = qBound(0, v * percent / 100, 255);
    c.setHsv(h, s, v);
    return c.name();
}
void MainWindow::onAuctionDeletedBroadcast(int auctionId, QString title)
{
    Q_UNUSED(auctionId);
    addLogMessage(QString("🗑️ '%1' đã bị xóa").arg(title), "INFO");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}
void MainWindow::checkAuctionWarnings()
{
    if (!currentUser.isInRoom()) return;
    
    for (const Auction& a : auctions) {
        if (!a.isActive()) continue;
        
        int timeLeft = a.getTimeLeft();
        
        // Warning at 30s - CHỈ 1 LẦN
        if (timeLeft <= 30 && timeLeft > 0 && 
            !warnedAuctions.contains(a.auctionId)) {
            
            warnedAuctions.insert(a.auctionId);
            
            // POPUP NỔI BẬT
            QMessageBox *warningBox = new QMessageBox(this);
            warningBox->setWindowTitle("⚠️ CẢNH BÁO");
            warningBox->setText(QString(
                "<h2 style='color: #f57c00;'>⚠️ ĐẤU GIÁ SẮP KẾT THÚC!</h2>"
                "<p style='font-size: 16px;'><b>%1</b></p>"
                "<p style='font-size: 14px;'>Còn <b style='color: red;'>%2 giây</b></p>"
                "<p>Giá: <b>%3</b></p>"
            ).arg(a.title)
             .arg(timeLeft)
             .arg(Formatters::formatCurrency(a.currentPrice)));
            warningBox->setIcon(QMessageBox::Warning);
            warningBox->setStyleSheet(
                "QMessageBox { background: #fff3e0; } "
                "QLabel { font-size: 14px; }"
            );
            warningBox->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
            warningBox->show();
            
            // Auto close after 5s
            QTimer::singleShot(5000, warningBox, &QMessageBox::accept);
            
            addLogMessage(QString("⚠️ %1 còn %2s!")
                .arg(a.title).arg(timeLeft), "WARNING");
        }
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("🎪 Đấu Giá Online - " + currentUser.username);
    setMinimumSize(1400, 850);
    
    setStyleSheet("QMainWindow { background: #f5f7fa; }");
    
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // TOP BAR
    QHBoxLayout *topBar = new QHBoxLayout();
    
    userInfoLabel = new QLabel();
    userInfoLabel->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); "
        "color: white; padding: 15px 25px; border-radius: 10px; font-size: 15px; font-weight: bold; }"
    );
    updateUserInfo();
    topBar->addWidget(userInfoLabel, 1);
    
    QPushButton *refreshAllBtn = createStyledButton("🔄 Làm mới", "#4CAF50");
    connect(refreshAllBtn, &QPushButton::clicked, [this]() {
        network->sendListRooms();
        if (currentUser.isInRoom()) {
            network->sendListAuctions(currentUser.currentRoomId);
        }
    });
    topBar->addWidget(refreshAllBtn);
    QPushButton *roomInfoBtn = createStyledButton("ℹ️ Thông tin", "#2196F3");
    connect(roomInfoBtn, &QPushButton::clicked, this, &MainWindow::on_roomInfoButton_clicked);
    topBar->addWidget(roomInfoBtn);
    QPushButton *historyBtn = createStyledButton("📜 Lịch sử", "#9C27B0");
connect(historyBtn, &QPushButton::clicked, this, &MainWindow::on_viewHistoryButton_clicked);
topBar->addWidget(historyBtn);

// ← THÊM 2 BUTTONS MỚI:
QPushButton *participatedBtn = createStyledButton("📊 Tham gia", "#9c27b0");
connect(participatedBtn, &QPushButton::clicked, 
        this, &MainWindow::on_viewParticipatedHistoryButton_clicked);
topBar->addWidget(participatedBtn);

QPushButton *sellerBtn = createStyledButton("👤 Làm chủ", "#ff9800");
connect(sellerBtn, &QPushButton::clicked, 
        this, &MainWindow::on_viewSellerHistoryButton_clicked);
topBar->addWidget(sellerBtn);
QPushButton *roomHistoryBtn = createStyledButton("🏛️ Lịch sử phòng", "#795548");
connect(roomHistoryBtn, &QPushButton::clicked, 
        this, &MainWindow::on_viewRoomHistoryButton_clicked);
topBar->addWidget(roomHistoryBtn);

QPushButton *logoutBtn = createStyledButton("🚪 Thoát", "#f44336");
connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
topBar->addWidget(logoutBtn);
    mainLayout->addLayout(topBar);
    
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    
    // LEFT - ROOMS
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);
    
    QLabel *roomsTitle = new QLabel("🏠 PHÒNG");
    roomsTitle->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #667eea, stop:1 #764ba2); "
        "color: white; padding: 12px; font-size: 16px; font-weight: bold; border-radius: 8px; }"
    );
    leftLayout->addWidget(roomsTitle);
    
    roomsList = new QListWidget();
    roomsList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 8px; font-size: 13px; } "
        "QListWidget::item { padding: 12px; border-bottom: 1px solid #f0f0f0; "
        "border-radius: 6px; margin: 3px; } "
        "QListWidget::item:hover { background: #e3f2fd; } "
        "QListWidget::item:selected { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e3f2fd, stop:1 #bbdefb); color: #1976d2; border: 2px solid #2196f3; font-weight: bold; }"
    );
    connect(roomsList, &QListWidget::itemDoubleClicked, this, &MainWindow::on_roomsList_itemDoubleClicked);
    leftLayout->addWidget(roomsList, 1);
    
    QGridLayout *roomBtns = new QGridLayout();
    roomBtns->setSpacing(8);
    
    QPushButton *createRoomBtn = createStyledButton("➕ Tạo", "#2196f3");
    QPushButton *joinRoomBtn = createStyledButton("🚪 Vào", "#4caf50");
    QPushButton *leaveRoomBtn = createStyledButton("👋 Rời", "#ff9800");
    QPushButton *refreshRoomsBtn = createStyledButton("🔄 Mới", "#9e9e9e");
    
    connect(createRoomBtn, &QPushButton::clicked, this, &MainWindow::on_createRoomButton_clicked);
    connect(joinRoomBtn, &QPushButton::clicked, this, &MainWindow::on_joinRoomButton_clicked);
    connect(leaveRoomBtn, &QPushButton::clicked, this, &MainWindow::on_leaveRoomButton_clicked);
    connect(refreshRoomsBtn, &QPushButton::clicked, this, &MainWindow::on_refreshRoomsButton_clicked);
    
    roomBtns->addWidget(createRoomBtn, 0, 0);
    roomBtns->addWidget(joinRoomBtn, 0, 1);
    roomBtns->addWidget(leaveRoomBtn, 1, 0);
    roomBtns->addWidget(refreshRoomsBtn, 1, 1);
    leftLayout->addLayout(roomBtns);
    
    splitter->addWidget(leftPanel);
    
    // CENTER - AUCTIONS
    QWidget *centerPanel = new QWidget();
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPanel);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(10);
    
    roomStatusLabel = new QLabel("❌ Chưa vào phòng");
    roomStatusLabel->setStyleSheet(
        "QLabel { background: #ffebee; color: #c62828; padding: 12px; "
        "border-radius: 8px; font-weight: bold; font-size: 14px; }"
    );
    centerLayout->addWidget(roomStatusLabel);
    
    QLabel *auctionsTitle = new QLabel("🔨 ĐẤU GIÁ");
    auctionsTitle->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f093fb, stop:1 #f5576c); "
        "color: white; padding: 12px; font-size: 16px; font-weight: bold; border-radius: 8px; }"
    );
    centerLayout->addWidget(auctionsTitle);
    
    auctionsList = new QListWidget();
    auctionsList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 8px; font-size: 13px; } "
        "QListWidget::item { padding: 14px; border-bottom: 1px solid #f0f0f0; "
        "border-radius: 6px; margin: 3px; } "
        "QListWidget::item:hover { background: #fff3e0; } "
        "QListWidget::item:selected { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #e8f5e9, stop:1 #c8e6c9); color: #2e7d32; border: 2px solid #4caf50; font-weight: bold; }"
    );
    auctionsList->setSelectionMode(QAbstractItemView::SingleSelection);
    centerLayout->addWidget(auctionsList, 2);
    
    QLabel *queueTitle = new QLabel("⏳ HÀNG ĐỢI");
    queueTitle->setStyleSheet(
        "QLabel { background: #ff9800; color: white; padding: 8px; font-size: 14px; "
        "font-weight: bold; border-radius: 6px; }"
    );
    centerLayout->addWidget(queueTitle);
    
    queueList = new QListWidget();
    queueList->setStyleSheet(
        "QListWidget { border: 2px solid #e0e0e0; border-radius: 8px; background: #fff8e1; "
        "padding: 5px; font-size: 12px; max-height: 120px; } "
        "QListWidget::item { padding: 8px; border-radius: 4px; margin: 2px; }"
    );
    centerLayout->addWidget(queueList);
    
    QGridLayout *auctionBtns = new QGridLayout();
    auctionBtns->setSpacing(8);
    
    QPushButton *createAuctionBtn = createStyledButton("➕ Tạo", "#2196f3");
    QPushButton *viewDetailsBtn = createStyledButton("👁️ Chi tiết", "#009688");
    QPushButton *activateBtn = createStyledButton("▶️ Kích hoạt", "#ff9800");
    QPushButton *deleteBtn = createStyledButton("🗑️ Xóa", "#f44336");
    QPushButton *searchBtn = createStyledButton("🔍 Tìm", "#9c27b0");
    QPushButton *bidHistoryBtn = createStyledButton("📊 Lịch sử giá", "#607d8b");
    
    connect(createAuctionBtn, &QPushButton::clicked, this, &MainWindow::on_createAuctionButton_clicked);
    connect(viewDetailsBtn, &QPushButton::clicked, this, &MainWindow::on_viewAuctionDetailsButton_clicked);
    connect(activateBtn, &QPushButton::clicked, this, &MainWindow::on_activateAuctionButton_clicked);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteAuctionButton_clicked);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::on_searchAuctionsButton_clicked);
    connect(bidHistoryBtn, &QPushButton::clicked, this, &MainWindow::on_bidHistoryButton_clicked);
    
    auctionBtns->addWidget(createAuctionBtn, 0, 0);
    auctionBtns->addWidget(viewDetailsBtn, 0, 1);
    auctionBtns->addWidget(activateBtn, 0, 2);
    auctionBtns->addWidget(deleteBtn, 1, 0);
    auctionBtns->addWidget(searchBtn, 1, 1);
    auctionBtns->addWidget(bidHistoryBtn, 1, 2);
    centerLayout->addLayout(auctionBtns);
    
    QHBoxLayout *bidBtns = new QHBoxLayout();
    bidBtns->setSpacing(10);
    
    QPushButton *bidBtn = new QPushButton("💰 ĐẶT GIÁ");
    bidBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f093fb, stop:1 #f5576c); "
        "color: white; border: none; padding: 18px; border-radius: 10px; "
        "font-size: 18px; font-weight: bold; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e084eb, stop:1 #e5475c); }"
    );
    
    QPushButton *buyBtn = new QPushButton("⚡ MUA NGAY");
    buyBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4facfe, stop:1 #00f2fe); "
        "color: white; border: none; padding: 18px; border-radius: 10px; "
        "font-size: 18px; font-weight: bold; } "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3f9cee, stop:1 #00e2ee); }"
    );
    
    connect(bidBtn, &QPushButton::clicked, this, &MainWindow::on_placeBidButton_clicked);
    connect(buyBtn, &QPushButton::clicked, this, &MainWindow::on_buyNowButton_clicked);
    bidBtns->addWidget(bidBtn);
    bidBtns->addWidget(buyBtn);
    centerLayout->addLayout(bidBtns);
    
    splitter->addWidget(centerPanel);
    
    // RIGHT - LOG
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);
    
    QLabel *logTitle = new QLabel("📋 NHẬT KÝ");
    logTitle->setStyleSheet(
        "QLabel { background: #607d8b; color: white; padding: 12px; font-size: 16px; "
        "font-weight: bold; border-radius: 8px; }"
    );
    rightLayout->addWidget(logTitle);
    
    activityLog = new QTextEdit();
    activityLog->setReadOnly(true);
    activityLog->setStyleSheet(
        "QTextEdit { border: 2px solid #e0e0e0; border-radius: 10px; background: white; "
        "padding: 10px; font-family: 'Segoe UI', Arial, sans-serif; font-size: 14px; }"
    );
    rightLayout->addWidget(activityLog, 1);
    
    QPushButton *clearLogBtn = createStyledButton("🗑️ Xóa", "#9e9e9e");
    connect(clearLogBtn, &QPushButton::clicked, [this]() {
        activityLog->clear();
    });
    rightLayout->addWidget(clearLogBtn);
    
    splitter->addWidget(rightPanel);
    
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    splitter->setStretchFactor(2, 1);
    
    mainLayout->addWidget(splitter, 1);
    
    setCentralWidget(central);
    statusBar()->showMessage("✅ Kết nối");
}

void MainWindow::updateUserInfo()
{
    userInfoLabel->setText(QString("👤 %1  |  💰 %2")
        .arg(currentUser.username)
        .arg(Formatters::formatCurrency(currentUser.balance)));
}

void MainWindow::updateRoomStatus()
{
    if (currentUser.isInRoom()) {
        roomStatusLabel->setText(QString("✅ Phòng: %1").arg(currentUser.currentRoomName));
        roomStatusLabel->setStyleSheet(
            "QLabel { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c8e6c9, stop:1 #a5d6a7); "
            "color: #1b5e20; padding: 12px; border-radius: 8px; font-weight: bold; font-size: 14px; }"
        );
    } else {
        roomStatusLabel->setText("❌ Chưa vào phòng");
        roomStatusLabel->setStyleSheet(
            "QLabel { background: #ffebee; color: #c62828; padding: 12px; "
            "border-radius: 8px; font-weight: bold; font-size: 14px; }"
        );
    }
}

void MainWindow::addLogMessage(const QString& message, const QString& type)
{
    QString color, icon;
    int fontSize = 14;
    
    if (type == "SUCCESS") {
        color = "#2e7d32"; icon = "✅"; fontSize = 15;
    } else if (type == "ERROR") {
        color = "#c62828"; icon = "❌"; fontSize = 15;
    } else if (type == "WARNING") {
        color = "#f57c00"; icon = "⚠️"; fontSize = 15;
    } else if (type == "BID") {
        color = "#1976d2"; icon = "💰"; fontSize = 16;
    } else if (type == "WIN") {
        color = "#7b1fa2"; icon = "🎉"; fontSize = 17;
    } else {
        color = "#455a64"; icon = "ℹ️"; fontSize = 13;
    }
    
    QTextCursor cursor = activityLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat timeFormat;
    timeFormat.setForeground(QColor("#9e9e9e"));
    timeFormat.setFontPointSize(11);
    cursor.insertText(QString("[%1] ").arg(QTime::currentTime().toString("hh:mm:ss")), timeFormat);
    
    QTextCharFormat iconFormat;
    iconFormat.setFontPointSize(fontSize);
    cursor.insertText(icon + " ", iconFormat);
    
    QTextCharFormat msgFormat;
    msgFormat.setForeground(QColor(color));
    msgFormat.setFontPointSize(fontSize);
    if (type != "INFO") {
        msgFormat.setFontWeight(QFont::Bold);
    }
    cursor.insertText(message + "\n", msgFormat);
    
    activityLog->setTextCursor(cursor);
    activityLog->verticalScrollBar()->setValue(activityLog->verticalScrollBar()->maximum());
}

void MainWindow::showError(const QString& title, const QString& message)
{
    QMessageBox::warning(this, title, message);
    addLogMessage(message, "ERROR");
}

void MainWindow::showSuccess(const QString& title, const QString& message)
{
    QMessageBox::information(this, title, message);
    addLogMessage(message, "SUCCESS");
}

Room MainWindow::getSelectedRoom() const
{
    int row = roomsList->currentRow();
    return (row >= 0 && row < rooms.size()) ? rooms[row] : Room();
}

Auction MainWindow::getSelectedAuction() const
{
    int row = auctionsList->currentRow();
    return (row >= 0 && row < auctions.size()) ? auctions[row] : Auction();
}

bool MainWindow::userHasActiveBids()
{
    for (const Auction& a : auctions) {
        if (a.isActive() && a.currentBidderName == currentUser.username) {
            return true;
        }
    }
    return false;
}

// ========== BUTTON HANDLERS ==========

void MainWindow::on_refreshRoomsButton_clicked()
{
    network->sendListRooms();
}

void MainWindow::on_createRoomButton_clicked()
{
    CreateRoomDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        network->sendCreateRoom(currentUser.userId, dlg.getRoomName(),
            dlg.getDescription(), dlg.getMaxParticipants(), dlg.getDuration());
    }
}
void MainWindow::on_viewRoomHistoryButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Lỗi", "Vào phòng để xem lịch sử");
        return;
    }
    
    network->sendRoomHistory(currentUser.currentRoomId);
}void MainWindow::onRoomHistoryReceived(const QString& history)
{
    if (history.isEmpty()) {
        QMessageBox::information(this, "Lịch sử phòng", 
            "Phòng chưa có đấu giá nào kết thúc");
        return;
    }
    
    QStringList auctions = history.split('|', Qt::SkipEmptyParts);
    
    qDebug() << "Room history - Total auctions:" << auctions.size();
    
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("🏛️ LỊCH SỬ PHÒNG ĐẤU GIÁ");
    dialog->setMinimumSize(1100, 650);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Title
    QLabel *title = new QLabel("🏛️ LỊCH SỬ PHÒNG ĐẤU GIÁ");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #795548; "
        "padding: 15px; background: white; border-radius: 8px;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    // Table - 9 columns
    QTableWidget *table = new QTableWidget(auctions.size(), 9, dialog);
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối", 
        "👤 Người thắng", "📊 Tổng lượt", "👥 Người tham gia", 
        "👨‍💼 Chủ đấu giá", "📈 Kết quả"
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #8d6e63, stop:1 #5d4037); color: white; padding: 10px; "
        "font-weight: bold; border: none; }"
    );
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    double totalValue = 0;
    int totalBids = 0;
    int soldCount = 0;
    
    int row = 0;
    for (const QString& auctionData : auctions) {
        QStringList fields = auctionData.split(';');
        
        qDebug() << "Row" << row << "fields:" << fields.size() << fields;
        
        // Server format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status;seller
        if (fields.size() >= 9) {
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int auctionTotalBids = fields[5].toInt();
            int participants = fields[6].toInt();
            // QString status = fields[7];
            QString seller = fields[8];
            
            bool sold = (winner != "No winner");
            if (sold) {
                soldCount++;
                totalValue += finalPrice;
            }
            totalBids += auctionTotalBids;
            
            // #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            
            // Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);
            
            // Start price
            table->setItem(row, 2, new QTableWidgetItem(
                Formatters::formatCurrency(startPrice)));
            
            // Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(sold ? QColor("#4caf50") : QColor("#666"));
            finalItem->setFont(QFont("Arial", 11, sold ? QFont::Bold : QFont::Normal));
            table->setItem(row, 3, finalItem);
            
            // Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(sold ? QColor("#4caf50") : QColor("#999"));
            table->setItem(row, 4, winnerItem);
            
            // Total bids
            QTableWidgetItem *bidsItem = new QTableWidgetItem(QString::number(auctionTotalBids));
            bidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 5, bidsItem);
            
            // Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(QString::number(participants));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, partItem);
            
            // Seller
            QTableWidgetItem *sellerItem = new QTableWidgetItem(seller);
            sellerItem->setForeground(QColor("#1976d2"));
            sellerItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 7, sellerItem);
            
            // Result
            QString result = sold ? "✅ Đã bán" : "❌ Chưa bán";
            QTableWidgetItem *resultItem = new QTableWidgetItem(result);
            resultItem->setFont(QFont("Arial", 11, QFont::Bold));
            resultItem->setForeground(sold ? QColor("#4caf50") : QColor("#f44336"));
            resultItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 8, resultItem);
            
            row++;
        }
    }
    
    table->resizeColumnsToContents();
    layout->addWidget(table);
    
    // Stats
    QLabel *stats = new QLabel(QString(
        "📊 <b>Tổng:</b> %1 phiên | "
        "<b style='color:#4caf50;'>✅ Đã bán:</b> %2 | "
        "<b style='color:#f44336;'>❌ Chưa bán:</b> %3 | "
        "<b>Tổng lượt đặt:</b> %4 | "
        "<b style='color:#4caf50;'>💰 Tổng giá trị:</b> %5"
    ).arg(auctions.size())
     .arg(soldCount)
     .arg(auctions.size() - soldCount)
     .arg(totalBids)
     .arg(Formatters::formatCurrency(totalValue)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;"
    );
    layout->addWidget(stats);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #795548; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #5d4037; }"
    );
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    dialog->exec();
    delete dialog;
}
void MainWindow::on_joinRoomButton_clicked()
{
    Room room = getSelectedRoom();
    if (room.roomId == 0) {
        showError("Lỗi", "Chọn phòng");
        return;
    }
    
    // FIX: Check if already in a room
    if (currentUser.isInRoom()) {
        showError("Lỗi", "Rời phòng hiện tại trước");
        return;
    }
    
    network->sendJoinRoom(currentUser.userId, room.roomId);
}

void MainWindow::on_leaveRoomButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Lỗi", "Chưa vào phòng");
        return;
    }
    
    if (userHasActiveBids()) {
        QMessageBox::warning(this, "Không thể rời",
            "Bạn đang có giá đặt trong đấu giá!\nĐợi khi đấu giá kết thúc.");
        addLogMessage("Không thể rời: đang có bid", "WARNING");
        return;
    }
    
    network->sendLeaveRoom(currentUser.userId);
}

void MainWindow::on_refreshAuctionsButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Lỗi", "Vào phòng trước");
        return;
    }
    network->sendListAuctions(currentUser.currentRoomId);
}

void MainWindow::on_createAuctionButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Lỗi", "Vào phòng trước");
        return;
    }
    
    CreateAuctionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        network->sendCreateAuction(currentUser.userId, currentUser.currentRoomId,
            dlg.getTitle(), dlg.getDescription(), dlg.getStartPrice(),
            dlg.getBuyNowPrice(), dlg.getMinIncrement(), dlg.getDuration());
    }
}

void MainWindow::on_viewAuctionDetailsButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    network->sendViewAuction(auction.auctionId);
}

void MainWindow::on_placeBidButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    if (!auction.canBid()) {
        showError("Lỗi", "Không đang đấu giá");
        return;
    }
    
    BidPlaceDialog dlg(auction, this);
    if (dlg.exec() == QDialog::Accepted) {
        double amount = dlg.getBidAmount();
        network->sendPlaceBid(auction.auctionId, currentUser.userId, amount);
    }
}

void MainWindow::on_buyNowButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    if (!auction.hasBuyNow()) {
        showError("Lỗi", "Không có giá mua ngay");
        return;
    }
    
    auto reply = QMessageBox::question(this, "Xác nhận",
        QString("Mua %1 với giá %2?")
            .arg(auction.title)
            .arg(Formatters::formatCurrency(auction.buyNowPrice)),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        network->sendBuyNow(auction.auctionId, currentUser.userId);
    }
}

void MainWindow::on_deleteAuctionButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    auto reply = QMessageBox::question(this, "Xác nhận",
        QString("Xóa %1?").arg(auction.title),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        network->sendDeleteAuction(auction.auctionId, currentUser.userId);
    }
}

void MainWindow::on_activateAuctionButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    network->sendActivateAuction(auction.auctionId, currentUser.userId);
}
void MainWindow::on_searchAuctionsButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Lỗi", "Vào phòng trước");
        return;
    }
    
    bool ok;
    QString keyword = QInputDialog::getText(this, "Tìm kiếm", 
                                           "Nhập từ khóa:", 
                                           QLineEdit::Normal, "", &ok);
    
    if (ok && !keyword.isEmpty()) {
        network->sendSearchAuctions(currentUser.currentRoomId, keyword, -1, -1);
        addLogMessage(QString("🔍 Tìm: %1").arg(keyword), "INFO");
    }
}

void MainWindow::on_bidHistoryButton_clicked()
{
    Auction auction = getSelectedAuction();
    if (auction.auctionId == 0) {
        showError("Lỗi", "Chọn sản phẩm");
        return;
    }
    
    network->sendBidHistory(auction.auctionId);
}

void MainWindow::on_viewHistoryButton_clicked()
{
    network->sendAuctionHistory(currentUser.userId);
}

void MainWindow::on_roomInfoButton_clicked()
{
    if (!currentUser.isInRoom()) {
        showError("Thông tin", "Chưa vào phòng");
        return;
    }
    
    Room currentRoom;
    for (const Room& r : rooms) {
        if (r.roomId == currentUser.currentRoomId) {
            currentRoom = r;
            break;
        }
    }
    
    QString info = QString(
        "🏠 PHÒNG: %1\n\n"
        "👤 Người tạo: %2\n"
        "👥 Người: %3/%4\n"
        "🔨 Đấu giá: %5 active + %6 chờ\n"
        "📊 Bid của bạn: %7"
    ).arg(currentRoom.name.isEmpty() ? currentUser.currentRoomName : currentRoom.name)
     .arg(currentRoom.creatorName.isEmpty() ? "N/A" : currentRoom.creatorName)
     .arg(currentRoom.currentParticipants)
     .arg(currentRoom.maxParticipants)
     .arg(auctions.size())
     .arg(queueList->count())
     .arg(userHasActiveBids() ? "CÓ ⚠️" : "Không");
    
    QMessageBox::information(this, "Thông tin phòng", info);
}

void MainWindow::on_logoutButton_clicked()
{
    if (userHasActiveBids()) {
        auto reply = QMessageBox::question(this, "Xác nhận",
            "Bạn đang có bid!\nVẫn đăng xuất?",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }
    
    network->sendLogout(currentUser.userId);
    close();
}

void MainWindow::on_roomsList_itemDoubleClicked(QListWidgetItem*)
{
    on_joinRoomButton_clicked();
}

void MainWindow::on_roomsList_itemSelectionChanged() {}
void MainWindow::on_auctionsList_itemSelectionChanged() {}

// ========== NETWORK HANDLERS ==========

void MainWindow::onRoomListReceived(const QList<Room>& newRooms)
{
    rooms = newRooms;
    roomsList->clear();
    
    for (const Room& room : rooms) {
        QString icon = room.isFull() ? "🔒" : "🏠";
        QString text = QString("%1 %2 | 👥 %3/%4")
            .arg(icon).arg(room.name)
            .arg(room.currentParticipants).arg(room.maxParticipants);
        
        QListWidgetItem *item = new QListWidgetItem(text);
        if (room.isFull()) {
            item->setForeground(QColor("#9e9e9e"));
        }
        roomsList->addItem(item);
    }
}

void MainWindow::onJoinedRoom(int roomId, const QString& roomName)
{
    currentUser.currentRoomId = roomId;
    currentUser.currentRoomName = roomName;
    updateRoomStatus();
    addLogMessage(QString("Vào phòng: %1").arg(roomName), "SUCCESS");
    
    network->sendListAuctions(roomId);
}

void MainWindow::onLeftRoom()
{
    QString oldRoom = currentUser.currentRoomName;
    currentUser.currentRoomId = 0;
    currentUser.currentRoomName.clear();
    updateRoomStatus();
    auctionsList->clear();
    queueList->clear();
    auctions.clear();
    addLogMessage(QString("Rời phòng: %1").arg(oldRoom), "INFO");
}

void MainWindow::onRoomCreated(int roomId)
{
    Q_UNUSED(roomId);
    network->sendListRooms();
}

void MainWindow::onAuctionListReceived(const QList<Auction>& newAuctions)
{
    int currentRow = auctionsList->currentRow();
    int selectedAuctionId = (currentRow >= 0 && currentRow < auctions.size()) ? 
                           auctions[currentRow].auctionId : -1;
    
    auctions = newAuctions;
    
    QList<Auction> activeAuctions, waitingAuctions;
    for (const Auction& a : auctions) {
        if (a.isActive()) {
            activeAuctions.append(a);
        } else if (a.isWaiting()) {
            waitingAuctions.append(a);
        }
    }
    
    auctionsList->clear();
    for (const Auction& a : activeAuctions) {
        QString status = Formatters::formatTime(a.getTimeLeft());
        QString text = QString("🔨 %1 | 💰 %2 | ⏱️ %3")
            .arg(a.title)
            .arg(Formatters::formatCurrency(a.currentPrice))
            .arg(status);
        auctionsList->addItem(text);
    }
    
    queueList->clear();
    for (const Auction& a : waitingAuctions) {
        QString text = QString("⏳ %1 - %2")
            .arg(a.title)
            .arg(Formatters::formatCurrency(a.startPrice));
        queueList->addItem(text);
    }
    
    if (selectedAuctionId > 0) {
        for (int i = 0; i < auctions.size(); i++) {
            if (auctions[i].auctionId == selectedAuctionId) {
                int displayIndex = -1;
                for (int j = 0; j < activeAuctions.size(); j++) {
                    if (activeAuctions[j].auctionId == selectedAuctionId) {
                        displayIndex = j;
                        break;
                    }
                }
                if (displayIndex >= 0) {
                    auctionsList->setCurrentRow(displayIndex);
                }
                break;
            }
        }
    }
}

void MainWindow::onAuctionCreated(int auctionId)
{
    Q_UNUSED(auctionId);
    addLogMessage("Tạo đấu giá OK", "SUCCESS");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionActivated()
{
    addLogMessage("Kích hoạt OK", "SUCCESS");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onBidPlaced()
{
    addLogMessage("Đặt giá thành công", "SUCCESS");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onBuyNowSuccess()
{
    addLogMessage("Mua ngay thành công", "WIN");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionDeleted()
{
    addLogMessage("Đã xóa", "INFO");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionDetails(const Auction& auction)
{
    QString details = QString(
        "🏷️ %1\n\n"
        "💵 Giá: %2\n"
        "⚡ Mua ngay: %3\n"
        "👤 Cao nhất: %4\n"
        "📊 Lượt: %5\n"
        "⏰ Trạng thái: %6\n"
        "⏱️ Còn: %7"
    ).arg(auction.title)
     .arg(Formatters::formatCurrency(auction.currentPrice))
     .arg(auction.hasBuyNow() ? Formatters::formatCurrency(auction.buyNowPrice) : "Không")
     .arg(auction.currentBidderName.isEmpty() ? "Chưa có" : auction.currentBidderName)
     .arg(auction.totalBids)
     .arg(auction.getStatusText())
     .arg(auction.isActive() ? Formatters::formatTime(auction.getTimeLeft()) : "N/A");
    
    QMessageBox::information(this, "Chi tiết", details);
}

void MainWindow::onBidHistoryReceived(const QString& history)
{
    QMessageBox::information(this, "Lịch sử giá", history.isEmpty() ? "Chưa có bid" : history);
}// ==================== LỊCH SỬ THAM GIA ====================
void MainWindow::on_viewParticipatedHistoryButton_clicked()
{
    network->sendAuctionHistory(currentUser.userId);  // Existing
}
void MainWindow::onAuctionHistoryReceived(const QString& history)
{
    if (history.isEmpty()) {
        QMessageBox::information(this, "Lịch sử tham gia", 
            "Bạn chưa tham gia đấu giá nào");
        return;
    }
    
    QStringList auctions = history.split('|', Qt::SkipEmptyParts);
    
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 LỊCH SỬ THAM GIA ĐẤU GIÁ");
    dialog->setMinimumSize(1100, 650);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Title
    QLabel *title = new QLabel("📊 CÁC PHIÊN ĐẤU GIÁ BẠN ĐÃ THAM GIA");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #9c27b0; "
        "padding: 15px; background: white; border-radius: 8px;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    // Table
    QTableWidget *table = new QTableWidget(auctions.size(), 9, dialog);
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối", 
        "👤 Người thắng", "🎯 Lượt của bạn", "📊 Tổng lượt", 
        "👥 Người tham gia", "🏆 Kết quả"
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #ab47bc, stop:1 #7b1fa2); color: white; padding: 10px; "
        "font-weight: bold; border: none; }"
    );
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    int wonCount = 0, lostCount = 0;
    double totalSpent = 0;
    int totalBidsPlaced = 0;
    
    int row = 0;
    for (const QString& auctionData : auctions) {
        QStringList fields = auctionData.split(';');
        
        // Server format: auctionId;title;startPrice;finalPrice;winner;userBidCount;totalBids;participants;status
        if (fields.size() >= 9) {
            // int auctionId = fields[0].toInt();
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int userBidCount = fields[5].toInt();
            int totalBids = fields[6].toInt();
            int participantCount = fields[7].toInt();
            // QString status = fields[8];
            
            bool userWon = (winner == currentUser.username);
            if (userWon) {
                wonCount++;
                totalSpent += finalPrice;
            } else {
                lostCount++;
            }
            totalBidsPlaced += userBidCount;
            
            // #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            
            // Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);
            
            // Start price
            table->setItem(row, 2, new QTableWidgetItem(
                Formatters::formatCurrency(startPrice)));
            
            // Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(QColor("#4caf50"));
            finalItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 3, finalItem);
            
            // Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(userWon ? QColor("#4caf50") : QColor("#666"));
            winnerItem->setFont(QFont("Arial", 11, userWon ? QFont::Bold : QFont::Normal));
            table->setItem(row, 4, winnerItem);
            
            // User bid count
            QTableWidgetItem *userBidsItem = new QTableWidgetItem(
                QString::number(userBidCount));
            userBidsItem->setTextAlignment(Qt::AlignCenter);
            userBidsItem->setFont(QFont("Arial", 11, QFont::Bold));
            userBidsItem->setForeground(QColor("#1976d2"));
            table->setItem(row, 5, userBidsItem);
            
            // Total bids
            QTableWidgetItem *totalBidsItem = new QTableWidgetItem(
                QString::number(totalBids));
            totalBidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, totalBidsItem);
            
            // Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(
                QString::number(participantCount));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 7, partItem);
            
            // Result
            QString result = userWon ? "🎉 THẮNG" : "😢 THUA";
            QTableWidgetItem *resultItem = new QTableWidgetItem(result);
            resultItem->setFont(QFont("Arial", 12, QFont::Bold));
            resultItem->setForeground(userWon ? QColor("#4caf50") : QColor("#f57c00"));
            resultItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 8, resultItem);
            
            row++;
        }
    }
    
    table->resizeColumnsToContents();
    layout->addWidget(table);
    
    // Stats
    QLabel *stats = new QLabel(QString(
        "📊 <b>Tổng:</b> %1 phiên | "
        "<b style='color:#4caf50;'>🎉 Thắng:</b> %2 | "
        "<b style='color:#f57c00;'>😢 Thua:</b> %3 | "
        "<b>Tỷ lệ:</b> %4%% | "
        "<b>Tổng lượt đặt:</b> %5 | "
        "<b style='color:#e91e63;'>💰 Tổng chi:</b> %6"
    ).arg(auctions.size())
     .arg(wonCount)
     .arg(lostCount)
     .arg(auctions.size() > 0 ? wonCount * 100 / auctions.size() : 0)
     .arg(totalBidsPlaced)
     .arg(Formatters::formatCurrency(totalSpent)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;"
    );
    layout->addWidget(stats);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #9c27b0; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #7b1fa2; }"
    );
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    dialog->exec();
    delete dialog;
}
// ==================== LỊCH SỬ LÀM CHỦ ====================
void MainWindow::on_viewSellerHistoryButton_clicked()
{
    // Request seller history
    network->sendSellerHistory(currentUser.userId);
}void MainWindow::onBalanceUpdated(double newBalance)
{
    currentUser.balance = newBalance;
    updateUserInfo();
}void MainWindow::onSellerHistoryReceived(const QString& history)
{
    if (history.isEmpty()) {
        QMessageBox::information(this, "Lịch sử làm chủ", 
            "Bạn chưa tạo đấu giá nào");
        return;
    }
    
    QStringList auctions = history.split('|', Qt::SkipEmptyParts);
    
    qDebug() << "Seller history - Total auctions:" << auctions.size();
    
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("👤 LỊCH SỬ LÀM CHỦ ĐẤU GIÁ");
    dialog->setMinimumSize(1000, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Title
    QLabel *title = new QLabel("👤 CÁC PHIÊN ĐẤU GIÁ BẠN ĐÃ TẠO");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #ff9800; "
        "padding: 15px; background: white; border-radius: 8px;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    // Table
    QTableWidget *table = new QTableWidget(auctions.size(), 8, dialog);
    table->setHorizontalHeaderLabels({
        "#", "🏷️ Sản phẩm", "💵 Giá KĐ", "💰 Giá cuối", 
        "👤 Người thắng", "📊 Tổng lượt", "👥 Người tham gia", "📈 Trạng thái"
    });
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #ff9800, stop:1 #f57c00); color: white; padding: 10px; "
        "font-weight: bold; border: none; }"
    );
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    double totalRevenue = 0;
    int totalBids = 0;
    int soldCount = 0;
    
    int row = 0;
    for (const QString& auctionData : auctions) {
        QStringList fields = auctionData.split(';');
        
        qDebug() << "Row" << row << "fields:" << fields.size() << fields;
        
        // Server format: auctionId;title;startPrice;finalPrice;winner;totalBids;participants;status
        if (fields.size() >= 8) {
            QString auctionTitle = fields[1];
            double startPrice = fields[2].toDouble();
            double finalPrice = fields[3].toDouble();
            QString winner = fields[4];
            int auctionTotalBids = fields[5].toInt();
            int participants = fields[6].toInt();
            // QString status = fields[7];
            
            bool sold = (winner != "No winner");
            if (sold) {
                soldCount++;
                totalRevenue += finalPrice;
            }
            totalBids += auctionTotalBids;
            
            // Column 0: #
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            
            // Column 1: Title
            QTableWidgetItem *titleItem = new QTableWidgetItem(auctionTitle);
            titleItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 1, titleItem);
            
            // Column 2: Start price
            table->setItem(row, 2, new QTableWidgetItem(
                Formatters::formatCurrency(startPrice)));
            
            // Column 3: Final price
            QTableWidgetItem *finalItem = new QTableWidgetItem(
                Formatters::formatCurrency(finalPrice));
            finalItem->setForeground(sold ? QColor("#4caf50") : QColor("#666"));
            finalItem->setFont(QFont("Arial", 11, sold ? QFont::Bold : QFont::Normal));
            table->setItem(row, 3, finalItem);
            
            // Column 4: Winner
            QTableWidgetItem *winnerItem = new QTableWidgetItem(winner);
            winnerItem->setForeground(sold ? QColor("#4caf50") : QColor("#999"));
            table->setItem(row, 4, winnerItem);
            
            // Column 5: Total bids
            QTableWidgetItem *bidsItem = new QTableWidgetItem(QString::number(auctionTotalBids));
            bidsItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 5, bidsItem);
            
            // Column 6: Participants
            QTableWidgetItem *partItem = new QTableWidgetItem(QString::number(participants));
            partItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 6, partItem);
            
            // Column 7: Status
            QTableWidgetItem *statusItem = new QTableWidgetItem(sold ? "✅ Đã bán" : "❌ Chưa bán");
            statusItem->setForeground(sold ? QColor("#4caf50") : QColor("#f44336"));
            statusItem->setFont(QFont("Arial", 11, QFont::Bold));
            statusItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(row, 7, statusItem);
            
            row++;
        }
    }
    
    table->resizeColumnsToContents();
    layout->addWidget(table);
    
    // Stats
    QLabel *stats = new QLabel(QString(
        "📊 <b>Tổng:</b> %1 phiên | "
        "<b style='color:#4caf50;'>✅ Đã bán:</b> %2 | "
        "<b style='color:#f44336;'>❌ Chưa bán:</b> %3 | "
        "<b>Tổng lượt đặt:</b> %4 | "
        "<b style='color:#4caf50;'>💰 Doanh thu:</b> %5"
    ).arg(auctions.size())
     .arg(soldCount)
     .arg(auctions.size() - soldCount)
     .arg(totalBids)
     .arg(Formatters::formatCurrency(totalRevenue)));
    stats->setStyleSheet(
        "font-size: 14px; padding: 15px; background: white; "
        "border-radius: 8px; font-weight: bold;"
    );
    layout->addWidget(stats);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #ff9800; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #f57c00; }"
    );
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    dialog->exec();
    delete dialog;
}
void MainWindow::onNotification(const QString& type, const QString& message)
{
    Q_UNUSED(type); Q_UNUSED(message);
}
void MainWindow::onNewBid(int auctionId, double amount, const QString& bidder)
{
    Q_UNUSED(auctionId);  // ← Đang bỏ qua auctionId!
    
    // Tìm tên auction
    QString auctionTitle = "sản phẩm";
    for (const Auction& a : auctions) {
        if (a.auctionId == auctionId) {
            auctionTitle = a.title;
            break;
        }
    }
    
    addLogMessage(QString("💰 %1 đặt %2 cho '%3'")
        .arg(bidder)
        .arg(Formatters::formatCurrency(amount))
        .arg(auctionTitle), "BID");
    
    // Refresh
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onNewAuction(int auctionId, const QString& title)
{
    Q_UNUSED(auctionId);
    addLogMessage(QString("Mới: %1").arg(title), "INFO");
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onAuctionWarning(int auctionId, int secondsLeft)
{
    Q_UNUSED(auctionId);
    // FIX: Only show if > 0
    if (secondsLeft > 0) {
        addLogMessage(QString("Sắp kết thúc: %1s").arg(secondsLeft), "WARNING");
    }
}void MainWindow::showBidHistoryDialog(const QString& history)
{
    if (history.isEmpty()) {
        QMessageBox::information(this, "Lịch sử đặt giá", 
            "Chưa có lượt đặt giá nào cho sản phẩm này");
        return;
    }
    
    // Parse: bidId;username;amount;timestamp|...
    QStringList bids = history.split('|', Qt::SkipEmptyParts);
    
    // Create dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Lịch Sử Đặt Giá");
    dialog->setMinimumSize(700, 500);
    dialog->setStyleSheet("QDialog { background: #f5f7fa; }");
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Title
    QLabel *title = new QLabel("📊 LỊCH SỬ ĐẶT GIÁ");
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #1976d2; "
        "padding: 15px; background: white; border-radius: 8px;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    // Table
    QTableWidget *table = new QTableWidget(bids.size(), 5, dialog);
    table->setHorizontalHeaderLabels({"#", "👤 Người đặt", "💰 Giá đặt", "📈 Tăng", "🕐 Thời gian"});
    table->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; font-size: 13px; } "
        "QHeaderView::section { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #667eea, stop:1 #764ba2); color: white; padding: 10px; "
        "font-weight: bold; border: none; } "
        "QTableWidget::item { padding: 8px; } "
        "QTableWidget::item:selected { background: #e3f2fd; color: #1976d2; }"
    );
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    
    double prevPrice = 0;
    int row = 0;
    for (const QString& bid : bids) {
        QStringList fields = bid.split(';');
        if (fields.size() >= 4) {
            QString username = fields[1];
            double amount = fields[2].toDouble();
            qint64 timestamp = fields[3].toLongLong();
            QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
            
            // Column 0: #
            QTableWidgetItem *numItem = new QTableWidgetItem(QString::number(row + 1));
            numItem->setTextAlignment(Qt::AlignCenter);
            numItem->setFont(QFont("Arial", 11, QFont::Bold));
            table->setItem(row, 0, numItem);
            
            // Column 1: Username
            QTableWidgetItem *userItem = new QTableWidgetItem(username);
            userItem->setFont(QFont("Arial", 11, QFont::Bold));
            userItem->setForeground(QColor("#1976d2"));
            table->setItem(row, 1, userItem);
            
            // Column 2: Amount
            QTableWidgetItem *amountItem = new QTableWidgetItem(
                Formatters::formatCurrency(amount));
            amountItem->setFont(QFont("Arial", 12, QFont::Bold));
            amountItem->setForeground(QColor("#4caf50"));
            amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(row, 2, amountItem);
            
            // Column 3: Increase
            QString increase = "−";
            if (prevPrice > 0) {
                double diff = amount - prevPrice;
                increase = QString("+%1").arg(Formatters::formatCurrency(diff));
            }
            QTableWidgetItem *incItem = new QTableWidgetItem(increase);
            incItem->setForeground(QColor("#f57c00"));
            incItem->setFont(QFont("Arial", 10, QFont::Bold));
            incItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            table->setItem(row, 3, incItem);
            
            // Column 4: Time
            QTableWidgetItem *timeItem = new QTableWidgetItem(
                dt.toString("dd/MM/yyyy\nhh:mm:ss"));
            timeItem->setFont(QFont("Arial", 10));
            timeItem->setForeground(QColor("#666"));
            table->setItem(row, 4, timeItem);
            
            prevPrice = amount;
            row++;
        }
    }
    
    table->resizeColumnsToContents();
    layout->addWidget(table);
    
    // Stats
    QLabel *stats = new QLabel(QString(
        "📊 Tổng cộng: <b>%1 lượt</b> đặt giá"
    ).arg(bids.size()));
    stats->setStyleSheet(
        "font-size: 14px; padding: 10px; background: white; "
        "border-radius: 8px; color: #666;"
    );
    layout->addWidget(stats);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("✅ Đóng");
    closeBtn->setStyleSheet(
        "QPushButton { background: #4caf50; color: white; padding: 12px; "
        "font-size: 14px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #45a049; }"
    );
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    dialog->exec();
    delete dialog;
}

void MainWindow::onDisconnected()
{
    addLogMessage("MẤT KẾT NỐI!", "ERROR");
    
    if (userHasActiveBids()) {
        QMessageBox::critical(this, "Mất kết nối",
            "⚠️ BẠN ĐANG CÓ GIÁ ĐẶT!\n\n"
            "Kết nối bị mất. Giá của bạn vẫn hiệu lực.\n"
            "Kết nối lại càng sớm càng tốt!");
    }
    
    // Offer reconnect
    QTimer::singleShot(3000, this, [this]() {
        if (!network->isConnected()) {
            auto reply = QMessageBox::question(this, "Kết nối lại",
                "Kết nối lại?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                // Reconnect...
            }
        }
    });
}void MainWindow::showAuctionEndedPopup(const Auction& auction, 
                                       const QString& winner, 
                                       double finalPrice)
{
    bool userWon = (winner == currentUser.username);
    bool userParticipated = false;
    
    // Check if user placed any bid
    for (const Auction& a : auctions) {
        if (a.auctionId == auction.auctionId) {
            // Simplified: assume participated if totalBids > 0 and user in room
            userParticipated = (a.totalBids > 0);
            break;
        }
    }
    
    // Only show popup if user won or participated
    if (!userWon && !userParticipated) {
        // Just log for observers
        addLogMessage(QString("🎉 '%1' kết thúc - %2 thắng với %3")
            .arg(auction.title)
            .arg(winner)
            .arg(Formatters::formatCurrency(finalPrice)), "WIN");
        return;
    }
    
    // Create detailed popup
    QString icon = userWon ? "🎉" : "😢";
    QString title = userWon ? "CHÚC MỪNG BẠN!" : "KẾT QUẢ ĐẤU GIÁ";
    QString bgColor = userWon ? "#e8f5e9" : "#fff3e0";
    QString titleColor = userWon ? "#4caf50" : "#f57c00";
    
    QString message = QString(
        "<div style='background: %1; padding: 20px; border-radius: 10px;'>"
        "<h2 style='color: %2; margin: 0;'>%3 %4</h2>"
        "<hr style='border: 1px solid #e0e0e0;'>"
        "<table style='width: 100%%; font-size: 14px; line-height: 2;'>"
        "<tr><td style='color: #666;'><b>Sản phẩm:</b></td><td><b>%5</b></td></tr>"
        "<tr><td style='color: #666;'><b>Giá khởi điểm:</b></td><td>%6</td></tr>"
        "<tr><td style='color: #666;'><b>Giá cuối:</b></td>"
            "<td style='color: #4caf50; font-size: 16px;'><b>%7</b></td></tr>"
        "<tr><td style='color: #666;'><b>Người thắng:</b></td>"
            "<td style='color: #1976d2;'><b>%8</b></td></tr>"
        "<tr><td style='color: #666;'><b>Tổng lượt đặt:</b></td><td>%9</td></tr>"
        "</table>"
        "<hr style='border: 1px solid #e0e0e0;'>"
        "<p style='font-size: 14px; color: #666; margin: 10px 0;'>%10</p>"
        "</div>"
    ).arg(bgColor)
     .arg(titleColor)
     .arg(icon)
     .arg(title)
     .arg(auction.title)
     .arg(Formatters::formatCurrency(auction.startPrice))
     .arg(Formatters::formatCurrency(finalPrice))
     .arg(winner)  // ← USERNAME sẽ hiện ở đây
     .arg(auction.totalBids)
     .arg(userWon ? 
          QString("🎉 Chúc mừng <b>%1</b>, bạn đã thắng đấu giá!").arg(currentUser.username) :
          QString("Người dùng <b>%1</b> đã đặt giá cao hơn bạn.").arg(winner));
    
    QMessageBox *endBox = new QMessageBox(this);
    endBox->setWindowTitle(title);
    endBox->setText(message);
    endBox->setIcon(userWon ? QMessageBox::Information : QMessageBox::Warning);
    endBox->setStandardButtons(QMessageBox::Ok);
    endBox->exec();
    delete endBox;
}void MainWindow::onAuctionEnded(int auctionId, const QString& winner, double finalPrice)
{
    // Remove from warned list
    warnedAuctions.remove(auctionId);
    
    // Find auction
    Auction endedAuction;
    for (const Auction& a : auctions) {
        if (a.auctionId == auctionId) {
            endedAuction = a;
            break;
        }
    }
    
    // Show popup if auction found
    if (endedAuction.auctionId > 0) {
        showAuctionEndedPopup(endedAuction, winner, finalPrice);
    }
    
    addLogMessage(QString("🎉 %1 thắng: %2")
        .arg(winner)
        .arg(Formatters::formatCurrency(finalPrice)), "WIN");
    
    if (currentUser.isInRoom()) {
        network->sendListAuctions(currentUser.currentRoomId);
    }
}

void MainWindow::onUserJoinedRoom(const QString& username)
{
    addLogMessage(QString("👋 %1 vào phòng").arg(username), "INFO");
    
    // Refresh room list để update số người
    network->sendListRooms();
}

void MainWindow::onUserLeftRoom(const QString& username)
{
    addLogMessage(QString("%1 rời").arg(username), "INFO");
    // FIX: Update room list
    network->sendListRooms();
}

void MainWindow::onRoomError(const QString& error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::onAuctionError(const QString& error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::onBidError(const QString& error)
{
    addLogMessage(error, "ERROR");
}

void MainWindow::updateCountdowns()
{
    if (currentUser.isInRoom() && !auctions.isEmpty()) {
        int currentRow = auctionsList->currentRow();
        
        int displayIndex = 0;
        for (int i = 0; i < auctions.size(); i++) {
            if (auctions[i].isActive()) {
                if (displayIndex < auctionsList->count()) {
                    QString status = Formatters::formatTime(auctions[i].getTimeLeft());
                    QString text = QString("🔨 %1 | 💰 %2 | ⏱️ %3")
                        .arg(auctions[i].title)
                        .arg(Formatters::formatCurrency(auctions[i].currentPrice))
                        .arg(status);
                    auctionsList->item(displayIndex)->setText(text);
                    displayIndex++;
                }
            }
        }
        
        if (currentRow >= 0) {
            auctionsList->setCurrentRow(currentRow);
        }
    }
}