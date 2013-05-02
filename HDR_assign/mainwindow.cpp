#include "mainwindow.h"
#include "radiance.h"
#include "hdr_maker.h"

#include <QtGui>
#include <QtGui>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), fps_sum(0), tonemap_count(0), origImg(NULL)
{
	ui.setupUi(this);
	setWindowTitle("High Dynamic Rage");
	setWindowIcon (QIcon("icon.ico"));
	setAcceptDrops(true);

	changeTonemapParams(0);

	image_view = new ImageView("Drag HDR image here", this);
	ui.centralWidget->layout()->addWidget(image_view);

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionMake, SIGNAL(triggered()), this, SLOT(makeHdr()));
	connect(ui.make_button, SIGNAL(clicked()), this, SLOT(makeHdr()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openImage()));
	connect(ui.open_button, SIGNAL(clicked()), this, SLOT(openImage()));
	connect(ui.tonemap_button, SIGNAL(clicked()), this, SLOT(tonemap()));
	connect(ui.algo_select, SIGNAL(activated(int)), this, SLOT(changeTonemapParams(int)));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(restore()));
	connect(ui.rad_button, SIGNAL(clicked()), this, SLOT(radiance()));
	connect(ui.save_hdr_button, SIGNAL(clicked()), this, SLOT(saveHDR()));
	connect(ui.save_ldr_button, SIGNAL(clicked()), this, SLOT(saveLDR()));
	QDir::setCurrent(QDir::homePath());
	initial_state = saveState();
}

MainWindow::~MainWindow()
{
}

void MainWindow::restore()
{
	restoreState(initial_state);
}

void MainWindow::about()
{
	QString text("High Dynamic Rage<br>\
				 Fedor 'fm' Morozov<br>\
				 f-morozov@ya.ru<br>\
				 <br>\
				 Uses Qt and CUDA<br>\
				 Icon by Hawk9mm<br>\
				 CAWABANGA!<br>");

	QMessageBox::about(this, "High Dynamic Rage", text);
}

void MainWindow::help()
{
	QString text(" \
				 View readme.txt...\
				 ");

	QMessageBox::information(this, "High Dynamic Rage Help", text);
}

void MainWindow::changeTonemapParams(int i)
{
	ui.algo_select->setCurrentIndex(i);
	static struct
	{
		bool enabled1; QString text1; double value1;
		bool enabled2; QString text2; double value2;
		bool enabled3; QString text3; double value3;
	} settings[tm_count] = {{false, "", 0, false, "", 0, false, "", 0},
		    			    {true, "Bias", 0.85, false, "", 0, false, "", 0},
			    	        {true, "Contrast", 50, true, "Lum Sigma", 2, true, "Space Sigma", 2},
							{true, "Betta", 0.6, false, "", 0, false, "", 0},
							{true, "Intensity", 0, true, "Color Correction", 0, true, "Adaptation", 1},
							{true, "k", 8, true, "Sigma", 100, true, "Correction", 2},
				            };

	ui.param_label_1->setVisible(settings[i].enabled1);
	ui.param_value_1->setVisible(settings[i].enabled1);
	ui.param_label_1->setText(settings[i].text1);
	ui.param_value_1->setValue(settings[i].value1);
	ui.param_label_2->setVisible(settings[i].enabled2);
	ui.param_value_2->setVisible(settings[i].enabled2);
	ui.param_label_2->setText(settings[i].text2);
	ui.param_value_2->setValue(settings[i].value2);
	ui.param_label_3->setVisible(settings[i].enabled3);
	ui.param_value_3->setVisible(settings[i].enabled3);
	ui.param_label_3->setText(settings[i].text3);
	ui.param_value_3->setValue(settings[i].value3);
}

void MainWindow::readFolder(QString &path, vector<QImage>& images, vector<float>& evalues)
{
	QDir::setCurrent(path);
	FILE *file = fopen("info.txt", "r");
	if(!file)
		return;

	float evalue = 0;
	while(fscanf(file, "%f", &evalue) == 1) {
	
		evalues.push_back(evalue);
	}
	fclose(file);
	for(int im = 0; im < evalues.size(); im++) {

		QString name = QString("img") + QString::number(im);
		QImage img(name);
		img = img.convertToFormat(QImage::Format_RGB32);
		images.push_back(img);
	}
}

