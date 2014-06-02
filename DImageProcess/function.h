
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Dib.h"
#include <math.h>
#include <direct.h>
#include <complex>

#define pi 3.1415926535    // ������
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


//���ڸ�������
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

double AngleToRation(int angle)//�Ƕȵ����ȵ�ת��
{
	return ((angle)*pi / 180.0);
}

BYTE FloatToByte(double f)//�������˫���ȱ���ת��ΪBYTE�ͱ���
{
	if (f <= 0)return (BYTE)0;
	else if (f >= 255) return (BYTE)255;
	else return (BYTE)(f + 0.5);
}

char FloatToChar(double f)//�������˫�����ͱ���ת��Ϊchar�ͱ���
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
* �������ƣ�Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth,
long lDstHeight,long lDstLineBytes,float	fXZoomRatio, float fYZoomRatio)
* ��������:
*   LPSTR lpSrcDib,ָ��ԴDIB��ָ��
*   LPSTR lpSrcStartBits,ָ��ԴDIB����ʼ���ص�ָ��
*   long lWidth,ԴDIBͼ����
*   long lHeight,ԴDIBͼ��߶�
*	 long lLineBytes,ԴDIBͼ���ֽڿ�ȣ�4�ı�����
*	 WORD palSize,ԴDIBͼ���ɫ���С
*	 long lDstWidth,Ŀ��ͼ����
*	 long lDstHeight,Ŀ��DIBͼ��߶�
*	 long lDstLineBytes,Ŀ��DIBͼ�����ֽ�����4�ı�����
*	 float	fhorRatio,ˮƽ���ű���
*	 float fverRatio,��ֱ���ű���
* ��������:HGLOBAL
* ��������:��������DIBͼ��
************************************************************************/

HGLOBAL  Zoom(LPSTR lpSrcDib, LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lLineBytes, WORD palSize, long lDstWidth, long lDstLineBytes, long lDstHeight,
	float fhorRatio, float fverRatio)
{
	LPSTR	lpDstDib;		//ָ����ʱͼ���ָ��	
	long i;                 //��ѭ������
	long j;                 //��ѭ������
	long i1;                 //��ѭ������
	long j1;                 //��ѭ������
	LPSTR	lpSrcDIBBits;	//ָ��Դ���ص�ָ��
	LPSTR	lpDstDIBBits;	//ָ����ʱͼ���Ӧ���ص�ָ��
	LPSTR	lpDstStartBits;	//ָ����ʱͼ���Ӧ���ص�ָ�� 		

	LPBITMAPINFOHEADER lpbmi;// ָ��BITMAPINFO�ṹ��ָ��

	// �����ڴ棬�Ա������ź��DIB
	HGLOBAL hDIB = (HGLOBAL) ::GlobalAlloc(GHND, lDstLineBytes* lDstHeight + *(LPDWORD)lpSrcDib + palSize);
	if (hDIB == NULL)// �ж��Ƿ�����Ч��DIB����
	{
		return FALSE;// ���ǣ��򷵻�
	}
	lpDstDib = (char *)::GlobalLock((HGLOBAL)hDIB);// �����ڴ�		
	memcpy(lpDstDib, lpSrcDib, *(LPDWORD)lpSrcDib + palSize);// ����DIB��Ϣͷ�͵�ɫ��		

	lpDstStartBits = lpDstDib + *(LPDWORD)lpDstDib// �ҵ���DIB������ʼλ��
		+ palSize;// ��������ʼλ��,������ͬ::FindDIBBits(lpSrcDib)�����ﳢ��ʹ�������ַ������Ա����ȫ�ֺ����ĵ���		
	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;// ��ȡָ��

	lpbmi->biWidth = lDstWidth;// ����DIB��ͼ��ĸ߶ȺͿ��
	lpbmi->biHeight = lDstHeight;

	for (i = 0; i < lDstHeight; i++)// �в���
	{
		for (j = 0; j < lDstWidth; j++)// �в���
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDstDIBBits = (char *)lpDstStartBits + lDstLineBytes * (lDstHeight - 1 - i) + j;
			i1 = (long)(i / fverRatio + 0.5);// �����������ԴDIB�е�����
			j1 = (long)(j / fhorRatio + 0.5);

			if ((j1 >= 0) && (j1< lWidth) && (i1 >= 0) && (i1< lHeight))
			{// �ж��Ƿ���Դͼ��				
				lpSrcDIBBits = (char *)lpSrcStartBits + lLineBytes * (lHeight - 1 - i1) + j1;// ָ��ԴDIB��i�У���j�����ص�ָ��								
				*lpDstDIBBits = *lpSrcDIBBits;// ��������
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) = 255;// Դͼ�в����ڵ����أ���Ϊ255
			}
		}
	}
	return hDIB;
}


/*************************************************************************
* �������ƣ�Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lXOffset,
long lYOffset,long lLineBytes,long lDstLineBytes)
* ��������:
*   LPSTR	lpSrcStartBits��ָ��ԴDIB��ʼ���ص�ָ��
* 	 long	lWidth��DIBͼ��Ŀ��
* 	 long	lHeight��DIBͼ��ĸ߶�
* 	 long	lXOffset,X����ƫ����
* 	 long	lYOffset,Y����ƫ����
* 	 long	lLineBytes��DIBͼ������ֽ�����Ϊ4�ı���
* 	 long	lDstLineBytes����ʱDIBͼ������ֽ�����Ϊ4�ı���
* ��������:BOOL
* ��������:�ú�������ƽ��DIBͼ��
************************************************************************/

BOOL Translation(LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lXOffset, long lYOffset, long lLineBytes, long lDstLineBytes)
{
	long i;//��ѭ������
	long j;//��ѭ������
	LPSTR lpSrcDIBBits;//ָ��Դ���ص�ָ��
	LPSTR lpDstDIBBits;//ָ����ʱͼ���Ӧ���ص�ָ��
	LPSTR lpDstStartBits;//ָ����ʱͼ���Ӧ���ص�ָ��
	HLOCAL hDstDIBBits;//��ʱͼ����

	hDstDIBBits = LocalAlloc(LHND, lWidth*lDstLineBytes);//������ʱ�ڴ�
	lpDstStartBits = (char*)LocalLock(hDstDIBBits);//�����ڴ�

	if (hDstDIBBits==NULL)//�ж��Ƿ��ڴ����
	{
		return FALSE;
	}

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			//ָ����DIB��i�С���j�����ص�ָ��
			lpDstDIBBits = (char*)lpDstStartBits + lLineBytes*(lHeight - 1 - i) + j;

			//�ж��Ƿ���Դͼ��Χ��
			if ((j - lYOffset >= 0) && (j - lYOffset < lWidth) &&      // ������ԴDIB�е�����j-lXOffset
				(i - lXOffset >= 0) && (i - lXOffset < lHeight))
			{
				//ָ��ԴDIB��i0�С���j0�����ص�ָ��
				lpSrcDIBBits = (char*)lpSrcStartBits + lLineBytes*(lHeight - 1 - (i - lXOffset)) + (j - lYOffset);
				*lpDstDIBBits = *lpSrcDIBBits;//��������
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) == 255;//Դͼ��û�е����أ���ֵΪ255
			}
		}
	}
	memcpy(lpSrcStartBits, lpDstStartBits, lLineBytes * lHeight);//����ͼ��
	LocalUnlock(hDstDIBBits);//�ͷ��ڴ�
	LocalFree(hDstDIBBits);
	return TRUE;
}