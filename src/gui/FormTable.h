/*
 *	Dualword-pmc is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-pmc is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-pmc. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#ifndef FORMTABLE_H_
#define FORMTABLE_H_

#include "Form.h"

class FormDbConnection;
class IndexModel;
class FormViewer;
class TableView;

class FormTable  : public Form {
  Q_OBJECT

public:
	FormTable(QWidget *p = 0);
	virtual ~FormTable();
	virtual QString getTitle() const;
	void init();

public slots:
	void contextMenuRequested(const QPoint&);
	void refresh();


signals:

protected:
	void showEvent(QShowEvent *e);
	void hideEvent(QHideEvent *e);

private slots:
	void createUi();
	void connectSlots();
	void deleteDoc();
	void saveDoc();
	void openDoc();
	void openUrl();
	void doSearch();
	void setSort();

private:
	TableView *table;
	QLineEdit *search, *count;
	QCompleter *completer;
	QStringListModel *searchModel;
	FormDbConnection *db;
	FormViewer* view;
	QCheckBox *sort;

};

#endif /* FORMTABLE_H_ */
