#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QtGui>
#include <QString>

class ImageView : public QLabel
{
	Q_OBJECT
public:
	ImageView(QWidget *parent = 0, Qt::WindowFlags flag = 0);
	ImageView(const QString& text, QWidget *parent = 0, Qt::WindowFlags flag = 0);
	~ImageView();
	void loadImage(QImage &new_img);
protected:
	QImage img;
	void drawImg();
	void resizeEvent ( QResizeEvent* e);
};

#endif