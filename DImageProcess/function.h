
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Dib.h"
#include <math.h>
#include <direct.h>
#include <complex>

#define pi 3.1415926535    // 常数π
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define PI 3.14159265358979323846

using namespace std;

typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;


struct CplexNum
{
	double re;
	double im;
};


//用于复数运算
CplexNum Add(CplexNum c1, CplexNum c2)
{
	CplexNum c;
	c.re = c1.re + c2.re;
	c.im = c1.im + c2.im;
	return c;
}

CplexNum Sub(CplexNum c1, CplexNum c2)
{
	CplexNum c;
	c.re = c1.re - c2.re;
	c.im = c1.im - c2.im;
	return c;
}

CplexNum Mul(CplexNum c1, CplexNum c2)
{
	CplexNum c;
	c.re = c1.re*c2.re - c1.im*c2.im;
	c.im = c1.re*c2.im + c2.re*c1.im;
	return c;
}

double AngleToRation(int angle)//角度到弧度的转换
{
	return ((angle)*pi / 180.0);
}

BYTE FloatToByte(double f)//将输入的双精度变量转换为BYTE型变量
{
	if (f <= 0)return (BYTE)0;
	else if (f >= 255) return (BYTE)255;
	else return (BYTE)(f + 0.5);
}

char FloatToChar(double f)//将输入的双精度型变量转换为char型变量
{
	if (f >= 0)
	{
		if (f >= 127.0)
		{
			return (char)127;
		}
		else
		{
			return (char)(f + 0.5);
		}
	}
	else
	{
		if (f <= -128)
		{
			return (char)-128;
		}
		else
		{
			return -(char)(-f + 0.5);
		}
	}
}

/*************************************************************************
* 函数名称：Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth,
long lDstHeight,long lDstLineBytes,float	fXZoomRatio, float fYZoomRatio)
* 函数参数:
*   LPSTR lpSrcDib,指向源DIB的指针
*   LPSTR lpSrcStartBits,指向源DIB的起始像素的指针
*   long lWidth,源DIB图像宽度
*   long lHeight,源DIB图像高度
*	 long lLineBytes,源DIB图像字节宽度（4的倍数）
*	 WORD palSize,源DIB图像调色板大小
*	 long lDstWidth,目标图像宽度
*	 long lDstHeight,目标DIB图像高度
*	 long lDstLineBytes,目标DIB图像行字节数（4的倍数）
*	 float	fhorRatio,水平缩放比率
*	 float fverRatio,垂直缩放比率
* 函数类型:HGLOBAL
* 函数功能:用来缩放DIB图像
************************************************************************/

