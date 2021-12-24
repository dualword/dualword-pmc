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

Doc::Doc(QObject *p) : QObject(p), ctx(nullptr), doc(nullptr), zoom(150), res(100),
	pageNum(0),	pageCount(0), imageCount(0) {

}

Doc::~Doc() {
	if(doc){
		fz_drop_document(ctx, doc);
		doc = nullptr;
	}
	if(ctx) {
		fz_drop_context(ctx);
		ctx = nullptr;
	}
}

void Doc::open(){
	fz_stream* fs;
	ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	fz_register_document_handlers(ctx);
    fz_try(ctx){
    	fs = fz_open_memory(ctx, (unsigned char *)data.constData(), data.length());
    	doc = fz_open_document_with_stream(ctx, ".pdf", fs);
    	pageCount = fz_count_pages(ctx,doc);
    }fz_always(ctx)	{
    	fz_drop_stream(ctx, fs);
	}fz_catch(ctx){
		//throw dualword_exception("Couldn't open pdf:" + std::string(ctx->error->message));
    }
}

void Doc::loadPage(int pi){
	int p = --pi;
	if(p < 0 || p > pageCount) return;
	pageNum = p;

	fz_page *page;
	fz_pixmap *pix;
	fz_device *dev;
	fz_try(ctx){
	    page = fz_load_page(ctx, doc, pageNum);
	    fz_matrix transform;
	    float z =zoom/res;
	    transform = fz_pre_scale(fz_rotate(0), z, z);
		fz_rect rect = fz_bound_page(ctx, page);
		rect = fz_transform_rect(rect, transform);
		int w1 = 90 * (rect.x1 - rect.x0) / 72;
		int h1 = 90 * (rect.y1 - rect.y0) / 72;
		rect  = fz_make_rect(0,0, (w1 * z ) , (h1 * z ) );
		fz_irect irect = fz_round_rect(rect);
		pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), irect, nullptr, 1);
		fz_clear_pixmap_with_value(ctx, pix, 0xff);
		dev = fz_new_draw_device(ctx, transform, pix);
		fz_run_page(ctx, page, dev, transform, NULL);
		int w = fz_pixmap_width(ctx, pix);
		int h = fz_pixmap_height(ctx, pix);
		QImage image(w, h,QImage::Format_ARGB32);
		rgba(image, pix->samples, w, h);
		emit newPage(&image);
	}fz_always(ctx)	{
		fz_drop_pixmap(ctx, pix);
		fz_drop_page(ctx, page);
		fz_close_device(ctx, dev);
    	fz_drop_device(ctx, dev);
	}fz_catch(ctx){
		//char *msg = ctx->error->message;
	}
}

void Doc::toText(QString& s){
	fz_try(ctx){
		QString tmp;

		for(int j=0;j<pageCount;j++){
			fz_stext_page *text = NULL;
			fz_matrix ctm;
			fz_rect rect;
			fz_device *dev = NULL;
			fz_output *out = NULL;
			fz_cookie *cookie;
			fz_var(text);
			fz_page *page = fz_load_page(ctx, doc, j);
			rect = fz_bound_page(ctx, page);
			ctm = fz_pre_scale(fz_rotate(0), zoom/res, zoom/res);
			fz_stext_options stext_options;
			stext_options.flags = 0;
			text = fz_new_stext_page(ctx, rect);
			dev = fz_new_stext_device(ctx,  text, &stext_options);
			fz_run_page(ctx, page, dev, ctm, cookie);
			fz_close_device(ctx, dev);
			fz_drop_device(ctx, dev);
			unsigned char *data;
			fz_buffer *buf = fz_new_buffer(ctx, 256);
			out = fz_new_output_with_buffer(ctx, buf);
			fz_print_stext_page_as_text(ctx, out, text);
			fz_buffer_storage(ctx, buf, &data);
			tmp.append((char*) data);
			fz_drop_buffer(ctx, buf);
			fz_drop_output(ctx, out);
			fz_drop_device(ctx, dev);
			fz_drop_page(ctx, page);
		}
		s = tmp;
	}fz_always(ctx){
		//
	}fz_catch(ctx){
		//char *msg = ctx->error->message;
	}
}

void Doc::getImages(){
	fz_stream* fs;
	pdf_document *d;
    fz_try(ctx){
    	fs = fz_open_memory(ctx, (unsigned char *)data.constData(), data.length());
    	d = pdf_open_document_with_stream(ctx, fs);
    	int len = pdf_count_objects(ctx, d);

    	for (int i = 1; i < len; i++){
    		pdf_obj *obj = pdf_load_object(ctx, d, i);
    		pdf_obj *type = pdf_dict_get(ctx, obj, PDF_NAME(Subtype));
    		if (pdf_name_eq(ctx, type, PDF_NAME(Image))){
        		fz_image *image;
        		fz_pixmap *pix;
        		pdf_obj *ref;
        		ref = pdf_new_indirect(ctx, d, i, 0);
        		image = pdf_load_image(ctx, d, ref);
        		pix = fz_get_pixmap_from_image(ctx, image, NULL, NULL, 0, 0);
        		fz_drop_image(ctx, image);
				if (!pix) continue;
				int w = fz_pixmap_width(ctx, pix);
				int h = fz_pixmap_height(ctx, pix);
				if (pix->n == 4) {
					imageCount++;
					QImage image(w, h, QImage::Format_ARGB32);
					rgba(image, pix->samples, w, h);
					emit newImage(&image);
				}else{
					imageCount++;
				}
        		fz_drop_pixmap(ctx, pix);
        		pdf_drop_obj(ctx, ref);
    		}
    		pdf_drop_obj(ctx, obj);
    	}
    }fz_always(ctx)	{
    	pdf_drop_document(ctx, d);
    	fz_drop_stream(ctx, fs);
	}fz_catch(ctx){
		//qDebug() << ctx->error->message;
    }
}

void Doc::rgba(QImage& img, unsigned char* pix, int w, int h){
	for(int i = 0; i < h; ++i){
		for ( int j = 0; j < w; ++j, pix += 4 ) {
			img.setPixel(j, i, qRgba(pix[0], pix[1], pix[2], pix[3]));
		}
	}
}
