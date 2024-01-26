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
	void handleCopyButton() const;
	void handleLengthChange();
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
	void updateSimilarCharsVisibilityState() const;
	void updateAmbiguousSymbolsVisibilityState() const;
	static void centerWidgetHorizontally(QWidget* widget);
private:
    Ui::MainWindow *m_Ui;
	Rpg m_Rpg;
    RpgUi m_RpgUi;

	const int MINIMUM_PASS_LENGTH = 6;
};

int qw_width(const QWidget& widget);
int qw_height(const QWidget& widget);
int qw_x_pos(const QWidget& widget);
int qw_y_pos(const QWidget& widget);


#endif // MAINWINDOW_H
