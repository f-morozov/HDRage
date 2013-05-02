#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <ctime>
#include <iostream>
#include <vector>
#include "ui_mainwindow.h"
#include "image_view.h"
#include "hdr_loader.h"
#include "hdr_image.h"
#include "tonemap.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

public slots:
	void help();
	void about();
	void makeHdr();
	void openImage();
	void tonemap();
	void changeTonemapParams(int i);
	void restore();
	void radiance();
	void setPath(QString &p)
	{
		path = p;
	}
	void saveHDR();
	void saveLDR();

private:
	void readFolder(QString &path, vector<QImage>& images, vector<float>& evalues);
	void updateFps(int fps);
	void dropEvent(QDropEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void setImage(HDRImage *img, bool scalefull);
	enum tonemap_algorithms
	{
		tm_linear,
		tm_log,
		tm_bilateral,
		tm_histogram,
		tm_perceptual,
		tm_blur,
		tm_count
	};
	Ui::MainWindowClass ui;
	void loadImage(QString& path);
	QByteArray initial_state;
	ImageView *image_view;
	int fps_sum;
	int tonemap_count;
	HDRImage *origImg;
	QString path;
	QImage ldr;
};

#endif // MAINWINDOW_H
