// Copyright (c) 2012-2022 The Sumcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef SUMCOIN_QT_MINTINGFILTERPROXY_H
#define SUMCOIN_QT_MINTINGFILTERPROXY_H

#include <QSortFilterProxyModel>

class MintingFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MintingFilterProxy(QObject *parent = 0);
};

#endif // SUMCOIN_QT_MINTINGFILTERPROXY_H
