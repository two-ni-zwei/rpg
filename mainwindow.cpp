#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QClipboard>

#include <string>

constexpr int FIXED_WINDOW_WIDTH = 640;
constexpr int FIXED_WINDOW_HEIGHT = 480;
constexpr int VERTICAL_SPACING = 10;
constexpr int HORIZONTAL_SPACING = 10;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_Rpg(12, true, true, true, true)
{
    setFixedSize(FIXED_WINDOW_WIDTH, FIXED_WINDOW_HEIGHT);

    setupUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleRefresh()
{
	m_RpgUi.password = m_Rpg.Generate().c_str();
    m_RpgUi.pw_text->setPlainText(m_RpgUi.password);
	m_RpgUi.pw_text->setAlignment(Qt::AlignCenter);
}

void MainWindow::handleCopyButton()
{
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_RpgUi.password);
}

void MainWindow::handleLengthChange(int index)
{
	QByteArray ba = m_RpgUi.length_combo->currentText().toLocal8Bit();
	const char *c = ba.data();
	int new_length = std::stoi(c);
	if (m_Rpg.Length() != new_length)
	{
		m_Rpg.SetLength(new_length);
		handleRefresh();
	} 
}

void MainWindow::handleUpperStateChange(int state)
{
	m_Rpg.UseUpper(state);
	handleRefresh();
}

void MainWindow::handleLowerStateChange(int state)
{
	m_Rpg.UseLower(state);
	handleRefresh();
}

void MainWindow::handleNumberStateChange(int state)
{
	m_Rpg.UseNumbers(state);
	handleRefresh();
}

void MainWindow::handleSymbolStateChange(int state)
{
	m_Rpg.UseSymbols(state);
	handleRefresh();
}

void MainWindow::handleSimilarCharsStateChange(int state)
{
	m_Rpg.ExcludeSimilarChars(state);
	handleRefresh();
}

void MainWindow::handleAmbiguousSymbolsStateChange(int state)
{
	m_Rpg.ExcludeAmbiguousSymbols(state);
	handleRefresh();
}

void MainWindow::setupUi()
{
	setWindowTitle("RPG");
    setupHeader();
	setupPwText();
	setupPushButtons();
	setupLengthTool();
	setupCheckBoxes();
	setupCheckBoxLabels();
}

void MainWindow::setupHeader()
{
    m_RpgUi.header = new QLabel("RPG - The Random Password Generator", this);
    QFont font = m_RpgUi.header->font();
    font.setPointSize(20);
    m_RpgUi.header->setFont(font);
    m_RpgUi.header->adjustSize();
    float width = m_RpgUi.header->geometry().width();
    float height = m_RpgUi.header->geometry().height();
	float x = FIXED_WINDOW_WIDTH / 2 - width / 2;
	float y = height * 3;
    m_RpgUi.header->setGeometry(x, y, width, height);
}

void MainWindow::setupPwText()
{
	m_RpgUi.password = m_Rpg.Generate().c_str();
	m_RpgUi.pw_text = new QTextEdit("", this);
	m_RpgUi.pw_text->setPlainText(m_RpgUi.password);
	m_RpgUi.pw_text->setAlignment(Qt::AlignCenter);
	QFont font = m_RpgUi.pw_text->font();
	font.setPointSize(12);
	m_RpgUi.pw_text->setFont(font);
	float header_y = m_RpgUi.header->geometry().y();
	float width = FIXED_WINDOW_WIDTH / 3;
	float height = m_RpgUi.header->geometry().height();
	float x = FIXED_WINDOW_WIDTH / 2 - width / 2 - height * 2;
	float y = header_y + height + VERTICAL_SPACING;
	m_RpgUi.pw_text->setGeometry(x, y, width, height);
}

void setupButton(QPushButton* button, const QRect& geometry, MainWindow* window, float offset = 0.0)
{
	float x = geometry.x() + geometry.width() + offset;
	float y = geometry.y();
	float height = geometry.height();
	button->setGeometry(x, y, height, height);
	button->setIconSize(QSize(height, height));
}

void MainWindow::setupPushButtons()
{
	m_RpgUi.refresh_icon = new QIcon(QString(RPG_ASSET_DIR) + "/refresh.png");
	m_RpgUi.refresh_button = new QPushButton(*m_RpgUi.refresh_icon, "", this);
	setupButton(m_RpgUi.refresh_button, m_RpgUi.pw_text->geometry(), this);
	connect(m_RpgUi.refresh_button, SIGNAL (clicked(bool)), this, SLOT (handleRefresh()));

	m_RpgUi.copy_icon = new QIcon(QString(RPG_ASSET_DIR) + "/copy.png");
	m_RpgUi.copy_button = new QPushButton(*m_RpgUi.copy_icon, "", this);
	setupButton(m_RpgUi.copy_button, m_RpgUi.refresh_button->geometry(), this, 5.0);
	connect(m_RpgUi.copy_button, SIGNAL (clicked(bool)), this, SLOT (handleCopyButton()));
}

void fillLengthCombo(QComboBox& combo)
{
	for (int i = 6; i <= 128; ++i)
	{
		combo.addItem(QString(std::to_string(i).c_str()));
	}

	int multiplied_total = 128;
	for (int i = 0; i != 5; ++i)
	{
		combo.addItem(QString(std::to_string(multiplied_total *= 2).c_str()));
	}
}

