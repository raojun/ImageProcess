
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

/*************************************************************************
* �������ƣ�Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits,long lWidth, long lHeight,
long lLineBytes,	WORD palSize, long lDstWidth,
long lDstHeight,long lDstLineBytes,float	fSina, float fCosa)
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
*	 float	fSina,��ת�ǵ����ң�˵����Ϊ�˱���������ȡ�����ң�������Ϊ����������������
*	 float fCosa,��ת�ǵ�����
* ��������:HGLOBAL
* ��������:������תDIBͼ��
************************************************************************/

HGLOBAL Rotate(LPSTR lpSrcDib, LPSTR lpSrcStartBits, long lWidth, long lHeight,
	long lLineBytes, WORD palSize, long lDstWidth, long lDstHeight,
	long lDstLineBytes, float fSina, float fCosa)
{
	float varFloat1;//�����������1
	float varFloat2;//�����������2
	LPSTR lpDstDib;//ָ����ʱͼ���ָ��
	long i;//��ѭ������
	long i1;//��ѭ������
	long j;//��ѭ������
	long j1;//��ѭ������
	LPSTR lpSrcDIBBits;//ָ��Դ���ص�ָ��
	LPSTR lpDstDIBBits;//ָ����ʱͼ���Ӧ���ص�ָ��
	LPSTR lpDstStartBits;//ָ����ʱͼ���Ӧ���ص�ָ��
	LPBITMAPINFOHEADER lpbmi;//ָ��BITMAPINFOHEADER�ṹ��ָ��

	//�������õ�����������������Ա���Ϊ����ʹ��
	varFloat1 = (float)(-0.5*(lDstWidth - 1)*fCosa - 0.5*(lDstHeight - 1)*fSina
		+ 0.5*(lDstWidth - 1));
	varFloat2 = (float)(0.5*(lDstWidth - 1)*fSina - 0.5*(lDstHeight - 1)*fCosa
		+ 0.5*(lDstHeight - 1));

	//�����ڴ棬�Ա�����DIB
	HGLOBAL hDIB = (HGLOBAL)::GlobalAlloc(GHND, lDstLineBytes*lDstHeight + *(LPDWORD)lpSrcDib + palSize);

	if (hDIB == NULL)//�ж��Ƿ�����Ч��DIB����
	{
		return FALSE;
	}

	lpDstDib = (char*)::GlobalLock((HGLOBAL)hDIB);//�����ڴ�
	memcpy(lpDstDib, lpSrcDib, *(LPDWORD)lpSrcDib + palSize);
	//����DIB��Ϣͷ�͵�ɫ��
	lpbmi = (LPBITMAPINFOHEADER)lpDstDib;//��ȡָ��
	lpbmi->biHeight = lDstHeight;//����DIB��ͼ��ĸ߶ȺͿ��
	lpbmi->biWidth = lDstWidth;

	lpDstStartBits = lpDstDib + *(LPDWORD)lpDstDib + palSize;
	for (i = 0; i < lDstHeight; i++)//�в���
	{
		for (j = 0; j < lDstWidth; j++)//�в���
		{
			//ָ����DIB��i�С���j�����ص�ָ��
			lpDstDIBBits = (char*)lpDstStartBits + lDstLineBytes*(lDstHeight - 1 - i) + j;
			
			//�����������ԴDIB�е�����
			i1 = (long)(-((float)j)*fSina + ((float)i)*fCosa + varFloat2 + 0.5);
			j1 = (long)(((float)j)*fCosa + ((float)i)*fSina + varFloat1 + 0.5);

			//�ж��Ƿ���Դͼ��
			if ((j1>=0)&&(j1<lWidth)&&(i1>=0)&&(i1<lHeight))
			{
				//ָ��ԴDIB��i1�С���j1�����ص�ָ��
				lpSrcDIBBits = (char*)lpSrcStartBits + lLineBytes*(lHeight - 1 - i1) + j1;
				*lpDstDIBBits = *lpSrcDIBBits;//��������
			}
			else
			{
				*((unsigned char*)lpDstDIBBits) == 255;//Դͼ�в����ڵ����أ���ֵΪ255
			}
		}
	}
	return hDIB;
}


//�������ܣ���n��ʵ�Գƾ���Ϊ�Գ�������
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
		if (h + 1.0 == 1.0)//���һ��ȫΪ0
		{
			pHypoCross[i] = 0.0;
			if (i == 1)
			{
				pHypoCross[i] = pQMatrix[i*rank + i - 1];
			}
			pMainCross[i] = 0.0;
		}
		else//���������������ֵ
		{
			pHypoCross[i] = sqrt(h);//��ζԽ�Ԫ�ص�ֵ
			u = i*rank + i - 1;
			if (pQMatrix[u] > 0.0)//�жϵ�i�С���i-1��Ԫ���Ƿ����0
			{
				pHypoCross[i] = -pHypoCross[i];
			}
			h = h - pQMatrix[u] * pHypoCross[i];
			pQMatrix[u] = pQMatrix[u] - pHypoCross[i];
			f = 0.0;
			for (j = 0; j <= i - 1; j++)//householder�任
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
				for (j = 0; j <= i - 1; j++)//�����������ֵ
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
		for (i = 0; i <= rank - 2; i++)//��0ֵ
		{
			pHypoCross[i] = pHypoCross[i + 1];
		}
		pHypoCross[rank - 1] = 0.0;
		pMainCross[0] = 0.0;
		for (i = 0; i <= rank - 1; i++)
		{
			if ((pMainCross[i] != 0.0) && (i - 1 >= 0))//���Խ���Ԫ�صļ���
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
			u = i*rank + i;//�洢���Խ��ߵ�Ԫ��
			pMainCross[i] = pQMatrix[u];
			pQMatrix[u] = 1.0;
			if (i - 1 >= 0)//�����Խ������е�Ԫ�ظ�0ֵ
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

