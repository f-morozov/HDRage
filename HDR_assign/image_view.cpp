#include "image_view.h"

ImageView::ImageView(QWidget * parent, Qt::WindowFlags f) 
		: QLabel(parent, f)
	{
		setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}

ImageView::ImageView(const QString& text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(text, parent, f)
{
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	setFont(QFont("MS Shell Dlg 2", 16));
}

ImageView::~ImageView()
	{
	}

void ImageView::loadImage(QImage &new_img)
{
	img = new_img;
	drawImg();
}

void ImageView::drawImg()
{
	if(img.width() <= 0 || img.height() <= 0){
		return;
	}
	QImage draw_img = img;
	if(img.width() > width() || img.height() > height()) {

		draw_img = img.scaled(width(), height(), Qt::KeepAspectRatio);
	}
	setPixmap(QPixmap::fromImage(draw_img));
}

void ImageView::resizeEvent ( QResizeEvent* e)
{
	drawImg();
}