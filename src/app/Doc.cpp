/*
 *	Dualword-PMC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-PMC is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-PMC. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "global.h"

Doc::Doc(QObject *p) : QObject(p), doc(nullptr), zoom(150), res(100), pageNum(0), pageCount(0), imageCount(0) {

}

Doc::~Doc() {
	FPDF_CloseDocument(doc);
}

void Doc::open(){
    doc = FPDF_LoadMemDocument(data.constData(), data.size(), nullptr);
    pageCount = FPDF_GetPageCount(doc);
}

void Doc::loadPage(int i){
	int p = --i;
	if(p < 0 || p > pageCount) return;
	pageNum = p;
	FPDF_PAGE page = FPDF_LoadPage(doc, i);
	QImage image(FPDF_GetPageWidth(page)*(zoom/res), FPDF_GetPageHeight(page)*(zoom/res), QImage::Format_RGBA8888);
	FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(image.width(), image.height(), FPDFBitmap_BGRA,
			image.scanLine(0), image.bytesPerLine());
    FPDF_RenderPageBitmap(bitmap, page, 0, 0, image.width(), image.height(), 0, 0);
    FPDFBitmap_Destroy(bitmap);
    FPDF_ClosePage(page);
    emit newPage(&image);
}


void Doc::toText(QString& s){
	QString tmp;
	for(int j=0; j<pageCount; j++){
		FPDF_PAGE page = FPDF_LoadPage(doc, j);
		FPDF_TEXTPAGE tpage = FPDFText_LoadPage(page);
		int len = FPDFText_CountChars(tpage);
		ushort *buf = new ushort[len+1];
		FPDFText_GetText(tpage,0, len, buf);
		tmp.append(QString::fromUtf16(buf, len));
        FPDF_ClosePage(page);
        FPDFText_ClosePage(tpage);
	}
	s = tmp;
}

void Doc::getImages(){
	for (int i=0;i<pageCount;i++){
	    FPDF_PAGE page = FPDF_LoadPage(doc, i);
		int oc = FPDFPage_CountObjects(page);
		for (int j=0;j<oc;j++){
			auto o = FPDFPage_GetObject(page, j);
			if(o==nullptr) continue;
			if( FPDFPageObj_GetType(o) == FPDF_PAGEOBJ_IMAGE ) {
				FPDF_BITMAP bitmap = FPDFImageObj_GetRenderedBitmap(doc,page,o);
				FPDF_IMAGEOBJ_METADATA meta;
				FPDFImageObj_GetImageMetadata(o, page, &meta);
				if(meta.colorspace == FPDF_COLORSPACE_DEVICERGB || meta.colorspace ==FPDF_COLORSPACE_CALRGB
				   || meta.colorspace == FPDF_COLORSPACE_DEVICECMYK || meta.colorspace == FPDF_COLORSPACE_ICCBASED){
					int w = FPDFBitmap_GetWidth(bitmap);
					int h = FPDFBitmap_GetHeight(bitmap);
					QImage image((uchar*)FPDFBitmap_GetBuffer(bitmap), w, h, FPDFBitmap_GetStride(bitmap), QImage::Format_ARGB32);
				    emit newImage(&image);
					imageCount++;
				} else{
					imageCount++;
				}
			    FPDFBitmap_Destroy(bitmap);
			}
		}
	    FPDF_ClosePage(page);
	}
}

void Doc::rgba(QImage& img, uchar* pix, int w, int h){
	for(int i = 0; i < h; ++i){
		for ( int j = 0; j < w; ++j, pix += 4 ) {
			img.setPixel(j, i, qRgba(pix[0], pix[1], pix[2], pix[3]));
		}
	}
}
