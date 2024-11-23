#include <memory>
#include <QApplication>
#include <QTranslator>
#include <QPixmap>
#include "Server.h"
#include "Client.h"
#include "WindowServer.h"
#include "Arry.h"
#include "objects.h"
#include "paint.h"
#include "QTPainter.h"
#include "mainwindow.h"
#include "SaveSettingsApplications.h"
#include "LoadSettingsApplications.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(R"(..\Static\logo\logo2.ico)"));
    QApplication::setStyle("Fusion");
    /*   QPixmap splashPixmap("C:\\Users\\Tim\\CLionProjects\\OurPaint\\Static\\logo\\eye.png");
    QPixmap scaledPixmap = splashPixmap.scaled(splashPixmap.width() * 10, splashPixmap.height() * 10, Qt::KeepAspectRatio);
       QSplashScreen splash(splashPixmap);
       splash.setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
       splash.show();*/

    MainWindow w;
    w.show();

    auto painter = std::make_unique<QTPainter>(w.getUi(), w.getWorkWindow());
    painter->setParent(w.getWorkWindow());
    painter->show();
    QString username = "User";
    Paint screen(painter.get());
    const std::string File = "SettingsSet";
    std::ifstream inFile(File);
    if (!(inFile.good() && inFile.peek() == std::ifstream::traits_type::eof())) {
        LoadSettingsApplications LoadSet("Settings", &w);
        std::vector<bool> settings;
        QString NameUsers;
        LoadSet.LoadSettings(settings, NameUsers);
        w.loadSettings(settings, NameUsers);
        if (!NameUsers.isEmpty()) {
            username = NameUsers;
        }
    }
    inFile.close();
    Server server(username);
    Client client(username);
    bool isConnected = false;
    bool isServer = false;

    /*  QTimer::singleShot(1000, [&]() {
          splash.finish(&w);

      });*/
    auto updateState = [&screen, &w, &painter]() {
        painter->getUsers(false);
        screen.paint();
        painter->draw();
        auto [figures, requirements, settings, name] = w.saveSettings();
        w.Print_LeftMenu(0, "Clear", {});
        std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();

        for (auto element: elements) {
            std::string name;

            for (int i = 0; i < figures.size(); ++i) {
                if (element.first.id == figures[i][1].toLongLong()) {
                    name = figures[i][0].toStdString();
                    break;
                }
            }

            if (element.second.et == ET_POINT) {
                double x = element.second.params[1];
                double y = element.second.params[1];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x, y});
                } else {
                    w.Print_LeftMenu(element.first.id, "Point", {x, y});
                }
            } else if (element.second.et == ET_SECTION) {
                double x1 = element.second.params[0];
                double y1 = element.second.params[1];
                double x2 = element.second.params[2];
                double y2 = element.second.params[3];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x1, y1, x2, y2});
                } else {
                    w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
                }
            } else if (element.second.et == ET_CIRCLE) {
                double x = element.second.params[0];
                double y = element.second.params[1];
                double r = element.second.params[2];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x, y, r});
                } else {
                    w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
                }
            }
        }
        std::vector<std::pair<ID, RequirementData>> req = screen.getAllRequirementsInfo();

        w.Requar_LeftMenu(0, "Clear", 0, 0, 0);
        for (const auto &element: req) {
            QString text;
            double param = 0;
            if (element.second.params.size() == 1) {
                param = element.second.params[0];
            }
            switch (element.second.req) {
                case ET_POINTSECTIONDIST:
                    text = QString("PointSectionDist");
                    break;
                case ET_POINTONSECTION:
                    text = QString("PointOnSection");
                    break;
                case ET_POINTPOINTDIST:
                    text = QString("PointPointDist");
                    break;
                case ET_POINTONPOINT:
                    text = QString("PointOnPoint");
                    break;
                case ET_SECTIONCIRCLEDIST:
                    text = QString("SectionCircleDist");
                    break;
                case ET_SECTIONONCIRCLE:
                    text = QString("SectionOnCircle");
                    break;
                case ET_SECTIONINCIRCLE:
                    text = QString("SectionInCircle");
                    break;
                case ET_SECTIONSECTIONPARALLEL:
                    text = QString("ParallelSections");
                    break;
                case ET_SECTIONSECTIONPERPENDICULAR:
                    text = QString("PerpendicularSections");
                    break;
                case ET_SECTIONSECTIONANGEL:
                    text = QString("AngleBetweenSections");
                    break;
                case ET_POINTONCIRCLE:
                    text = QString("PointOnCircle");
                    break;
            }
            w.Requar_LeftMenu(element.first.id, text.toStdString(), element.second.objects[0].id,
                              element.second.objects[1].id, param);
        }
    };

    auto handler = [&w, &screen, &painter, &updateState](const QString &command) {
        QStringList commandParts = command.split(' ');
        bool commandRight = false;

        if (commandParts[0] == "point" && commandParts.size() == 3) {
            bool xOk, yOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);

            if (xOk && yOk) {
                ElementData point;
                point.et = ET_POINT;
                point.params.push_back(x);
                point.params.push_back(y);
                ID id = screen.addElement(point);
                w.setSave(false);
                commandRight = true;
            }

        } else if (commandParts[0] == "circle" && commandParts.size() == 4) {
            bool xOk, yOk, rOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);
            double r = commandParts[3].toDouble(&rOk);

            if (xOk && yOk && rOk) {

                ElementData circle;
                circle.et = ET_CIRCLE;
                circle.params.push_back(x);
                circle.params.push_back(y);
                circle.params.push_back(r);
                ID id = screen.addElement(circle);
                w.setSave(false);
                commandRight = true;
            }

        } else if (commandParts[0] == "section" && commandParts.size() == 5) {
            bool xOk, yOk, zOk, rOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);
            double z = commandParts[3].toDouble(&zOk);
            double r = commandParts[4].toDouble(&rOk);
            if (xOk && yOk && zOk && rOk) {
                ElementData section;
                section.et = ET_SECTION;
                section.params.push_back(x);
                section.params.push_back(y);
                section.params.push_back(z);
                section.params.push_back(r);
                ID id = screen.addElement(section);
                w.setSave(false);
                commandRight = true;
            }

        } else if (commandParts[0] == "exit") {
            commandRight = true;
            w.close();
        } else if (commandParts[0] == "clear") {
            commandRight = true;
            w.setSave(true);
            painter->clear();
            w.Print_LeftMenu(0, "Clear", {});
            screen.clear();
        } else if (commandParts[0] == "addreq" && commandParts.size() > 3) {
            commandRight = true;
            int req = commandParts[1].toInt();
            ID obj1 = commandParts[2].toInt();
            ID obj2 = commandParts[3].toInt();
            RequirementData reqData;
            Requirement type;
            double parameters = 0;

            if (commandParts.size() == 5) {
                parameters = commandParts[4].toDouble();
            }
            try {
                switch (req) {
                    case 1:
                        type = ET_POINTSECTIONDIST;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        reqData.params.push_back(parameters);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 2:
                        type = ET_POINTONSECTION;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 3:
                        type = ET_POINTPOINTDIST;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        reqData.params.push_back(parameters);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 4:
                        type = ET_POINTONPOINT;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 5:
                        type = ET_SECTIONCIRCLEDIST;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        reqData.params.push_back(parameters);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 6:
                        type = ET_SECTIONONCIRCLE;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 7:
                        type = ET_SECTIONINCIRCLE;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 8:
                        type = ET_SECTIONSECTIONPARALLEL;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 9:
                        type = ET_SECTIONSECTIONPERPENDICULAR;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    case 10:
                        type = ET_SECTIONSECTIONANGEL;
                        reqData.req = type;
                        reqData.objects.push_back(obj1);
                        reqData.objects.push_back(obj2);
                        reqData.params.push_back(parameters);
                        screen.addRequirement(reqData);
                        w.setSave(false);
                        break;
                    default:
                        commandRight = false;
                        w.showError("Not right number of req");
                        break;
                }
            } catch (std::exception &e) {
                commandRight = false;
                w.showError(e.what());
            }
        }
        if (commandRight) {
            updateState();
        }
    };

    QObject::connect(&w, &MainWindow::parameterChanged,
                     [&w, &screen, &painter](unsigned long long id, const std::vector<double> &parameters) {
                         w.setSave(false);
                         screen.paint();
                         painter->draw();
                     });

    QObject::connect(painter.get(), &QTPainter::SigPoint,
                     [&painter, &w, &screen, &updateState, &isConnected, &isServer, &server, &client](QPoint Position) {
                         if (isConnected) {
                             if (isServer) {
                                 ElementData point;
                                 point.et = ET_POINT;
                                 point.params.push_back(Position.x());
                                 point.params.push_back(Position.y());
                                 ID id = screen.addElement(point);
                                 w.setSave(false);
                                 updateState();
                                 server.sendToClients(QString::fromStdString(screen.to_string()));
                             } else {
                                 client.sendCommandToServer("point " + QString::number(Position.x()) + " " +
                                                            QString::number(Position.y()));
                             }
                         } else {
                             ElementData point;
                             point.et = ET_POINT;
                             point.params.push_back(Position.x());
                             point.params.push_back(Position.y());
                             ID id = screen.addElement(point);
                             w.setSave(false);
                             updateState();
                         }
                     });

    QObject::connect(painter.get(), &QTPainter::SigCircle,
                     [&painter, &w, &screen, &updateState, &isConnected, &isServer, &server, &client](
                             QPoint centerPoint, int radius) {
                         ElementData circle;
                         if (isConnected) {
                             if (isServer) {
                                 circle.et = ET_CIRCLE;
                                 circle.params.push_back(centerPoint.x());
                                 circle.params.push_back(centerPoint.y());
                                 circle.params.push_back(radius);
                                 ID id = screen.addElement(circle);
                                 w.setSave(false);
                                 updateState();
                                 server.sendToClients(QString::fromStdString(screen.to_string()));
                             } else {
                                 client.sendCommandToServer("circle " + QString::number(centerPoint.x()) + " " +
                                                            QString::number(centerPoint.y()) + " " +
                                                            QString::number(radius));
                             }
                         } else {
                             circle.et = ET_CIRCLE;
                             circle.params.push_back(centerPoint.x());
                             circle.params.push_back(centerPoint.y());
                             circle.params.push_back(radius);
                             ID id = screen.addElement(circle);
                             w.setSave(false);
                             updateState();
                         }
                     });

    QObject::connect(painter.get(), &QTPainter::SigSection,
                     [&painter, &w, &screen, &updateState, &isConnected, &isServer, &server, &client](int startX,
                                                                                                      int startY,
                                                                                                      int endX,
                                                                                                      int endY) {
                         ElementData section;
                         if (isConnected) {
                             if (isServer) {
                                 section.et = ET_SECTION;
                                 section.params.push_back(startX);
                                 section.params.push_back(startY);
                                 section.params.push_back(endX);
                                 section.params.push_back(endY);
                                 ID id = screen.addElement(section);
                                 w.setSave(false);
                                 updateState();
                                 server.sendToClients(QString::fromStdString(screen.to_string()));
                             } else {
                                 client.sendCommandToServer("section " + QString::number(startX) + " " +
                                                            QString::number(startY) + " " +
                                                            QString::number(endX) + " " +
                                                            QString::number(endY));
                             }
                         } else {
                             section.et = ET_SECTION;
                             section.params.push_back(startX);
                             section.params.push_back(startY);
                             section.params.push_back(endX);
                             section.params.push_back(endY);
                             ID id = screen.addElement(section);
                             w.setSave(false);
                             updateState();
                         }
                     });


    // Выбор фигур
    QObject::connect(&w, &MainWindow::SigMoving, [&painter]() {
        painter->setEditor(false);
        painter->setCircle(false);
        painter->setSection(false);
        painter->setPoint(false);
    });
    QObject::connect(&w, &MainWindow::SigPoint, [&painter]() {
        painter->setEditor(true);
        painter->setPoint(true);
        painter->setCircle(false);
        painter->setSection(false);
    });
    QObject::connect(&w, &MainWindow::SigSection, [&painter]() {
        painter->setEditor(true);
        painter->setSection(true);
        painter->setCircle(false);
        painter->setPoint(false);
    });
    QObject::connect(&w, &MainWindow::SigCircle, [&painter]() {
        painter->setEditor(true);
        painter->setCircle(true);
        painter->setSection(false);
        painter->setPoint(false);
    });

    // Настройки
    QObject::connect(&w, &MainWindow::GridOn, [&painter](bool T) {
        painter->setCell(T);
    });

    // Чатик
    QObject::connect(&w, &MainWindow::EnterMessage, [&](const QString &text) {
        if (isConnected) {
            if (isServer) {
                w.setMessage(username.toStdString(), text.toStdString());
                server.sendChatToClients(text, username);
            } else {
                if (!text.isEmpty()) {
                    client.sendChatMessage(text);
                }
            }
        } else {
            w.showError("Firstly connect to server!");
            return;
        }
    });
    QObject::connect(&client, &Client::serverShutdown, [&w, &isConnected, &isServer]() {
        w.showSuccess("Server shutdown!(");
        isConnected = false;
        isServer = false;
    });

    QObject::connect(&w, &MainWindow::NameUsers, [&isConnected, &server, &client, &username](const QString &text) {
        username = text;
        if (!isConnected) {
            server.setName(username);
            client.setName(username);
        }
    });

    QObject::connect(&client, &Client::disconnectedFromServer, [&w, &isConnected]() {
        isConnected = false;
        w.showSuccess("You disconnected from server");
    });
    //Кнопки сервера
    QObject::connect(&w, &MainWindow::SigExitSession, [&w, &server, &client, &isConnected, &isServer]() {
        if (isConnected) {
            if (isServer) {
                server.stopServer();
                isServer = false;
                isConnected = false;
                w.showSuccess("Successfully stopped!");
            } else {
                client.disconnectFromServer();
                isConnected = false;
                w.showSuccess("Successfully disconnected!");
            }
        } else {
            w.showError("Firstly connect to server!");
        }
    });
    QObject::connect(&w, &MainWindow::SigOpenServer, [&](const QString &text) {
        if (isConnected || isServer) {
            w.showError("Firstly disconnect!");
            return;
        }
        bool ok = false;
        text.toUShort(&ok);
        if (!ok) {
            w.showError("Error! This is not valid port!");
            return;
        }
        server.startServer(text.toUShort(&ok));
        isServer = true;
        isConnected = true;
        w.showSuccess("Successfully connected to server!");
    });
    QObject::connect(&w, &MainWindow::SigJoinServer, [&](const QString &text) {
        if (isConnected || isServer) {
            w.showError("Firstly disconnect!");
            return;
        }
        bool ok = false;
        QStringList texts = text.split(':');
        qDebug() << texts[1];
        texts[1].toUShort(&ok);
        if (!ok) {
            w.showError("Error! This is not valid port!");
            return;
        }
        client.connectToServer(texts[0], texts[1].toUShort(&ok));
        isConnected = true;
        w.showSuccess("Successfully connected to server!");
    });

    // Сервера
    QObject::connect(&server, &Server::newCommandReceived, [&](const QString &cmd) {
        handler(cmd);
        server.sendToClients(QString::fromStdString(screen.to_string()));
    });
    QObject::connect(&client, &Client::newStateReceived, [&](const QString &state) {
        screen.loadFromString(state.toStdString());
        updateState();
    });
    //Chat
    QObject::connect(&client, &Client::newChatMessageReceived, [&](const QString &msg, const QString &name) {
        w.setMessage(name.toStdString(), msg.toStdString());
        updateState();
    });
    QObject::connect(&server, &Server::newMessageReceived, [&](const QString &msg, const QString &name) {
        w.setMessage(name.toStdString(), msg.toStdString());
        updateState();
    });
    QObject::connect(&server, &Server::newConnection, [&server, &screen](){
        server.sendToClients(QString::fromStdString(screen.to_string()));
    });

    QObject::connect(&w, &MainWindow::EnterPressed, [&](const QString &command) {
        QStringList commandParts = command.split(' ');
        if (isConnected) {
            if (isServer) {
                handler(command);
                server.sendToClients(QString::fromStdString(screen.to_string()));
            } else {
                client.sendCommandToServer(command);
            }
        } else {
            handler(command);
        }
    });