HGLOBAL  Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lLineBytes, WORD palSize, long lDstWidth, long lDstLineBytes, long lDstHeight,
	float fhorRatio, float fverRatio)
{
	LPSTR	lpDstDib;		//指向临时图像的指针	
	long i;                 //行循环变量
	long j;                 //列循环变量
	long i1;                 //行循环变量
	long j1;                 //列循环变量
	LPSTR	lpSrcDIBBits;	//指向源像素的指针
	LPSTR	lpDstDIBBits;	//指向临时图像对应像素的指针
	LPSTR	lpDstStartBits;	//指向临时图像对应像素的指针 		

	LPBITMAPINFOHEADER lpbmi;// 指向BITMAPINFO结构的指针

	// 分配内存，以保存缩放后的DIB
	HGLOBAL hDIB = (HGLOBAL) ::GlobalAlloc(GHND, lDstLineBytes* lDstHeight + *(LPDWORD)lpSrcDib + palSize);
	if (hDIB == NULL)// 判断是否是有效的DIB对象
	{
		return FALSE;// 不是，则返回
	}
	lpDstDib = (char *)::GlobalLock((HGLOBAL)hDIB);// 锁定内存		
	memcpy(lpDstDib, lpSrcDib, *(LPDWORD)lpSrcDib + palSize);// 复制DIB信息头和调色板		

	lpDstStartBits = lpDstDib + *(LPDWORD)lpDstDib// 找到新DIB像素起始位置
		+ palSize;// 求像素起始位置,作用如同::FindDIBBits(lpSrcDib)，这里尝试使用了这种方法，以避免对全局函数的调用		
	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;// 获取指针

	lpbmi->biWidth = lDstWidth;// 更新DIB中图像的高度和宽度
	lpbmi->biHeight = lDstHeight;

	for (i = 0; i < lDstHeight; i++)// 行操作
	{
		for (j = 0; j < lDstWidth; j++)// 列操作
		{
			// 指向新DIB第i行，第j个像素的指针
			lpDstDIBBits = (char *)lpDstStartBits + lDstLineBytes * (lDstHeight - 1 - i) + j;
			i1 = (long)(i / fverRatio + 0.5);// 计算该像素在源DIB中的坐标
			j1 = (long)(j / fhorRatio + 0.5);

			if ((j1 >= 0) && (j1< lWidth) && (i1 >= 0) && (i1< lHeight))
			{// 判断是否在源图内				
				lpSrcDIBBits = (char *)lpSrcStartBits + lLineBytes * (lHeight - 1 - i1) + j1;// 指向源DIB第i行，第j个像素的指针								
				*lpDstDIBBits = *lpSrcDIBBits;// 复制像素
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) = 255;// 源图中不存在的像素，赋为255
			}
		}
	}
	return hDIB;
}


/*************************************************************************
* 函数名称：Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lXOffset,
long lYOffset,long lLineBytes,long lDstLineBytes)
* 函数参数:
*   LPSTR	lpSrcStartBits，指向源DIB起始像素的指针
* 	 long	lWidth，DIB图像的宽度
* 	 long	lHeight，DIB图像的高度
* 	 long	lXOffset,X方向偏移量
* 	 long	lYOffset,Y方向偏移量
* 	 long	lLineBytes，DIB图像的行字节数，为4的倍数
* 	 long	lDstLineBytes，临时DIB图像的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来平移DIB图像
************************************************************************/

BOOL Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lXOffset, long lYOffset, long lLineBytes, long lDstLineBytes)
{
	long i;//行循环变量
	long j;//列循环变量
	LPSTR lpSrcDIBBits;//指向源像素的指针
	LPSTR lpDstDIBBits;//指向临时图像对应像素的指针
	LPSTR lpDstStartBits;//指向临时图像对应像素的指针
	HLOCAL hDstDIBBits;//临时图像句柄

	hDstDIBBits = LocalAlloc(LHND, lWidth*lDstLineBytes);//分配临时内存
	lpDstStartBits = (char*)LocalLock(hDstDIBBits);//锁定内存

	if (hDstDIBBits==NULL)//判断是否内存分配
	{
		return FALSE;
	}

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			//指向新DIB第i行、第j个像素的指针
			lpDstDIBBits = (char*)lpDstStartBits + lLineBytes*(lHeight - 1 - i) + j;

			//判断是否在源图范围内
			if ((j - lYOffset >= 0) && (j - lYOffset < lWidth) &&      // 像素在源DIB中的坐标j-lXOffset
				(i - lXOffset >= 0) && (i - lXOffset < lHeight))
			{
				//指向源DIB第i0行、第j0个像素的指针
				lpSrcDIBBits = (char*)lpSrcStartBits + lLineBytes*(lHeight - 1 - (i - lXOffset)) + (j - lYOffset);
				*lpDstDIBBits = *lpSrcDIBBits;//复制像素
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) == 255;//源图中没有的像素，赋值为255
			}
		}
	}
	memcpy(lpSrcStartBits, lpDstStartBits, lLineBytes * lHeight);//复制图像
	LocalUnlock(hDstDIBBits);//释放内存
	LocalFree(hDstDIBBits);
	return TRUE;
}

