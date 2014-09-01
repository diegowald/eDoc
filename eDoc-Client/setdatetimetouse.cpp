#include "setdatetimetouse.h"
#include "ui_setdatetimetouse.h"

setDateTimeToUse::setDateTimeToUse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setDateTimeToUse)
{
    ui->setupUi(this);
}

setDateTimeToUse::~setDateTimeToUse()
{
    delete ui;
}

void setDateTimeToUse::setData(const QDateTime &dateAndTime, bool useCurrent)
{
    ui->dateEdit->setDate(dateAndTime.date());
    ui->timeEdit->setTime(dateAndTime.time());
    ui->checkBox->setCheckState(useCurrent ? Qt::Checked : Qt::Unchecked);
}

bool setDateTimeToUse::isUsingCurrent() const
{
    return ui->checkBox->checkState() == Qt::Checked;
}

QDateTime setDateTimeToUse::getDateAndTime()
{
    QDateTime response(ui->dateEdit->date(), ui->timeEdit->time());
    return response;
}
