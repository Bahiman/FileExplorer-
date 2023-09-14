#pragma once
#include<QListView>
class CustomListView : public QListView {
public:
	QSize sizeHint() const override {
		if (model()->rowCount() == 0) return QSize(width(), 0);
		return QSize(width(), model()->rowCount() * sizeHintForRow(0));
	}
};

