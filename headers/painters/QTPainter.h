#ifndef QTPAINTER_H
#define QTPAINTER_H

#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QFrame>
#include <vector>
#include "paint.h"
#include "mainwindow.h"
#include "scaling.h"

class QTPainter : public QFrame, public Painter {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<point> points;
    std::vector<circle> circles;
    std::vector<section> sections;
    Scaling scaling;

    const int CellSize;    // Изначальный размер клетки при отрисовке
    const int maxCellSize; // Максимальный размер клетки при масштабировании
    const int minCellSize; // Минимальный размер клетки при масштабировании
    int currentCellSize;   // Текущий размер клетки
    int cursorX;
    int cursorY;
    bool CellView;         // Флаг отрисовки сетки
    bool editor;
    bool Circle;
    bool Section;
    bool Point;
    QPoint centerPoint;
    QPoint perimeterPoint;
    QPoint sectionStartPoint;
    QPoint sectionEndPoint;
    bool Drawing;
    bool leftClick;
    bool leftDoubleClick;

public:
    void setCircle(bool T){Circle=T;}
    void setSection(bool T){Section=T;}
    void setPoint(bool T){Point=T;}
    bool getDoubleClick(){return leftDoubleClick;}

    bool moving(int x,int y);
    bool moving(int x,int y,int r);
    bool moving(int x1, int y1, int x2,int y2);
    // Функция включения сетки
    void setCell(bool On_Off);

    void draw();

    void setEditor(bool T){editor=T;}

    void getUsers(bool var) { scaling.getUsers(var); }

    void setZoomPlus();

    void setZoomMinus();

    void setZoomZero();

    QTPainter(Ui::MainWindow *ui, QWidget *parent);

    void clear(); // Очистка

    std::vector<double> FindMaxMin();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void drawPoint(point pt, bool isWhite = false) override;

    void drawCircle(circle c, bool isWhite = false) override;

    void drawSection(section sec, bool isWhite = false) override;

    void paintEvent(QPaintEvent *event) override; // Отрисовка

    void changeSize(const rectangle &allObjects) override;

    unsigned long long getWeight() override;

    unsigned long long getHeight() override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;


signals:

    void RightPress();
    void SigPoint(QPoint Position);
    void SigCircle(QPoint centerPoint, int radius);
    void SigSection(int startX, int startY, int endX, int endY);
    void Move(Element F,int x,int y);
    void Move(Element F,int x,int y,int r);
    void Move(Element F,int x,int y,int x1,int y1);




private slots:

    void onWorkWindowResized();


};

#endif // QTPAINTER_H