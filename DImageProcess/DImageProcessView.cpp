
// DImageProcessView.cpp : CDImageProcessView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DImageProcess.h"
#endif

#include "DImageProcessDoc.h"
#include "DImageProcessView.h"

#include "function.h"
#include "DlgZoom.h"
#include "DlgTran.h"
#include "DlgRot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDImageProcessView

IMPLEMENT_DYNCREATE(CDImageProcessView, CView)

BEGIN_MESSAGE_MAP(CDImageProcessView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_geo_zoom, &CDImageProcessView::OnZoom)
	ON_COMMAND(ID_geo_translation, &CDImageProcessView::OnTranslation)
	ON_COMMAND(ID_geo_rotation, &CDImageProcessView::OnRotation)
END_MESSAGE_MAP()

// CDImageProcessView 构造/析构

CDImageProcessView::CDImageProcessView()
{
	// TODO:  在此处添加构造代码
	
	m_pDbImage = NULL;
	m_nDWTCurDepth = 0;
}

CDImageProcessView::~CDImageProcessView()
{
}

BOOL CDImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDImageProcessView 绘制

void CDImageProcessView::OnDraw(CDC* pDC)
{
	CDImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO:  在此处为本机数据添加绘制代码

	HGLOBAL hDIB = pDoc->GetHObject();
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

		// 获取DIB宽度
		int cxDIB = (int)pDoc->m_dib.GetWidth(lpDibSection);

		// 获取DIB高度
		int cyDIB = (int)pDoc->m_dib.GetHeight(lpDibSection);

		::GlobalUnlock((HGLOBAL)hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;

		CRect rcDest = rcDIB;
		// 输出DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}
}


// CDImageProcessView 打印

BOOL CDImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CDImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CDImageProcessView 诊断

#ifdef _DEBUG
void CDImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDImageProcessDoc* CDImageProcessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDImageProcessDoc)));
	return (CDImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CDImageProcessView 消息处理程序


void CDImageProcessView::OnZoom()
{
	// TODO:  在此添加命令处理程序代码
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long	lDstWidth;      //临时图像的宽度和高度
	long	lDstHeight;
	long lDstLineBytes;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD palSize = pDoc->m_dib.GetPalSize(lpSrcDib);

	CDlgZoom ZoomPara;// 创建对话框,设定平移量
	if (ZoomPara.DoModal() != IDOK)
	{
		return;
	}
	float fX = ZoomPara.m_horZoom;// 获取设定的平移量,缩放比率
	float fY = ZoomPara.m_verZoom;

	lDstWidth = (long)(lSrcWidth*fX + 0.5);// 计算缩放后的图像实际宽度,加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);	//转换后图像应有的行字节数，为4的倍数
	lDstHeight = (long)(lSrcHeight * fY + 0.5);// 计算缩放后的图像高度

	HGLOBAL hDstDIB = NULL;// 创建新DIB	
	hDstDIB = (HGLOBAL)Zoom(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight,
		lSrcLineBytes, palSize, lDstWidth, lDstLineBytes, lDstHeight, fX, fY);// 调用Zoom()函数转置DIB		

	if (hDstDIB != NULL)// 判断旋转是否成功
	{
		pDoc->UpdateObject(hDstDIB);// 替换DIB，同时释放旧DIB对象		
		pDoc->SetDib();// 更新DIB大小和调色板				
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());					// 解除锁定
	}
	else
	{
		AfxMessageBox(_T("分配内存失败！"));
	}
	// 警告	
}


void CDImageProcessView::OnTranslation()
{
	// TODO:  在此添加命令处理程序代码
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long lDstLineBytes;		//新图像每行的字节数
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}											//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// 计算新图像每行的字节数	


	CDlgTran TranPara;// 创建对话框	
	if (TranPara.DoModal() != IDOK)// 显示对话框，提示用户设定量
		return;
	int temver = TranPara.m_verOff;
	int temhor = TranPara.m_horOff;

	if (Translation(lpSrcStartBits, lSrcWidth, lSrcHeight,// 调用Translation()函数平移DIB
		temver, temhor, lSrcLineBytes, lDstLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());					// 解除锁定
	}
	else
	{
		AfxMessageBox(_T("分配内存失败！"));
	}//警告	
}


void CDImageProcessView::OnRotation()
{
	// TODO:  在此添加命令处理程序代码
	CDImageProcessDoc* pDoc = GetDocument();
	
	long lSrcLineBytes;//图像每行的字节数
	long lSrcWidth;//图像的宽度
	long lSrcHeight;//图像的高度
	LPSTR lpSrcDib;//指向源图像的指针
	LPSTR lpSrcStartBits;//指向源像素的指针
	long lDstWidth;//临时图像的宽度和高度
	long lDstHeight;

	lpSrcDib = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHObject());//锁定内存

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)//判断是否是8-bpp位图，不是则返回
	{
		AfxMessageBox(_T("对不起，不是256色位图！"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());//解除锁定
		return;
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);//找到DIB图像像素起始位置
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);//获取图像的宽度
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);//获取图像的高度
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);//计算图像每行的字节数
	
	long lDstLineBytes; //新图像每行的字节数

	CDlgRot RotPara;//创建对话框
	if (RotPara.DoModal() != IDOK)//显示对话框，设定旋转角度
	{
		return;
	}

	DWORD palSize = pDoc->m_dib.GetPalSize(lpSrcDib);
	//将旋转角度从度转换为弧度
	float fRotateAngle = (float)AngleToRation(RotPara.m_rotAngle);
	float fSina = (float)sin((double)fRotateAngle);//计算旋转角度的正余弦
	float fCosa = (float)cos((double)fRotateAngle);

	//旋转前4个角的坐标，（以图像中心为坐标系原点）
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	//旋转后4个角的坐标
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	//计算源图的4个角的坐标
	fSrcX1 = (float)(-(lSrcWidth - 1) / 2);
	fSrcY1 = (float)((lSrcHeight - 1) / 2);
	fSrcX2 = (float)((lSrcWidth - 1) / 2);
	fSrcY2 = (float)((lSrcHeight - 1) / 2);
	fSrcX3 = (float)(-(lSrcWidth - 1) / 2);
	fSrcY3 = (float)(-(lSrcHeight - 1) / 2);
	fSrcX4 = (float)((lSrcWidth - 1) / 2);
	fSrcY4 = (float)(-(lSrcHeight - 1) / 2);

	//计算新图的4个角的坐标
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	//计算旋转后的图像实际宽度和高度
	lDstWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	lDstHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	//计算新图像每行的字节数
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);

	HGLOBAL hDstDIB = NULL;//创建新DIB

	//调用Rotate()函数旋转DIB
	hDstDIB = (HGLOBAL)Rotate(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, lSrcLineBytes,
		palSize, lDstWidth, lDstHeight, lDstLineBytes, fSina, fCosa);

	//判断是否旋转成功
	if (hDstDIB != NULL)
	{
		pDoc->UpdateObject(hDstDIB);//替换DIB，同时释放旧DIB对象
		pDoc->SetDib();//更新DIB大小和调色板
		pDoc->SetModifiedFlag(TRUE);//设置修改标记
		pDoc->UpdateAllViews(NULL);//更新视图
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());//解除锁定
	}
	else
	{
		AfxMessageBox(_T("分配内存失败！"));
	}
}
