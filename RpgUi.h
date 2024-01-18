#ifndef RPGUI_H
#define RPGUI_H

#include <QLabel>
#include <QTextEdit>
#include <QIcon>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QCheckBox>
#include <QString>

struct RpgUi
{
    QLabel 				*header;
	QTextEdit 			*pw_text;
	QIcon 				*refresh_icon;
	QIcon 				*copy_icon;
	QPushButton 		*refresh_button;
	QPushButton 		*copy_button;
	QLabel 				*length_selection_label;
	QComboBox 			*length_combo;
	QCheckBox			*upper_checkbox;
	QCheckBox			*lower_checkbox;
	QCheckBox			*numbers_checkbox;
	QCheckBox			*symbols_checkbox;
	QCheckBox			*exclude_similar_checkbox;
	QCheckBox			*exclude_ambiguous_checkbox;
	QLabel				*similar_characters_label;
	QLabel				*ambiguous_characters_label;
	QString				password;
};

#endif // RPGUI_H
