#ifndef HELP_H
#define HELP_H

#include <QMainWindow>
#include "ui_help.h"
#include <QMouseEvent>

class Help : public QMainWindow
{
Q_OBJECT

public:
    explicit Help(QWidget *parent = nullptr);
    ~Help();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void showCommands();
    void showRequirements();
    void showAbout();
    void GuideInf();
    void handleAnchorClicked(const QUrl &url);

private:
    Ui::Help *ui;
    QPoint dragPosition;
};

#endif // HELP_H