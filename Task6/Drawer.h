#pragma once

#include <afxwin.h>
#include <vector>
#include <time.h>
//#include <amp_graphics.h>
//#include "Analys.h"

using namespace std;

struct PotPt
{
	//���������
	double phi;
	//int i;
	//int j;
	//����������
	double x, y;

	PotPt& operator=(PotPt& other)
	{
		//i = other.i;
		//j = other.j;
		x = other.x;
		y = other.y;

		phi = other.phi;
		return *this;
	}

};


//			   1  -----------  2
//				 |   	   / |
//				 Y   	 /   |
//				 |     / pt  |
//				 |	 /       |
//				 | /	     |
//        	   4  -------X---  3

//������
struct Cell
{

	bool upperTri; //������� �����������
	bool lowerTri; //������ �����������
	PotPt  pt1, pt2, pt3, pt4;
	double nx_upper, ny_upper;
	double nx_lower, ny_lower;

	//��������, ��� ����� � ������
	void IsPointIn(double x, double y)
	{
		/// ��� ������� ������� ��� ������������ ������������ ��� � �������� ��������� �����. ������ ��� ������������ ��� ������ y=kx
		//
		//		Y2 - Y1
		//	K = --------	(X2; Y2) - ��� ������� ������ ����� ��������, (X1; Y1) - ���������� ��������������� �����
		//		X2 - X1
		//	
		// ��� ��� � ������������ ����, � � ������, ���� k>= -1, �� ����� ����� � ������ ������������
		double k;
		if (x >= pt1.x && x <= pt3.x && y >= pt1.y && y <= pt3.y)
		{
			k = (y - pt2.y) / (x - pt2.x);
			if (k >= -1) { upperTri = false; lowerTri = true; }
			else { upperTri = true; lowerTri = false; }

		}
		
	}
	//������� ��������
	void CalculateNormals()
	{

		double length;
		nx_upper = (pt3.y - pt1.y) * (pt2.phi - pt1.phi) - (pt2.y - pt1.y) * (pt3.phi - pt1.phi);
		ny_upper = -(pt3.x - pt1.x) * (pt2.phi - pt1.phi) + (pt2.x - pt1.x) * (pt3.phi - pt1.phi);

		nx_lower = (pt4.y - pt1.y) * (pt3.phi - pt1.phi) - (pt3.y - pt1.y) * (pt4.phi - pt1.phi);
		ny_lower = -(pt4.x - pt1.x) * (pt3.phi - pt1.phi) + (pt3.x - pt1.x) * (pt4.phi - pt1.phi);


		length = sqrt(ny_upper * ny_upper + nx_upper * nx_upper);
		if (length != 0) {
			nx_upper /= length;
			ny_upper /= length;
		}
		length = sqrt(ny_lower * ny_lower + nx_lower * nx_lower);
		if (length != 0)
		{
			nx_lower /= length;
			ny_lower /= length;
		}
	}

	// ��� �������� � ��������� ������ �������
	void Propagate(double& x, double& y, double& step)
	{
		if (upperTri)
		{
			x += step * nx_upper;
			y += step * ny_upper;
		}
		else if (lowerTri)
		{
			x += step * nx_lower;
			y += step * ny_lower;
		}
	}
};


class Drawer
{
	// ������������� ������� ���������.
	CRect frame;
	// ��������� �� ������ ����, ������� ���������.
	CWnd * wnd;
	// �������� ���������, ����������� � ������� ���������.
	CDC * dc;
	// �������� ���������, ����������� � ������.
	CDC memDC;
	// ������ ��� ��������� ��������� memDC.
	CBitmap bmp;
	// ���� ��� ������������ ��������� ������������� ������.
	bool init;
public:
	// ������������������� ������ ������ �� ������ HWND.
	void Create(HWND hWnd);

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);

	double convert_range_d(double data, double outmax, double outmin, double inmax, double inmin);

	// ���������� ������.
	void DrawOneSig(vector<double>& data, CString Ox, CString Oy, vector<double>& y);
	void DrawOneSigFromZero(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX, double posX/*, CDC* mem*/);
	void DrawWave(std::vector<std::vector<Cell>>& cells, const vector<vector <double>>& wave, int Width, int Height,  double phi_max, bool Iflag, bool Pflag);
	double MaxF(vector<double> y);

	double MinF(vector<double> y);



	//void DrawSr(vector<double>& data, signal s, CString Ox, CString Oy, double data_x_min, double data_x_max, double step_x);
};

