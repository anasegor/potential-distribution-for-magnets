#include "pch.h"
#include "Drawer.h"
struct Point {
	Point(){}
	Point(double xx, double yy)
	{
		this->x = xx;
		this->y = yy;
	}
	double x, y;
};
double sign(double x)
{
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}
void Drawer::Create(HWND hWnd)
{
	// ѕолучаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// ѕолучаем пр€моугольник окна.
	wnd->GetClientRect(frame);
	// ѕолучаем контекст дл€ рисовани€ в этом окне.
	dc = wnd->GetDC();

	// —оздаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// —оздаем растр дл€ контекста рисовани€.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// ¬ыбираем растр дл€ использовани€ буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;
}

vector<double> Drawer::convert_range(vector<double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}

double Drawer::convert_range_d(double data, double outmax, double outmin, double inmax, double inmin)
{
	double output = data;
	double k = (outmax - outmin) / (inmax - inmin);

	return (output - inmin) * k + outmin;
}

void Drawer::DrawOneSig(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX/*, CDC* mem*/)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(37, 52, 148));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				//  урсив
		FALSE,				// ѕодчеркнутый
		FALSE,				// ѕеречеркнутый
		DEFAULT_CHARSET,	// Ќабор символов
		OUT_OUTLINE_PRECIS,	// “очность соответстви€.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	//  ачество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
		);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Ѕелый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// –исуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
		data_y_max = MaxF(data);
		data_y_min = MinF(data);
	

	double data_x_max = MaxF(dataX);
	double data_x_min = -data_x_max;
	if (data.empty()) return;

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush);

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
	vector<double> x = convert_range(dataX, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(x[0], y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(x[i], y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		
		str.Format(L"%.1g", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1g", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);

	
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawOneSigFromZero(vector<double>& data, CString Ox, CString Oy, vector<double>& dataX, double posX/*, CDC* mem*/)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen points_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen line_pen(PS_SOLID, 2, RGB(32, 125, 57));
	CBrush points_brush;
	points_brush.CreateSolidBrush(RGB(0, 0, 255));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				//  урсив
		FALSE,				// ѕодчеркнутый
		FALSE,				// ѕеречеркнутый
		DEFAULT_CHARSET,	// Ќабор символов
		OUT_OUTLINE_PRECIS,	// “очность соответстви€.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	//  ачество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 40;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	// Ѕелый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// –исуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	double data_y_max, data_y_min;
	data_y_max = MaxF(data);
	data_y_min = MinF(data);


	double data_x_max = MaxF(dataX);
	double data_x_min = 0;
	if (data.empty()) return;

	memDC.SelectObject(&line_pen);
	//memDC.SelectObject(&points_brush);

	posX= convert_range_d(posX, actual_right, actual_left, data_x_max, data_x_min);
	memDC.MoveTo(posX, convert_range_d(data_y_min, actual_top, actual_bottom, data_y_max, data_y_min));
	memDC.LineTo(posX, convert_range_d(data_y_max, actual_top, actual_bottom, data_y_max, data_y_min));

	memDC.SelectObject(&points_pen);
	memDC.SelectObject(&points_brush); 

	vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
	vector<double> x = convert_range(dataX, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(x[0], y[0]);

	for (unsigned int i = 1; i < y.size(); i++)
	{
		memDC.LineTo(x[i], y[i]);
	}

	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	CString str;
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{

		str.Format(L"%.1g", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1g", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}
	//str.Format(L"x");
	memDC.TextOutW(actual_left - 10, actual_top - 30, Oy);

	//str.Format(L"t");
	memDC.TextOutW(actual_right + 20, actual_bottom + 20, Ox);


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}


// сортировка используетс€ в алгоритме рисовки изолиний
void Sort(double arr[3], Point points[3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				swap(points[j], points[j + 1]);
			}
		}
	}
}
long double FindMaxDouble(const vector<vector<long double>>&vec)
{
	long double max = vec[0][0];
	for (int i = 0; i < vec.size(); i++)
	{
		long double temp = *max_element(vec[i].begin(), vec[i].end());
		if (temp > max) max = temp;
	}
	return max;
}
long double FindMinDouble(const vector<vector<double>> &vec)
{
	double min = vec[0][0];
	for (int i = 0; i < vec.size(); i++)
	{
		double temp = *min_element(vec[i].begin(), vec[i].end());
		if (temp < min) min = temp;
	}
	return min;
}
void Drawer::DrawWave(std::vector<std::vector<Cell>> &cells,const vector<vector<double>>& wave, int Width, int Height, double phi_max, bool Iflag,bool Pflag)
{
	if (!init) return;

	int padding = 0;
	int left_keys_padding = 15;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;

	// Ѕелый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	double y_max = Height;
	double y_min = 0;
	double x_max = Width;
	double x_min = 0;

	double stepX = 1;
	double stepY = 1;
	for (int i = 0; i < wave.size(); i++)
	{
		double x1 = convert_range_d(i * stepX + x_min, actual_right, actual_left, x_max, x_min);
		double x2 = convert_range_d(i * stepX + x_min + stepX, actual_right, actual_left, x_max, x_min);
		for (int j = 0; j < wave[0].size(); j++)
		{
			if (wave[i][j] >= 0)
			{
				int col = wave[i][j] * 255 / phi_max;//нормировка
				CBrush br(RGB(col, 0, 0));
				memDC.SelectObject(&br);
				CPen br_pen(PS_SOLID, 1, RGB(col, 0, 0));
				memDC.SelectObject(&br_pen);
				double y1 = convert_range_d(j * stepY + y_min, actual_top, actual_bottom, y_max, y_min);
				double y2 = convert_range_d(j * stepY + y_min + stepY, actual_top, actual_bottom, y_max, y_min);
				memDC.Rectangle(x1, y1, x2, y2);
			}
			else
			{
				int col = -wave[i][j] * 255 / phi_max;//нормировка
				CBrush br(RGB(0, 0, col));
				memDC.SelectObject(&br);
				CPen br_pen(PS_SOLID, 1, RGB(0, 0, col));
				memDC.SelectObject(&br_pen);
				double y1 = convert_range_d(j * stepY + y_min, actual_top, actual_bottom, y_max, y_min);
				double y2 = convert_range_d(j * stepY + y_min + stepY, actual_top, actual_bottom, y_max, y_min);
				memDC.Rectangle(x1, y1, x2, y2);
			}
		}
	}
	if (Iflag)
	{
		int numIsolines = 14;
		CPen is_pen(PS_SOLID, 1, RGB(255, 255, 255));
		memDC.SelectObject(&is_pen);
		double PhiMax = phi_max;
		double PhiMin = FindMinDouble(wave);
		for (int m = 1; m < numIsolines; m++)
		{
			double isoline = PhiMin + m * (PhiMax - PhiMin) / numIsolines;
			double nodes[3];
			Point points[3];
			for (int i = 0; i < wave.size() - 1; i++)
			{
				for (int j = 0; j < wave[0].size() - 1; j++)
				{
					for (int k = 0; k < 2; k++) {
						double values[2], keys[2];
						if (k == 0) {
							nodes[0] = wave[i][j];
							nodes[1] = wave[i + 1][j];
							nodes[2] = wave[i][j + 1];

							points[0].x = i * stepX + x_min;
							points[0].y = j * stepY + y_min;
							points[1].x = i * stepX + x_min + stepX;
							points[1].y = j * stepY + y_min;
							points[2].x = i * stepX + x_min;
							points[2].y = j * stepY + y_min + stepY;
						}
						else {
							nodes[0] = wave[i + 1][j];
							nodes[1] = wave[i + 1][j + 1];
							nodes[2] = wave[i][j + 1];

							points[0].x = i * stepX + x_min + stepX;
							points[0].y = j * stepY + y_min;
							points[1].x = i * stepX + x_min + stepX;
							points[1].y = j * stepY + y_min + stepY;
							points[2].x = i * stepX + x_min;
							points[2].y = j * stepY + y_min + stepY;
						}
						Sort(nodes, points);

						if (isoline < nodes[0] || isoline > nodes[2])
							continue;

						double x1, y1, x2, y2;

						if (isoline >= nodes[0] && isoline < nodes[1]) {
							x1 = points[1].x * 1. - (nodes[1] - isoline) *
								(points[1].x * 1. - points[0].x * 1.) / (nodes[1] - nodes[0]);
							y1 = points[1].y * 1. - (nodes[1] - isoline) *
								(points[1].y * 1. - points[0].y * 1.) / (nodes[1] - nodes[0]);
						}
						else {
							x1 = points[2].x * 1. - (nodes[2] - isoline) *
								(points[2].x * 1. - points[1].x * 1.) / (nodes[2] - nodes[1]);
							y1 = points[2].y * 1. - (nodes[2] - isoline) *
								(points[2].y * 1. - points[1].y * 1.) / (nodes[2] - nodes[1]);
						}

						x2 = points[2].x * 1. - (nodes[2] - isoline) * (points[2].x * 1. - points[0].x * 1.) / (nodes[2] - nodes[0]);
						y2 = points[2].y * 1. - (nodes[2] - isoline) * (points[2].y * 1. - points[0].y * 1.) / (nodes[2] - nodes[0]);

						x1 = convert_range_d(x1, actual_right, actual_left, x_max, x_min);
						x2 = convert_range_d(x2, actual_right, actual_left, x_max, x_min);
						y1 = convert_range_d(y1, actual_top, actual_bottom, y_max, y_min);
						y2 = convert_range_d(y2, actual_top, actual_bottom, y_max, y_min);
						memDC.MoveTo(x1, y1);
						memDC.LineTo(x2, y2);
					}
				}
			}
		}
	}
	if (Pflag)
	{
		CPen is_pen(PS_SOLID, 1, RGB(0, 255, 0));
		memDC.SelectObject(&is_pen);
		//рисуем от границ к скобам
		int step_for = 2;//шаг dx=dy
		for (int x = 1; x < Width - 1; x += step_for) 
		{
			for (int y = 1; y < Height - 1; y += step_for) 
			{
				if (Width - 2 - x < step_for) 
					x = Width - 2;
				if (Height - 2 - y < step_for) 
					y = Height - 2;
				if (x == 1 || y == 1 || x == Width - 2 || y == Height - 2)//от границ с каждой из строн
				{
					//if (wave[x][y] == 0) break;
					double xx = x, yy = y, step = 1* sign(wave[x][y]);
					Point p1(xx, yy), p2 = p1, p0 = p1;
					//перемещаем зар€д
					cells[int(xx)][int(yy)].IsPointIn(xx, yy);//смотрит какой это треугольник в квадрате
					cells[int(xx)][int(yy)].Propagate(xx, yy, step);
					//сохран€ем в точку, чтобы нарисовать
					p2.x = xx; p2.y = yy;
					memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
					memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
					p1 = p2;
					//пока не дойдем до границ
					while (xx > 0 && xx < Width && yy > 0 && yy < Height - 1) {
						//перемещаем зар€д
						cells[int(xx)][int(yy)].IsPointIn(xx, yy);
						cells[int(xx)][int(yy)].Propagate(xx, yy, step);
						//сохран€ем в точку, чтобы нарисовать
						p2.x = xx; p2.y = yy;
						memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
						memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
						//запоминаем новое положение
						//если оказались в той же точке или шагнули в предыдущую
						if ((abs(p2.x - p1.x) < 0.01 && abs(p2.y - p1.y) < 0.01) || (abs(p2.x - p0.x) < 0.01 && abs(p2.y - p0.y) < 0.01)) break;
						p0 = p1;
						p1 = p2;
					}
				}
			}
		}
		//рисовка в центре
		step_for = 4;
		int x = Width / 2;
		for (int y = 1; y < Height - 1; y += step_for) 
		{
			if (wave[x][y] == 0) break;

			//координаты пробного зар€да
			double xx = x, yy = y, step = -0.4 * sign(wave[x][y]);
			Point p1(xx, yy), p2 = p1, p0 = p1;
			//перемещаем зар€д
			cells[int(xx)][int(yy)].IsPointIn(xx, yy);
			cells[int(xx)][int(yy)].Propagate(xx, yy, step);
			//сохран€ем в точку, чтобы нарисовать
			p2.x = xx; p2.y = yy;
			memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
			memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
			p1 = p2;
			//пока не дойдем до границ
			while (xx > 0 && xx < Width && yy > 0 && yy < Height - 1) {
				//перемещаем зар€д
				cells[int(xx)][int(yy)].IsPointIn(xx, yy);
				cells[int(xx)][int(yy)].Propagate(xx, yy, step);
				//сохран€ем в точку, чтобы нарисовать
				p2.x = xx; p2.y = yy;
				memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
				memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
				//запоминаем новое положение
				//если оказались в той же точке или шагнули в предыдущую
				if ((abs(p2.x - p1.x) < 0.01 && abs(p2.y - p1.y) < 0.01) || (abs(p2.x - p0.x) < 0.01 && abs(p2.y - p0.y) < 0.01)) break;
				p0 = p1;
				p1 = p2;
			}
		}

		for (int y = 1; y < Height - 1; y += step_for) 
		{
			if (wave[x][y] == 0) break;

			//координаты пробного зар€да
			double xx = x, yy = y, step = 0.4 * sign(wave[x][y]);
			Point p1(xx, yy), p2 = p1, p0 = p1;
			//перемещаем зар€д
			cells[int(xx)][int(yy)].IsPointIn(xx, yy);
			cells[int(xx)][int(yy)].Propagate(xx, yy, step);
			//сохран€ем в точку, чтобы нарисовать
			p2.x = xx; p2.y = yy;
			memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
			memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
			p1 = p2;
			//пока не дойдем до границ
			while (xx > 0 && xx < Width && yy > 0 && yy < Height - 1) {
				//перемещаем зар€д
				cells[int(xx)][int(yy)].IsPointIn(xx, yy);
				cells[int(xx)][int(yy)].Propagate(xx, yy, step);
				//сохран€ем в точку, чтобы нарисовать
				p2.x = xx; p2.y = yy;
				memDC.MoveTo(convert_range_d(p1.x, actual_right, actual_left, x_max, x_min), convert_range_d(p1.y, actual_top, actual_bottom, y_max, y_min));
				memDC.LineTo(convert_range_d(p2.x, actual_right, actual_left, x_max, x_min), convert_range_d(p2.y, actual_top, actual_bottom, y_max, y_min));
				//запоминаем новое положение
				//если оказались в той же точке или шагнули в предыдущую
				if ((abs(p2.x - p1.x) < 0.01 && abs(p2.y - p1.y) < 0.01) || (abs(p2.x - p0.x) < 0.01 && abs(p2.y - p0.y) < 0.01)) break;
				p0 = p1;
				p1 = p2;
			}
		}
		
	}
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	
}

double Drawer::MaxF(vector<double> y)
{
	if (y.empty()) return 0;
	double max = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] > max)
		{
			pr = max;
			max = y[i];
			y[i] = pr;
		}
	}

	return max;
}

double Drawer::MinF(vector<double> y)
{
	double min = y[0], pr;
	for (int i = 1; i < y.size(); i++)
	{
		if (y[i] < min)
		{
			pr = min;
			min = y[i];
			y[i] = pr;
		}
	}

	return min;
}


