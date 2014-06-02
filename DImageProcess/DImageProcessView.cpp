
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
