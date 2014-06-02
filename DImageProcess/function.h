
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