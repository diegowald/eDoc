#ifndef SETDATETIMETOUSE_H
#define SETDATETIMETOUSE_H

#include <QDialog>

namespace Ui {
class setDateTimeToUse;
}

class setDateTimeToUse : public QDialog
{
    Q_OBJECT

public:
    explicit setDateTimeToUse(QWidget *parent = 0);
    ~setDateTimeToUse();

    void setData(const QDateTime &dateAndTime, bool useCurrent);
    bool isUsingCurrent() const;
    QDateTime getDateAndTime();
private:
    Ui::setDateTimeToUse *ui;
};

#endif // SETDATETIMETOUSE_H