void MainWindow::setupLengthTool()
{
	m_RpgUi.length_selection_label = new QLabel("Length", this);
	m_RpgUi.length_combo = new QComboBox(this);
	
	auto pw_geometry = m_RpgUi.pw_text->geometry();
	auto combo_geometry = m_RpgUi.length_combo->geometry();
	float length_label_w = m_RpgUi.length_selection_label->geometry().width();
	float length_label_h = m_RpgUi.length_selection_label->geometry().height();
	float length_label_x = FIXED_WINDOW_WIDTH / 2 - length_label_w / 2 - combo_geometry.width() / 2;
	float length_label_y = pw_geometry.y() + pw_geometry.height() + VERTICAL_SPACING;
    m_RpgUi.length_selection_label->setGeometry(length_label_x, length_label_y, length_label_w, length_label_h);

	m_RpgUi.length_combo->setStyleSheet("combobox-popup: 0;");
	m_RpgUi.length_combo->setGeometry(length_label_x + length_label_w, length_label_y, combo_geometry.width(), combo_geometry.height() );
	fillLengthCombo(*m_RpgUi.length_combo);
	m_RpgUi.length_combo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_RpgUi.length_combo->setCurrentIndex(6);

	connect(m_RpgUi.length_combo, SIGNAL (currentIndexChanged(int)), this, SLOT (handleLengthChange(int)));
}

void setupCheckBoxRow(std::vector<QCheckBox*> cbs, float y, bool checked, bool add_icon_width = false)
{
	int offset = 0;
	float box_width = qw_width(*cbs.front());
	float row_width = box_width * cbs.size();

	if (add_icon_width)
	{
		QSize icon_size = cbs.front()->iconSize();
		row_width += icon_size.width() * cbs.size() + HORIZONTAL_SPACING;
	}
	float starting_x = FIXED_WINDOW_WIDTH / 2 - row_width / 2;
	for (auto box : cbs)
	{
		add_icon_width && offset ? starting_x += HORIZONTAL_SPACING: starting_x;
		box->setGeometry(starting_x + box_width * offset, y, box_width, qw_height(*box));
		if(checked)
		{
			box->setCheckState(Qt::Checked);
		}
		offset++;
	}
}

void MainWindow::setupCheckBoxes()
{
	m_RpgUi.upper_checkbox = new QCheckBox("Uppercase", this);
	m_RpgUi.lower_checkbox = new QCheckBox("Lowercase", this);
	m_RpgUi.numbers_checkbox = new QCheckBox("Numbers", this);
	m_RpgUi.symbols_checkbox = new QCheckBox("Symbols", this);
	m_RpgUi.exclude_similar_checkbox = new QCheckBox("Exclude similar characters", this);
	m_RpgUi.exclude_ambiguous_checkbox = new QCheckBox("Exclude ambiguous symbols", this);

	m_RpgUi.exclude_similar_checkbox->setFixedWidth(181);
	m_RpgUi.exclude_ambiguous_checkbox->setFixedWidth(192);

	float starting_y = m_RpgUi.length_selection_label->geometry().y() + qw_height(*m_RpgUi.length_selection_label);

	setupCheckBoxRow({m_RpgUi.upper_checkbox, m_RpgUi.lower_checkbox, m_RpgUi.numbers_checkbox, m_RpgUi.symbols_checkbox}, starting_y, true);
	setupCheckBoxRow({m_RpgUi.exclude_similar_checkbox, m_RpgUi.exclude_ambiguous_checkbox}, starting_y + qw_height(*m_RpgUi.upper_checkbox), false, true);

	connect(m_RpgUi.upper_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleUpperStateChange(int)));
	connect(m_RpgUi.lower_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleLowerStateChange(int)));
	connect(m_RpgUi.numbers_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleNumberStateChange(int)));
	connect(m_RpgUi.symbols_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleSymbolStateChange(int)));
	connect(m_RpgUi.exclude_similar_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleSimilarCharsStateChange(int)));
	connect(m_RpgUi.exclude_ambiguous_checkbox, SIGNAL (stateChanged(int)), this, SLOT (handleAmbiguousSymbolsStateChange(int)));
}

void MainWindow::setupCheckBoxLabels()
{
	m_RpgUi.similar_characters_label = new QLabel("(0,O,o,L,l,i,1)", this);
	m_RpgUi.ambiguous_characters_label = new QLabel("{}[]()/\\'\"`~,;:.<>", this);
	
	auto similar_geo = m_RpgUi.exclude_similar_checkbox->geometry();
	auto ambiguous_geo = m_RpgUi.exclude_ambiguous_checkbox->geometry();

	float similar_x = similar_geo.x() + similar_geo.width() - similar_geo.width() /2 - qw_width(*m_RpgUi.similar_characters_label) / 2;
	float ambiguous_x = ambiguous_geo.x() + ambiguous_geo.width() - ambiguous_geo.width() /2 - qw_width(*m_RpgUi.ambiguous_characters_label) / 2;

	float similar_width = m_RpgUi.similar_characters_label->geometry().width();
	float ambiguous_width = m_RpgUi.ambiguous_characters_label->geometry().width();
	float row_width = similar_width + ambiguous_width;
	float y_position = m_RpgUi.exclude_similar_checkbox->geometry().y() + m_RpgUi.exclude_similar_checkbox->geometry().height() / 1.3;
	float x_position = FIXED_WINDOW_WIDTH / 2 - row_width / 2;

	m_RpgUi.similar_characters_label->setGeometry(similar_x, y_position, similar_width, m_RpgUi.similar_characters_label->geometry().height());
	m_RpgUi.ambiguous_characters_label->setGeometry(ambiguous_x, y_position, ambiguous_width, m_RpgUi.ambiguous_characters_label->geometry().height());
}

float qw_width(const QWidget& widget)
{
	return widget.geometry().width();
}

float qw_height(const QWidget& widget)
{
	return widget.geometry().height();
}

float qw_x_pos(const QWidget& widget)
{
	return widget.geometry().x();
}

float qw_y_pos(const QWidget& widget)
{
	return widget.geometry().y();
}




