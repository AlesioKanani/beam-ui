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

#ifdef BEAM_HW_WALLET
#include "keykeeper/hw_wallet.h"
#endif

#include <set>

class WalletModel
    : public QObject
    , public beam::wallet::WalletClient
#ifdef BEAM_HW_WALLET
    , public beam::wallet::HWWallet::IHandler
#endif
{
    Q_OBJECT
public:

    using Ptr = std::shared_ptr<WalletModel>;

    WalletModel(beam::wallet::IWalletDB::Ptr walletDB, const std::string& nodeAddr, beam::io::Reactor::Ptr reactor);
    ~WalletModel() override;

    QString GetErrorString(beam::wallet::ErrorType type);
    bool isOwnAddress(const beam::wallet::WalletID& walletID) const;
    bool isAddressWithCommentExist(const std::string& comment) const;

    beam::Amount getAvailable() const;
    beam::Amount getReceiving() const;
    beam::Amount getReceivingIncoming() const;
    beam::Amount getReceivingChange() const;
    beam::Amount getSending() const;
    beam::Amount getMaturing() const;
    beam::Height getCurrentHeight() const;
    beam::Timestamp getCurrentHeightTimestamp() const;
    beam::Block::SystemState::ID getCurrentStateID() const;
    beam::Amount getLinked() const;
    beam::Amount getUnlinked() const;
    beam::Amount getShielded() const;

signals:
    void walletStatus(const beam::wallet::WalletStatus& status);
    void transactionsChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::TxDescription>& items);
    void syncProgressUpdated(int done, int total);
    void changeCalculated(beam::Amount change);
    void allUtxoChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::Coin>& utxos);
    void addressesChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::WalletAddress>& addresses);
    void addressesChanged(bool own, const std::vector<beam::wallet::WalletAddress>& addresses);
    void swapOffersChanged(beam::wallet::ChangeAction action, const std::vector<beam::wallet::SwapOffer>& offers);
    void generatedNewAddress(const beam::wallet::WalletAddress& walletAddr);
    void swapParamsLoaded(const beam::ByteBuffer& params);
    void newAddressFailed();
    void nodeConnectionChanged(bool isNodeConnected);
    void walletError(beam::wallet::ErrorType error);
    void sendMoneyVerified();
    void cantSendToExpired();
    void paymentProofExported(const beam::wallet::TxID& txID, const QString& proof);
    void addressChecked(const QString& addr, bool isValid);

    void availableChanged();
    void receivingChanged();
    void receivingIncomingChanged();
    void receivingChangeChanged();
    void sendingChanged();
    void maturingChanged();
    void stateIDChanged();
    void functionPosted(const std::function<void()>&);
#if defined(BEAM_HW_WALLET)
    void showTrezorMessage();
    void hideTrezorMessage();
    void showTrezorError(const QString& error);
#endif
    void txHistoryExportedToCsv(const QString& data);
    
    void exchangeRatesUpdate(const std::vector<beam::wallet::ExchangeRate>&);
    void notificationsChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::Notification>&);

    void linkedChanged();
    void unlinkedChanged();
    void shieldedChanged();

private:
    void onStatus(const beam::wallet::WalletStatus& status) override;
    void onTxStatus(beam::wallet::ChangeAction, const std::vector<beam::wallet::TxDescription>& items) override;
    void onSyncProgressUpdated(int done, int total) override;
    void onChangeCalculated(beam::Amount change) override;
    void onAllUtxoChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::Coin>& utxos) override;
    void onAddressesChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::WalletAddress>& items) override;
    void onAddresses(bool own, const std::vector<beam::wallet::WalletAddress>& addrs) override;
#ifdef BEAM_ATOMIC_SWAP_SUPPORT
    void onSwapOffersChanged(beam::wallet::ChangeAction action, const std::vector<beam::wallet::SwapOffer>& offers) override;
#endif  // BEAM_ATOMIC_SWAP_SUPPORT
    void onGeneratedNewAddress(const beam::wallet::WalletAddress& walletAddr) override;
    void onSwapParamsLoaded(const beam::ByteBuffer& token) override;
    void onNewAddressFailed() override;
    void onNodeConnectionChanged(bool isNodeConnected) override;
    void onWalletError(beam::wallet::ErrorType error) override;
    void FailedToStartWallet() override;
    void onSendMoneyVerified() override;
    void onCantSendToExpired() override;
    void onPaymentProofExported(const beam::wallet::TxID& txID, const beam::ByteBuffer& proof) override;
    void onCoinsByTx(const std::vector<beam::wallet::Coin>& coins) override;
    void onAddressChecked(const std::string& addr, bool isValid) override;
    void onImportRecoveryProgress(uint64_t done, uint64_t total) override;
    void onNoDeviceConnected() override;
    void onImportDataFromJson(bool isOk) override;
    void onExportDataToJson(const std::string& data) override;
    void onExportTxHistoryToCsv(const std::string& data) override;
    void onExchangeRates(const std::vector<beam::wallet::ExchangeRate>&) override;
    void onNotificationsChanged(beam::wallet::ChangeAction, const std::vector<beam::wallet::Notification>&) override;
#ifdef BEAM_HW_WALLET
    void ShowKeyKeeperMessage() override;
    void HideKeyKeeperMessage() override;
    void ShowKeyKeeperError(const std::string&) override;
#endif // BEAM_HW_WALLET
    void onPostFunctionToClientContext(MessageFunction&& func) override;
    beam::Version getLibVersion() const override;
    uint32_t getClientRevision() const override;

private slots:
    void setStatus(const beam::wallet::WalletStatus& status);
    void setAddresses(bool own, const std::vector<beam::wallet::WalletAddress>& addrs);
    void doFunction(const std::function<void()>& func);

private:
    std::set<beam::wallet::WalletID> m_myWalletIds;
    std::set<std::string> m_myAddrLabels;
    beam::wallet::WalletStatus m_status;
};
