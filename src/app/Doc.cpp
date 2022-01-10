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

#include "pdfium/public/fpdfview.h"
#include "pdfium/public/fpdf_text.h"
#include "pdfium/core/fpdfapi/parser/cpdf_document.h"
#include "pdfium/core/fpdfapi/page/cpdf_page.h"
#include "pdfium/core/fpdftext/cpdf_textpage.h"

Doc::Doc(QObject *p) : QObject(p), doc(nullptr), zoom(150), res(100), pageNum(0), pageCount(0), imageCount(0) {
	FPDF_InitLibrary();
}

Doc::~Doc() {
	FPDF_CloseDocument(doc);
	FPDF_DestroyLibrary();
}

void Doc::open(){
    void* ptr = FPDF_LoadMemDocument(data.constData(), data.size(), (QString()).toUtf8().constData());
    doc = static_cast<CPDF_Document*>(ptr);
    pageCount = doc->GetPageCount();

}

void Doc::loadPage(int i){
	int p = --i;
	if(p < 0 || p > pageCount) return;
	pageNum = p;

    CPDF_Page *page = static_cast<CPDF_Page*>(FPDF_LoadPage(doc, i));
	QImage image(page->GetPageWidth()*(zoom/res), page->GetPageHeight()*(zoom/res), QImage::Format_RGBA8888);
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
	    CPDF_Page *page = static_cast<CPDF_Page*>(FPDF_LoadPage(doc, j));
		CPDF_TextPage *tpage = static_cast<CPDF_TextPage*>(FPDFText_LoadPage(page));
		tmp.append(QString::fromWCharArray(tpage->GetPageText(0, tpage->CountChars()).c_str()));
        FPDF_ClosePage(page);
        FPDFText_ClosePage(tpage);
	}
	s = tmp;
}

void Doc::getImages(){
//	fz_stream* fs;
//	pdf_document *d;
//    fz_try(ctx){
//    	fs = fz_open_memory(ctx, (unsigned char *)data.constData(), data.length());
//    	d = pdf_open_document_with_stream(ctx, fs);
//    	int len = pdf_count_objects(ctx, d);
//
//    	for (int i = 1; i < len; i++){
//    		pdf_obj *obj = pdf_load_object(ctx, d, i);
//    		pdf_obj *type = pdf_dict_get(ctx, obj, PDF_NAME(Subtype));
//    		if (pdf_name_eq(ctx, type, PDF_NAME(Image))){
//        		fz_image *image;
//        		fz_pixmap *pix;
//        		pdf_obj *ref;
//        		ref = pdf_new_indirect(ctx, d, i, 0);
//        		image = pdf_load_image(ctx, d, ref);
//        		pix = fz_get_pixmap_from_image(ctx, image, NULL, NULL, 0, 0);
//        		fz_drop_image(ctx, image);
//				if (!pix) continue;
//				int w = fz_pixmap_width(ctx, pix);
//				int h = fz_pixmap_height(ctx, pix);
//				if (pix->n == 4) {
//					imageCount++;
//					QImage image(w, h, QImage::Format_ARGB32);
//					rgba(image, pix->samples, w, h);
//					emit newImage(&image);
//				}else{
//					imageCount++;
//				}
//        		fz_drop_pixmap(ctx, pix);
//        		pdf_drop_obj(ctx, ref);
//    		}
//    		pdf_drop_obj(ctx, obj);
//    	}
//    }fz_always(ctx)	{
//    	pdf_drop_document(ctx, d);
//    	fz_drop_stream(ctx, fs);
//	}fz_catch(ctx){
//		//qDebug() << ctx->error->message;
//    }
}

void Doc::rgba(QImage& img, unsigned char* pix, int w, int h){
	for(int i = 0; i < h; ++i){
		for ( int j = 0; j < w; ++j, pix += 4 ) {
			img.setPixel(j, i, qRgba(pix[0], pix[1], pix[2], pix[3]));
		}
	}
}
