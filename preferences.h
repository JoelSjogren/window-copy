#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(WId selected READ selected)
    
public:
    explicit Preferences(QWidget *parent = 0);
    ~Preferences();

    WId selected() const
    {
        return m_selected;
    }
public slots:
    void updateSelected(int index);

private:
    Ui::Preferences *ui;
    QList<WId> widList;
    QList<QString> titleList;
    void populateLists();
    WId m_selected;
};

#endif // PREFERENCES_H
