#ifndef BSPLINEWINDOW_H
#define BSPLINEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QProgressBar>
#include "bsplinemodel.h"
#include "bsplinefield.h"
#include "pthread.h"
namespace Ui {
class BSplineWindow;
}

class BSplineWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void closed();

public:
    explicit BSplineWindow(QImage* image, QWidget *parent = nullptr);
    QProgressBar* getProgressBar();
    void updateImage();
    ~BSplineWindow();
protected:
    virtual void closeEvent(QCloseEvent* event) override;
private slots:
    void on_progressBar_valueChanged(int value);

private:
    Ui::BSplineWindow *ui;
    BSplineField *graphicsView;
};

#endif // BSPLINEWINDOW_H
