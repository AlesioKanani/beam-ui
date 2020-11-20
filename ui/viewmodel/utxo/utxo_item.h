// Copyright 2018 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#include <QObject>
#include "wallet/client/wallet_client.h"
#include "utxo_view_status.h"
#include "utxo_view_type.h"

class BaseUtxoItem : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString amount                 READ getAmountWithCurrency     CONSTANT)
        Q_PROPERTY(QString maturity               READ maturity                  CONSTANT)
        Q_PROPERTY(QString maturityPercentage     READ maturityPercentage        CONSTANT)
        Q_PROPERTY(int status                     READ status                    CONSTANT)
        Q_PROPERTY(int type                       READ type                      CONSTANT)
public:

    BaseUtxoItem() = default;
    bool operator==(const BaseUtxoItem& other) const;

    virtual uint64_t getHash() const = 0;
    virtual QString getAmountWithCurrency() const = 0;
    virtual QString getAmount() const = 0;
    virtual QString maturity() const = 0;
    virtual QString maturityPercentage() const = 0;
    virtual UtxoViewStatus::EnStatus status() const = 0;
    virtual UtxoViewType::EnType type() const = 0;

    virtual beam::Amount rawAmount() const = 0;
    virtual beam::Height rawMaturity() const = 0;
};

class UtxoItem : public BaseUtxoItem
{
public:

    UtxoItem() = default;
    UtxoItem(const beam::wallet::Coin& coin);
    uint64_t getHash() const override;

    QString getAmountWithCurrency() const override;
    QString getAmount() const override;
    QString maturity() const override;
    QString maturityPercentage() const override;
    UtxoViewStatus::EnStatus status() const override;
    UtxoViewType::EnType type() const override;

    beam::Amount rawAmount() const override;
    beam::Height rawMaturity() const override;
    const beam::wallet::Coin::ID& get_ID() const;
private:
    beam::wallet::Coin _coin;
};

class ShieldedCoinItem : public BaseUtxoItem
{
public:

    ShieldedCoinItem() = default;
    ShieldedCoinItem(const beam::wallet::ShieldedCoin& coin, const beam::TxoID& shieldedCount);
    uint64_t getHash() const override;

    QString getAmountWithCurrency() const override;
    QString getAmount() const override;
    QString maturity() const override;
    QString maturityPercentage() const override;
    UtxoViewStatus::EnStatus status() const override;
    UtxoViewType::EnType type() const override;

    beam::Amount rawAmount() const override;
    beam::Height rawMaturity() const override;
private:
    beam::wallet::ShieldedCoin _coin;
    beam::TxoID _shieldedCount = std::numeric_limits<beam::TxoID>::max();
};