#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bsplinewindow.h"
#include "wireframermodel.h"
#include "wireframefield.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    virtual void keyPressEvent(QKeyEvent * event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actioneditor_triggered();

    void on_actionrefresh_triggered();

    void on_actionabout_triggered();

    void on_actionhelp_triggered();

    void on_saveButton_clicked();

    void on_openButton_clicked();

    void on_editorButton_clicked();

    void on_refreshButton_clicked();

    void on_helpButton_clicked();

    void on_aboutButton_clicked();

private:
    Ui::MainWindow *ui;
    BSplineWindow* bsplineWindow;
    WireFrameField* graphicsView;
    WireFrameRModel* model;
};
#endif // MAINWINDOW_H
