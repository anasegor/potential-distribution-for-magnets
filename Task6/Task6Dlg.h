
// Task6Dlg.h: файл заголовка
//
#include <iostream>
#include <vector>
#include "Drawer.h"
#include "func.h"
#pragma once


// Диалоговое окно CTask6Dlg
class CTask6Dlg : public CDialogEx
{
// Создание
public:
	CTask6Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK6_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton Isol;
	CButton powerLines;
	double Q;
	int a;
	int l;
	int d;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheck2();
	void InitCells();

	int x_mid,
		y_mid;
	double S;
	double po;
	int Width = 100;
	int Height = 100;
	double dx = 1;
	double dy = 1;
	std::vector<std::vector<Cell>> cells; //Массив ячеек
	std::vector<std::vector<double>> potential0; //значения потенциала стартовые
	std::vector<std::vector<double>> potentialTime; //значения потенциала меняющиеся во времени
	double phi;
	double C;
	Drawer main;
	bool Iflag;
	bool Pflag;
};
