#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QClipboard>

#include <iterator>
#include <string>

constexpr int FIXED_WINDOW_WIDTH = 640;
constexpr int FIXED_WINDOW_HEIGHT = 480;
constexpr int VERTICAL_SPACING = 10;
constexpr int HORIZONTAL_SPACING = 10;
constexpr int DEFAULT_PASSWORD_LENGTH = 12;
constexpr int HEADER_FONT_SIZE = 20;
constexpr int BODY_FONT_SIZE = 12;
constexpr int BUTTON_OFFSET = 5;
constexpr int DEFAULT_PASS_LENGTH_INDEX = 6;
constexpr int EXCLUDE_SIMILAR_FIXED_WIDTH = 181;
constexpr int EXCLUDE_AMBIGUOUS_FIXED_WIDTH = 192;
constexpr int ICON_WIDTH = 16;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_Ui(new Ui::MainWindow), m_Rpg(DEFAULT_PASSWORD_LENGTH, true, true, true, true)
{
    setFixedSize(FIXED_WINDOW_WIDTH, FIXED_WINDOW_HEIGHT);

    setupUi();
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

void MainWindow::handleRefresh()
{
	m_RpgUi.password = m_Rpg.generate().c_str();
    m_RpgUi.pw_text->setPlainText(m_RpgUi.password);
	m_RpgUi.pw_text->setAlignment(Qt::AlignCenter);
}

void MainWindow::handleCopyButton() const
{
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(m_RpgUi.password);
}

void MainWindow::handleLengthChange()
{
	QByteArray len8 = m_RpgUi.length_combo->currentText().toLocal8Bit();
	const char *len = len8.data();
	const int new_length = std::stoi(len);
	if (m_Rpg.length() != new_length)
	{
		m_Rpg.setLength(new_length);
		handleRefresh();
	} 
}

void MainWindow::handleUpperStateChange(int state)
{
	if (m_RpgUi.lower_checkbox->checkState() == 0U && m_RpgUi.numbers_checkbox->checkState() == 0U && m_RpgUi.symbols_checkbox->checkState() == 0U)
	{
		m_RpgUi.upper_checkbox->setCheckState(Qt::Checked);
		return;
	}
	updateSimilarCharsVisibilityState();
	m_Rpg.useUpper(static_cast<bool>(state));
	handleRefresh();
}

void MainWindow::handleLowerStateChange(int state)
{
	if (m_RpgUi.upper_checkbox->checkState() == 0U && m_RpgUi.numbers_checkbox->checkState() == 0U && m_RpgUi.symbols_checkbox->checkState() == 0U)
	{
		m_RpgUi.lower_checkbox->setCheckState(Qt::Checked);
		return;
	}
	updateSimilarCharsVisibilityState();
	m_Rpg.useLower(static_cast<bool>(state));
	handleRefresh();
}

void MainWindow::handleNumberStateChange(int state)
{
	if (m_RpgUi.upper_checkbox->checkState() == 0U && m_RpgUi.lower_checkbox->checkState() == 0U && m_RpgUi.symbols_checkbox->checkState() == 0U)
	{
		m_RpgUi.numbers_checkbox->setCheckState(Qt::Checked);
		return;
	}
	m_Rpg.useNumbers(static_cast<bool>(state));
	handleRefresh();
}

void MainWindow::handleSymbolStateChange(int state)
{
	if (m_RpgUi.upper_checkbox->checkState() == 0U && m_RpgUi.numbers_checkbox->checkState() == 0U && m_RpgUi.lower_checkbox->checkState() == 0U)
	{
		m_RpgUi.symbols_checkbox->setCheckState(Qt::Checked);
		return;
	}
	updateAmbiguousSymbolsVisibilityState();
	m_Rpg.useSymbols(static_cast<bool>(state));
	handleRefresh();
}

void MainWindow::handleSimilarCharsStateChange(int state)
{
	m_Rpg.excludeSimilarChars(static_cast<bool>(state));
	handleRefresh();
}

void MainWindow::handleAmbiguousSymbolsStateChange(int state)
{
	m_Rpg.excludeAmbiguousSymbols(static_cast<bool>(state));
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
    font.setPointSize(HEADER_FONT_SIZE);
    m_RpgUi.header->setFont(font);
    m_RpgUi.header->adjustSize();
    const int width = m_RpgUi.header->geometry().width();
    const int height = m_RpgUi.header->geometry().height();
	const int xpos = FIXED_WINDOW_WIDTH / 2 - width / 2;
	const int ypos = height * 3;
    m_RpgUi.header->setGeometry(xpos, ypos, width, height);
}

void MainWindow::setupPwText()
{
	m_RpgUi.password = m_Rpg.generate().c_str();
	m_RpgUi.pw_text = new QTextEdit("", this);
	m_RpgUi.pw_text->setPlainText(m_RpgUi.password);
	m_RpgUi.pw_text->setAlignment(Qt::AlignCenter);
	QFont font = m_RpgUi.pw_text->font();
	font.setPointSize(BODY_FONT_SIZE);
	m_RpgUi.pw_text->setFont(font);
	const int header_y = m_RpgUi.header->geometry().y();
	const int width = FIXED_WINDOW_WIDTH / 3;
	const int height = m_RpgUi.header->geometry().height();
	const int xpos = FIXED_WINDOW_WIDTH / 2 - width / 2 - height * 2;
	const int ypos = header_y + height + VERTICAL_SPACING;
	m_RpgUi.pw_text->setGeometry(xpos, ypos, width, height);
}

void setupButton(QPushButton* button, const QRect& geometry, const int offset = 0)
{
	const int xpos = geometry.x() + geometry.width() + offset;
	const int ypos = geometry.y();
	const int height = geometry.height();
	button->setGeometry(xpos, ypos, height, height);
	button->setIconSize(QSize(height, height));
}

void MainWindow::setupPushButtons()
{
	m_RpgUi.refresh_icon = new QIcon(QString(RPG_ASSET_DIR) + "/refresh.png");
	m_RpgUi.refresh_button = new QPushButton(*m_RpgUi.refresh_icon, "", this);
	setupButton(m_RpgUi.refresh_button, m_RpgUi.pw_text->geometry());
	connect(m_RpgUi.refresh_button, SIGNAL (clicked(bool)), this, SLOT (handleRefresh()));

	m_RpgUi.copy_icon = new QIcon(QString(RPG_ASSET_DIR) + "/copy.png");
	m_RpgUi.copy_button = new QPushButton(*m_RpgUi.copy_icon, "", this);
	setupButton(m_RpgUi.copy_button, m_RpgUi.refresh_button->geometry(), BUTTON_OFFSET);
	connect(m_RpgUi.copy_button, SIGNAL (clicked(bool)), this, SLOT (handleCopyButton()));
}

void fillLengthCombo(QComboBox& combo, const int min_length)
{
	const int TERMINATOR = 128;
	for (int i = min_length; i <= TERMINATOR; ++i)
	{
		combo.addItem(QString(std::to_string(i).c_str()));
	}

	int multiplied_total = TERMINATOR;
	const int TERMINATOR_2 = 5;
	for (int i = 0; i != TERMINATOR_2; ++i)
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
	const int length_label_w = m_RpgUi.length_selection_label->geometry().width();
	const int length_label_h = m_RpgUi.length_selection_label->geometry().height();
	const int length_label_x = FIXED_WINDOW_WIDTH / 2 - length_label_w / 2 - combo_geometry.width() / 2;
	const int length_label_y = pw_geometry.y() + pw_geometry.height() + VERTICAL_SPACING;
    m_RpgUi.length_selection_label->setGeometry(length_label_x, length_label_y, length_label_w, length_label_h);

	m_RpgUi.length_combo->setStyleSheet("combobox-popup: 0;");
	m_RpgUi.length_combo->setGeometry(length_label_x + length_label_w, length_label_y, combo_geometry.width(), combo_geometry.height() );
	fillLengthCombo(*m_RpgUi.length_combo, MINIMUM_PASS_LENGTH);
	m_RpgUi.length_combo->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_RpgUi.length_combo->setCurrentIndex(DEFAULT_PASS_LENGTH_INDEX);

	connect(m_RpgUi.length_combo, SIGNAL (currentIndexChanged(int)), this, SLOT (handleLengthChange()));
}

void setupCheckBoxRow(std::vector<QCheckBox*> cbs, const int ypos, bool checked, bool add_icon_width = false)
{
	int offset = 0;
	const int box_width = qw_width(*cbs.front());
	int row_width = box_width * static_cast<int>(cbs.size());

	if (add_icon_width)
	{
		const QSize icon_size = cbs.front()->iconSize();
		row_width += static_cast<int>(icon_size.width()) * static_cast<int>(cbs.size()) + HORIZONTAL_SPACING;
	}
	int starting_x = FIXED_WINDOW_WIDTH / 2 - row_width / 2;

	for (auto *box : cbs)
	{
		add_icon_width && offset != 0 ? starting_x += HORIZONTAL_SPACING: starting_x;
		box->setGeometry(starting_x + box_width * offset, ypos, box_width, qw_height(*box));
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

	m_RpgUi.exclude_similar_checkbox->setFixedWidth(EXCLUDE_SIMILAR_FIXED_WIDTH);
	m_RpgUi.exclude_ambiguous_checkbox->setFixedWidth(EXCLUDE_AMBIGUOUS_FIXED_WIDTH);

	const int starting_y = m_RpgUi.length_selection_label->geometry().y() + qw_height(*m_RpgUi.length_selection_label);

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

	const int similar_x = similar_geo.x() + similar_geo.width() - similar_geo.width() /2 - qw_width(*m_RpgUi.similar_characters_label) / 2;
	const int ambiguous_x = ambiguous_geo.x() + ambiguous_geo.width() - ambiguous_geo.width() /2 - qw_width(*m_RpgUi.ambiguous_characters_label) / 2;

	const int similar_width = m_RpgUi.similar_characters_label->geometry().width();
	const int ambiguous_width = m_RpgUi.ambiguous_characters_label->geometry().width();
	const int y_position = m_RpgUi.exclude_similar_checkbox->geometry().y() + m_RpgUi.exclude_similar_checkbox->geometry().height() / 2;

	m_RpgUi.similar_characters_label->setGeometry(similar_x, y_position, similar_width, m_RpgUi.similar_characters_label->geometry().height());
	m_RpgUi.ambiguous_characters_label->setGeometry(ambiguous_x, y_position, ambiguous_width, m_RpgUi.ambiguous_characters_label->geometry().height());
	m_RpgUi.similar_characters_label->lower();
	m_RpgUi.ambiguous_characters_label->lower();
}

void MainWindow::updateSimilarCharsVisibilityState() const
{
	static const int ambiguous_label_x = m_RpgUi.ambiguous_characters_label->geometry().x();
	static const int ambiguous_label_y = m_RpgUi.ambiguous_characters_label->geometry().y();
	static const int ambiguous_checkbox_x = m_RpgUi.exclude_ambiguous_checkbox->geometry().x();
	static const int ambiguous_checkbox_y = m_RpgUi.exclude_ambiguous_checkbox->geometry().y();
	
	if(m_RpgUi.upper_checkbox->checkState() == 0U && m_RpgUi.lower_checkbox->checkState() == 0U)
	{
		m_RpgUi.similar_characters_label->setVisible(false);
		m_RpgUi.exclude_similar_checkbox->setVisible(false);
		centerWidgetHorizontally(m_RpgUi.ambiguous_characters_label);
		centerWidgetHorizontally(m_RpgUi.exclude_ambiguous_checkbox);
	}
	else if(!m_RpgUi.similar_characters_label->isVisible())
	{
		m_RpgUi.similar_characters_label->setVisible(true);
		m_RpgUi.exclude_similar_checkbox->setVisible(true);
		m_RpgUi.ambiguous_characters_label->move(ambiguous_label_x, ambiguous_label_y);
		m_RpgUi.exclude_ambiguous_checkbox->move(ambiguous_checkbox_x, ambiguous_checkbox_y);
		
	}
}

void MainWindow::updateAmbiguousSymbolsVisibilityState() const
{
	static const int similar_label_x = m_RpgUi.similar_characters_label->geometry().x();
	static const int similar_label_y = m_RpgUi.similar_characters_label->geometry().y();
	static const int similar_checkbox_x = m_RpgUi.exclude_similar_checkbox->geometry().x();
	static const int similar_checkbox_y = m_RpgUi.exclude_similar_checkbox->geometry().y();

	if(m_RpgUi.symbols_checkbox->checkState() == 0U)
	{
		m_RpgUi.ambiguous_characters_label->setVisible(false);
		m_RpgUi.exclude_ambiguous_checkbox->setVisible(false);
		centerWidgetHorizontally(m_RpgUi.similar_characters_label);
		centerWidgetHorizontally(m_RpgUi.exclude_similar_checkbox);
	}
	else if(!m_RpgUi.ambiguous_characters_label->isVisible())
	{
		m_RpgUi.ambiguous_characters_label->setVisible(true);
		m_RpgUi.exclude_ambiguous_checkbox->setVisible(true);
		m_RpgUi.similar_characters_label->move(similar_label_x, similar_label_y);
		m_RpgUi.exclude_similar_checkbox->move(similar_checkbox_x, similar_checkbox_y);
	}
}

void MainWindow::centerWidgetHorizontally(QWidget* widget)
{
	const int width = widget->geometry().width();
	const int ypos = widget->geometry().y();
	const int new_xpos = FIXED_WINDOW_WIDTH / 2 - width / 2;
	widget->move(new_xpos, ypos);
}

int qw_width(const QWidget& widget)
{
	return widget.geometry().width();
}

int qw_height(const QWidget& widget)
{
	return widget.geometry().height();
}

int qw_x_pos(const QWidget& widget)
{
	return widget.geometry().x();
}

int qw_y_pos(const QWidget& widget)
{
	return widget.geometry().y();
}