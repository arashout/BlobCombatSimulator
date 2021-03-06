TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    bullet.cpp \
    sfmlvector.cpp \
    agent.cpp \
    game.cpp \
    fieldofvision.cpp \
    dna.cpp \
    neuralnetwork.cpp \
    simulation.cpp \
    statusindicator.cpp

LIBS += -L"/home/arash/SFML/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "/home/arash/SFML/include"
DEPENDPATH += "/home/arash/SFML/include"

DISTFILES += \
    BlobCombatSimulator.pro.user \
    arial.ttf

HEADERS += \
    bullet.hpp \
    constants.hpp \
    entity.hpp \
    sfmlvector.hpp \
    agent.hpp \
    game.hpp \
    neuralnetwork.hpp \
    simulation.hpp \
    parameters.hpp \
    dna.hpp \
    fieldofvision.hpp \
    statusindicator.hpp
