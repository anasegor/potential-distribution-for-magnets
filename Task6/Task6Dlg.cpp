
// Task6Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Task6.h"
#include "Task6Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CTask6Dlg



CTask6Dlg::CTask6Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK6_DIALOG, pParent)
	, Q(100)
	, a(20)
	, l(30)
	, d(5)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTask6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, Isol);
	DDX_Control(pDX, IDC_CHECK2, powerLines);
	DDX_Text(pDX, IDC_EDIT1, Q);
	DDX_Text(pDX, IDC_EDIT2, a);
	DDX_Text(pDX, IDC_EDIT3, l);
	DDX_Text(pDX, IDC_EDIT5, d);
}

BEGIN_MESSAGE_MAP(CTask6Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTask6Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CTask6Dlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTask6Dlg::OnClickedCheck2)
END_MESSAGE_MAP()


// Обработчики сообщений CTask6Dlg

BOOL CTask6Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	main.Create(GetDlgItem(IDC_PIC)->GetSafeHwnd());
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTask6Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTask6Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTask6Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTask6Dlg::InitCells()
{
	//инициализация всех клеток
	for (int x = 0; x < Height; x++)
	{
		for (int y = 0; y < Width; y++)
		{
			cells[x][y].pt1.x = x;
			cells[x][y].pt1.y = y;
			cells[x][y].pt1.phi = 0;

			cells[x][y].pt2.x = x + 1;
			cells[x][y].pt2.y = y;
			cells[x][y].pt2.phi = 0;

			cells[x][y].pt3.x = x + 1;
			cells[x][y].pt3.y = y + 1;
			cells[x][y].pt3.phi = 0;

			cells[x][y].pt4.x = x;
			cells[x][y].pt4.y = y + 1;
			cells[x][y].pt4.phi = 0;
		}
	}
	//левая скобка
	for (int x = x_mid - l / 2 - a/2; x < x_mid - l / 2; x++) {
		for (int y = y_mid + a / 2-d; y < y_mid + a / 2; y++) {
			cells[x][y].pt1.phi = phi;
			cells[x][y].pt2.phi = phi;
			cells[x][y].pt3.phi = phi;
			cells[x][y].pt4.phi = phi;
			potential0[x][y] = phi;
		}
		for (int y = y_mid-a/2; y < y_mid - a / 2+d; y++) {
			cells[x][y].pt1.phi = phi;
			cells[x][y].pt2.phi = phi;
			cells[x][y].pt3.phi = phi;
			cells[x][y].pt4.phi = phi;
			potential0[x][y] = phi;
		}
		if (x < (x_mid - l / 2 - a / 2 + d))
		{
			for (int y = y_mid - a / 2+d; y < y_mid + a / 2 - d; y++) {
				cells[x][y].pt1.phi = phi;
				cells[x][y].pt2.phi = phi;
				cells[x][y].pt3.phi = phi;
				cells[x][y].pt4.phi = phi;
				potential0[x][y] = phi;
			}
		}
	}

	//правая скобка
	for (int x = x_mid + l / 2 ; x < x_mid + l / 2+a/2; x++) {
		for (int y = y_mid + a / 2 - d; y < y_mid + a / 2; y++) {
			cells[x][y].pt1.phi = -phi;
			cells[x][y].pt2.phi = -phi;
			cells[x][y].pt3.phi = -phi;
			cells[x][y].pt4.phi = -phi;
			potential0[x][y] = -phi;
		}
		for (int y = y_mid - a / 2; y < y_mid - a / 2 + d; y++) {
			cells[x][y].pt1.phi = -phi;
			cells[x][y].pt2.phi = -phi;
			cells[x][y].pt3.phi = -phi;
			cells[x][y].pt4.phi = -phi;
			potential0[x][y] = -phi;
		}
		if (x > (x_mid + l / 2 + a / 2 - d))
		{
			for (int y = y_mid - a / 2 + d; y < y_mid + a / 2 - d; y++) {
				cells[x][y].pt1.phi = -phi;
				cells[x][y].pt2.phi = -phi;
				cells[x][y].pt3.phi = -phi;
				cells[x][y].pt4.phi = -phi;
				potential0[x][y] = -phi;
			}
		}
	}
}

void CTask6Dlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (a <= 0 || d <= 0 || l <= 0) {
		MessageBox(L"Задай значения больше 0", L"");
		return;
	}
	if (a > 40 || l > 40 || d > a/2) {
		MessageBox(L"Задай значения правильно", L"");
		return;
	}
	S = (a * a - d * d)/2.;
	po = Q / S;
	C = 10;
	phi = Q / C;
	cells = vector<vector<Cell>>(Width, vector<Cell>(Height));
	potential0= std::vector<std::vector<double>>(Width, vector<double>(Height, 0));
	potentialTime = potential0;
	x_mid = Width / 2;
	y_mid = Height / 2;
	InitCells();
	//рассчитаем потенциал
	for (int i = 0; i < 5000; i++)
	{
		potentialTime = PotentialInNextTime(potentialTime, potential0);
	}

	//сохраним значения в клетки
	for (int x = 0; x < Width - 1; x++) {
		for (int y = 0; y < Height - 1; y++) {
			cells[x][y].pt1.phi = potentialTime[x][y];
			cells[x][y].pt2.phi = potentialTime[x + 1][y];
			cells[x][y].pt3.phi = potentialTime[x + 1][y + 1];
			cells[x][y].pt4.phi = potentialTime[x][y + 1];

		}
	}
	//посчитаем нормали для силовых линий
	for (int x = 0; x < Width; x++) {
		for (int y = 0; y < Height; y++) {
			cells[x][y].CalculateNormals();
		}
	}
	main.DrawWave(cells,potentialTime, Width, Height, phi, Iflag, Pflag);
	Isol.EnableWindow(1);
	powerLines.EnableWindow(1);
	UpdateData(FALSE);
}


void CTask6Dlg::OnClickedCheck1()//изолинии
{
	Iflag=Isol.GetCheck();
	main.DrawWave(cells,potentialTime, Width, Height, phi, Iflag, Pflag);
}


void CTask6Dlg::OnClickedCheck2()//силовые линии
{
	Pflag = powerLines.GetCheck();
	main.DrawWave(cells,potentialTime, Width, Height, phi, Iflag, Pflag);
}
