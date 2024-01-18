#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RpgUi.h"
#include "Rpg.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleRefresh();
	void handleCopyButton();
	void handleLengthChange(int index);
	void handleUpperStateChange(int state);
	void handleLowerStateChange(int state);
	void handleNumberStateChange(int state);
	void handleSymbolStateChange(int state);
	void handleSimilarCharsStateChange(int state);
	void handleAmbiguousSymbolsStateChange(int state);
private:
    void setupUi();
    void setupHeader();
	void setupPwText();
	void setupPushButtons();
	void setupLengthTool();
	void setupCheckBoxes();
	void setupCheckBoxLabels();
private:
    Ui::MainWindow *ui;
	PassGenerator m_Rpg;
    RpgUi m_RpgUi;
};

float qw_width(const QWidget& widget);
float qw_height(const QWidget& widget);
float qw_x_pos(const QWidget& widget);
float qw_y_pos(const QWidget& widget);


#endif // MAINWINDOW_H