/*************************************************************************
* 函数名称：Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth,
long lDstHeight,long lDstLineBytes,float	fSina, float fCosa)
* 函数参数:
*   LPSTR lpSrcDib,指向源DIB的指针
*   LPSTR lpSrcStartBits,指向源DIB的起始像素的指针
*   long lWidth,源DIB图像宽度
*   long lHeight,源DIB图像高度
*	 long lLineBytes,源DIB图像字节宽度（4的倍数）
*	 WORD palSize,源DIB图像调色板大小
*	 long lDstWidth,目标图像宽度
*	 long lDstHeight,目标DIB图像高度
*	 long lDstLineBytes,目标DIB图像行字节数（4的倍数）
*	 float	fSina,旋转角的余弦，说明：为了避免两次求取正余弦，这里作为两个函数参数来用
*	 float fCosa,旋转角的正弦
* 函数类型:HGLOBAL
* 函数功能:用来旋转DIB图像
************************************************************************/

HGLOBAL Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lLineBytes, WORD palSize, long lDstWidth, long lDstHeight,
	long lDstLineBytes, float fSina, float fCosa)
{
	float varFloat1;//浮点参数变量1
	float varFloat2;//浮点参数变量2
	LPSTR lpDstDib;//指向临时图像的指针
	long i;//行循环变量
	long i1;//行循环变量
	long j;//列循环变量
	long j1;//列循环变量
	LPSTR lpSrcDIBBits;//指向源像素的指针
	LPSTR lpDstDIBBits;//指向临时图像对应像素的指针
	LPSTR lpDstStartBits;//指向临时图像对应像素的指针
	LPBITMAPINFOHEADER lpbmi;//指向BITMAPINFOHEADER结构的指针

	//将经常用到的两个常数求出，以便作为常数使用
	varFloat1 = (float)(-0.5*(lDstWidth - 1)*fCosa - 0.5*(lDstHeight - 1)*fSina
		+ 0.5*(lDstWidth - 1));
	varFloat2 = (float)(0.5*(lDstWidth - 1)*fSina - 0.5*(lDstHeight - 1)*fCosa
		+ 0.5*(lDstHeight - 1));

	//分配内存，以保存新DIB
	HGLOBAL hDIB = (HGLOBAL)::GlobalAlloc(GHND, lDstLineBytes*lDstHeight + *(LPDWORD)lpSrcDib + palSize);

	if (hDIB == NULL)//判断是否是有效的DIB对象
	{
		return FALSE;
	}

	lpDstDib = (char*)::GlobalLock((HGLOBAL)hDIB);//锁定内存
	memcpy(lpDstDib, lpSrcDib, *(LPDWORD)lpSrcDib + palSize);
	//复制DIB信息头和调色板
	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;//获取指针
	lpbmi->biHeight = lDstHeight;//更新DIB中图像的高度和宽度
	lpbmi->biWidth = lDstWidth;

	lpDstStartBits = lpDstDib + *(LPDWORD)lpDstDib + palSize;
	for (i = 0; i < lDstHeight; i++)//行操作
	{
		for (j = 0; j < lDstWidth; j++)//列操作
		{
			//指向新DIB第i行、第j个像素的指针
			lpDstDIBBits = (char*)lpDstStartBits + lDstLineBytes*(lDstHeight - 1 - i) + j;
			
			//计算该像素在源DIB中的坐标
			i1 = (long)(-((float)j)*fSina + ((float)i)*fCosa + varFloat2 + 0.5);
			j1 = (long)(((float)j)*fCosa + ((float)i)*fSina + varFloat1 + 0.5);

			//判断是否在源图内
			if ((j1>=0)&&(j1<lWidth)&&(i1>=0)&&(i1<lHeight))
			{
				//指向源DIB第i1行、第j1个像素的指针
				lpSrcDIBBits = (char*)lpSrcStartBits + lLineBytes*(lHeight - 1 - i1) + j1;
				*lpDstDIBBits = *lpSrcDIBBits;//复制像素
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) == 255;//源图中不存在的像素，赋值为255
			}
		}
	}
	return hDIB;
}