void MainWindow::makeHdr()
{
	QString path = QFileDialog::getExistingDirectory(this, "LDR images folder");
	vector<QImage> images;
	vector<float> evalues;
	readFolder(path, images, evalues);
	if(images.size() < 1)
		return;
	float gamma = QInputDialog::getDouble(this, "Gamma", "Pre gamma:", 2.2f);
	HDRImage *res = composeHDR(images, evalues, gamma);
	setImage(res, true);
	saveHdrFile("img.hdr", res);
}

void MainWindow::openImage()
{
	QString folder = QDir::currentPath();
	QString path = QFileDialog::getOpenFileName(this, "Open HDR Image", folder, "HDR (*.hdr *.pic)");
	QDir::setCurrent(QFileInfo(path).absolutePath());
	if(!path.isEmpty()){
		loadImage(path);
	}
}

void MainWindow::tonemap()
{
	if(!origImg)
		return;
	HDRImage *img = origImg->clone();
	int start = clock();
	switch(ui.algo_select->currentIndex()) {

	case tm_log: 
		logMap(img, ui.param_value_1->value());
		break;
	case tm_bilateral:
		bilateralMap(img, ui.param_value_1->value(), ui.param_value_2->value(), ui.param_value_3->value());
		break;
	case tm_histogram:
		histogramMap(img, 0, ui.param_value_1->value());
		break;
	case tm_perceptual:
		recMap(img, -1, ui.param_value_1->value(), ui.param_value_2->value(), ui.param_value_3->value());
		break;
	case tm_blur:
		blurMap(img, ui.param_value_1->value(), ui.param_value_2->value(), ui.param_value_3->value());
		break;
	}
	linearMap(img, true);
	gammaCorrection(img, 2.2f);

	int end = clock();
	ldr = hdrToQImage(img);
	image_view->loadImage(ldr);
	
	QDir::setCurrent(QFileInfo(path).absolutePath());
	ldr.save("tonemapped.jpg");

	int fps = CLOCKS_PER_SEC / (end - start);
	tonemap_count++;
	updateFps(fps);
}

void MainWindow::updateFps(int fps)
{
	fps_sum += fps;
	int mean;
	if(tonemap_count) {
		mean = (fps_sum + tonemap_count - 1) / tonemap_count;
	} else {
		mean = 0;
	}
	ui.last_fps_display->display(fps);
	ui.mean_fps_display->display(mean);
}

void MainWindow::dropEvent(QDropEvent *e)
{
	if(e->mimeData()->hasUrls())
	{
		loadImage(e->mimeData()->urls()[0].path().remove(0, 1));
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasUrls())
	{
		e->acceptProposedAction();
	}
}

void MainWindow::loadImage(QString& path)
{
	setImage(loadHdrFile(path), true);
}

void MainWindow::setImage(HDRImage *img, bool scalefull)
{
	tonemap_count = fps_sum = 0;
	if(origImg)
		delete origImg;
	origImg = img;
	linearMap(origImg, scalefull);
	HDRImage *show = origImg->clone();
	gammaCorrection(show, 2.2f);
	
	ldr = hdrToQImage(show);
	image_view->loadImage(ldr);
	
	delete show;
}

void MainWindow::radiance()
{
	float power = QInputDialog::getDouble(this, "Power", "Source light power:", 100);
	float size =  QInputDialog::getDouble(this, "Height", "Source height:", 1);
	float ssize =  QInputDialog::getDouble(this, "Source size", "Square source size (0 for point):", 0);
	float t = ssize > 0 ? QInputDialog::getDouble(this, "Source angle", "Source angle:", 0) : 0;
	QDir::setCurrent(QFileInfo(path).absolutePath());
	QString surface_path = QFileDialog::getOpenFileName(this, "Surface Spectra");
	QString spectra_path = QFileDialog::getOpenFileName(this, "Source Spectra");

	if(surface_path.size() == 0 || spectra_path.size() == 0)
		return;

	QDir::setCurrent(QFileInfo(path).absolutePath());
	HDRImage *res = calcRadiance(256, power, size, ssize, t, surface_path, spectra_path);
	setImage(res, false);
	QDir::setCurrent(QFileInfo(path).absolutePath());
	ldr.save("tonemapped.jpg");
}

void MainWindow::saveLDR()
{
	QString path = QFileDialog::getSaveFileName(this, "Save LDR");
	ldr.save(path);
}

void MainWindow::saveHDR()
{
	QString path = QFileDialog::getSaveFileName(this, "Save LDR");
	saveHdrFile(path, origImg);
}