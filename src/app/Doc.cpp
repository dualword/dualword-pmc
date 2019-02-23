/*
 *	Dualword-pmc is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-pmc is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-pmc. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "Doc.h"
#include "global.h"

Doc::Doc(QObject *p) : QObject(p), ctx(nullptr), doc(nullptr), zoom(150),
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
	Q_ASSERT( pi >= 0 );

	int p = --pi;
	if(p < 0 || p > pageCount) return;
	pageNum = p;

	fz_page *page;
	fz_pixmap *pix;
	fz_device *dev;
	fz_try(ctx){
	    page = fz_load_page(ctx, doc, pageNum);
		int rotation = 0;
	    fz_matrix transform = fz_identity;
	    fz_rotate(&transform, rotation);
	    fz_pre_scale(&transform, zoom / 100.0f, zoom / 100.0f);

		fz_rect bounds;
		fz_bound_page(ctx, page, &bounds);
		fz_transform_rect(&bounds, &transform);

		fz_irect bbox;
		fz_round_rect(&bbox, &bounds);
		pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), &bbox);
		fz_clear_pixmap_with_value(ctx, pix, 0xff);

		dev = fz_new_draw_device(ctx, pix);
		fz_run_page(ctx, page, dev, &transform, NULL);

		int w = fz_pixmap_width(ctx, pix);
		int h = fz_pixmap_height(ctx, pix);
		QImage image(pix->samples, w, h, QImage::Format_ARGB32);
		emit newPage(&image);
	}fz_always(ctx)	{
		fz_drop_pixmap(ctx, pix);
		fz_drop_page(ctx, page);
    	fz_drop_device(ctx, dev);
	}fz_catch(ctx){
		//char *msg = ctx->error->message;
	}

}

void Doc::toText(QString& s){
	fz_try(ctx){
		QString tmp;

		for(int j=0;j<pageCount;j++){
			fz_page *page = fz_load_page(ctx, doc, j);
			fz_text_sheet *sheet = fz_new_text_sheet(ctx);
			fz_text_page *text = fz_new_text_page(ctx);
			fz_device *dev = fz_new_text_device(ctx, sheet, text);
			fz_run_page(ctx, page, dev, &fz_identity, NULL);
			fz_buffer *buf = fz_new_buffer(ctx, 256);
			fz_output *out = fz_new_output_with_buffer(ctx, buf);
			fz_print_text_page(ctx, out, text);
			tmp.append((char*) buf->data);
			fz_drop_output(ctx, out);
			fz_drop_buffer(ctx, buf);
			fz_drop_device(ctx, dev);
			fz_drop_text_page(ctx, text);
			fz_drop_text_sheet(ctx, sheet);
			fz_drop_page(ctx, page);
		}
		s = tmp;

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
    		pdf_obj *obj = pdf_load_object(ctx, d, i, 0);
    		pdf_obj *type = pdf_dict_get(ctx, obj, PDF_NAME_Subtype);
    		if (pdf_name_eq(ctx, type, PDF_NAME_Image)){
        		fz_image *image;
        		fz_pixmap *pix;
        		pdf_obj *ref;
        		char buf[32];
        		ref = pdf_new_indirect(ctx, d, i, 0);
        		image = pdf_load_image(ctx, d, ref);
        		pix = fz_image_get_pixmap(ctx, image, 0, 0);
        		fz_drop_image(ctx, image);
        		int rgb = 0;
        			if (!pix) continue;
        			if (pix->n == 4) {
            			imageCount++;
        				int w = fz_pixmap_width(ctx, pix);
        				int h = fz_pixmap_height(ctx, pix);
        	    		QImage img(pix->samples, w, h, QImage::Format_ARGB32);
        	    		emit newImage(&img);
        			}
        		fz_drop_pixmap(ctx, pix);
    		}
    		pdf_drop_obj(ctx, obj);
    	}
    }fz_always(ctx)	{
    	pdf_close_document(ctx, d);
    	fz_drop_stream(ctx, fs);
	}fz_catch(ctx){
		//qDebug() << ctx->error->message;
    }
}
