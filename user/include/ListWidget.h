#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <qt4/QtGui/QListWidgetItem>

class ListWidgetItem : public QListWidgetItem
{
public:
    ListWidgetItem(QListWidget *parent = 0, int type = Type);
    ListWidgetItem(const QString & text, QListWidget *parent = 0, int type = Type);
    ListWidgetItem(const QIcon & icon, const QString &text, QListWidget *parent = 0, int type = Type);
    ListWidgetItem(const QListWidgetItem & other);
    virtual ~ListWidgetItem ();
};

#endif // LISTWIDGETITEM_H
