#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "clientmodel.h"
#include "types.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"

#include "./overviewpage.moc"

OverviewPage::OverviewPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverviewPage),
    currentBalance(-1),
    currentStake(0),
    currentUnconfirmedBalance(-1),
    txdelegate(new TxViewDelegate())
{
    ui->setupUi(this);

    // Balance: <balance>
    ui->labelBalance->setFont(QFont("Monospace", -1, QFont::Bold));
    ui->labelBalance->setToolTip(tr("Your current balance"));
    ui->labelBalance->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    // ppcoin: stake: <stake>
    ui->labelStake->setFont(QFont("Monospace", -1, QFont::Bold));
    ui->labelStake->setToolTip(tr("Your current stake"));
    ui->labelStake->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    // Unconfirmed balance: <balance>
    ui->labelUnconfirmed->setFont(QFont("Monospace", -1, QFont::Bold));
    ui->labelUnconfirmed->setToolTip(tr("Total of transactions that have yet to be confirmed, and do not yet count toward the current balance"));
    ui->labelUnconfirmed->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    ui->labelNumTransactions->setToolTip(tr("Total number of transactions in wallet"));

    // Recent transactions
    ui->listTransactions->setStyleSheet("QListView { background:transparent }");
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SIGNAL(transactionClicked(QModelIndex)));
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

void OverviewPage::setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance)
{
    int unit = model->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentStake = stake;
    currentUnconfirmedBalance = unconfirmedBalance;
    ui->labelBalance->setText(BitcoinUnits::formatWithUnit(unit, balance));
    ui->labelStake->setText(BitcoinUnits::formatWithUnit(unit, stake));
    ui->labelUnconfirmed->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
}

void OverviewPage::setNumTransactions(int count)
{
    ui->labelNumTransactions->setText(QLocale::system().toString(count));
}

void OverviewPage::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;

    if (clientModel)
    {
        this->setHeadHash(clientModel->getHeadHash());
        connect(clientModel, SIGNAL(headChanged(QString)), this, SLOT(setHeadHash(QString)));
        connect(clientModel, SIGNAL(headChanged(QString)), this, SLOT(refreshEstimatedStakeTime()));

        this->setNumBlocks(clientModel->getNumBlocks());
        connect(clientModel, SIGNAL(numBlocksChanged(int)), this, SLOT(setNumBlocks(int)));
    }
}

void OverviewPage::setHeadHash(QString headHash)
{
    this->ui->labelHead->setText(headHash.right(8));
}

void OverviewPage::setNumBlocks(int count)
{
    this->ui->labelHeight->setText(QLocale::system().toString(count));
}

void OverviewPage::setModel(WalletModel *model)
{
    this->model = model;

    if (model)
    {
        // Set up transaction list
        TransactionFilterProxy *filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        this->ui->listTransactions->setModel(filter);
        this->ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        this->refreshEstimatedStakeTime();
        connect(model, SIGNAL(encryptionStatusChanged(int)), this, SLOT(refreshEstimatedStakeTime()));

        this->setBalance(model->getBalance(), model->getStake(), model->getUnconfirmedBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        this->setNumTransactions(model->getNumTransactions());
        connect(model, SIGNAL(numTransactionsChanged(int)), this, SLOT(setNumTransactions(int)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(displayUnitChanged()));
    }
}

void OverviewPage::refreshEstimatedStakeTime()
{
    timestamp_t estimatedStakeTime = this->model->getEstimatedStakeTime();

    if (estimatedStakeTime == -1)
    {
        this->ui->labelEstimatedStakeTime->setText(tr("Not minting") + " <sup><a href='https://github.com/NeuCoin/neucoin-dev/blob/master/doc/faq.md#what-does-estimated-time-to-stake-means'>" + tr("(why?)") + "</a></sup>");
    }
    else if (estimatedStakeTime >= MONTH)
    {
        timestamp_t months = estimatedStakeTime / MONTH + 1;
        this->ui->labelEstimatedStakeTime->setText(tr("± %n month(s)", NULL, months));
    }
    else if (estimatedStakeTime >= WEEK)
    {
        timestamp_t weeks = estimatedStakeTime / WEEK + 1;
        this->ui->labelEstimatedStakeTime->setText(tr("± %n week(s)", NULL, weeks));
    }
    else if (estimatedStakeTime >= DAY)
    {
        timestamp_t days = estimatedStakeTime / DAY + 1;
        this->ui->labelEstimatedStakeTime->setText(tr("± %n day(s)", NULL, days));
    }
    else if (estimatedStakeTime >= HOUR)
    {
        timestamp_t hours = estimatedStakeTime / HOUR + 1;
        this->ui->labelEstimatedStakeTime->setText(tr("± %n hour(s)", NULL, hours));
    }
    else
    {
        timestamp_t minutes = estimatedStakeTime / MINUTE + 1;
        this->ui->labelEstimatedStakeTime->setText(tr("± %n minute(s)", NULL, minutes));
    }
}

void OverviewPage::displayUnitChanged()
{
    if(!model || !model->getOptionsModel())
        return;
    if(currentBalance != -1)
        setBalance(currentBalance, currentStake, currentUnconfirmedBalance);

    txdelegate->unit = model->getOptionsModel()->getDisplayUnit();
    ui->listTransactions->update();
}