// Изменение обьектов левого меню
    QObject::connect(&w, &MainWindow::ChangeLeftMenu, [&w, &screen, &painter]() {
        w.setSave(false);
        w.Print_LeftMenu(0, "Clear", {});
        std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
        for (auto element: elements) {
            if (element.second.et == ET_POINT) {
                double x = element.second.params[0];
                double y = element.second.params[1];
                w.Print_LeftMenu(element.first.id, "Point", {x, y});
            } else if (element.second.et == ET_CIRCLE) {
                double x = element.second.params[0];
                double y = element.second.params[1];
                double r = element.second.params[2];
                w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
            } else if (element.second.et == ET_SECTION) {
                double x1 = element.second.params[0];
                double y1 = element.second.params[1];
                double x2 = element.second.params[2];
                double y2 = element.second.params[3];
                w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
            }
        }

        screen.paint();
        painter->draw();
    });


    QObject::connect(&w, &MainWindow::resized, [&screen, &painter]() {
        screen.paint();
        painter->draw();
    });

    QObject::connect(&w, &MainWindow::KeyPlus, [&screen, &painter]() {
        painter->getUsers(true);
        painter->setZoomPlus();
        screen.paint();
        painter->draw();
    });
    QObject::connect(&w, &MainWindow::KeyMinus, [&screen, &painter]() {
        painter->getUsers(true);
        painter->setZoomMinus();
        screen.paint();
        painter->draw();
        painter->draw();
    });
    QObject::connect(&w, &MainWindow::KeyZero, [&screen, &painter]() {
        painter->getUsers(true);
        painter->setZoomZero();
        screen.paint();
        painter->draw();
    });

    QObject::connect(&w, &MainWindow::REDO, [&screen, &painter, &w, &updateState]() {
        try {
            screen.redo();
            updateState();
            w.setSave(true);

            auto [figures, requirements, settings, name] = w.saveSettings();
            w.Print_LeftMenu(0, "Clear", {});
            std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
            for (auto element: elements) {
                std::string name;
                for (int i = 0; i < figures.size(); ++i) {
                    if (element.first.id == figures[i][1].toLongLong()) {
                        name = figures[i][0].toStdString();
                        break;
                    }
                }
                if (element.second.et == ET_POINT) {
                    double x = element.second.params[0];
                    double y = element.second.params[1];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x, y});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Point", {x, y});
                    }
                } else if (element.second.et == ET_CIRCLE) {
                    double x = element.second.params[0];
                    double y = element.second.params[1];
                    double r = element.second.params[2];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x, y, r});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
                    }
                } else if (element.second.et == ET_SECTION) {
                    double x1 = element.second.params[0];
                    double y1 = element.second.params[1];
                    double x2 = element.second.params[2];
                    double y2 = element.second.params[3];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x1, y1, x2, y2});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
                    }
                }
            }
        } catch (std::exception &e) {
            w.showWarning(e.what());
        }

    });

    QObject::connect(&w, &MainWindow::UNDO, [&screen, &painter, &w, &updateState]() {
        try {
            screen.undo();
            updateState();
            w.setSave(true);

            auto [figures, requirements, settings, name] = w.saveSettings();
            w.Print_LeftMenu(0, "Clear", {});
            std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
            for (auto element: elements) {
                std::string name;
                for (int i = 0; i < figures.size(); ++i) {
                    if (element.first.id == figures[i][1].toLongLong()) {
                        name = figures[i][0].toStdString();
                        break;
                    }
                }
                if (element.second.et == ET_POINT) {
                    double x = element.second.params[0];
                    double y = element.second.params[1];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x, y});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Point", {x, y});
                    }
                } else if (element.second.et == ET_CIRCLE) {
                    double x = element.second.params[0];
                    double y = element.second.params[1];
                    double r = element.second.params[2];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x, y, r});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
                    }
                } else if (element.second.et == ET_SECTION) {
                    double x1 = element.second.params[0];
                    double y1 = element.second.params[1];
                    double x2 = element.second.params[2];
                    double y2 = element.second.params[3];
                    if (!name.empty()) {
                        w.Print_LeftMenu(element.first.id, name, {x1, y1, x2, y2});
                    } else {
                        w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
                    }
                }
            }
        } catch (std::exception &e) {
        }
    });


    QObject::connect(&w, &MainWindow::changeSettings, [&w]() {
        SaveSettingsApplications saveSet("Settings", &w);
        saveSet.clear();
        auto [figures, requirements, settings, name] = w.saveSettings();
        saveSet.SaveSettings(settings, name);
    });

    QObject::connect(&w, &MainWindow::saveBMP, [&screen, &w, &painter](const QString &fileName) {
        try {
            screen.exportToBMP(fileName.toStdString().c_str());
            w.showSuccess("Saved to " + fileName);
        }catch (std::exception &e) {
            w.showWarning(e.what());
        }
    });

    QObject::connect(&w, &MainWindow::loadBMP, [&](const QString &fileName) {

    });

    QObject::connect(&w, &MainWindow::projectSaved, [&screen, &w, &painter](const QString &fileName) {
        std::string File = fileName.toStdString();
        auto [figures, requirements, settings, name] = w.saveSettings();
        SaveSettingsApplications saveSet(File.c_str(), &w);
        saveSet.clear();
        saveSet.SaveFigures(figures);
        saveSet.SaveRequirements(requirements);
        saveSet.SaveSettings(settings, name);
        screen.saveToFile(File.c_str());
        screen.paint();
        painter->draw();
    });

    QObject::connect(&w, &MainWindow::LoadFile, [&](const QString &fileName) {
        painter->clear();
        w.Print_LeftMenu(0, "Clear", {});
        std::string File = fileName.toStdString();
        screen.loadFromFile(File.c_str());
        screen.paint();
        painter->draw();

        LoadSettingsApplications LoadSet(File.c_str(), &w);
        std::vector<std::vector<QString>> figures;
        std::vector<std::vector<QString>> requirements;
        std::vector<bool> settings;
        QString NameUsers;
        LoadSet.LoadFigures(figures);
        LoadSet.LoadRequirements(requirements);
        LoadSet.LoadSettings(settings, NameUsers);

        std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
        for (auto element: elements) {
            std::string name;
            for (int i = 0; i < figures.size(); ++i) {
                if (element.first.id == figures[i][1].toLongLong()) {
                    name = figures[i][0].toStdString();
                    break;
                }
            }
            if (element.second.et == ET_POINT) {
                double x = element.second.params[0];
                double y = element.second.params[1];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x, y});
                } else {
                    w.Print_LeftMenu(element.first.id, "Point", {x, y});
                }
            } else if (element.second.et == ET_CIRCLE) {
                double x = element.second.params[0];
                double y = element.second.params[1];
                double r = element.second.params[2];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x, y, r});
                } else {
                    w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
                }
                break;
            } else if (element.second.et == ET_SECTION) {
                double x1 = element.second.params[0];
                double y1 = element.second.params[1];
                double x2 = element.second.params[2];
                double y2 = element.second.params[3];
                if (!name.empty()) {
                    w.Print_LeftMenu(element.first.id, name, {x1, y1, x2, y2});
                } else {
                    w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
                }
                break;
            }
        }


        w.loadSettings(settings, NameUsers);


    });







    /*while (true) {
        char command[64];
        std::cin >> command;
        if (strcmp(command, "q") == 0) {
            std::cout << "Are you sure you want to exit without saving your work? Y/N" << std::endl;
            std::cin >> command;
            if (strcmp(command, "Y") == 0) {
                break;
            }
            if (strcmp(command, "N") == 0) {
                continue;
            }
        }
        if (strcmp(command, "help") == 0) {
            std::cout << "---------------------Commands:---------------------" << std::endl;
            std::cout << "point x y \t-\t Draw point at (x,y)" << std::endl;
            std::cout << "section x1 y1 x2 y2 \t-\t Draw section between two points (x1,y1) and (x2,y2)" << std::endl;
            std::cout << "cirle x y r \t-\t Draw circle with radius r and center at (x,y)" << std::endl;
            std::cout << "getelement ID  \t-\t Get info about element with ID" << std::endl;
            std::cout << "clear \t- \tClears the screen" << std::endl;
            std::cout << "export file_name.bmp \t- \tExports the image to file_name.bmp" << std::endl;
            std::cout << "save file_name.db \t- \tload the information about objects to file_name.db like a table" << std::endl;
            std::cout << "load <bmp/db> file_name.<bmp/db> \t- \tload the information about objects from file_name.db if db or"
                "just load the image from file_name.bmp" << std::endl;
            std::cout << "addreq <instruction> <objects> <param> \t- \tdo something with objects \n"
                "                 Types of instructions:\n"
                "                 '1'  - Take 2 args: one point, and one section and distant that you want between them on <param>.\n"
                "                 '2'  - Take point and section and set it on each other(<param> can be empty)\n"
                "                 '3'  - Take 2 point(they may be on figure) and set distance between them on <param>\n"
                "                 '4'  - Take 2 point(they may be on figure) and set it on each other(<param> can be empty)\n"
                "                 '5'  - Take circle and section and set distance between them on <param>\n"
                "                 '6'  - Take circle and section and set section to be on circle (<param> can be empty)\n"
                "                 '7'  - Take circle and section, set the section completely inside circle (<param> can be empty)\n"
                "                 '8'  - Take 2 sections, set distance and parallel alignment between them on <param>\n"
                "                 '9'  - Take 2 sections, set them perpendicular to each other (<param> can be empty)\n"
                "                 '10' - Take 2 sections, set a specific angle between them on <param>\n"
                << std::endl;
            std::cout << "q \t- \tQuit" << std::endl;
            std::cout << "help \t- \tHelp that show you commands to use" << std::endl;
        }
        if (strcmp(command, "point") == 0) {
            double x, y;
            std::cin >> x >> y;
            ElementData point;
            point.et = ET_POINT;
            point.params.push_back(x);
            point.params.push_back(y);
            ID id = screen.push_back(point);
            std::cout << "Point (" << x << ", " << y << ") added with id " << id.id << std::endl;
        }
        if (strcmp(command, "circle") == 0) {
            double x, y, r;
            std::cin >> x >> y >> r;
            ElementData circle;
            circle.et = ET_CIRCLE;
            circle.params.push_back(x);
            circle.params.push_back(y);
            circle.params.push_back(r);
            ID id = screen.push_back(circle);
            std::cout << "Circle with center on(" << x << ", " << y << ") and radius " << r << " added with id " << id.id << std::endl;
        }
        if (strcmp(command, "section") == 0) {
            double x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;
            ElementData section;
            section.et = ET_SECTION;
            section.params.push_back(x1);
            section.params.push_back(y1);
            section.params.push_back(x2);
            section.params.push_back(y2);
            ID id = screen.push_back(section);
            std::cout << "Section between points (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ") added with id " << id.id << std::endl;
        }
        if (strcmp(command, "export") == 0) {
            char fileName[256];
            std::cin >> fileName;
            try {
                screen.exportToBMP(fileName);
                std::cout << "Exported to " << fileName << " SUCCESS!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Exported to " << fileName << " FAILED (file not found)" << std::endl;
            }
        }
        if (strcmp(command, "save") == 0) {
            char fileName[256];
            std::cin >> fileName;
            try {
//                screen.saveToFile(fileName);
                std::cout << "Saved to " << fileName << " SUCCESS!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Saved to " << fileName << " FAILED (file not found)" << std::endl;
            }
        }
        if (strcmp(command, "load") == 0) {
            char fileResolution[4];
            char fileName[256];
            std::cin >> fileResolution >> fileName;
            try {
                if (strcmp(fileResolution, "db") == 0) {
 // unsecure                  screen.loadFromFile(fileName);
                }
                if (strcmp(fileResolution, "bmp") == 0) {
                    screen.changeBMP(fileName);
                }
                std::cout << "Loaded from " << fileName << " SUCCESS!" << std::endl;
            }
            catch (...) {
                std::cout << "Loaded from " << fileName << " FAILED!" << std::endl;
            }
        }
        if (strcmp(command, "getelement")==0){
            ID id;
            std::cin >> id.id;
            ElementData element = screen.getElementInfo(id);
            switch   (element.et)   {
                case ET_SECTION:
                    std::cout   <<  "TYPE: SECTION" <<std::endl;
                    std::cout << "START POINT: (" << element.params[0]  <<  ",  "  << element.params[1]  <<  ")"  << std::endl;
                    std::cout  <<  "END POINT:  ("  << element.params[2]  <<  ",   "  << element.params[3]  <<  ")"  << std::endl;
                    break;
                case ET_POINT:
                    std::cout  << "TYPE: POINT" << std::endl;
                    std::cout   << "X: " << element.params[0] << std::endl;
                    std::cout   <<  "Y:  "  << element.params[1]  << std::endl;
                    break;
                case ET_CIRCLE:
                    std::cout   <<  "TYPE: CIRCLE"  << std::endl;
                    std::cout << "CENTER: (" << element.params[0]  << ",  "  << element.params[1]  << ")"  << std::endl;
                    std::cout   <<   "RADIUS:   "   << element.params[2]   << std::endl;
                    break;
            }
        }
        if (strcmp(command, "addreq")  ==  0)  {
            int req;
            RequirementData reqData;
            ID obj1;
            ID obj2;
            Requirement type;
            double parameters;
            std::cin  >> req;
            switch  (req)  {
            case 1:
                type = ET_POINTSECTIONDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                reqData.params.push_back(parameters);
                screen.addRequirement(reqData);
                break;
            case 2:
                type = ET_POINTONSECTION;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 3:
                type = ET_POINTPOINTDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                reqData.params.push_back(parameters);
                screen.addRequirement(reqData);
                break;
            case 4:
                type = ET_POINTONPOINT;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 5:
                type = ET_SECTIONCIRCLEDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                reqData.params.push_back(parameters);
                screen.addRequirement(reqData);
                break;
            case 6:
                type = ET_SECTIONONCIRCLE;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 7:
                type = ET_SECTIONINCIRCLE;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 8:
                type = ET_SECTIONSECTIONPARALLEL;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 9:
                type = ET_SECTIONSECTIONPERPENDICULAR;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                screen.addRequirement(reqData);
                break;
            case 10:
                type = ET_SECTIONSECTIONANGEL;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.push_back(obj1);
                reqData.objects.push_back(obj2);
                reqData.params.push_back(parameters);
                screen.addRequirement(reqData);
                break;
            default:
                std::cout << "Unknown requirement. Please read types of instructions by help command" << std::endl;
                break;
            }
        }
        if (strcmp(command, "remr") == 0){
            ID req;
            std::cin >> req.id;
            screen.deleteRequirement(req);
        }
        if (strcmp(command, "undo") == 0){
            try {
                screen.undo();
            }catch (std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
        if (strcmp(command, "redo") == 0){
            try{
            screen.redo();
            }catch (std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
    }*/

    return a.exec();
}