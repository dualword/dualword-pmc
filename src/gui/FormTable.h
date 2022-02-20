/*
 *	Dualword-PMC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-PMC is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-PMC. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#ifndef FORMTABLE_H_
#define FORMTABLE_H_

#include "Form.h"

class FormDbConnection;
class IndexModel;
class FormViewer;
class TableView;

class FormTable  : public MainForm<FormTable> {
  Q_OBJECT

public:
	FormTable(QWidget *p = 0);
	virtual ~FormTable();
	virtual QString getTitle() const;
	virtual void init();

public slots:
	void contextMenuRequested(const QPoint&);
	void refresh();

protected:
	void showEvent(QShowEvent *e);
	void hideEvent(QHideEvent *e);

private slots:
	void createUi();
	void connectSlots();
	void deleteDoc();
	void saveDoc();
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
