#ifndef BSPLINEWINDOW_H
#define BSPLINEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "bsplinemodel.h"
namespace Ui {
class BSplineWindow;
}

class BSplineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BSplineWindow(QWidget *parent = nullptr);
    ~BSplineWindow();
protected:
    virtual void keyPressEvent(QKeyEvent * event) override;


private slots:

    void on_spinBoxN_valueChanged(int arg1);

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_normalizeButton_clicked();

private:
    Ui::BSplineWindow *ui;
    QGraphicsView *graphicsView;
    BSplineModel *model;
};

#endif // BSPLINEWINDOW_H
