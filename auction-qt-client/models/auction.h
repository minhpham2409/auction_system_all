#ifndef AUCTION_H
#define AUCTION_H

#include <QString>
#include <QDateTime>

class Auction
{
public:
    int auctionId;
    int sellerId;
    int roomId;
    QString title;
    QString description;
    QString sellerName;
    double startPrice;
    double currentPrice;
    double buyNowPrice;
    double minIncrement;
    int currentBidderId;
    QString currentBidderName;
    int totalBids;
    qint64 endTime;
    QString status;
    
    Auction() 
        : auctionId(0), 
          sellerId(0), 
          roomId(0),
          startPrice(0.0), 
          currentPrice(0.0), 
          buyNowPrice(0.0),
          minIncrement(1000.0),
          currentBidderId(0), 
          totalBids(0), 
          endTime(0)
    {}
    
    bool isActive() const { 
        return status == "active"; 
    }
    
    bool isWaiting() const { 
        return status == "waiting"; 
    }
    
    bool isEnded() const { 
        return status == "ended"; 
    }
    
    bool canBid() const { 
        return isActive() && getTimeLeft() > 0; 
    }
    
    bool hasBuyNow() const { 
        return buyNowPrice > 0; 
    }
    
    int getTimeLeft() const {
        if (!isActive() || endTime == 0) return 0;
        qint64 now = QDateTime::currentSecsSinceEpoch();
        int left = endTime - now;
        return (left > 0) ? left : 0;
    }
    
    QString getStatusText() const {
        if (isActive()) return "Đang đấu giá";
        if (isWaiting()) return "Chờ kích hoạt";
        if (isEnded()) return "Đã kết thúc";
        return "Không xác định";
    }
};

#endif // AUCTION_H