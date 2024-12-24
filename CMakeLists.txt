cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(ourpaint)
# Для использования этого приложения необходимо установить QT6. Если у вас есть вопросы, обращайтесь к @ardrasss
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(FetchContent)

# Загрузка и подключение Eigen
FetchContent_Declare(
        Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0  # Замените на нужную версию Eigen
)

FetchContent_MakeAvailable(Eigen)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(ourpaint
  main.cpp
  src/paint.cpp
  src/objects.cpp
  src/requirements.cpp
  src/scaling.cpp

  src/painters/BMPpainter.cpp
  src/painters/QTPainter.cpp

  src/GUI/mainwindow.cpp
  src/GUI/FrameOverlay.cpp
  src/GUI/CastomeConsole.cpp
  src/GUI/RightClickFilter.cpp
  src/GUI/EmojiWidget.cpp

  src/GUI/Windows/help.cpp

  src/saveload/objectInFile.cpp
  src/saveload/FileOurP.cpp
  src/saveload/requirementInFile.cpp
  src/saveload/SaveSettingsApplications.cpp
  src/saveload/LoadSettingsApplications.cpp

  src/ClientServer/Server.cpp
  src/ClientServer/Client.cpp
  src/ClientServer/LocalScanner.cpp
  src/ClientServer/PortCheckTask.cpp

  src/App/Application.cpp

  headers/enums.h
  headers/objects.h
  headers/paint.h
  headers/requirements.h
  headers/scaling.h

  headers/ClientServer/Client.h
  headers/ClientServer/LocalScanner.h
  headers/ClientServer/PortCheckTask.h
  headers/ClientServer/Server.h

  headers/GUI/Windows/CastomeWindowError.h
  headers/GUI/Windows/CastomeWindowWarning.h
  headers/GUI/Windows/CastomeWindowSuccessful.h
  headers/GUI/Windows/help.h
  headers/GUI/Windows/CastomeIpListWindow.h
  headers/GUI/Windows/WindowServer.h
  headers/GUI/Windows/SaveDialog.h
  headers/GUI/Windows/ui_help.h
  headers/GUI/DrawMode.h

  headers/GUI/CastomeConsole.h
  headers/GUI/EmojiWidget.h
  headers/GUI/FrameOverlay.h
  headers/GUI/mainwindow.h
  headers/GUI/RightClickFilter.h
  headers/GUI/ui_mainwindow.h

  headers/painters/Painter.h
  headers/painters/QTPainter.h
  headers/painters/BMPpainter.h

  headers/saveload/FileOurP.h
  headers/saveload/requirementInFile.h
  headers/saveload/LoadSettingsApplications.h
  headers/saveload/objectInFile.h
  headers/saveload/SaveSettingsApplications.h

  headers/App/Application.h
)


include_directories(
  additions
  UndoRedo
  BMPfile/code
  math/headers
  math/headers/decomposition
  math/headers/Optimizers
  math/headers/graph
  headers
  headers/painters
  headers/saveload
  headers/ClientServer
  headers/GUI
  headers/GUI/Windows
  headers/App
)

add_subdirectory(BMPfile)
# add_subdirectory(test)
add_subdirectory(math)

target_link_libraries(ourpaint PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Network
  Eigen3::Eigen
  bitmap
  MinimizerOptimizer
)
