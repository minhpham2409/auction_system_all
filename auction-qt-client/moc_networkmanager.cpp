/****************************************************************************
** Meta object code from reading C++ file 'networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "core/networkmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkManager_t {
    QByteArrayData data[70];
    char stringdata0[845];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkManager_t qt_meta_stringdata_NetworkManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetworkManager"
QT_MOC_LITERAL(1, 15, 9), // "connected"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "disconnected"
QT_MOC_LITERAL(4, 39, 15), // "connectionError"
QT_MOC_LITERAL(5, 55, 5), // "error"
QT_MOC_LITERAL(6, 61, 21), // "searchResultsReceived"
QT_MOC_LITERAL(7, 83, 7), // "results"
QT_MOC_LITERAL(8, 91, 12), // "loginSuccess"
QT_MOC_LITERAL(9, 104, 4), // "User"
QT_MOC_LITERAL(10, 109, 4), // "user"
QT_MOC_LITERAL(11, 114, 11), // "loginFailed"
QT_MOC_LITERAL(12, 126, 6), // "reason"
QT_MOC_LITERAL(13, 133, 15), // "registerSuccess"
QT_MOC_LITERAL(14, 149, 14), // "registerFailed"
QT_MOC_LITERAL(15, 164, 13), // "logoutSuccess"
QT_MOC_LITERAL(16, 178, 23), // "auctionDeletedBroadcast"
QT_MOC_LITERAL(17, 202, 9), // "auctionId"
QT_MOC_LITERAL(18, 212, 5), // "title"
QT_MOC_LITERAL(19, 218, 21), // "sellerHistoryReceived"
QT_MOC_LITERAL(20, 240, 7), // "history"
QT_MOC_LITERAL(21, 248, 19), // "roomHistoryReceived"
QT_MOC_LITERAL(22, 268, 11), // "roomCreated"
QT_MOC_LITERAL(23, 280, 6), // "roomId"
QT_MOC_LITERAL(24, 287, 16), // "roomListReceived"
QT_MOC_LITERAL(25, 304, 11), // "QList<Room>"
QT_MOC_LITERAL(26, 316, 5), // "rooms"
QT_MOC_LITERAL(27, 322, 10), // "joinedRoom"
QT_MOC_LITERAL(28, 333, 8), // "roomName"
QT_MOC_LITERAL(29, 342, 8), // "leftRoom"
QT_MOC_LITERAL(30, 351, 9), // "roomError"
QT_MOC_LITERAL(31, 361, 14), // "auctionCreated"
QT_MOC_LITERAL(32, 376, 19), // "auctionListReceived"
QT_MOC_LITERAL(33, 396, 14), // "QList<Auction>"
QT_MOC_LITERAL(34, 411, 8), // "auctions"
QT_MOC_LITERAL(35, 420, 14), // "auctionDetails"
QT_MOC_LITERAL(36, 435, 7), // "Auction"
QT_MOC_LITERAL(37, 443, 7), // "auction"
QT_MOC_LITERAL(38, 451, 14), // "auctionDeleted"
QT_MOC_LITERAL(39, 466, 16), // "auctionActivated"
QT_MOC_LITERAL(40, 483, 12), // "auctionError"
QT_MOC_LITERAL(41, 496, 9), // "bidPlaced"
QT_MOC_LITERAL(42, 506, 8), // "bidError"
QT_MOC_LITERAL(43, 515, 13), // "buyNowSuccess"
QT_MOC_LITERAL(44, 529, 18), // "bidHistoryReceived"
QT_MOC_LITERAL(45, 548, 14), // "balanceUpdated"
QT_MOC_LITERAL(46, 563, 10), // "newBalance"
QT_MOC_LITERAL(47, 574, 22), // "auctionHistoryReceived"
QT_MOC_LITERAL(48, 597, 12), // "notification"
QT_MOC_LITERAL(49, 610, 4), // "type"
QT_MOC_LITERAL(50, 615, 7), // "message"
QT_MOC_LITERAL(51, 623, 14), // "userJoinedRoom"
QT_MOC_LITERAL(52, 638, 8), // "username"
QT_MOC_LITERAL(53, 647, 12), // "userLeftRoom"
QT_MOC_LITERAL(54, 660, 10), // "newAuction"
QT_MOC_LITERAL(55, 671, 6), // "newBid"
QT_MOC_LITERAL(56, 678, 6), // "amount"
QT_MOC_LITERAL(57, 685, 6), // "bidder"
QT_MOC_LITERAL(58, 692, 14), // "auctionWarning"
QT_MOC_LITERAL(59, 707, 11), // "secondsLeft"
QT_MOC_LITERAL(60, 719, 12), // "auctionEnded"
QT_MOC_LITERAL(61, 732, 6), // "winner"
QT_MOC_LITERAL(62, 739, 10), // "finalPrice"
QT_MOC_LITERAL(63, 750, 6), // "kicked"
QT_MOC_LITERAL(64, 757, 11), // "onConnected"
QT_MOC_LITERAL(65, 769, 14), // "onDisconnected"
QT_MOC_LITERAL(66, 784, 11), // "onReadyRead"
QT_MOC_LITERAL(67, 796, 7), // "onError"
QT_MOC_LITERAL(68, 804, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(69, 833, 11) // "socketError"

    },
    "NetworkManager\0connected\0\0disconnected\0"
    "connectionError\0error\0searchResultsReceived\0"
    "results\0loginSuccess\0User\0user\0"
    "loginFailed\0reason\0registerSuccess\0"
    "registerFailed\0logoutSuccess\0"
    "auctionDeletedBroadcast\0auctionId\0"
    "title\0sellerHistoryReceived\0history\0"
    "roomHistoryReceived\0roomCreated\0roomId\0"
    "roomListReceived\0QList<Room>\0rooms\0"
    "joinedRoom\0roomName\0leftRoom\0roomError\0"
    "auctionCreated\0auctionListReceived\0"
    "QList<Auction>\0auctions\0auctionDetails\0"
    "Auction\0auction\0auctionDeleted\0"
    "auctionActivated\0auctionError\0bidPlaced\0"
    "bidError\0buyNowSuccess\0bidHistoryReceived\0"
    "balanceUpdated\0newBalance\0"
    "auctionHistoryReceived\0notification\0"
    "type\0message\0userJoinedRoom\0username\0"
    "userLeftRoom\0newAuction\0newBid\0amount\0"
    "bidder\0auctionWarning\0secondsLeft\0"
    "auctionEnded\0winner\0finalPrice\0kicked\0"
    "onConnected\0onDisconnected\0onReadyRead\0"
    "onError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      37,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  219,    2, 0x06 /* Public */,
       3,    0,  220,    2, 0x06 /* Public */,
       4,    1,  221,    2, 0x06 /* Public */,
       6,    1,  224,    2, 0x06 /* Public */,
       8,    1,  227,    2, 0x06 /* Public */,
      11,    1,  230,    2, 0x06 /* Public */,
      13,    0,  233,    2, 0x06 /* Public */,
      14,    1,  234,    2, 0x06 /* Public */,
      15,    0,  237,    2, 0x06 /* Public */,
      16,    2,  238,    2, 0x06 /* Public */,
      19,    1,  243,    2, 0x06 /* Public */,
      21,    1,  246,    2, 0x06 /* Public */,
      22,    1,  249,    2, 0x06 /* Public */,
      24,    1,  252,    2, 0x06 /* Public */,
      27,    2,  255,    2, 0x06 /* Public */,
      29,    0,  260,    2, 0x06 /* Public */,
      30,    1,  261,    2, 0x06 /* Public */,
      31,    1,  264,    2, 0x06 /* Public */,
      32,    1,  267,    2, 0x06 /* Public */,
      35,    1,  270,    2, 0x06 /* Public */,
      38,    0,  273,    2, 0x06 /* Public */,
      39,    0,  274,    2, 0x06 /* Public */,
      40,    1,  275,    2, 0x06 /* Public */,
      41,    0,  278,    2, 0x06 /* Public */,
      42,    1,  279,    2, 0x06 /* Public */,
      43,    0,  282,    2, 0x06 /* Public */,
      44,    1,  283,    2, 0x06 /* Public */,
      45,    1,  286,    2, 0x06 /* Public */,
      47,    1,  289,    2, 0x06 /* Public */,
      48,    2,  292,    2, 0x06 /* Public */,
      51,    1,  297,    2, 0x06 /* Public */,
      53,    1,  300,    2, 0x06 /* Public */,
      54,    2,  303,    2, 0x06 /* Public */,
      55,    3,  308,    2, 0x06 /* Public */,
      58,    2,  315,    2, 0x06 /* Public */,
      60,    3,  320,    2, 0x06 /* Public */,
      63,    1,  327,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      64,    0,  330,    2, 0x08 /* Private */,
      65,    0,  331,    2, 0x08 /* Private */,
      66,    0,  332,    2, 0x08 /* Private */,
      67,    1,  333,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   17,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   23,   28,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, 0x80000000 | 36,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::Double,   46,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   49,   50,
    QMetaType::Void, QMetaType::QString,   52,
    QMetaType::Void, QMetaType::QString,   52,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   17,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::QString,   17,   56,   57,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   17,   59,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Double,   17,   61,   62,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 68,   69,

       0        // eod
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->searchResultsReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->loginSuccess((*reinterpret_cast< User(*)>(_a[1]))); break;
        case 5: _t->loginFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->registerSuccess(); break;
        case 7: _t->registerFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->logoutSuccess(); break;
        case 9: _t->auctionDeletedBroadcast((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->sellerHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->roomHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->roomCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->roomListReceived((*reinterpret_cast< const QList<Room>(*)>(_a[1]))); break;
        case 14: _t->joinedRoom((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 15: _t->leftRoom(); break;
        case 16: _t->roomError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->auctionCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->auctionListReceived((*reinterpret_cast< const QList<Auction>(*)>(_a[1]))); break;
        case 19: _t->auctionDetails((*reinterpret_cast< const Auction(*)>(_a[1]))); break;
        case 20: _t->auctionDeleted(); break;
        case 21: _t->auctionActivated(); break;
        case 22: _t->auctionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->bidPlaced(); break;
        case 24: _t->bidError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->buyNowSuccess(); break;
        case 26: _t->bidHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 27: _t->balanceUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 28: _t->auctionHistoryReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 29: _t->notification((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 30: _t->userJoinedRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 31: _t->userLeftRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->newAuction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 33: _t->newBid((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 34: _t->auctionWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 35: _t->auctionEnded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 36: _t->kicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 37: _t->onConnected(); break;
        case 38: _t->onDisconnected(); break;
        case 39: _t->onReadyRead(); break;
        case 40: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 40:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::searchResultsReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(User );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::loginSuccess)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::loginFailed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::registerSuccess)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::registerFailed)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::logoutSuccess)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDeletedBroadcast)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::sellerHistoryReceived)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomHistoryReceived)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomCreated)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QList<Room> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomListReceived)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::joinedRoom)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::leftRoom)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::roomError)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionCreated)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QList<Auction> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionListReceived)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const Auction & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDetails)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionDeleted)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionActivated)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionError)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidPlaced)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidError)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::buyNowSuccess)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::bidHistoryReceived)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::balanceUpdated)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionHistoryReceived)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::notification)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::userJoinedRoom)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::userLeftRoom)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::newAuction)) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , double , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::newBid)) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionWarning)) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(int , const QString & , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::auctionEnded)) {
                *result = 35;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkManager::kicked)) {
                *result = 36;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkManager.data,
    qt_meta_data_NetworkManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkManager::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkManager::searchResultsReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkManager::loginSuccess(User _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkManager::loginFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkManager::registerSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void NetworkManager::registerFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void NetworkManager::logoutSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void NetworkManager::auctionDeletedBroadcast(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkManager::sellerHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkManager::roomHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkManager::roomCreated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkManager::roomListReceived(const QList<Room> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void NetworkManager::joinedRoom(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkManager::leftRoom()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void NetworkManager::roomError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void NetworkManager::auctionCreated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void NetworkManager::auctionListReceived(const QList<Auction> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void NetworkManager::auctionDetails(const Auction & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void NetworkManager::auctionDeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void NetworkManager::auctionActivated()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void NetworkManager::auctionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void NetworkManager::bidPlaced()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void NetworkManager::bidError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void NetworkManager::buyNowSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void NetworkManager::bidHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void NetworkManager::balanceUpdated(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void NetworkManager::auctionHistoryReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void NetworkManager::notification(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void NetworkManager::userJoinedRoom(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void NetworkManager::userLeftRoom(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void NetworkManager::newAuction(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void NetworkManager::newBid(int _t1, double _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void NetworkManager::auctionWarning(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void NetworkManager::auctionEnded(int _t1, const QString & _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void NetworkManager::kicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
