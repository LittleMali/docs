#include "ScanButton.h"
#include "CustomScanDialog.h"
#include "scancommon/ScanCommon.h"

#define FASTSCAN_NORMAL_STYLE "border-style:none;color:#FFFFFF;font:22px;background-color: #00D473"
#define FASTSCAN_HOVER_STYLE "border-style:none;color:#FFFFFF;font:22px;background-color: #00D493"

#define MENU_NORMAL_STYLE "border-style:none;color:white;font:22px;background-color: #00D473"
#define MENU_HOVER_STYLE "border-style:none;color:white;font:22px;background-color: #00D493"

#define MENU_SYTLE "QMenu {\
                       font-size: 22px; \
                       background-color: #00CA72;\
                       border: 0px;\
                    }\
                    QMenu::item {\
                       color: #FFFFFF;\
                       background-color: #00CA72;\
                       padding-left:26px; \
                       padding-right:22px; \
                       padding-top:8px; \
                       padding-bottom:8px; \
                       border-bottom:0px solid; \
                    }\
                    QMenu::item:selected {\
                       color: #FFFFFF;\
                       background-color: #00CA92;\
                    }"

ScanButton::ScanButton(QWidget *parent): QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setFixedSize(180, 48);

    Init();
}

ScanButton::~ScanButton()
{
    Uninit();
}

void ScanButton::Init()
{
    m_pMainLayout = new QHBoxLayout(this);

    InitControl();
    InitMenu();

    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void ScanButton::Uninit()
{
}

void ScanButton::InitControl()
{
    QSplitter* pButtonSplitter = new QSplitter(this);
    pButtonSplitter->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pButtonSplitter->setOrientation(Qt::Horizontal);
    pButtonSplitter->setHandleWidth(1);
    pButtonSplitter->setStyleSheet("QSplitter::handle { background-color: white }");

    CustomToolButton* pFastScanButton = new CustomToolButton(this, 139, 48);
    pFastScanButton->setFixedSize(139, 48);
    pFastScanButton->SetText(tr("闪电杀毒"));
    pFastScanButton->setContentsMargins(0, 0, 0, 0);

    pFastScanButton->SetNormalStyle(FASTSCAN_NORMAL_STYLE);
    pFastScanButton->SetHoverStyle(FASTSCAN_HOVER_STYLE);
    connect(pFastScanButton, SIGNAL(clicked()), this, SLOT(OnClickFastScan()));

    pButtonSplitter->addWidget(pFastScanButton);

    m_pMenuButton = new CustomToolButton(this, 40, 48);
    m_pMenuButton->setFixedSize(40, 48);
    m_pMenuButton->SetText(tr("∨"));
    m_pMenuButton->SetTextStyle("font:12px");
    m_pMenuButton->setContentsMargins(0, 0, 0, 0);
    m_pMenuButton->SetNormalStyle(MENU_NORMAL_STYLE);
    m_pMenuButton->SetHoverStyle(MENU_HOVER_STYLE);
    connect(m_pMenuButton , SIGNAL(clicked()), this, SLOT(OnMenuButtonClicked()));

    pButtonSplitter->addWidget(m_pMenuButton);

    QSplitterHandle *buttonHandle = pButtonSplitter->handle(1);
    buttonHandle->setEnabled(false);

    m_pMainLayout->addWidget(pButtonSplitter, 0, Qt::AlignLeft);
}

void ScanButton::InitMenu()
{
    m_pMenu = new QMenu(this);
    QAction *pFullScanButton = new QAction(this);
    QAction *pCustomScanButton = new QAction(this);

    pFullScanButton->setText("全盘杀毒");
    pCustomScanButton->setText("指定位置杀毒");

    m_pMenu->addAction(pFullScanButton);
    m_pMenu->addAction(pCustomScanButton);

    m_pMenu->setStyleSheet(MENU_SYTLE);

    connect(pFullScanButton, SIGNAL(triggered()), this, SLOT(OnClickFullScan()));
    connect(pCustomScanButton, SIGNAL(triggered()), this, SLOT(OnClickCustomScan()));
}

void ScanButton::OnMenuButtonClicked()
{
    QPoint pos;
    pos.setX(-140);
    pos.setY(48);
    m_pMenu->exec(m_pMenuButton->mapToGlobal(pos));
}

void ScanButton::OnClickFastScan()
{
    std::vector<std::string> pathList;
    CScanUtil::GetFastScanPaths(pathList);

    Scan(ST_FAST_SCAN, pathList);
}

void ScanButton::OnClickFullScan()
{
    std::vector<std::string> pathList;
    CScanUtil::GetFullScanPaths(pathList);

    Scan(ST_FULL_SCAN, pathList);
}

void ScanButton::OnClickCustomScan()
{
    CustomScanDialog dialog;
    connect(&dialog, SIGNAL(OnCustomScan(std::vector<std::string>&)), this, SLOT(OnCustomScan(std::vector<std::string>&)));
    dialog.exec();
}

void ScanButton::OnCustomScan(std::vector<std::string> &pathList)
{
    Scan(ST_CUSTOM_SCAN, pathList);
}
