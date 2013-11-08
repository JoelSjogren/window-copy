#include "preferences.h"
#include "ui_preferences.h"
#include <QxtGui/QxtWindowSystem>
Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    populateLists();
    ui->listWidget->addItems(titleList);
    setWindowTitle("Choose target window");
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::updateSelected(int index)
{
    m_selected = widList[index];
}

void Preferences::populateLists()
{
    foreach (WId i, QxtWindowSystem::windows()) {
        widList << i;
        titleList << QxtWindowSystem::windowTitle(i);
    }
}
