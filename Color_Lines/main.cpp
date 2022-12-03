#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "gametablemodel.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    qmlRegisterType<GameTableModel>("GameModel", 1, 0, "GameTableModel");
    qmlRegisterUncreatableMetaObject(CellState::staticMetaObject, "GameModel", 1, 0, "CellState", "Uncreatable enum");
    engine.load(url);

    return app.exec();
}