//函数功能：将n阶实对称矩阵化为对称三角阵
BOOL ThreeCrossMat(double *pMatrix, int rank, double *pQMatrix, double *pMainCross, double *pHypoCross)
{
	int i, j, k, u;
	double h, f, g, h2;

	for (i = 0; i <= rank - 1; i++)
	{
		for (j = 0; j <= rank - 1; j++)
		{
			u = i*rank + j;
			pQMatrix[u] = pMatrix[u];
		}
	}

	for (i = rank - 1; i >= 1; i--)
	{
		h = 0.0;
		if (i > 1)
		{
			for (k = 0; k <= i - 1; k++)
			{
				u = i*rank + k;
				h = h + pQMatrix[u] * pQMatrix[u];
			}
		}
		if (h + 1.0 == 1.0)//如果一行全为0
		{
			pHypoCross[i] = 0.0;
			if (i == 1)
			{
				pHypoCross[i] = pQMatrix[i*rank + i - 1];
			}
			pMainCross[i] = 0.0;
		}
		else//否则求正交矩阵的值
		{
			pHypoCross[i] = sqrt(h);//求次对角元素的值
			u = i*rank + i - 1;
			if (pQMatrix[u] > 0.0)//判断第i行、第i-1列元素是否大于0
			{
				pHypoCross[i] = -pHypoCross[i];
			}
			h = h - pQMatrix[u] * pHypoCross[i];
			pQMatrix[u] = pQMatrix[u] - pHypoCross[i];
			f = 0.0;
			for (j = 0; j <= i - 1; j++)//householder变换
			{
				pQMatrix[j*rank + i] = pQMatrix[i*rank + j] / h;
				g = 0.0;
				for (k = 0; k <= j; k++)
				{
					g = g + pQMatrix[j*rank + k] * pQMatrix[i*rank + k];
				}
				if (j + 1 <= i - 1)
				{
					for (k = j + 1; k <= i - 1; k++)
					{
						g = g + pQMatrix[k*rank + j] * pQMatrix[i*rank + k];
					}
					pHypoCross[j] = g / h;
					f = f + g*pQMatrix[j*rank + i];
				}
				h2 = f / (h + h);
				for (j = 0; j <= i - 1; j++)//求正交矩阵的值
				{
					f = pQMatrix[i*rank + j];
					g = pHypoCross[j] - h2*f;
					pHypoCross[j] = g;
					for (k = 0; k <= j; k++)
					{
						u = j*rank + k;
						pQMatrix[u] = pQMatrix[u] - f*pQMatrix[i*rank + k];
					}
				}
				pMainCross[i] = h;
			}
		}
		for (i = 0; i <= rank - 2; i++)//赋0值
		{
			pHypoCross[i] = pHypoCross[i + 1];
		}
		pHypoCross[rank - 1] = 0.0;
		pMainCross[0] = 0.0;
		for (i = 0; i <= rank - 1; i++)
		{
			if ((pMainCross[i] != 0.0) && (i - 1 >= 0))//主对角线元素的计算
			{
				for (j = 0; j <= i - 1; j++)
				{
					g = 0.0;
					for (k = 0; k <= i - 1; k++)
					{
						g = g + pQMatrix[i*rank + k] * pQMatrix[k*rank + j];
					}
					for (k = 0; k <= i - 1; k++)
					{
						u = k*rank + j;
						pQMatrix[u] = pQMatrix[u] - g*pQMatrix[k*rank + i];
					}
				}
			}
			u = i*rank + i;//存储主对角线的元素
			pMainCross[i] = pQMatrix[u];
			pQMatrix[u] = 1.0;
			if (i - 1 >= 0)//将三对角外所有的元素赋0值
			{
				for (j = 0; j <= i - 1; j++)
				{
					pQMatrix[i*rank + j] = 0.0;
					pQMatrix[j*rank + i] = 0.0;
				}
			}
		}
	}
	return(TRUE);
}

